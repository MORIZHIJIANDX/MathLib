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
