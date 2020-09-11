#include "Image.h"

Image::Image(std::size_t width, std::size_t height, Dash::EDASH_FORMAT format)
	: mWidth(width)
	, mHeight(height)
	, mFormat(format)
	, mData(nullptr)
{
	std::size_t formatSize = Dash::GetDashFormatSize(format);
	mData = new std::uint8_t[mWidth * mHeight * formatSize];
}

Image::~Image()
{
	if (mData != nullptr)
	{
		delete[] mData;
	}
}
