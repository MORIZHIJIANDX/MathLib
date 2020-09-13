#include "ImageHelper.h"
#include "Exception.h"
#include <wrl.h>

namespace Dash
{
	FORCEINLINE static uint8_t GetSRGBFromLinear(float value)
	{
		float FloatR = FMath::Clamp(value, 0.0f, 1.0f);

		FloatR = FloatR <= 0.0031308f ? FloatR * 12.92f : FMath::Pow(FloatR, 1.0f / 2.4f) * 1.055f - 0.055f;

		return FMath::FloorToInt(FloatR * 255.999f);
	}

	void GetImageColor(FColor& color, const FVector2i& index, const FTexture* image, bool repeat)
	{
		ASSERT(image != nullptr);

		EDASH_FORMAT format;

		switch (format)
		{
		case Dash::EDASH_FORMAT::R8G8B8A8_UINT:
		{
			color = image->GetPixel<FColor>(index.x, index.y);
			break;
		}
		case Dash::EDASH_FORMAT::R32_FLOAT:
		{
			float fvalue = image->GetPixel<float>(index.x, index.y);
			const uint8_t ivalue = GetSRGBFromLinear(fvalue);
			color = repeat ? FColor{ ivalue, ivalue, ivalue, 255 } : FColor{ ivalue, 0, 0, 255 };
			break;
		}
		case Dash::EDASH_FORMAT::R32G32_FLOAT:
		{
			FVector2f fvalue = image->GetPixel<FVector2f>(index.x, index.y);
			const uint8_t ir = GetSRGBFromLinear(fvalue.x);
			const uint8_t ig = GetSRGBFromLinear(fvalue.y);
			color = FColor{ ir, ig, 0, 255 };
			break;
		}
		case Dash::EDASH_FORMAT::R32G32B32_FLOAT:
		{
			FVector3f fvalue = image->GetPixel<FVector3f>(index.x, index.y);
			const uint8_t ir = GetSRGBFromLinear(fvalue.x);
			const uint8_t ig = GetSRGBFromLinear(fvalue.y);
			const uint8_t ib = GetSRGBFromLinear(fvalue.z);
			color = FColor{ ir, ig, ib, 255 };
			break;
		}
		case Dash::EDASH_FORMAT::R32G32B32A32_FLOAT:
		{
			FVector4f fvalue = image->GetPixel<FVector4f>(index.x, index.y);
			const uint8_t ir = GetSRGBFromLinear(fvalue.x);
			const uint8_t ig = GetSRGBFromLinear(fvalue.y);
			const uint8_t ib = GetSRGBFromLinear(fvalue.z);
			const uint8_t ia = FMath::FloorToInt(FMath::Clamp(fvalue.w, 0.0f, 1.0f) * 255.999f);
			color = FColor{ ir, ig, ib, ia };
			break;
		}
		default:
			ASSERT_FAIL("Unsupported Type!");
			break;
		}
	}

	FORCEINLINE void SavePPMImage(const FTexture* image, const std::string& name)
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
				FColor color;

				GetImageColor(color, FVector2i{ j, i }, image);

				std::uint8_t r = color.r;
				std::uint8_t g = color.g;
				std::uint8_t b = color.b;

