#pragma once

#include "Application.h"

#include <wrl.h>


#include <dxgi1_6.h>
#include <DirectXMath.h>

//for d3d12
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3dcompiler.h>
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
		ApplicationDX12();
		~ApplicationDX12();

		void OnRender(const RenderEventArgs& e) override;
		void OnUpdate(const UpdateEventArgs& e) override;

	public:
		static const uint8_t BackBufferFrameCount = 3u;

	private:

		Microsoft::WRL::ComPtr<IDXGIFactory5> mDXGIFactory;
		Microsoft::WRL::ComPtr<IDXGIAdapter1> mDXGIAdapter;
		Microsoft::WRL::ComPtr<ID3D12Device> mD3DDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mDXGISwapChain;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> mD3DCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandList> mD3DCommandList;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mD3DCommandAllocator;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mBackBufferDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> mBackBuffers[BackBufferFrameCount];
		Microsoft::WRL::ComPtr<ID3D12RootSignature> mD3DRootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> mD3DPipelineState;
		Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Fence> mD3DFence;

		D3D12_VIEWPORT mD3DViewport;
		D3D12_RECT mD3DScissorRect;
	};

}