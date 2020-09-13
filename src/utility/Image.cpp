#include "Image.h"

namespace Dash
{
	FTexture::FTexture()
		: mWidth(0)
		, mHeight(0)
		, mFormat(EDASH_FORMAT::UnKwon)
		, mBitPerPixel(0)
		, mRowAlignment(1)
	{
	}

	FTexture::FTexture(size_t width, size_t height, EDASH_FORMAT format, size_t alignment)
		: mWidth(width)
		, mHeight(height)
		, mFormat(format)
		, mBitPerPixel(GetDashFormatSize(format))
		, mRowAlignment(alignment)
	{
		size_t alignmentRow = height == 0 ? 0 : (height - 1);
		size_t picRowPitch = UPPER_ALIGNMENT(width * mBitPerPixel, alignment);
		mData.resize(picRowPitch * alignmentRow + width * mBitPerPixel);
	}

	FTexture::FTexture(const FTexture& other)
		: mWidth(other.mWidth)
		, mHeight(other.mHeight)
		, mFormat(other.mFormat)
		, mBitPerPixel(other.mBitPerPixel)
		, mRowAlignment(other.mRowAlignment)
		, mData(other.mData)
	{
	}

	FTexture::FTexture(FTexture&& other) noexcept
		: mWidth(std::exchange(other.mWidth, 0))
		, mHeight(std::exchange(other.mHeight, 0))
		, mFormat(std::exchange(other.mFormat, EDASH_FORMAT::UnKwon))
		, mBitPerPixel(std::exchange(other.mBitPerPixel, 0))
		, mRowAlignment(std::exchange(other.mRowAlignment, 1))
		, mData(std::move(other.mData))
	{
	}

	FTexture& FTexture::operator=(const FTexture& other) noexcept
	{
		if (this != &other)
		{
			mWidth = other.mWidth;
			mHeight = other.mHeight;
			mFormat = other.mFormat;
			mBitPerPixel = other.mBitPerPixel;
			mRowAlignment = other.mRowAlignment;
			mData = other.mData;
		}
		return *this;
	}

	FTexture& FTexture::operator=(FTexture&& other) noexcept
	{
		if (this != &other)
		{
			mWidth = std::exchange(other.mWidth, 0);
			mHeight = std::exchange(other.mHeight, 0);
			mFormat = std::exchange(other.mFormat, EDASH_FORMAT::UnKwon);
			mBitPerPixel = std::exchange(other.mBitPerPixel, 0);
			mRowAlignment = std::exchange(other.mRowAlignment, 1);
			mData = std::move(other.mData);
		}
		return *this;
	}

	void FTexture::Resize(size_t x, size_t y)
	{
		mWidth = x;
		mHeight = y;

		size_t alignmentRow = mHeight == 0 ? 0 : (mHeight - 1);
		size_t picRowPitch = UPPER_ALIGNMENT(mWidth * mBitPerPixel, mRowAlignment);
		mData.resize(picRowPitch * alignmentRow + mWidth * mBitPerPixel);
	}

	void FTexture::Resize(const FVector2i& size)
	{
		Resize(size.x, size.y);
	}

}