				output << r << g << b;
			}
		}

		output.close();
	}

	FORCEINLINE FTexture LoadPPMImage(const std::string& name)
	{
		std::ifstream input;
		input.open(name, std::ios::binary);
		FTexture image;

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

		image.Resize(imageWidth, imageHeight);

		input.ignore(256, '\n');

		FColor currentPixel{ 0, 0, 0, 255 };
		for (std::size_t i = 0; i < imageHeight; i++)
		{
			for (std::size_t j = 0; j < imageWidth; j++)
			{
				input.read(reinterpret_cast<char*>(&(currentPixel.AlignmentDummy)), 3);

				FLinearColor color{ currentPixel };

				image.SetPixel(color, j, i);
			}
		}

		input.close();
		return image;
	}


	bool GetTargetPixelFormat(const GUID* pSourceFormat, GUID* pTargetFormat)
	{//查表确定兼容的最接近格式是哪个
		*pTargetFormat = *pSourceFormat;
		for (size_t i = 0; i < _countof(g_WICConvert); ++i)
		{
			if (InlineIsEqualGUID(g_WICConvert[i].Source, *pSourceFormat))
			{
				*pTargetFormat = g_WICConvert[i].Target;
				return true;
			}
		}
		return false;
	}

	EDASH_FORMAT GetDXGIFormatFromPixelFormat(const GUID* pPixelFormat)
	{//查表确定最终对应的DXGI格式是哪一个
		for (size_t i = 0; i < _countof(g_WICFormats); ++i)
		{
			if (InlineIsEqualGUID(g_WICFormats[i].Wic, *pPixelFormat))
			{
				return g_WICFormats[i].Format;
			}
		}
		return EDASH_FORMAT::UnKwon;
	}

	FTexture ReadWICTexture(const std::wstring& fileName)
	{
		Microsoft::WRL::ComPtr<IWICImagingFactory>			pIWICFactory;
		Microsoft::WRL::ComPtr<IWICBitmapDecoder>			pIWICDecoder;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode>		pIWICFrame;
		EDASH_FORMAT stTextureFormat;

		UINT nTextureW;
		UINT nTextureH;
		UINT nBPP;

		// 16、使用WIC创建并加载一个2D纹理
		//使用纯COM方式创建WIC类厂对象，也是调用WIC第一步要做的事情
		ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory)));

		//使用WIC类厂对象接口加载纹理图片，并得到一个WIC解码器对象接口，图片信息就在这个接口代表的对象中了
		const WCHAR* pszTexcuteFileName = fileName.c_str();

		ThrowIfFailed(pIWICFactory->CreateDecoderFromFilename(
			pszTexcuteFileName,              // 文件名
			NULL,                            // 不指定解码器，使用默认
			GENERIC_READ,                    // 访问权限
			WICDecodeMetadataCacheOnDemand,  // 若需要就缓冲数据 
			&pIWICDecoder                    // 解码器对象
		));

		// 获取第一帧图片(因为GIF等格式文件可能会有多帧图片，其他的格式一般只有一帧图片)
		// 实际解析出来的往往是位图格式数据
		ThrowIfFailed(pIWICDecoder->GetFrame(0, &pIWICFrame));

		WICPixelFormatGUID wpf = {};
		//获取WIC图片格式
		ThrowIfFailed(pIWICFrame->GetPixelFormat(&wpf));
		GUID tgFormat = {};

		//通过第一道转换之后获取DXGI的等价格式
		if (GetTargetPixelFormat(&wpf, &tgFormat))
		{
			stTextureFormat = GetDXGIFormatFromPixelFormat(&tgFormat);
		}

		if (EDASH_FORMAT::UnKwon == stTextureFormat)
		{// 不支持的图片格式 目前退出了事 
		 // 一般 在实际的引擎当中都会提供纹理格式转换工具，
		 // 图片都需要提前转换好，所以不会出现不支持的现象
			ASSERT_FAIL("Unsupported texture format!");
		}

		// 定义一个位图格式的图片数据对象接口
		Microsoft::WRL::ComPtr<IWICBitmapSource>pIBMP;

		if (!InlineIsEqualGUID(wpf, tgFormat))
		{// 这个判断很重要，如果原WIC格式不是直接能转换为DXGI格式的图片时
		 // 我们需要做的就是转换图片格式为能够直接对应DXGI格式的形式
		 //创建图片格式转换器
			Microsoft::WRL::ComPtr<IWICFormatConverter> pIConverter;
			ThrowIfFailed(pIWICFactory->CreateFormatConverter(&pIConverter));

			//初始化一个图片转换器，实际也就是将图片数据进行了格式转换
			ThrowIfFailed(pIConverter->Initialize(
				pIWICFrame.Get(),                // 输入原图片数据
				tgFormat,						 // 指定待转换的目标格式
				WICBitmapDitherTypeNone,         // 指定位图是否有调色板，现代都是真彩位图，不用调色板，所以为None
				NULL,                            // 指定调色板指针
				0.f,                             // 指定Alpha阀值
				WICBitmapPaletteTypeCustom       // 调色板类型，实际没有使用，所以指定为Custom
			));
			// 调用QueryInterface方法获得对象的位图数据源接口
			ThrowIfFailed(pIConverter.As(&pIBMP));
		}
		else
		{
			//图片数据格式不需要转换，直接获取其位图数据源接口
			ThrowIfFailed(pIWICFrame.As(&pIBMP));
		}
		//获得图片大小（单位：像素）
		ThrowIfFailed(pIBMP->GetSize(&nTextureW, &nTextureH));

		//获取图片像素的位大小的BPP（Bits Per Pixel）信息，用以计算图片行数据的真实大小（单位：字节）
		Microsoft::WRL::ComPtr<IWICComponentInfo> pIWICmntinfo;
		ThrowIfFailed(pIWICFactory->CreateComponentInfo(tgFormat, pIWICmntinfo.GetAddressOf()));

		WICComponentType type;
		ThrowIfFailed(pIWICmntinfo->GetComponentType(&type));

		if (type != WICPixelFormat)
		{
			ASSERT_FAIL("Unsupported texture format!");
		}

		Microsoft::WRL::ComPtr<IWICPixelFormatInfo> pIWICPixelinfo;
		ThrowIfFailed(pIWICmntinfo.As(&pIWICPixelinfo));

		// 到这里终于可以得到BPP了，这也是我看的比较吐血的地方，为了BPP居然饶了这么多环节
		ThrowIfFailed(pIWICPixelinfo->GetBitsPerPixel(&nBPP));

		// 计算图片实际的行大小（单位：字节），这里使用了一个上取整除法即（A+B-1）/B ，
		// 这曾经被传说是微软的面试题,希望你已经对它了如指掌
		UINT nPicRowPitch = (uint64_t(nTextureW) * uint64_t(nBPP) + 7u) / 8u;

		//const UINT64 n64UploadBufferSize = GetRequiredIntermediateSize(pITexcute.Get(), 0, 1);
		const UINT64 n64UploadBufferSize = 0;

		void* pbPicData = ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, n64UploadBufferSize);
		if (nullptr == pbPicData)
		{
			ASSERT_FAIL("Unsupported texture format!");
		}

		ThrowIfFailed(pIBMP->CopyPixels(nullptr
			, nPicRowPitch
			, static_cast<UINT>(nPicRowPitch * nTextureH)   //注意这里才是图片数据真实的大小，这个值通常小于缓冲的大小
			, reinterpret_cast<BYTE*>(pbPicData)));
	}
}