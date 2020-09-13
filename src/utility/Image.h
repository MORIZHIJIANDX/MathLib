#pragma once

#include "../math/MathType.h"
#include <vector>
#include <fstream>
#include <vector>
#include <algorithm>

namespace Dash
{
	class FTexture
	{
	public:
		FTexture();
		FTexture(size_t width, size_t height, EDASH_FORMAT format, size_t alignment = 1);
		FTexture(const FTexture& other);
		FTexture(FTexture&& other) noexcept;
		~FTexture() {};

		FTexture& operator=(const FTexture& other) noexcept;
		FTexture& operator=(FTexture&& other) noexcept;

		EDASH_FORMAT GetFormat() const { return mFormat; }

		size_t GetBitPerPixel() const { return mBitPerPixel; }

		size_t GetWidth() const { return mWidth; }

		size_t GetHeight() const { return mHeight; }

		const void* GetRawData() const { return mData.data(); }

		void* GetRawData() { return mData.data(); }

		template<typename T>
		void SetPixel(const T& value, size_t x, size_t y);

		template<typename T>
		void SetPixel(const T& value, const FVector2i& index);

		template<typename T>
		const T& GetPixel(size_t x, size_t y) const;

		template<typename T>
		const T& GetPixel(const FVector2i& index) const;

		template<typename T>
		void ClearImage(const T& value = T{ FZero{} });

		void Resize(size_t x, size_t y);

		void Resize(const FVector2i& size);

	private:
		size_t mWidth;
		size_t mHeight;
		EDASH_FORMAT mFormat;
		size_t mRowAlignment;
		size_t mBitPerPixel;
		std::vector<uint8_t> mData;
	};

	template<typename T>
	FORCEINLINE void FTexture::SetPixel(const T& value, size_t x, size_t y)
	{
		ASSERT(x < mWidth&& y < mHeight);
		reinterpret_cast<T&>(mData[x + y * mWidth]) = value;
	}

	template<typename T>
	FORCEINLINE void FTexture::SetPixel(const T& value, const FVector2i& index)
	{
		SetPixel(value, index.x, index.y);
	}

	template<typename T>
	FORCEINLINE const T& FTexture::GetPixel(size_t x, size_t y) const
	{
		ASSERT(x < mWidth&& y < mHeight);

		ASSERT(mFormat == GetFormatForType<T>());

		return reinterpret_cast<const T&>(mData[x + y * mWidth]);
	}

	template<typename T>
	FORCEINLINE const T& FTexture::GetPixel(const FVector2i& index) const
	{
		GetPixel(index.x, index.y);
	}

	template<typename T>
	FORCEINLINE void FTexture::ClearImage(const T& value)
	{
		for (size_t h = 0; h < mHeight; h++)
		{
			for (size_t w = 0; w < mWidth; w++)
			{
				SetPixel(value, w, h);
			}
		}
	}
}