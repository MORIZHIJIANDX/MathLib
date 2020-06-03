#pragma once

#include "Shape.h"

namespace Dash
{
	class Sphere : public Shape
	{
	public:
		Sphere();
		~Sphere();

		virtual bool Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept = 0;
		bool IntersectionFast(const Math::Ray& r) const noexcept
		{
			return Intersection(r, nullptr, nullptr);
		}

		virtual Math::BoundingBox ObjectBound() const noexcept override;
		virtual Math::BoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

	private:
		Math::Vector3f mCenter;
		Math::Scalar mRadius;
	};
}