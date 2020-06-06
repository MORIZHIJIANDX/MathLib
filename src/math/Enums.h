#pragma once

#include "../consolid/consolid.h"

namespace Dash
{
	enum class DASH_FORMAT
	{
		R16_UINT,
		R32_UINT,
		R8G8B8A8_UINT,
		R32_FLOAT,
		R32G32_FLOAT,
		R32G32B32_FLOAT,
		R32G32B32A32_FLOAT,
	};


	FORCEINLINE std::size_t GetDashFormatSize(DASH_FORMAT format)
	{
		switch (format)
		{
		case DASH_FORMAT::R16_UINT:
			return 2;
			break;
		case DASH_FORMAT::R8G8B8A8_UINT:
		case DASH_FORMAT::R32_UINT:
		case DASH_FORMAT::R32_FLOAT:
			return 4;
			break;
		case DASH_FORMAT::R32G32_FLOAT:
			return 8;
			break;
		case DASH_FORMAT::R32G32B32_FLOAT:
			return 12;
			break;
		case DASH_FORMAT::R32G32B32A32_FLOAT:
			return 16;
			break;
		default:
			ASSERT(false);
			break;
		}

		return 0;
	}
}