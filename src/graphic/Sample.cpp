#include "Sample.h"
#include "App.h"
#include "../utility/Exception.h"
#include "../utility/Assert.h"

#include "d3dx12.h"

#include <vector>
#include <algorithm>

using namespace Microsoft::WRL;

DXSample::DXSample(UINT width, UINT height, std::wstring name) :
    m_width(width),
    m_height(height),
    m_title(name),
    m_useWarpDevice(false)
{
    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

DXSample::~DXSample()
{
}

void DXSample::OnInit()
{
//	mDXGIFactoryFlag = 0;
//#ifdef _DEBUG
//	Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
//
//	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
//	{
//		debugController->EnableDebugLayer();
//
//		mDXGIFactoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
//	}
//#endif
//
//	//Create DXGI Factory
//	CreateDXGIFactory2(mDXGIFactoryFlag, IID_PPV_ARGS(&mDXGIFactory));
//
//	struct AdapterInfo
//	{
//		UINT Index;
//		SIZE_T DedicatedSystemMemory;
//	};
//
//	std::vector<AdapterInfo> adapterList;
//
//	for (UINT AdapterIndex = 0; mDXGIFactory->EnumAdapters1(AdapterIndex, &mDXGIAdapter) != DXGI_ERROR_NOT_FOUND; ++AdapterIndex)
//	{
//		DXGI_ADAPTER_DESC1 adapterDesc;
//
//		mDXGIAdapter->GetDesc1(&adapterDesc);
//
//		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
//		{
//			continue;
//		}
//
//		if (SUCCEEDED(D3D12CreateDevice(mDXGIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), nullptr)))
//		{
//			//adapterList.emplace_back(AdapterIndex, adapterDesc.DedicatedSystemMemory);
//			AdapterInfo adapterInfo{ AdapterIndex , adapterDesc.DedicatedVideoMemory };
//			adapterList.push_back(adapterInfo);
//		}
//	}
//
//	ASSERT(adapterList.size() > 0);
//
//	std::sort(adapterList.begin(), adapterList.end(), [](const AdapterInfo& A, const AdapterInfo& B) {
//		return A.DedicatedSystemMemory > B.DedicatedSystemMemory;
//	});
//
//	ThrowIfFailed(mDXGIFactory->EnumAdapters1(adapterList[0].Index, &mDXGIAdapter));
//	//Create Device
//	ThrowIfFailed(D3D12CreateDevice(mDXGIAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mD3DDevice)));
//
//	//Create Command Queue
//	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
//	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
//	ThrowIfFailed(mD3DDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mD3DCommandQueue)));
//
//	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
//	swapChainDesc.BufferCount = BackBufferFrameCount;
//	swapChainDesc.Width = m_width;
//	swapChainDesc.Height = m_height;
//	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
//	swapChainDesc.SampleDesc.Count = 1;
//
//	//Create Swap Chain
//    HWND windowHandle = Win32Application::GetHwnd();
//	ThrowIfFailed(mDXGIFactory->CreateSwapChainForHwnd(mD3DCommandQueue.Get(), windowHandle, &swapChainDesc, nullptr, nullptr, &mDXGISwapChain));
//
//	ThrowIfFailed(mDXGIFactory->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_ALT_ENTER));
//
//	ThrowIfFailed(mDXGISwapChain.As(&mDXGISwapChain3));
//
//	//Get Current Back Buffer Index
//	mCurrentBackBufferIndex = mDXGISwapChain3->GetCurrentBackBufferIndex();
//
//	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
//	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//	rtvDescriptorHeapDesc.NumDescriptors = BackBufferFrameCount;
//	//rtvDescriptorHeapDesc.NodeMask = 0;
//	rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//
//	//Create Description Heap
//	ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&mBackBufferDescriptorHeap)));
//
//	mDescriptorHeapIncrementSize = mD3DDevice->GetDescriptorHandleIncrementSize(rtvDescriptorHeapDesc.Type);
//
//	//Create Render Target View
//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(mBackBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
//	for (size_t i = 0; i < rtvDescriptorHeapDesc.NumDescriptors; i++)
//	{
//		ThrowIfFailed(mDXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&mBackBuffers[i])));
//		mD3DDevice->CreateRenderTargetView(mBackBuffers[i].Get(), nullptr, rtvHandle);
//		rtvHandle.ptr = SIZE_T((INT64)rtvHandle.ptr + (INT64)mDescriptorHeapIncrementSize);
//	}
//
//		//Create Command Allocator
//	ThrowIfFailed(mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mD3DCommandAllocator)));
//
//	//Create Command List
//	//ASSERT(SUCCEEDED(mD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mD3DCommandAllocator.Get(), mD3DPipelineState.Get(), IID_PPV_ARGS(&mD3DCommandList))));
//	ThrowIfFailed(mD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mD3DCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mD3DCommandList)));
//	ThrowIfFailed(mD3DCommandList->Close());
//
//	//Create Fence
//	ThrowIfFailed(mD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mD3DFence)));
//	mFenceValue = 1;
//
//	//Create Event
//	mFanceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
//	ASSERT(mFanceEvent != nullptr);
//
//	mViewPort = D3D12_VIEWPORT{ 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) };
//	mScissorRect = D3D12_RECT{ 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };

    LoadPipeline();
    LoadAssets();
}

void DXSample::OnUpdate()
{

}

void DXSample::OnRender()
{
    PopulateCommandList();

    ID3D12CommandList* commandLists[] = { mD3DCommandList.Get() };
    mD3DCommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

    ThrowIfFailed(mDXGISwapChain3->Present(1, 0));

        // Execute the command list.
    //ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
    //m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    //// Present the frame.
    //ThrowIfFailed(m_swapChain->Present(1, 0));

    WaitForPreviousFrame();
}

void DXSample::OnDestroy()
{
    WaitForPreviousFrame();

    CloseHandle(mFanceEvent);
   // CloseHandle(m_fenceEvent);
}

// Helper function for resolving the full path of assets.
std::wstring DXSample::GetAssetFullPath(LPCWSTR assetName)
{
    return m_assetsPath + assetName;
}

// Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
_Use_decl_annotations_
void DXSample::GetHardwareAdapter(
    IDXGIFactory1* pFactory,
    IDXGIAdapter1** ppAdapter,
    bool requestHighPerformanceAdapter)
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;

    ComPtr<IDXGIFactory6> factory6;
    if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for (
            UINT adapterIndex = 0;
            DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
                adapterIndex,
                requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                IID_PPV_ARGS(&adapter));
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }
    else
    {
        for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    *ppAdapter = adapter.Detach();
}

