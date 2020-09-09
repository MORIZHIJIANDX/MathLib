#pragma once

#include "Image.h"

namespace Dash
{
	/*

	//Helper Function

	FORCEINLINE void GetImageColor(Vector3f& color, const Vector2i& index, std::shared_ptr<Image> image, bool repeat = false)
	{
		switch (image->GetFormat())
		{
		case DASH_FORMAT::R32_FLOAT:
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
		case DASH_FORMAT::R32G32_FLOAT:
		{
			Vector2f temp = image->GetPixel<Vector2f>(index);
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
		case DASH_FORMAT::R32G32B32_FLOAT:
		{
			Dash::Vector3f temp = image->GetPixel<Dash::Vector3f>(index);

			color.x = temp.x * 255;
			color.y = temp.y * 255;
			color.z = temp.z * 255;

			return;
		}
		break;
		case DASH_FORMAT::R32G32B32A32_FLOAT:
		{
			Vector4f temp = image->GetPixel<Vector4f>(index);

			color.x = temp.x * 255;
			color.y = temp.y * 255;
			color.z = temp.z * 255;

			return;
		}
		break;
		case DASH_FORMAT::R8G8B8A8_UINT:
		{
			Vector4<uint8_t> temp = image->GetPixel<Vector4<uint8_t>>(index.x, image->GetHeight() - 1 - index.y);

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
				Vector3f color;

				GetImageColor(color, Vector2i{ j, i }, image);

				std::uint8_t r = static_cast<std::uint8_t>(Math::Max(0.0f, Math::Min(maxValue, color.x + 0.5f)));
				std::uint8_t g = static_cast<std::uint8_t>(Math::Max(0.0f, Math::Min(maxValue, color.y + 0.5f)));
				std::uint8_t b = static_cast<std::uint8_t>(Math::Max(0.0f, Math::Min(maxValue, color.z + 0.5f)));

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

		image = std::make_shared<Image>(imageWidth, imageHeight, DASH_FORMAT::R32G32B32_FLOAT);

		input.ignore(256, '\n');

		std::uint8_t currentPixel[3];
		for (std::size_t i = 0; i < imageHeight; i++)
		{
			for (std::size_t j = 0; j < imageWidth; j++)
			{
				input.read(reinterpret_cast<char*>(currentPixel), 3);

				Vector3f color{ currentPixel[0] * invMaxValue, currentPixel[1] * invMaxValue, currentPixel[2] * invMaxValue };

				image->SetPixel(color, j, i);
			}
		}

		input.close();
		return image;
	}


	*/
}