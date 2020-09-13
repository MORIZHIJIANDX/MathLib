#pragma once

#include "../consolid/consolid.h"

namespace Dash
{
	enum class EDASH_FORMAT : uint16_t
	{
		UnKwon = 0,
		R8_UNORM,
		R16_UNORM,
		R16_UINT,
		R32_UINT,
		R16_FLOAT,
		R32_FLOAT,
		R32G32_FLOAT,
		R32G32B32_FLOAT,
		R8G8B8A8_UINT,
		R8G8B8A8_UNORM,
		B8G8R8A8_UNORM,
		R10G10B10A2_UNORM,
		R32G32B32A32_FLOAT,
	};

	FORCEINLINE std::size_t GetDashFormatSize(EDASH_FORMAT format)
	{
		switch (format)
		{
		case EDASH_FORMAT::R16_UINT:
			return 2;
			break;
		case EDASH_FORMAT::R8G8B8A8_UINT:
		case EDASH_FORMAT::R32_UINT:
		case EDASH_FORMAT::R32_FLOAT:
			return 4;
			break;
		case EDASH_FORMAT::R32G32_FLOAT:
			return 8;
			break;
		case EDASH_FORMAT::R32G32B32_FLOAT:
			return 12;
			break;
		case EDASH_FORMAT::R32G32B32A32_FLOAT:
			return 16;
			break;
		default:
			ASSERT(false);
			break;
		}

		return 0;
	}
}