// Helper function for setting the window's title text.
void DXSample::SetCustomWindowText(LPCWSTR text)
{
    std::wstring windowText = m_title + L": " + text;
    SetWindowTextW(Win32Application::GetHwnd(), windowText.c_str());
}

void DXSample::PopulateCommandList()
{
    ThrowIfFailed(mD3DCommandAllocator->Reset());
    ThrowIfFailed(mD3DCommandList->Reset(mD3DCommandAllocator.Get(), nullptr));

    mD3DCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBackBuffers[mCurrentBackBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mBackBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), mCurrentBackBufferIndex, mDescriptorHeapIncrementSize);

    const float clearColor[] = { 0.0f , 0.2f, 0.4f, 1.0f };

    mD3DCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    mD3DCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBackBuffers[mCurrentBackBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    ThrowIfFailed(mD3DCommandList->Close());

        // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
   // ThrowIfFailed(m_commandAllocator->Reset());

   // // However, when ExecuteCommandList() is called on a particular command 
   // // list, that command list can then be reset at any time and must be before 
   // // re-recording.
   // ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), nullptr));

   // D3D12_RESOURCE_BARRIER rtvBarrier;
   // rtvBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
   // rtvBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
   // rtvBarrier.Transition.pResource = m_renderTargets[m_frameIndex].Get();
   // rtvBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
   // rtvBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
   // rtvBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

   // // Indicate that the back buffer will be used as a render target.
   // //m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
   // m_commandList->ResourceBarrier(1, &rtvBarrier);

   // D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
   // rtvHandle.ptr = SIZE_T((INT64)rtvHandle.ptr + (INT64)(m_rtvDescriptorSize * m_frameIndex));

   // //CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);

   // // Record commands.
   // const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
   // m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

   // D3D12_RESOURCE_BARRIER presentBarrier;
   // presentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
   // presentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
   // presentBarrier.Transition.pResource = m_renderTargets[m_frameIndex].Get();
   // presentBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
   // presentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
   // presentBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;


   // // Indicate that the back buffer will now be used to present.
   //// m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
   // m_commandList->ResourceBarrier(1, &presentBarrier);

   // ThrowIfFailed(m_commandList->Close());
}

