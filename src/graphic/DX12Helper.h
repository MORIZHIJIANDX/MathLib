#pragma once

#include <wrl.h>
#include "d3dx12.h"
#include "../utility/Image.h"
#include "../utility/Exception.h"
#include "../design_patterns/Singleton.h"

/*
Create Mips
init : 创建 PSO, root signature
being : 创建command allocator, 创建command list
end : 传入 command queue, execute command list
释放  command list, command allocator
*/

namespace Dash
{
    FORCEINLINE uint32_t CountMips(uint32_t width, uint32_t height) noexcept
    {
        if (width == 0 || height == 0)
            return 0;

        uint32_t count = 1;
        while (width > 1 || height > 1)
        {
            width >>= 1;
            height >>= 1;
            count++;
        }
        return count;
    }

    // Helper sets a D3D resource name string (used by PIX and debug layer leak reporting).
    template<UINT TNameLength>
    FORCEINLINE void SetDebugObjectName(_In_ ID3D12DeviceChild* resource, _In_z_ const char(&name)[TNameLength]) noexcept
    {
#if !defined(NO_D3D12_DEBUG_NAME) && (defined(_DEBUG) || defined(PROFILE))
        wchar_t wname[MAX_PATH];
        int result = MultiByteToWideChar(CP_UTF8, 0, name, TNameLength, wname, MAX_PATH);
        if (result > 0)
        {
            resource->SetName(wname);
        }
#else
        UNREFERENCED_PARAMETER(resource);
        UNREFERENCED_PARAMETER(name);
#endif
    }

    template<UINT TNameLength>
    FORCEINLINE void SetDebugObjectName(_In_ ID3D12DeviceChild* resource, _In_z_ const wchar_t(&name)[TNameLength]) noexcept
    {
#if !defined(NO_D3D12_DEBUG_NAME) && (defined(_DEBUG) || defined(PROFILE))
        resource->SetName(name);
#else
        UNREFERENCED_PARAMETER(resource);
        UNREFERENCED_PARAMETER(name);
#endif
    }

    // Shorthand for creating a root signature
    FORCEINLINE HRESULT CreateRootSignature(
        Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const D3D12_ROOT_SIGNATURE_DESC* rootSignatureDesc,
        Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature,
        UINT nodeMask = 0) noexcept
    {
        Microsoft::WRL::ComPtr<ID3DBlob> pSignature;
        Microsoft::WRL::ComPtr<ID3DBlob> pError;
        HRESULT hr = D3D12SerializeRootSignature(rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, pSignature.GetAddressOf(), pError.GetAddressOf());
        if (SUCCEEDED(hr))
        {
            hr = device->CreateRootSignature(nodeMask, pSignature->GetBufferPointer(), pSignature->GetBufferSize(),
                IID_PPV_ARGS(&rootSignature)
            );
        }
        return hr;
    }

	static void UpdateTextureRegion(Microsoft::WRL::ComPtr<ID3D12Device>& device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
        Microsoft::WRL::ComPtr<ID3D12Resource>& destTexture, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer,
		const FTexture& texture, UINT destSubresourceIndex = 0)
	{
		D3D12_COMMAND_LIST_TYPE commandListType = commandList->GetType();
		ASSERT((commandListType == D3D12_COMMAND_LIST_TYPE_DIRECT) || (commandListType == D3D12_COMMAND_LIST_TYPE_COPY));

        UINT subresourceIndex = destSubresourceIndex;
        UINT subresourceCount = 1;
        UINT numRows = 0;
        UINT64 rowSizeInByte = 0;
        UINT64 totalByte = 0;
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;

        device->GetCopyableFootprints(
            &(destTexture->GetDesc()),
            subresourceIndex,
            subresourceCount,
            0,
            &footprint,
            &numRows,
            &rowSizeInByte,
            &totalByte
        );

        BYTE* resourceData = nullptr;

        HR(uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&resourceData)));

        size_t textureRowPitch = texture.GetRowPitch();
        const uint8_t* textureData = texture.GetRawData();

        ASSERT(footprint.Footprint.RowPitch == textureRowPitch);
        ASSERT(numRows == texture.GetHeight());
        ASSERT(footprint.Footprint.Width == texture.GetWidth());

        BYTE* resourceCopyStart = resourceData + footprint.Offset;
        for (size_t rowIdx = 0; rowIdx < numRows; rowIdx++)
        {
            memcpy(resourceCopyStart + rowIdx * footprint.Footprint.RowPitch,
                textureData + rowIdx * textureRowPitch,
                textureRowPitch);
        }

        uploadBuffer->Unmap(0, nullptr);

        CD3DX12_TEXTURE_COPY_LOCATION copyDst{ destTexture.Get(), 0 };
        CD3DX12_TEXTURE_COPY_LOCATION copySrc{ uploadBuffer.Get(), footprint };

        commandList->CopyTextureRegion(&copyDst, 0, 0, 0, &copySrc, nullptr);

        commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(destTexture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}

    struct FGenerateMips : public TSingleton<FGenerateMips>
    {
#pragma pack(push, 4)
        struct ConstantData
        {
            FVector2f InvOutTexelSize;
            uint32_t SrcMipIndex = 0;
        };
#pragma pack(pop)

        static const uint32_t Num32BitConstants = static_cast<uint32_t>(sizeof(ConstantData) / sizeof(uint32_t));
        static const uint32_t ThreadGroupSize = 8;

        enum RootParameterIndex
        {
            Constants,
            SourceTexture,
            TargetTexture,
            RootParameterCount
        };

        void Init(Microsoft::WRL::ComPtr<ID3D12Device>& device);

        void GenerateMipmap(Microsoft::WRL::ComPtr<ID3D12Device>& device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
            Microsoft::WRL::ComPtr<ID3D12Resource>& resource);
    
    private:
        Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateGenMipsRootSignature(
            Microsoft::WRL::ComPtr<ID3D12Device>& device);

        Microsoft::WRL::ComPtr<ID3D12PipelineState> CreateGenMipsPipelineState(
            Microsoft::WRL::ComPtr<ID3D12Device>& device,
            Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature,
            const void* bytecode,
            size_t bytecodeSize);

        void GenerateMips_UnorderedAccessPath(Microsoft::WRL::ComPtr<ID3D12Device>& device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
            Microsoft::WRL::ComPtr<ID3D12Resource>& resource, Microsoft::WRL::ComPtr<ID3D12PipelineState>& pso, Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature);
    };

    void GenerateMipmap(Microsoft::WRL::ComPtr<ID3D12Device>& device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
        Microsoft::WRL::ComPtr<ID3D12Resource>& resource);
}