#pragma once

#include "Shape.h"

namespace Dash
{
	class Box : public Shape
	{
	public:
		Box(const Math::Transform& objectToWorld, const Math::Transform& worldToObject, Math::Scalar width, Math::Scalar height, Math::Scalar depth);
		~Box();

	private:

	};
}