void DXSample::WaitForPreviousFrame()
{
    const UINT64 fenceValueToWait = mFenceValue;
    ThrowIfFailed(mD3DCommandQueue->Signal(mD3DFence.Get(), fenceValueToWait));
    ++mFenceValue;

    //UINT fenceCompletedValue = mD3DFence->GetCompletedValue();

    if (mD3DFence->GetCompletedValue() < fenceValueToWait)
    {
        ThrowIfFailed(mD3DFence->SetEventOnCompletion(fenceValueToWait, mFanceEvent));
        WaitForSingleObject(mFanceEvent, INFINITY);
    }

    mCurrentBackBufferIndex = mDXGISwapChain3->GetCurrentBackBufferIndex();

    // Signal and increment the fence value.
    //const UINT64 fence = m_fenceValue;
    //ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
    ////m_fenceValue++;
    //++m_fenceValue;

    //// Wait until the previous frame is finished.
    //if (m_fence->GetCompletedValue() < fence)
    //{
    //    ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
    //    WaitForSingleObject(m_fenceEvent, INFINITE);
    //}

    //m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

void DXSample::LoadPipeline()
{
//    UINT dxgiFactoryFlags = 0;
//
//#if defined(_DEBUG)
//    // Enable the debug layer (requires the Graphics Tools "optional feature").
//    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
//    {
//        ComPtr<ID3D12Debug> debugController;
//        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
//        {
//            debugController->EnableDebugLayer();
//
//            // Enable additional debug layers.
//            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
//        }
//    }
//#endif
//
//    ComPtr<IDXGIFactory4> factory;
//    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));
//
//    struct AdapterInfo
//	{
//		UINT Index;
//		SIZE_T DedicatedSystemMemory;
//	};
//
//	std::vector<AdapterInfo> adapterList;
//
//	for (UINT AdapterIndex = 0; factory->EnumAdapters1(AdapterIndex, &mDXGIAdapter) != DXGI_ERROR_NOT_FOUND; ++AdapterIndex)
//	{
//		DXGI_ADAPTER_DESC1 adapterDesc;
//
//		mDXGIAdapter->GetDesc1(&adapterDesc);
//
//		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
//		{
//			continue;
//		}
//
//		if (SUCCEEDED(D3D12CreateDevice(mDXGIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), nullptr)))
//		{
//			//adapterList.emplace_back(AdapterIndex, adapterDesc.DedicatedSystemMemory);
//			AdapterInfo adapterInfo{ AdapterIndex , adapterDesc.DedicatedVideoMemory };
//			adapterList.push_back(adapterInfo);
//		}
//	}
//
//	ASSERT(adapterList.size() > 0);
//
//	std::sort(adapterList.begin(), adapterList.end(), [](const AdapterInfo& A, const AdapterInfo& B) {
//		return A.DedicatedSystemMemory > B.DedicatedSystemMemory;
//	});
//
//	ThrowIfFailed(factory->EnumAdapters1(adapterList[0].Index, &mDXGIAdapter));
//	//Create Device
//	ThrowIfFailed(D3D12CreateDevice(mDXGIAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
//
//    // Describe and create the command queue.
//    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
//    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
//
//    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
//
//    // Describe and create the swap chain.
//    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
//    swapChainDesc.BufferCount = FrameCount;
//    swapChainDesc.Width = m_width;
//    swapChainDesc.Height = m_height;
//    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//    swapChainDesc.SampleDesc.Count = 1;
//
//    ComPtr<IDXGISwapChain1> swapChain;
//    ThrowIfFailed(factory->CreateSwapChainForHwnd(
//        m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
//        Win32Application::GetHwnd(),
//        &swapChainDesc,
//        nullptr,
//        nullptr,
//        &swapChain
//    ));
//
//    // This sample does not support fullscreen transitions.
//    ThrowIfFailed(factory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));
//
//    ThrowIfFailed(swapChain.As(&m_swapChain));
//    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
//
//    // Create descriptor heaps.
//    {
//        // Describe and create a render target view (RTV) descriptor heap.
//        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
//        rtvHeapDesc.NumDescriptors = FrameCount;
//        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//        ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
//
//        m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//    }
//
//    // Create frame resources.
//    {
//        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
//
//        // Create a RTV for each frame.
//        for (UINT n = 0; n < FrameCount; n++)
//        {
//            ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
//            m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
//            rtvHandle.Offset(1, m_rtvDescriptorSize);
//        }
//    }
//
//    ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));

	mDXGIFactoryFlag = 0;
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug> debugController;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();

		mDXGIFactoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	//Create DXGI Factory
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
			//adapterList.emplace_back(AdapterIndex, adapterDesc.DedicatedSystemMemory);
			AdapterInfo adapterInfo{ AdapterIndex , adapterDesc.DedicatedVideoMemory };
			adapterList.push_back(adapterInfo);
		}
	}

	ASSERT(adapterList.size() > 0);

	std::sort(adapterList.begin(), adapterList.end(), [](const AdapterInfo& A, const AdapterInfo& B) {
		return A.DedicatedSystemMemory > B.DedicatedSystemMemory;
	});

	ThrowIfFailed(mDXGIFactory->EnumAdapters1(adapterList[0].Index, &mDXGIAdapter));
	//Create Device
	ThrowIfFailed(D3D12CreateDevice(mDXGIAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mD3DDevice)));

	//Create Command Queue
	//D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	//commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	//ThrowIfFailed(mD3DDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mD3DCommandQueue)));

    // Describe and create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(mD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mD3DCommandQueue)));

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = BackBufferFrameCount;
    swapChainDesc.Width = m_width;
    swapChainDesc.Height = m_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(mDXGIFactory->CreateSwapChainForHwnd(
        mD3DCommandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        Win32Application::GetHwnd(),
        &swapChainDesc,
        nullptr,
        nullptr,
        &mDXGISwapChain
    ));

	ThrowIfFailed(mDXGIFactory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(mDXGISwapChain.As(&mDXGISwapChain3));
    mCurrentBackBufferIndex = mDXGISwapChain3->GetCurrentBackBufferIndex();

	//D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	//rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//rtvDescriptorHeapDesc.NumDescriptors = BackBufferFrameCount;
	//rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    // Describe and create a render target view (RTV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = BackBufferFrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mBackBufferDescriptorHeap)));

    mDescriptorHeapIncrementSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//Create Description Heap

	//Create Render Target View
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(mBackBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mBackBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	for (size_t i = 0; i < BackBufferFrameCount; i++)
	{
		ThrowIfFailed(mDXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&mBackBuffers[i])));
		mD3DDevice->CreateRenderTargetView(mBackBuffers[i].Get(), nullptr, rtvHandle);
		//rtvHandle.ptr = SIZE_T((INT64)rtvHandle.ptr + (INT64)mDescriptorHeapIncrementSize);
        rtvHandle.Offset(1, mDescriptorHeapIncrementSize);
	}

		//Create Command Allocator
	ThrowIfFailed(mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mD3DCommandAllocator)));
}

