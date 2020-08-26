#include "ApplicationDX12.h"
#include "../utility/Assert.h"
#include <algorithm>

namespace Dash
{
	ApplicationDX12::ApplicationDX12()
		: Application()
	{

		mDXGIFactoryFlag = 0;
#ifdef _DEBUG
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;

		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			mDXGIFactoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
		}		
#endif

		CreateDXGIFactory2(mDXGIFactoryFlag, IID_PPV_ARGS(&mDXGIFactory));

		struct AdapterInfo
		{
			UINT Index;
			SIZE_T DedicatedSystemMemory;
		};

		std::vector<AdapterInfo> adapterList;

		for (UINT AdapterIndex = 0; mDXGIFactory->EnumAdapters1(AdapterIndex, &mDXGIAdapter) != DXGI_ERROR_NOT_FOUND; ++AdapterIndex)
		{
			DXGI_ADAPTER_DESC1 adapterDesc;

			mDXGIAdapter->GetDesc1(&adapterDesc);

			if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			if (SUCCEEDED(D3D12CreateDevice(mDXGIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), nullptr)))
			{
				adapterList.emplace_back(AdapterIndex, adapterDesc.DedicatedSystemMemory);
			}
		}

		ASSERT(adapterList.size() > 0);

		std::sort(adapterList.begin(), adapterList.end(), [](const AdapterInfo& A, const AdapterInfo& B) {
			return A.DedicatedSystemMemory > B.DedicatedSystemMemory;
		});

		mDXGIFactory->EnumAdapters1(adapterList[0].Index, &mDXGIAdapter);
		D3D12CreateDevice(mDXGIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&mD3DDevice));

		D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
		mD3DDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mD3DCommandQueue));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
		swapChainDesc.BufferCount = ApplicationDX12::BackBufferFrameCount;
		swapChainDesc.Width = mWindow.GetWindowWidth();
		swapChainDesc.Height = mWindow.GetWindowHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		mDXGIFactory->CreateSwapChainForHwnd(mD3DCommandQueue.Get(), mWindow.GetWindowHandle(), &swapChainDesc, nullptr, nullptr, &mDXGISwapChain);

		ASSERT(SUCCEEDED(mDXGISwapChain.As(&mDXGISwapChain3)));

		mCurrentBackBufferIndex = mDXGISwapChain3->GetCurrentBackBufferIndex();

		D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc;
		rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescriptorHeapDesc.NumDescriptors = ApplicationDX12::BackBufferFrameCount;
		rtvDescriptorHeapDesc.NodeMask = 0;
		rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		mD3DDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&mBackBufferDescriptorHeap));

		mDescriptorHeapIncrementSize = mD3DDevice->GetDescriptorHandleIncrementSize(rtvDescriptorHeapDesc.Type);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(mBackBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		for (size_t i = 0; i < rtvDescriptorHeapDesc.NumDescriptors; i++)
		{
			mDXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&mBackBuffers[i]));
			mD3DDevice->CreateRenderTargetView(mBackBuffers[i].Get(), nullptr, rtvHandle);
			rtvHandle.ptr = SIZE_T((INT64)rtvHandle.ptr + (INT64)mDescriptorHeapIncrementSize);
		}

		mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mD3DCommandAllocator));


	}

	ApplicationDX12::~ApplicationDX12()
	{
	}

	void ApplicationDX12::OnRender(const RenderEventArgs& e)
	{
	}

	void ApplicationDX12::OnUpdate(const UpdateEventArgs& e)
	{
	}
}
