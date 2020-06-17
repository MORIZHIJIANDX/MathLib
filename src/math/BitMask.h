#pragma once

#include "Scalar.h"
#include "Algebra.h"

namespace Dash
{
	template<typename Scalar>
	class BitMask
	{
	public:
		BitMask();
		explicit BitMask(uint32_t bits);
		explicit BitMask(Zero);
		template<uint32_t I> BitMask(Unit<I>);

		operator uint32_t() const;
		bool operator[](int i) const;

	private:
		uint32_t mBits;
	};
}