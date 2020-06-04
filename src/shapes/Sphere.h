#pragma once

#include "Shape.h"

namespace Dash
{
	class Sphere : public Shape
	{
	public:
		Sphere(const Math::Transform& objectToWorld, const Math::Transform& worldToObject, Math::Scalar radius);
		~Sphere();

		virtual bool Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept;
		bool IntersectionFast(const Math::Ray& r) const noexcept
		{
			return Intersection(r, nullptr, nullptr);
		}

		Math::Vector3f GetCenter() const noexcept;
		Math::Scalar GetRadius() const noexcept;

		virtual Math::BoundingBox ObjectBound() const noexcept override;
		virtual Math::BoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

	private:
		Math::Scalar mRadius;
	};
}