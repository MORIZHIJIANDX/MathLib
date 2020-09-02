#include "ApplicationDX12.h"
#include "../utility/Assert.h"
#include "../math/MathType.h"
#include "../utility/Exception.h"
#include <DirectXMath.h>
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
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
		ThrowIfFailed(mD3DDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mD3DCommandQueue)));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		swapChainDesc.BufferCount = ApplicationDX12::BackBufferFrameCount;
		swapChainDesc.Width = mWindow.GetWindowWidth();
		swapChainDesc.Height = mWindow.GetWindowHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		//Create Swap Chain
		HWND windowHandle = mWindow.GetWindowHandle();
		ThrowIfFailed(mDXGIFactory->CreateSwapChainForHwnd(mD3DCommandQueue.Get(), windowHandle, &swapChainDesc, nullptr, nullptr, &mDXGISwapChain));
		
		ThrowIfFailed(mDXGIFactory->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_ALT_ENTER));

		ThrowIfFailed(mDXGISwapChain.As(&mDXGISwapChain3));

		//Get Current Back Buffer Index
		mCurrentBackBufferIndex = mDXGISwapChain3->GetCurrentBackBufferIndex();

		D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
		rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescriptorHeapDesc.NumDescriptors = ApplicationDX12::BackBufferFrameCount;
		//rtvDescriptorHeapDesc.NodeMask = 0;
		rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		//Create Description Heap
		ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&mBackBufferDescriptorHeap)));

		mDescriptorHeapIncrementSize = mD3DDevice->GetDescriptorHandleIncrementSize(rtvDescriptorHeapDesc.Type);
		
		//Create Render Target View
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(mBackBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		for (size_t i = 0; i < rtvDescriptorHeapDesc.NumDescriptors; i++)
		{
			ThrowIfFailed(mDXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&mBackBuffers[i])));
			mD3DDevice->CreateRenderTargetView(mBackBuffers[i].Get(), nullptr, rtvHandle);
			rtvHandle.ptr = SIZE_T((INT64)rtvHandle.ptr + (INT64)mDescriptorHeapIncrementSize);
		}
		
		/*

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 0;
		rootSignatureDesc.pParameters = nullptr;
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		//Serialize Root Signature
		Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> signatureErrorBlob;
		ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &signatureErrorBlob));

		//Create Root Signature
		ThrowIfFailed(mD3DDevice->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&mD3DRootSignature)));

		Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

		int shaderCompileFlags = 0;
#ifdef _DEBUG
		shaderCompileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		std::wstring shaderFileName{L"shader.hlsl"};
		ThrowIfFailed(D3DCompileFromFile(shaderFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", shaderCompileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(shaderFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", shaderCompileFlags, 0, &pixelShader, nullptr));

		D3D12_INPUT_ELEMENT_DESC inputElements[] = { 
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0} ,
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		D3D12_SHADER_BYTECODE vsShaderBytecode;
		vsShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();
		vsShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();

		D3D12_SHADER_BYTECODE psShaderBytecode;
		psShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();
		psShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();

		D3D12_RASTERIZER_DESC rasterizerDesc{};
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.ForcedSampleCount = 0;
		rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		D3D12_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};
		pipelineStateDesc.InputLayout = { inputElements, _countof(inputElements) };
		pipelineStateDesc.pRootSignature = mD3DRootSignature.Get();
		pipelineStateDesc.VS = vsShaderBytecode;
		pipelineStateDesc.PS = psShaderBytecode;
		pipelineStateDesc.RasterizerState = rasterizerDesc;
		pipelineStateDesc.BlendState = blendDesc;
		pipelineStateDesc.DepthStencilState.DepthEnable = FALSE;
		pipelineStateDesc.DepthStencilState.StencilEnable = FALSE;
		pipelineStateDesc.SampleMask = UINT_MAX;
		pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateDesc.NumRenderTargets = 1;
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineStateDesc.SampleDesc.Count = 1;
		
		//Create Graphics Pipeline State
		ThrowIfFailed(mD3DDevice->CreateGraphicsPipelineState(&pipelineStateDesc,IID_PPV_ARGS(&mD3DPipelineState)));

		struct PosColorVert
		{
			Vector3f Position;
			Vector3f Color;
		};

		PosColorVert triangleVertexes[] = {
			{Vector3f{0.0f, 0.5f, 1.0f}, Vector3f{1.0f, 0.0f, 0.0f}},
			{Vector3f{0.5f, -0.5f, 1.0f}, Vector3f{0.0f, 1.0f, 0.0f}},
			{Vector3f{-0.5f, -0.5f, 1.0f}, Vector3f{0.0f, 0.0f, 1.0f}},
		};

		const UINT triangleVertexBufferSize = sizeof(triangleVertexes);
		
		D3D12_HEAP_PROPERTIES vertexBufferHeapProperties{};
		vertexBufferHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		vertexBufferHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		vertexBufferHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		vertexBufferHeapProperties.VisibleNodeMask = 1;
		vertexBufferHeapProperties.CreationNodeMask = 1;

		D3D12_RESOURCE_DESC vertexBufferResourceDesc{};
		vertexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertexBufferResourceDesc.Alignment = 0;
		vertexBufferResourceDesc.Width = triangleVertexBufferSize;
		vertexBufferResourceDesc.Height = 1;
		vertexBufferResourceDesc.DepthOrArraySize = 1;
		vertexBufferResourceDesc.MipLevels = 1;
		vertexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		vertexBufferResourceDesc.SampleDesc.Count = 1;
		vertexBufferResourceDesc.SampleDesc.Quality = 0;
		vertexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vertexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		//Create Vertex Buffer
		ThrowIfFailed(mD3DDevice->CreateCommittedResource(&vertexBufferHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexBufferResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mVertexBuffer)));

		//Copy Data To Vertex Buffer
		UINT8* vertexBufferBegin = nullptr;
		D3D12_RANGE vertexBufferReadRange{0, 0};
		ThrowIfFailed(mVertexBuffer->Map(0, &vertexBufferReadRange, reinterpret_cast<void**>(&vertexBufferBegin)));
		memcpy(vertexBufferBegin, triangleVertexes, triangleVertexBufferSize);
		mVertexBuffer->Unmap(0, nullptr);

		mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes = sizeof(PosColorVert);
		mVertexBufferView.SizeInBytes = triangleVertexBufferSize;
		
		*/

		//Create Command Allocator
		ThrowIfFailed(mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mD3DCommandAllocator)));

		//Create Command List
		//ASSERT(SUCCEEDED(mD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mD3DCommandAllocator.Get(), mD3DPipelineState.Get(), IID_PPV_ARGS(&mD3DCommandList))));
		ThrowIfFailed(mD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mD3DCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mD3DCommandList)));
		ThrowIfFailed(mD3DCommandList->Close());

		//Create Fence
		ThrowIfFailed(mD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mD3DFence)));
		mFenceValue = 1;

		//Create Event
		mFanceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		ASSERT(mFanceEvent != nullptr);

		mViewPort = D3D12_VIEWPORT{ 0.0f, 0.0f, static_cast<float>(mWindow.GetWindowWidth()), static_cast<float>(mWindow.GetWindowHeight()) };
		mScissorRect = D3D12_RECT{ 0, 0, static_cast<LONG>(mWindow.GetWindowWidth()), static_cast<LONG>(mWindow.GetWindowHeight()) };
	}

	ApplicationDX12::~ApplicationDX12()
	{
		WaitForPreviousFrame();

		CloseHandle(mFanceEvent);
	}

	void ApplicationDX12::OnRender(const RenderEventArgs& e)
	{
		/*
		mD3DCommandList->SetGraphicsRootSignature(mD3DRootSignature.Get());
		mD3DCommandList->RSSetViewports(1, &mViewPort);
		mD3DCommandList->RSSetScissorRects(1, &mScissorRect);

		D3D12_RESOURCE_BARRIER rtvBarrier;
		rtvBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		rtvBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		rtvBarrier.Transition.pResource = mBackBuffers[mCurrentBackBufferIndex].Get();
		rtvBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		rtvBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		rtvBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		mD3DCommandList->ResourceBarrier(1, &rtvBarrier);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(mBackBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		rtvHandle.ptr = SIZE_T((INT64)rtvHandle.ptr + (INT64)(mDescriptorHeapIncrementSize * mCurrentBackBufferIndex));

		mD3DCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		float brightness = 0.0f;
		const float clearColor[] = { 0.0f , 0.2f, 0.4f, 1.0f };

		mD3DCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		mD3DCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mD3DCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);

		mD3DCommandList->DrawInstanced(3, 1, 0, 0);

		D3D12_RESOURCE_BARRIER presentBarrier;
		presentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		presentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		presentBarrier.Transition.pResource = mBackBuffers[mCurrentBackBufferIndex].Get();
		presentBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		presentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		presentBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		mD3DCommandList->ResourceBarrier(1, &presentBarrier);

		mD3DCommandList->Close();

		ID3D12CommandList* commandList[] = { mD3DCommandList.Get() };
		mD3DCommandQueue->ExecuteCommandLists(_countof(commandList), commandList);

		mDXGISwapChain3->Present(1, 0);

		UINT fenceValueToWait = mFenceValue;
		mD3DCommandQueue->Signal(mD3DFence.Get(), fenceValueToWait);
		++mFenceValue;

		if (mD3DFence->GetCompletedValue() < fenceValueToWait)
		{
			mD3DFence->SetEventOnCompletion(fenceValueToWait, mFanceEvent);
			WaitForSingleObject(mFanceEvent, INFINITY);
		}

		mCurrentBackBufferIndex = mDXGISwapChain3->GetCurrentBackBufferIndex();

		mD3DCommandAllocator->Reset();
		mD3DCommandList->Reset(mD3DCommandAllocator.Get(), mD3DPipelineState.Get());
		*/

		PopulateCommandList();

		ID3D12CommandList* commandList[] = { mD3DCommandList.Get() };
		mD3DCommandQueue->ExecuteCommandLists(_countof(commandList), commandList);

		ThrowIfFailed(mDXGISwapChain3->Present(1, 0));

		WaitForPreviousFrame();
	}

	void ApplicationDX12::OnUpdate(const UpdateEventArgs& e)
	{

	}

	void ApplicationDX12::PopulateCommandList()
	{
		ThrowIfFailed(mD3DCommandAllocator->Reset());
		ThrowIfFailed(mD3DCommandList->Reset(mD3DCommandAllocator.Get(), nullptr));

		D3D12_RESOURCE_BARRIER rtvBarrier;
		rtvBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		rtvBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		rtvBarrier.Transition.pResource = mBackBuffers[mCurrentBackBufferIndex].Get();
		rtvBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		rtvBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		rtvBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		mD3DCommandList->ResourceBarrier(1, &rtvBarrier);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(mBackBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		rtvHandle.ptr = SIZE_T((INT64)rtvHandle.ptr + (INT64)(mDescriptorHeapIncrementSize * mCurrentBackBufferIndex));

		const float clearColor[] = { 0.0f , 0.2f, 0.4f, 1.0f };

		mD3DCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		D3D12_RESOURCE_BARRIER presentBarrier;
		presentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		presentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		presentBarrier.Transition.pResource = mBackBuffers[mCurrentBackBufferIndex].Get();
		presentBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		presentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		presentBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		mD3DCommandList->ResourceBarrier(1, &presentBarrier);

		ThrowIfFailed(mD3DCommandList->Close());
	}

	void ApplicationDX12::WaitForPreviousFrame()
	{
		UINT fenceValueToWait = mFenceValue;
		ThrowIfFailed(mD3DCommandQueue->Signal(mD3DFence.Get(), fenceValueToWait));
		++mFenceValue;

		UINT fenceCompletedValue = mD3DFence->GetCompletedValue();

		if (fenceCompletedValue < fenceValueToWait)
		{
			ThrowIfFailed(mD3DFence->SetEventOnCompletion(fenceValueToWait, mFanceEvent));
			WaitForSingleObject(mFanceEvent, INFINITY);
		}

		mCurrentBackBufferIndex = mDXGISwapChain3->GetCurrentBackBufferIndex();
	}
}
