#pragma once

#include "Image.h"
#include <wincodec.h>

namespace Dash
{
	//Helper Function

	//template<typename T>
	//FORCEINLINE static FColor GetColorFromType(const T& pixel, bool repeat)
	//{
	//	ASSERT_FAIL("Not supported type!");
	//	return FColor{};
	//}

	//template<>
	//FORCEINLINE static FColor GetColorFromType<uint8_t>(const uint8_t& pixel, bool repeat)
	//{
	//	return repeat ? FColor{ pixel, pixel, pixel, 255 } : FColor{ pixel, 0, 0, 255 };
	//}

	//template<>
	//FORCEINLINE static FColor GetColorFromType<float>(const float& pixel, bool repeat)
	//{
	//	//const uint8_t value{ static_cast<uint8_t>(pixel * 255.0f + 0.5f) };
	//	
	//	const FLinearColor linearColor = repeat ? FLinearColor{ pixel, pixel, pixel, 1.0f } : FLinearColor{ pixel, 0, 0, 1.0f };
	//	return linearColor.ToFColor(true);
	//}

	//template<>
	//FORCEINLINE static FColor GetColorFromType<TVector4<uint8_t>>(const TVector4<uint8_t>& pixel, bool repeat)
	//{
	//	return FColor{ pixel };
	//}

	//template<>
	//FORCEINLINE static FColor GetColorFromType<FVector4f>(const FVector4f& pixel, bool repeat)
	//{
	//	const FLinearColor linearColor{ pixel };
	//	return linearColor.ToFColor(true);
	//}

	//template<>
	//FORCEINLINE static FColor GetColorFromType<FColor>(const FColor& pixel, bool repeat)
	//{
	//	return pixel;
	//}

	//template<>
	//FORCEINLINE static FColor GetColorFromType<FLinearColor>(const FLinearColor& pixel, bool repeat)
	//{
	//	return pixel.ToFColor(true);
	//}

	FORCEINLINE static void GetImageColor(FColor& color, const FVector2i& index, const FTexture* image, bool repeat = false);

	FORCEINLINE void SavePPMImage(const FTexture* image, const std::string& name);

	FORCEINLINE FTexture LoadPPMImage(const std::string& name);


	struct WICTranslate 
	{
		GUID Wic;
		EDASH_FORMAT Format;
	};

	static WICTranslate g_WICFormats[] =
	{//WIC格式与DXGI像素格式的对应表，该表中的格式为被支持的格式
		{ GUID_WICPixelFormat128bppRGBAFloat,	         EDASH_FORMAT::R32G32B32A32_FLOAT },

		{ GUID_WICPixelFormat128bppRGBAFloat,			 EDASH_FORMAT::R32_FLOAT },

		{ GUID_WICPixelFormat32bppRGBA,					 EDASH_FORMAT::R8G8B8A8_UNORM },
		{ GUID_WICPixelFormat32bppBGRA,					 EDASH_FORMAT::B8G8R8A8_UNORM }, // DXGI 1.1

		{ GUID_WICPixelFormat32bppRGBA1010102,			 EDASH_FORMAT::R10G10B10A2_UNORM },

		{ GUID_WICPixelFormat32bppGrayFloat,			 EDASH_FORMAT::R32_FLOAT },
		{ GUID_WICPixelFormat16bppGrayHalf,				 EDASH_FORMAT::R16_FLOAT },
		{ GUID_WICPixelFormat16bppGray,					 EDASH_FORMAT::R16_UNORM },
		{ GUID_WICPixelFormat8bppGray,					 EDASH_FORMAT::R8_UNORM },

		//{ GUID_WICPixelFormat32bppBGR,				     static_cast<uint16_t>(EDASH_FORMAT::R8G8B8A8_UNORM)}, // DXGI 1.1

		//{ GUID_WICPixelFormat64bppRGBAHalf,         DXGI_FORMAT_R16G16B16A16_FLOAT },
		//{ GUID_WICPixelFormat64bppRGBA,             DXGI_FORMAT_R16G16B16A16_UNORM },

		//{ GUID_WICPixelFormat32bppRGBA,             DXGI_FORMAT_R8G8B8A8_UNORM },
		//{ GUID_WICPixelFormat32bppBGRA,             DXGI_FORMAT_B8G8R8A8_UNORM }, // DXGI 1.1
		//{ GUID_WICPixelFormat32bppBGR,              DXGI_FORMAT_B8G8R8X8_UNORM }, // DXGI 1.1

		//{ GUID_WICPixelFormat32bppRGBA1010102XR,    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM }, // DXGI 1.1
		//{ GUID_WICPixelFormat32bppRGBA1010102,      DXGI_FORMAT_R10G10B10A2_UNORM },

		//{ GUID_WICPixelFormat16bppBGRA5551,         DXGI_FORMAT_B5G5R5A1_UNORM },
		//{ GUID_WICPixelFormat16bppBGR565,           DXGI_FORMAT_B5G6R5_UNORM },

		//{ GUID_WICPixelFormat32bppGrayFloat,        DXGI_FORMAT_R32_FLOAT },
		//{ GUID_WICPixelFormat16bppGrayHalf,         DXGI_FORMAT_R16_FLOAT },
		//{ GUID_WICPixelFormat16bppGray,             DXGI_FORMAT_R16_UNORM },
		//{ GUID_WICPixelFormat8bppGray,              DXGI_FORMAT_R8_UNORM },

		//{ GUID_WICPixelFormat8bppAlpha,             DXGI_FORMAT_A8_UNORM },
	};

