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
	{//���ȷ�����ݵ���ӽ���ʽ���ĸ�
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
	{//���ȷ�����ն�Ӧ��DXGI��ʽ����һ��
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

		// 16��ʹ��WIC����������һ��2D����
		//ʹ�ô�COM��ʽ����WIC�೧����Ҳ�ǵ���WIC��һ��Ҫ��������
		ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory)));

		//ʹ��WIC�೧����ӿڼ�������ͼƬ�����õ�һ��WIC����������ӿڣ�ͼƬ��Ϣ��������ӿڴ���Ķ�������
		const WCHAR* pszTexcuteFileName = fileName.c_str();

		ThrowIfFailed(pIWICFactory->CreateDecoderFromFilename(
			pszTexcuteFileName,              // �ļ���
			NULL,                            // ��ָ����������ʹ��Ĭ��
			GENERIC_READ,                    // ����Ȩ��
			WICDecodeMetadataCacheOnDemand,  // ����Ҫ�ͻ������� 
			&pIWICDecoder                    // ����������
		));

		// ��ȡ��һ֡ͼƬ(��ΪGIF�ȸ�ʽ�ļ����ܻ��ж�֡ͼƬ�������ĸ�ʽһ��ֻ��һ֡ͼƬ)
		// ʵ�ʽ���������������λͼ��ʽ����
		ThrowIfFailed(pIWICDecoder->GetFrame(0, &pIWICFrame));

		WICPixelFormatGUID wpf = {};
		//��ȡWICͼƬ��ʽ
		ThrowIfFailed(pIWICFrame->GetPixelFormat(&wpf));
		GUID tgFormat = {};

		//ͨ����һ��ת��֮���ȡDXGI�ĵȼ۸�ʽ
		if (GetTargetPixelFormat(&wpf, &tgFormat))
		{
			stTextureFormat = GetDXGIFormatFromPixelFormat(&tgFormat);
		}

		if (EDASH_FORMAT::UnKwon == stTextureFormat)
		{// ��֧�ֵ�ͼƬ��ʽ Ŀǰ�˳����� 
		 // һ�� ��ʵ�ʵ����浱�ж����ṩ�����ʽת�����ߣ�
		 // ͼƬ����Ҫ��ǰת���ã����Բ�����ֲ�֧�ֵ�����
			ASSERT_FAIL("Unsupported texture format!");
		}

		// ����һ��λͼ��ʽ��ͼƬ���ݶ���ӿ�
		Microsoft::WRL::ComPtr<IWICBitmapSource>pIBMP;

		if (!InlineIsEqualGUID(wpf, tgFormat))
		{// ����жϺ���Ҫ�����ԭWIC��ʽ����ֱ����ת��ΪDXGI��ʽ��ͼƬʱ
		 // ������Ҫ���ľ���ת��ͼƬ��ʽΪ�ܹ�ֱ�Ӷ�ӦDXGI��ʽ����ʽ
		 //����ͼƬ��ʽת����
			Microsoft::WRL::ComPtr<IWICFormatConverter> pIConverter;
			ThrowIfFailed(pIWICFactory->CreateFormatConverter(&pIConverter));

			//��ʼ��һ��ͼƬת������ʵ��Ҳ���ǽ�ͼƬ���ݽ����˸�ʽת��
			ThrowIfFailed(pIConverter->Initialize(
				pIWICFrame.Get(),                // ����ԭͼƬ����
				tgFormat,						 // ָ����ת����Ŀ���ʽ
				WICBitmapDitherTypeNone,         // ָ��λͼ�Ƿ��е�ɫ�壬�ִ��������λͼ�����õ�ɫ�壬����ΪNone
				NULL,                            // ָ����ɫ��ָ��
				0.f,                             // ָ��Alpha��ֵ
				WICBitmapPaletteTypeCustom       // ��ɫ�����ͣ�ʵ��û��ʹ�ã�����ָ��ΪCustom
			));
			// ����QueryInterface������ö����λͼ����Դ�ӿ�
			ThrowIfFailed(pIConverter.As(&pIBMP));
		}
		else
		{
			//ͼƬ���ݸ�ʽ����Ҫת����ֱ�ӻ�ȡ��λͼ����Դ�ӿ�
			ThrowIfFailed(pIWICFrame.As(&pIBMP));
		}
		//���ͼƬ��С����λ�����أ�
		ThrowIfFailed(pIBMP->GetSize(&nTextureW, &nTextureH));

		//��ȡͼƬ���ص�λ��С��BPP��Bits Per Pixel����Ϣ�����Լ���ͼƬ�����ݵ���ʵ��С����λ���ֽڣ�
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

		// ���������ڿ��Եõ�BPP�ˣ���Ҳ���ҿ��ıȽ���Ѫ�ĵط���Ϊ��BPP��Ȼ������ô�໷��
		ThrowIfFailed(pIWICPixelinfo->GetBitsPerPixel(&nBPP));

		// ����ͼƬʵ�ʵ��д�С����λ���ֽڣ�������ʹ����һ����ȡ����������A+B-1��/B ��
		// ����������˵��΢���������,ϣ�����Ѿ���������ָ��
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
			, static_cast<UINT>(nPicRowPitch * nTextureH)   //ע���������ͼƬ������ʵ�Ĵ�С�����ֵͨ��С�ڻ���Ĵ�С
			, reinterpret_cast<BYTE*>(pbPicData)));
	}
}