#include "ApplicationDX12.h"
#include "../utility/Assert.h"
#include "../math/MathType.h"
#include "../utility/Exception.h"
#include "d3dx12.h"
#include "../utility/ImageHelper.h"
#include <DirectXMath.h>
#include <algorithm>

namespace Dash
{
	FApplicationDX12::FApplicationDX12(size_t windowWidth, size_t windowHeight)
		: FApplication(windowWidth, windowHeight)
	{
        {
            mViewport.Width = (FLOAT)mWindow.GetWindowWidth();
            mViewport.Height = (FLOAT)mWindow.GetWindowHeight();
            mViewport.TopLeftX = 0.0f;
            mViewport.TopLeftY = 0.0f;
            mViewport.MinDepth = 0.0f;
            mViewport.MaxDepth = 1.0f;

            mScissorRect.left = 0;
            mScissorRect.right = (UINT)mWindow.GetWindowWidth();
            mScissorRect.top = 0;
            mScissorRect.bottom = (UINT)mWindow.GetWindowHeight();
        }

        LoadPipeline();
        LoadAssets();
	}

	FApplicationDX12::~FApplicationDX12()
	{
        WaitForPreviousFrame();

        CloseHandle(mFenceEvent);
	}

	void FApplicationDX12::OnRender(const FRenderEventArgs& e)
	{
        PopulateCommandList(e);

        ID3D12CommandList* commands[] = { mCommandList.Get() };
        mD3DCommandQueue->ExecuteCommandLists(_countof(commands), commands);

        ThrowIfFailed(mSwapChain->Present(1, 0));

        WaitForPreviousFrame();
	}

	void FApplicationDX12::OnUpdate(const FUpdateEventArgs& e)
	{

	}