	// WIC 像素格式转换表.
	struct WICConvert
	{
		GUID Source;
		GUID Target;
	};
	
	static WICConvert g_WICConvert[] =
	{
		// 目标格式一定是最接近的被支持的格式
		{ GUID_WICPixelFormatBlackWhite,            GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM

		{ GUID_WICPixelFormat1bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat2bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat4bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat8bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM

		{ GUID_WICPixelFormat2bppGray,              GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM
		{ GUID_WICPixelFormat4bppGray,              GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM

		{ GUID_WICPixelFormat16bppGrayFixedPoint,   GUID_WICPixelFormat16bppGrayHalf }, // DXGI_FORMAT_R16_FLOAT
		{ GUID_WICPixelFormat32bppGrayFixedPoint,   GUID_WICPixelFormat32bppGrayFloat }, // DXGI_FORMAT_R32_FLOAT

		{ GUID_WICPixelFormat16bppBGR555,           GUID_WICPixelFormat16bppBGRA5551 }, // DXGI_FORMAT_B5G5R5A1_UNORM

		{ GUID_WICPixelFormat32bppBGR101010,        GUID_WICPixelFormat32bppRGBA1010102 }, // DXGI_FORMAT_R10G10B10A2_UNORM

		{ GUID_WICPixelFormat24bppBGR,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat24bppRGB,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat32bppPBGRA,            GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat32bppPRGBA,            GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM

		{ GUID_WICPixelFormat48bppRGB,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat48bppBGR,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppBGRA,             GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppPRGBA,            GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppPBGRA,            GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

		{ GUID_WICPixelFormat48bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat48bppBGRFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat64bppRGBAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat64bppBGRAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat64bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat48bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
		{ GUID_WICPixelFormat64bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT

		{ GUID_WICPixelFormat128bppPRGBAFloat,      GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat128bppRGBFloat,        GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat128bppRGBAFixedPoint,  GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat128bppRGBFixedPoint,   GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT
		{ GUID_WICPixelFormat32bppRGBE,             GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT

		{ GUID_WICPixelFormat32bppCMYK,             GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat64bppCMYK,             GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat40bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat80bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

		{ GUID_WICPixelFormat32bppRGB,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
		{ GUID_WICPixelFormat64bppRGB,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
		{ GUID_WICPixelFormat64bppPRGBAHalf,        GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
	};

	bool GetTargetPixelFormat(const GUID* pSourceFormat, GUID* pTargetFormat);

	EDASH_FORMAT GetDXGIFormatFromPixelFormat(const GUID* pPixelFormat);

	FTexture ReadWICTexture(const std::wstring& fileName);

	/*

	FORCEINLINE void GetImageColor(FVector3f& color, const FVector2i& index, std::shared_ptr<Image> image, bool repeat = false)
	{
		switch (image->GetFormat())
		{
		case EDASH_FORMAT::R32_FLOAT:
		{
			float temp = image->GetPixel<Scalar>(index);
			if (repeat)
			{
				color.x = color.y = color.z = temp * 255;
			}
			else
			{
				color.x = temp * 255;
			}
			return;
		}
		break;
		case EDASH_FORMAT::R32G32_FLOAT:
		{
			FVector2f temp = image->GetPixel<FVector2f>(index);
			if (repeat)
			{
				color.x = temp.x * 255;
				color.y = temp.y * 255;
				color.z = temp.y * 255;
			}
			else
			{
				color.x = temp.x * 255;
				color.y = temp.y * 255;
			}
			return;
		}
		break;
		case EDASH_FORMAT::R32G32B32_FLOAT:
		{
			Dash::FVector3f temp = image->GetPixel<Dash::FVector3f>(index);

			color.x = temp.x * 255;
			color.y = temp.y * 255;
			color.z = temp.z * 255;

			return;
		}
		break;
		case EDASH_FORMAT::R32G32B32A32_FLOAT:
		{
			FVector4f temp = image->GetPixel<FVector4f>(index);

			color.x = temp.x * 255;
			color.y = temp.y * 255;
			color.z = temp.z * 255;

			return;
		}
		break;
		case EDASH_FORMAT::R8G8B8A8_UINT:
		{
			TVector4<uint8_t> temp = image->GetPixel<TVector4<uint8_t>>(index.x, image->GetHeight() - 1 - index.y);

			color.x = temp.z;
			color.y = temp.y;
			color.z = temp.x;

			return;
		}
		break;
		default:
			break;
		}
	}

	FORCEINLINE void SavePPMImage(std::shared_ptr<Image> image, const std::string& name)
	{
		std::ofstream output;
		output.open(name, std::ios::binary);

		if (output.fail())
		{
			output.close();
			return;
		}

		std::size_t imageWidth = image->GetWidth();
		std::size_t imageHeight = image->GetHeight();

		output << "P6\n" << imageWidth << " " << imageHeight << "\n";

		float maxValue = 255.0f;
		output << maxValue << "\n";

		for (std::size_t i = 0; i < imageHeight; ++i)
		{
			for (std::size_t j = 0; j < imageWidth; ++j)
			{
				FVector3f color;

				GetImageColor(color, FVector2i{ j, i }, image);

				std::uint8_t r = static_cast<std::uint8_t>(FMath::Max(0.0f, FMath::Min(maxValue, color.x + 0.5f)));
				std::uint8_t g = static_cast<std::uint8_t>(FMath::Max(0.0f, FMath::Min(maxValue, color.y + 0.5f)));
				std::uint8_t b = static_cast<std::uint8_t>(FMath::Max(0.0f, FMath::Min(maxValue, color.z + 0.5f)));

				output << r << g << b;
			}
		}

		output.close();
	}

	FORCEINLINE std::shared_ptr<Image> LoadPPMImage(const std::string& name)
	{
		std::ifstream input;
		input.open(name, std::ios::binary);
		std::shared_ptr<Image> image = nullptr;

		if (input.fail())
		{
			input.close();
			return image;
		}

		std::string fileType;
		input >> fileType;

		if (fileType != "P6")
		{
			input.close();
			return image;
		}

		std::size_t imageWidth;
		std::size_t imageHeight;
		float maxValue;
		input >> imageWidth >> imageHeight;
		input >> maxValue;

		float invMaxValue = 1 / maxValue;

		image = std::make_shared<Image>(imageWidth, imageHeight, EDASH_FORMAT::R32G32B32_FLOAT);

		input.ignore(256, '\n');

		std::uint8_t currentPixel[3];
		for (std::size_t i = 0; i < imageHeight; i++)
		{
			for (std::size_t j = 0; j < imageWidth; j++)
			{
				input.read(reinterpret_cast<char*>(currentPixel), 3);

				FVector3f color{ currentPixel[0] * invMaxValue, currentPixel[1] * invMaxValue, currentPixel[2] * invMaxValue };

				image->SetPixel(color, j, i);
			}
		}

		input.close();
		return image;
	}

	*/
	
}