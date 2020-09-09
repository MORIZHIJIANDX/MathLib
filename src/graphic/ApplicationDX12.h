#pragma once

#include "Application.h"

#include <wrl.h>
#include <shellapi.h>

//for d3d12
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <d3d12sdklayers.h>

//linker
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Dash
{
	class ApplicationDX12 : public Application
	{
	public:
		ApplicationDX12(size_t windowWidth = 720, size_t windowHeight = 480);
		virtual ~ApplicationDX12();

		void OnRender(const RenderEventArgs& e) override;
		void OnUpdate(const UpdateEventArgs& e) override;

	private:
        void PopulateCommandList(const RenderEventArgs& e);
        void WaitForPreviousFrame();

        void LoadPipeline();
        void LoadAssets();

        void GetHardwareAdapter(
            _In_ IDXGIFactory1* pFactory,
            _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
            bool requestHighPerformanceAdapter = false);

    public:
        static const UINT BackBufferFrameCount = 2;

    private:
        Microsoft::WRL::ComPtr<IDXGIFactory4> mDXGIFatory;
        Microsoft::WRL::ComPtr<IDXGISwapChain3> mSwapChain;

        Microsoft::WRL::ComPtr<ID3D12Device> mD3DDevice;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mD3DCommandQueue;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
        Microsoft::WRL::ComPtr<ID3D12Resource> mBackBuffers[BackBufferFrameCount];
        Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> mPipelineState;
        Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
        D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;

        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;
        Microsoft::WRL::ComPtr<ID3D12Fence> mFence;

        UINT mBackBufferIndex;
        UINT mDescriptorHandleIncrementSize;
        UINT mFenceValue;

        HANDLE mFenceEvent;

        D3D12_VIEWPORT mViewport;
        RECT mScissorRect;
	};

}