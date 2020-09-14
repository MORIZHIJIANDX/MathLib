#pragma once

#include "Image.h"
#include <wincodec.h>

namespace Dash
{
	FORCEINLINE static void GetImageColor(FColor& color, const FVector2i& index, const FTexture* image, bool repeat = false);

	FORCEINLINE EDASH_FORMAT DXGIFormatToDashFormat(DXGI_FORMAT format);

	FORCEINLINE DXGI_FORMAT DashFormatToDXGIFormat(EDASH_FORMAT format);

	FORCEINLINE void SavePPMImage(const FTexture* image, const std::string& name);

	FORCEINLINE FTexture LoadPPMImage(const std::string& name);

	enum class EWIC_LOADER_FLAGS : uint32_t
	{
		WIC_LOADER_DEFAULT = 0,
		WIC_LOADER_FORCE_SRGB = 0x1,
		WIC_LOADER_IGNORE_SRGB = 0x2,
		WIC_LOADER_SRGB_DEFAULT = 0x4,
		WIC_LOADER_MIP_AUTOGEN = 0x8,
		WIC_LOADER_MIP_RESERVE = 0x10,
		WIC_LOADER_FIT_POW2 = 0x20,
		WIC_LOADER_MAKE_SQUARE = 0x40,
		WIC_LOADER_FORCE_RGBA32 = 0x80,
	};

	FORCEINLINE EWIC_LOADER_FLAGS operator&(const EWIC_LOADER_FLAGS& a, const EWIC_LOADER_FLAGS& b);

	FORCEINLINE EWIC_LOADER_FLAGS operator|(const EWIC_LOADER_FLAGS& a, const EWIC_LOADER_FLAGS& b);

	FORCEINLINE uint32_t CountMips(uint32_t width, uint32_t height) noexcept;

	FTexture LoadWICTexture(const std::wstring& fileName, EWIC_LOADER_FLAGS loadFlags = EWIC_LOADER_FLAGS::WIC_LOADER_DEFAULT);

	void ExportWICTexture(const std::wstring& fileName, const FTexture& texture, REFGUID guidContainerFormat = GUID_ContainerFormatPng, const GUID* targetFormat = nullptr, bool forceSRGB = false);
}