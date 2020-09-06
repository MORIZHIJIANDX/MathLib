#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <d3d12sdklayers.h>
#include <wrl.h>

#include <string>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

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
    UINT GetWidth() const { return mWidth; }
    UINT GetHeight() const { return mHeight; }
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
    UINT mWidth;
    UINT mHeight;
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

public:
    static const UINT BackBufferFrameCount = 2;

private:
    ComPtr<IDXGIFactory4> mDXGIFatory;
    ComPtr<IDXGISwapChain3> mSwapChain;

    ComPtr<ID3D12Device> mD3DDevice;
    ComPtr<ID3D12CommandQueue> mD3DCommandQueue;
    ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
    ComPtr<ID3D12Resource> mBackBuffers[BackBufferFrameCount];
    ComPtr<ID3D12RootSignature> mRootSignature;
    ComPtr<ID3D12PipelineState> mPipelineState;
    ComPtr<ID3D12Resource> mVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;

    ComPtr<ID3D12CommandAllocator> mCommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> mCommandList;
    ComPtr<ID3D12Fence> mFence;

    UINT mBackBufferIndex;
    UINT mDescriptorHandleIncrementSize;
    UINT mFenceValue;

    HANDLE mFenceEvent;

    D3D12_VIEWPORT mViewport;
    RECT mScissorRect;
};