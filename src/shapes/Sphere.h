#pragma once

#include "Shape.h"

namespace Dash
{
	class Sphere : public Shape
	{
	public:
		Sphere(const Transform& objectToWorld, const Transform& worldToObject, Scalar radius, 
			std::uint16_t level = 16, std::uint16_t slice = 16);
		~Sphere();

		virtual bool Intersection(const Ray& r, Scalar* t, HitInfo* hitInfo) const noexcept;

		Vector3f GetCenter() const noexcept;
		Scalar GetRadius() const noexcept;

		virtual BoundingBox ObjectBound() const noexcept override;
		virtual BoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

		std::shared_ptr<TriangleMesh> CreateTessellatedTriangleMesh(uint16_t levels, uint16_t slices) const noexcept;

	private:
		Scalar mRadius;
	};
}