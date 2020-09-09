#pragma once

#include "../math/MathType.h"
#include <vector>
#include <fstream>
#include <vector>
#include <algorithm>

namespace Dash
{
	template<typename T>
	class RawImage
	{
	public:
		RawImage(std::size_t width, std::size_t height, const T& defaultVal = T{ Zero{} });
		RawImage(const RawImage& other);
		~RawImage();

		DASH_FORMAT GetFormat() const { return mFormat; }

		std::size_t GetWidth() const { return mWidth; }

		std::size_t GetHeight() const { return mHeight; }

		const void* GetRawData() const { return mData; }

		void SetPixel(const T& value, std::size_t x, std::size_t y);

		void SetPixel(const T& value, const Vector2i& index);

		const T& GetPixel(std::size_t x, std::size_t y) const;

		const T& GetPixel(const Vector2i& index) const;

		//T* operator[](std::size_t idx) { return &mVector[idx * mWidth]; }
		//const T* operator[](std::size_t idx) const { &mVector[idx * mWidth]; }

		void ClearImage(const T& value = T{ Zero{} });

		void Resize(std::size_t x, std::size_t y);

		void Resize(const Vector2i& size);

	private:
		DASH_FORMAT mFormat;
		std::size_t mWidth;
		std::size_t mHeight;
		std::vector<T> mData;
	};



	template<typename T>
	FORCEINLINE RawImage<T>::RawImage(std::size_t width, std::size_t height, const T& defaultVal)
		: mWidth(width)
		, mHeight(height)
		, mData(width* height, defaultVal)
	{
	}

	template<typename T>
	FORCEINLINE RawImage<T>::RawImage(const RawImage& other)
		: mWidth(other.mWidth)
		, mHeight(other.mHeight)
		, mFormat(other.mFormat)
	{
		mData = other.mData;
	}

	template<typename T>
	FORCEINLINE RawImage<T>::~RawImage()
	{
	}

	template<typename T>
	FORCEINLINE void RawImage<T>::SetPixel(const T& value, std::size_t x, std::size_t y)
	{
		ASSERT(x < mWidth&& y < mHeight);
		mData[x + y * mWidth] = value;
	}

	template<typename T>
	FORCEINLINE void RawImage<T>::SetPixel(const T& value, const Vector2i& index)
	{
		SetPixel(value, index.x, index.y);
	}

	template<typename T>
	FORCEINLINE const T& RawImage<T>::GetPixel(std::size_t x, std::size_t y) const
	{
		ASSERT(x < mWidth&& y < mHeight);

		return mData[x + y * mWidth];
	}

	template<typename T>
	FORCEINLINE const T& RawImage<T>::GetPixel(const Vector2i& index) const
	{
		return GetPixel(index.x, index.y);
	}

	template<typename T>
	FORCEINLINE void RawImage<T>::ClearImage(const T& value)
	{
		std::fill(mData.begin(), mData.end(), value);
	}

	template<typename T>
	FORCEINLINE void RawImage<T>::Resize(std::size_t x, std::size_t y)
	{
		mWidth = x;
		mHeight = y;

		mData.resize(mWidth * mHeight);
	}

	template<typename T>
	FORCEINLINE void RawImage<T>::Resize(const Vector2i& size)
	{
		Resize(size.x, size.y);
	}
}


//class Image
//{
//public:
//	Image(std::size_t width, std::size_t height, DASH_FORMAT format);
//	~Image();

//	DASH_FORMAT GetFormat() const { return mFormat; }

//	std::size_t GetWidth() const { return mWidth; }

//	std::size_t GetHeight() const { return mHeight; }

//	const void* GetRawData() const { return mData; }

//	template<typename T>
//	void SetPixel(const T& value, std::size_t x, std::size_t y);

//	template<typename T>
//	void SetPixel(const T& value, Vector2i index);

//	template<typename T>
//	T GetPixel(std::size_t x, std::size_t y);

//	template<typename T>
//	T GetPixel(Vector2i index);

//	template<typename T>
//	void ClearImage(const T& value = T{ Zero{} });

//private:
//	std::size_t mWidth;
//	std::size_t mHeight;
//	DASH_FORMAT mFormat;
//	void* mData;
//};

//template<typename T>
//FORCEINLINE void Image::SetPixel(const T& value, std::size_t x, std::size_t y)
//{
//	ASSERT(x < mWidth&& y < mHeight);
//	ASSERT(sizeof(T) == GetDashFormatSize(mFormat));

//	std::size_t pixelIndex = x + y * mWidth;
//	*(static_cast<T*>(mData) + pixelIndex) = value;
//}

//template<typename T>
//FORCEINLINE void Image::SetPixel(const T& value, Vector2i index)
//{
//	SetPixel(value, index.x, index.y);
//}

//template<typename T>
//FORCEINLINE T Image::GetPixel(std::size_t x, std::size_t y)
//{
//	ASSERT(x < mWidth&& y < mHeight);
//	ASSERT(sizeof(T) == GetDashFormatSize(mFormat));

//	std::size_t pixelIndex = x + y * mWidth;
//	return *(static_cast<T*>(mData) + pixelIndex);
//}

//template<typename T>
//FORCEINLINE T Image::GetPixel(Vector2i index)
//{
//	return GetPixel<T>(index.x, index.y);
//}

//template<typename T>
//FORCEINLINE void Image::ClearImage(const T& value)
//{
//	ASSERT(sizeof(T) == GetDashFormatSize(mFormat));

//	for (std::size_t i = 0; i < mWidth * mHeight; i++)
//	{
//		*(static_cast<T*>(mData) + i) = value;
//	}
//}