void DXSample::LoadAssets()
{
    //// Create the command list.
    //ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)));

    //// Command lists are created in the recording state, but there is nothing
    //// to record yet. The main loop expects it to be closed, so close it now.
    //ThrowIfFailed(m_commandList->Close());

    //// Create synchronization objects.
    //{
    //    ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    //    m_fenceValue = 1;

    //    // Create an event handle to use for frame synchronization.
    //    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    //    if (m_fenceEvent == nullptr)
    //    {
    //        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    //    }
    //}

    //Create Command List
	ThrowIfFailed(mD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mD3DCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mD3DCommandList)));
	ThrowIfFailed(mD3DCommandList->Close());

	//Create Fence
	ThrowIfFailed(mD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mD3DFence)));
	mFenceValue = 1;

	//Create Event
	mFanceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (mFanceEvent == nullptr)
    {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
}

// Helper function for parsing any supplied command line args.
_Use_decl_annotations_
void DXSample::ParseCommandLineArgs(WCHAR* argv[], int argc)
{
    for (int i = 1; i < argc; ++i)
    {
        if (_wcsnicmp(argv[i], L"-warp", wcslen(argv[i])) == 0 ||
            _wcsnicmp(argv[i], L"/warp", wcslen(argv[i])) == 0)
        {
            m_useWarpDevice = true;
            m_title = m_title + L" (WARP)";
        }
    }
}
