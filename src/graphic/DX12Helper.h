#pragma once

#include <wrl.h>
#include "d3dx12.h"
#include "../utility/Image.h"
#include "../utility/Exception.h"

namespace Dash
{
	static void UpdateTextureRegion(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList,
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

    static void GenerateMipmap()
    {
    
    }
}