	void FApplicationDX12::PopulateCommandList(const FRenderEventArgs& e)
	{
        ThrowIfFailed(mCommandAllocator->Reset());
        ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), mPipelineState.Get()));

        mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

        ID3D12DescriptorHeap* heaps[] = { mSRVDescriptorHeap.Get() };
        mCommandList->SetDescriptorHeaps(_countof(heaps), heaps);

        mCommandList->SetGraphicsRootDescriptorTable(0, mSRVDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

        mCommandList->RSSetViewports(1, &mViewport);
        mCommandList->RSSetScissorRects(1, &mScissorRect);

        mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBackBuffers[mBackBufferIndex].Get(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET));

        CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(mRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), mBackBufferIndex, mRTVDescriptorSize);

        mCommandList->OMSetRenderTargets(1, &descriptorHandle, FALSE, nullptr);

        Dash::FVector4f clearColor{ Dash::FMath::Sin((float)e.mTotalTime) * 0.5f + 0.5f, Dash::FMath::Cos((float)e.mTotalTime + 0.5f) * 0.5f + 0.5f, 0.5f, 1.0f };
        mCommandList->ClearRenderTargetView(descriptorHandle, clearColor, 0, nullptr);

        mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
        mCommandList->DrawInstanced(4, 1, 0, 0);

        mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBackBuffers[mBackBufferIndex].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT));

        ThrowIfFailed(mCommandList->Close());
	}

	void FApplicationDX12::WaitForPreviousFrame()
	{
        UINT valueToWait = mFenceValue;
        ThrowIfFailed(mD3DCommandQueue->Signal(mFence.Get(), valueToWait));
        ++mFenceValue;

        if (mFence->GetCompletedValue() < valueToWait)
        {
            ThrowIfFailed(mFence->SetEventOnCompletion(valueToWait, mFenceEvent));
            WaitForSingleObject(mFenceEvent, INFINITE);
        }

        mBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();
	}

	void FApplicationDX12::LoadPipeline()
	{
        UINT dxgiFactoryFlag = 0;

#ifdef _DEBUG
        Microsoft::WRL::ComPtr<ID3D12Debug3> debugInterface;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
        {
            debugInterface->EnableDebugLayer();

            dxgiFactoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
        }
#endif // DEBUG

        ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlag, IID_PPV_ARGS(&mDXGIFatory)));

        Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter;
        GetHardwareAdapter(mDXGIFatory.Get(), &dxgiAdapter, true);

        ThrowIfFailed(D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mD3DDevice)));

        D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
        commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        ThrowIfFailed(mD3DDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mD3DCommandQueue)));

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
        swapChainDesc.Width = (UINT)mWindow.GetWindowWidth();
        swapChainDesc.Height = (UINT)mWindow.GetWindowHeight();
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.BufferCount = BackBufferFrameCount;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
        ThrowIfFailed(mDXGIFatory->CreateSwapChainForHwnd(mD3DCommandQueue.Get(),
            mWindow.GetWindowHandle(),
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain));

        ThrowIfFailed(swapChain.As(&mSwapChain));

        ThrowIfFailed(mDXGIFatory->MakeWindowAssociation(mWindow.GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER));

        mBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

        {
            //Create rtv heap
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
            rtvHeapDesc.NumDescriptors = BackBufferFrameCount;
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRTVDescriptorHeap)));

            //Create srv heap
            D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
            srvHeapDesc.NumDescriptors = 1;
            srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

            ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSRVDescriptorHeap)));

            mRTVDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }

        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(mRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
            for (UINT i = 0; i < BackBufferFrameCount; i++)
            {
                ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mBackBuffers[i])));
                mD3DDevice->CreateRenderTargetView(mBackBuffers[i].Get(), nullptr, descriptorHandle);
                descriptorHandle.Offset(1, mRTVDescriptorSize);
            }
        }

        ThrowIfFailed(mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)));
	}

	void FApplicationDX12::LoadAssets()
	{
        D3D12_FEATURE_DATA_ROOT_SIGNATURE signatureFeatureData{};

        signatureFeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
        if (FAILED(mD3DDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &signatureFeatureData, sizeof(signatureFeatureData))))
        {
            signatureFeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        }

        CD3DX12_DESCRIPTOR_RANGE1 srvRanges[1];
        srvRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAGS::D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

        CD3DX12_ROOT_PARAMETER1 srvParameter[1];
        srvParameter[0].InitAsDescriptorTable(1, &srvRanges[0], D3D12_SHADER_VISIBILITY_PIXEL);

        D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
        samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        samplerDesc.MipLODBias = 0;
        samplerDesc.MaxAnisotropy = 0;
        samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        samplerDesc.MinLOD = 0.0f;
        samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
        samplerDesc.ShaderRegister = 0;
        samplerDesc.RegisterSpace = 0;
        samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Init_1_1(_countof(srvParameter), srvParameter, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        Microsoft::WRL::ComPtr<ID3DBlob> blob;
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
        ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, signatureFeatureData.HighestVersion, &blob, &errorBlob));
        ThrowIfFailed(mD3DDevice->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));

        {
            Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
            Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

            UINT shaderCompileFlag = 0;

#ifdef _DEBUG
            shaderCompileFlag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG

            ThrowIfFailed(D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", shaderCompileFlag, 0, &vertexShader, nullptr));
            ThrowIfFailed(D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", shaderCompileFlag, 0, &pixelShader, nullptr));

            D3D12_INPUT_ELEMENT_DESC inputElements[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
            };

            D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};
            pipelineStateDesc.pRootSignature = mRootSignature.Get();
            pipelineStateDesc.InputLayout = { inputElements , _countof(inputElements) };
            pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE{ vertexShader.Get() };
            pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE{ pixelShader.Get() };
            pipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC{ D3D12_DEFAULT };
            pipelineStateDesc.BlendState = CD3DX12_BLEND_DESC{ D3D12_DEFAULT };
            pipelineStateDesc.DepthStencilState.StencilEnable = FALSE;
            pipelineStateDesc.DepthStencilState.DepthEnable = FALSE;
            pipelineStateDesc.SampleMask = UINT_MAX;
            pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            pipelineStateDesc.NumRenderTargets = 1;
            pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
            pipelineStateDesc.SampleDesc.Count = 1;
            ThrowIfFailed(mD3DDevice->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&mPipelineState)));
        }

        ThrowIfFailed(mD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), mPipelineState.Get(), IID_PPV_ARGS(&mCommandList)));

        //Create Vertex Buffer
        {
            struct Vertex
            {
                Dash::FVector3f Pos;
                Dash::FVector2f Color;
            };

            Vertex triangle[] =
            {
                { Dash::FVector3f{ -0.5f,  -0.5f , 0.0f }, Dash::FVector2f{ 0.0f, 1.0f } },
                { Dash::FVector3f{ -0.5f,   0.5f , 0.0f }, Dash::FVector2f{ 0.0f, 0.0f } },
                { Dash::FVector3f{  0.5f,  -0.5f , 0.0f }, Dash::FVector2f{ 1.0f, 1.0f } },
                { Dash::FVector3f{  0.5f,   0.5f , 0.0f }, Dash::FVector2f{ 1.0f, 0.0f } }
            };

            const UINT vertexBufferSize = sizeof(triangle);

            ThrowIfFailed(mD3DDevice->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD },
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&mVertexBuffer)));

            UINT8* vertexDataBegin;
            CD3DX12_RANGE readRange{ 0, 0 };
            mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&vertexDataBegin));
            memcpy(vertexDataBegin, triangle, vertexBufferSize);
            mVertexBuffer->Unmap(0, nullptr);

            mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
            mVertexBufferView.SizeInBytes = vertexBufferSize;
            mVertexBufferView.StrideInBytes = sizeof(Vertex);
        };
        

        Microsoft::WRL::ComPtr<ID3D12Resource> uploadTextureBuffer;

        //Create Texture
        {
            
            Dash::FTexture texture = LoadWICTexture(L"coma.png");

            D3D12_RESOURCE_DESC resourceDesc{};
            resourceDesc.MipLevels = 1;
            resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            resourceDesc.Width = texture.GetWidth();
            resourceDesc.Height = texture.GetHeight();;
            resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
            resourceDesc.DepthOrArraySize = 1;
            resourceDesc.SampleDesc.Count = 1;
            resourceDesc.SampleDesc.Quality = 0;
            resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

            ThrowIfFailed(mD3DDevice->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT),
                D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &resourceDesc,
                D3D12_RESOURCE_STATE_COPY_DEST,
                nullptr,
                IID_PPV_ARGS(&mTextureResource)
            ));

            UINT uploadBufferSize = GetRequiredIntermediateSize(mTextureResource.Get(), 0, 1);

            //Microsoft::WRL::ComPtr<ID3D12Resource> uploadTextureBuffer;

            ThrowIfFailed(mD3DDevice->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&uploadTextureBuffer)
            ));

            
            UINT subresourceIndex = 0;
            UINT subresourceCount = 1;
            UINT numRows = 0;
            UINT64 rowSizeInByte = 0;
            UINT64 totalByte = 0;
            D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;

            mD3DDevice->GetCopyableFootprints(
                &(mTextureResource->GetDesc()),
                subresourceIndex,
                subresourceCount,
                0,
                &footprint,
                &numRows,
                &rowSizeInByte,
                &totalByte
            );

            BYTE* resourceData = nullptr;

            ThrowIfFailed(uploadTextureBuffer->Map(0, nullptr, reinterpret_cast<void**>(&resourceData)));

            size_t textureRowPitch = texture.GetRowPitch();
            uint8_t* textureData = texture.GetRawData();

            BYTE* resourceCopyStart = resourceData + footprint.Offset;
            for (size_t rowIdx = 0; rowIdx < numRows; rowIdx++)
            {
                memcpy(resourceCopyStart + rowIdx * footprint.Footprint.RowPitch, 
                    textureData + rowIdx * textureRowPitch,
                    textureRowPitch);
            }

            uploadTextureBuffer->Unmap(0, nullptr);

            CD3DX12_TEXTURE_COPY_LOCATION copyDst{ mTextureResource.Get(), 0};
            CD3DX12_TEXTURE_COPY_LOCATION copySrc{ uploadTextureBuffer.Get(), footprint };

            mCommandList->CopyTextureRegion(&copyDst, 0, 0, 0, &copySrc, nullptr);

            

            /*
            // Copy data to the intermediate upload heap and then schedule a copy 
            // from the upload heap to the Texture2D.

            D3D12_RESOURCE_DESC resourceDesc{};
            resourceDesc.MipLevels = 1;
            resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            resourceDesc.Width = TextureWidth;
            resourceDesc.Height = TextureHeight;
            resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
            resourceDesc.DepthOrArraySize = 1;
            resourceDesc.SampleDesc.Count = 1;
            resourceDesc.SampleDesc.Quality = 0;
            resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

            ThrowIfFailed(mD3DDevice->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT),
                D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &resourceDesc,
                D3D12_RESOURCE_STATE_COPY_DEST,
                nullptr,
                IID_PPV_ARGS(&mTextureResource)
            ));

            UINT uploadBufferSize = GetRequiredIntermediateSize(mTextureResource.Get(), 0, 1);

            ThrowIfFailed(mD3DDevice->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&uploadTextureBuffer)
            ));


            std::vector<UINT8> tempTexture = GenerateTextureData();

            D3D12_SUBRESOURCE_DATA textureData = {};
            textureData.pData = &tempTexture[0];
            textureData.RowPitch = TextureWidth * TexturePixelSize;
            textureData.SlicePitch = textureData.RowPitch * TextureHeight;

            UpdateSubresources(mCommandList.Get(), mTextureResource.Get(), uploadTextureBuffer.Get(), 0, 0, 1, &textureData);
            */
            mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mTextureResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

            //Create shader resource view
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.Format = resourceDesc.Format;
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;

            mD3DDevice->CreateShaderResourceView(mTextureResource.Get(), &srvDesc, mSRVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        }

        ThrowIfFailed(mCommandList->Close());
        ID3D12CommandList* commands[] = { mCommandList.Get() };
        mD3DCommandQueue->ExecuteCommandLists(_countof(commands), commands);

        ThrowIfFailed(mD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
        mFenceValue = 1;

        mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

        if (mFenceEvent == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }

        WaitForPreviousFrame();
	}

    void FApplicationDX12::GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter)
    {
        *ppAdapter = nullptr;

        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;

        Microsoft::WRL::ComPtr<IDXGIFactory6> factory6;
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

    std::vector<UINT8> FApplicationDX12::GenerateTextureData()
    {
        const UINT rowPitch = TextureWidth * TexturePixelSize;
        const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
        const UINT cellHeight = TextureWidth >> 3;    // The height of a cell in the checkerboard texture.
        const UINT textureSize = rowPitch * TextureHeight;

        std::vector<UINT8> data(textureSize);
        UINT8* pData = &data[0];

        for (UINT n = 0; n < textureSize; n += TexturePixelSize)
        {
            UINT x = n % rowPitch;
            UINT y = n / rowPitch;
            UINT i = x / cellPitch;
            UINT j = y / cellHeight;

            if (i % 2 == j % 2)
            {
                pData[n] = 0x00;        // R
                pData[n + 1] = 0x00;    // G
                pData[n + 2] = 0x00;    // B
                pData[n + 3] = 0xff;    // A
            }
            else
            {
                pData[n] = 0xff;        // R
                pData[n + 1] = 0xff;    // G
                pData[n + 2] = 0xff;    // B
                pData[n + 3] = 0xff;    // A
            }
        }

        return data;
    }
}
