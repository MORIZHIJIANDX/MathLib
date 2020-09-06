#include "Sample.h"
#include "App.h"
#include "../utility/Exception.h"
#include "../utility/Assert.h"

#include <D3Dcompiler.h>
#include <DirectXMath.h>

//#include "../math/MathType.h"

#include "d3dx12.h"

#include <vector>
#include <algorithm>

using namespace Microsoft::WRL;

DXSample::DXSample(UINT width, UINT height, std::wstring name) :
    mWidth(width),
    mHeight(height),
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
    LoadPipeline();
    LoadAssets();
}

void DXSample::LoadPipeline()
{
    UINT dxgiFactoryFlag = 0;

#ifdef _DEBUG
    ComPtr<ID3D12Debug3> debugInterface;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
    {
        debugInterface->EnableDebugLayer();

        dxgiFactoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif // DEBUG

    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlag, IID_PPV_ARGS(&mDXGIFatory)));

    ComPtr<IDXGIAdapter1> dxgiAdapter;
    GetHardwareAdapter(mDXGIFatory.Get(), &dxgiAdapter, true);

    ThrowIfFailed(D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&mD3DDevice)));

    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    ThrowIfFailed(mD3DDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mD3DCommandQueue)));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = mWidth;
    swapChainDesc.Height = mHeight;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferCount = BackBufferFrameCount;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(mDXGIFatory->CreateSwapChainForHwnd(mD3DCommandQueue.Get(),
        Win32Application::GetHwnd(),
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain));

    ThrowIfFailed(swapChain.As(&mSwapChain));

    ThrowIfFailed(mDXGIFatory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

    mBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
        heapDesc.NumDescriptors = BackBufferFrameCount;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap)));

        mDescriptorHandleIncrementSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        for (UINT i = 0; i < BackBufferFrameCount; i++)
        {
            ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mBackBuffers[i])));
            mD3DDevice->CreateRenderTargetView(mBackBuffers[i].Get(), nullptr, descriptorHandle);
            descriptorHandle.Offset(1, mDescriptorHandleIncrementSize);
        }
    }

    ThrowIfFailed(mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)));
}

void DXSample::LoadAssets()
{
    CD3DX12_ROOT_SIGNATURE_DESC rootSignature;
    rootSignature.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> blob;
    ComPtr<ID3DBlob> errorBlob;
    ThrowIfFailed(D3D12SerializeRootSignature(&rootSignature, D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &errorBlob));
    ThrowIfFailed(mD3DDevice->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));

    {
        ComPtr<ID3DBlob> vertexShader;
        ComPtr<ID3DBlob> pixelShader;

        UINT shaderCompileFlag = 0;

#ifdef _DEBUG
        shaderCompileFlag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG

        ThrowIfFailed(D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", shaderCompileFlag, 0, &vertexShader, nullptr));
        ThrowIfFailed(D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", shaderCompileFlag, 0, &pixelShader, nullptr));

        D3D12_INPUT_ELEMENT_DESC inputElements[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
        };

        D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{};
        pipelineStateDesc.pRootSignature = mRootSignature.Get();
        pipelineStateDesc.InputLayout = { inputElements , _countof(inputElements)};
        pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE{vertexShader.Get()};
        pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE{ pixelShader.Get() };
        pipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC{D3D12_DEFAULT};
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
    ThrowIfFailed(mCommandList->Close());

    {
        struct Vertex
        {
            DirectX::XMFLOAT3 Pos;
            DirectX::XMFLOAT4 Color;
        };

        Vertex triangle[] =
        {
            { {0.0f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },
            { {0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} },
            { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },
        };

        const UINT vertexBufferSize = sizeof(triangle);

        mD3DDevice->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES{ D3D12_HEAP_TYPE_UPLOAD },
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&mVertexBuffer));

        UINT8* vertexDataBegin;
        CD3DX12_RANGE readRange{0, 0};
        mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&vertexDataBegin));
        memcpy(vertexDataBegin, triangle, vertexBufferSize);
        mVertexBuffer->Unmap(0, nullptr);

        mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
        mVertexBufferView.SizeInBytes = vertexBufferSize;
        mVertexBufferView.StrideInBytes = sizeof(Vertex);
    };


    ThrowIfFailed(mD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
    mFenceValue = 1;

    mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (mFenceEvent == nullptr)
    {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }

    {
        mViewport.Width = mWidth;
        mViewport.Height = mHeight;
        mViewport.TopLeftX = 0.0f;
        mViewport.TopLeftY = 0.0f;
        mViewport.MinDepth = 0.0f;
        mViewport.MaxDepth = 1.0f;

        mScissorRect.left = 0.0f;
        mScissorRect.right = mWidth;
        mScissorRect.top = 0.0f;
        mScissorRect.bottom = mHeight;
    }
}

void DXSample::PopulateCommandList()
{
    ThrowIfFailed(mCommandAllocator->Reset());
    ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), mPipelineState.Get()));

    mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
    mCommandList->RSSetViewports(0, &mViewport);
    mCommandList->RSSetScissorRects(0, &mScissorRect);

    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBackBuffers[mBackBufferIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET));

    CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), mBackBufferIndex, mDescriptorHandleIncrementSize);

    mCommandList->OMSetRenderTargets(1, &descriptorHandle, FALSE, nullptr);

    const float clearColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
    mCommandList->ClearRenderTargetView(descriptorHandle, clearColor, 0, nullptr);
    mCommandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
    mCommandList->DrawInstanced(3, 1, 0, 0);

    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBackBuffers[mBackBufferIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT));

    ThrowIfFailed(mCommandList->Close());
}

void DXSample::WaitForPreviousFrame()
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

void DXSample::OnUpdate()
{
}

void DXSample::OnRender()
{
    PopulateCommandList();

    ID3D12CommandList* commands[] = { mCommandList.Get() };
    mD3DCommandQueue->ExecuteCommandLists(_countof(commands), commands);

    ThrowIfFailed(mSwapChain->Present(1, 0));

    WaitForPreviousFrame();
}

void DXSample::OnDestroy()
{
    WaitForPreviousFrame();

    CloseHandle(mFenceEvent);
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
