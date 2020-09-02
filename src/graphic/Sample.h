#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <d3d12sdklayers.h>
#include <wrl.h>

#include <string>

using Microsoft::WRL::ComPtr;

class DXSample
{
public:
    DXSample(UINT width, UINT height, std::wstring name);
    virtual ~DXSample();

    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnDestroy();

    // Samples override the event handlers to handle specific messages.
    virtual void OnKeyDown(UINT8 /*key*/) {}
    virtual void OnKeyUp(UINT8 /*key*/) {}

    // Accessors.
    UINT GetWidth() const { return m_width; }
    UINT GetHeight() const { return m_height; }
    const WCHAR* GetTitle() const { return m_title.c_str(); }

    void ParseCommandLineArgs(_In_reads_(argc) WCHAR* argv[], int argc);

protected:
    std::wstring GetAssetFullPath(LPCWSTR assetName);

    void GetHardwareAdapter(
        _In_ IDXGIFactory1* pFactory,
        _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
        bool requestHighPerformanceAdapter = false);

    void SetCustomWindowText(LPCWSTR text);

    // Viewport dimensions.
    UINT m_width;
    UINT m_height;
    float m_aspectRatio;

    // Adapter info.
    bool m_useWarpDevice;

private:
    // Root assets path.
    std::wstring m_assetsPath;

    // Window title.
    std::wstring m_title;



private:
    void PopulateCommandList();
    void WaitForPreviousFrame();

    void LoadPipeline();
    void LoadAssets();

//public:
    static const UINT BackBufferFrameCount = 2;
//
//private:
//
    Microsoft::WRL::ComPtr<IDXGIFactory4> mDXGIFactory;
    Microsoft::WRL::ComPtr<IDXGIAdapter1> mDXGIAdapter;
    Microsoft::WRL::ComPtr<ID3D12Device> mD3DDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> mDXGISwapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> mDXGISwapChain3;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mD3DCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mD3DCommandList;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mD3DCommandAllocator;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mBackBufferDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> mBackBuffers[BackBufferFrameCount];
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mD3DRootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> mD3DPipelineState;

    Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;

    D3D12_VIEWPORT mD3DViewport;
    D3D12_RECT mD3DScissorRect;

    Microsoft::WRL::ComPtr<ID3D12Fence> mD3DFence;
    UINT64 mFenceValue;
    HANDLE mFanceEvent = nullptr;

    UINT64 mCurrentBackBufferIndex;
    UINT64 mTotalFrame;

    UINT64 mDescriptorHeapIncrementSize;

    UINT mDXGIFactoryFlag;

    D3D12_VIEWPORT mViewPort;
    D3D12_RECT mScissorRect;

    static const UINT FrameCount = 2;

    // Pipeline objects.
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12PipelineState> m_pipelineState;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    UINT m_rtvDescriptorSize;

    // Synchronization objects.
    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValue;


};