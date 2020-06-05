#pragma once

#include "Shape.h"

namespace Dash
{
	class Sphere : public Shape
	{
	public:
		Sphere(const Math::Transform& objectToWorld, const Math::Transform& worldToObject, Math::Scalar radius, 
			std::size_t level, std::size_t slice);
		~Sphere();

		virtual bool Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept;

		Math::Vector3f GetCenter() const noexcept;
		Math::Scalar GetRadius() const noexcept;

		virtual Math::BoundingBox ObjectBound() const noexcept override;
		virtual Math::BoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

	private:
		Math::Scalar mRadius;
		std::size_t mLevels;
		std::size_t mSlices;
	};
}