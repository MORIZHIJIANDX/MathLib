#pragma once

#include "Shape.h"

namespace Dash
{
	class Plane : public Shape
	{
	public:
		Plane(const Transform& objectToWorld, const Transform& worldToObject, const Vector3f& normal,
			const Vector3f& topLeft, const Vector3f& topRight, const Vector3f& bottomLeft);
		~Plane();

		virtual bool Intersection(const Ray& r, Scalar* t, HitInfo* hitInfo) const noexcept;

		Vector3f GetCenter() const noexcept;
		Vector3f GetNormal() const noexcept;
		Scalar GetWidth() const noexcept;
		Scalar GetHeight() const noexcept;

		virtual BoundingBox ObjectBound() const noexcept override;
		virtual BoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

		std::shared_ptr<TriangleMesh> CreateTessellatedTriangleMesh(uint16_t levels, uint16_t slices) const noexcept;

	private:
		Vector3f mNormal;
		Vector3f mTangent;
		Vector3f mBinormal;
		Vector3f mTopLeft;
		Scalar mWidth;
		Scalar mHeight;
	};
}