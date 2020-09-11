#pragma once

#include "Shape.h"

namespace Dash
{
	class Plane : public Shape
	{
	public:
		Plane(const FTransform& objectToWorld, const FTransform& worldToObject, const FVector3f& normal,
			const FVector3f& topLeft, const FVector3f& topRight, const FVector3f& bottomLeft);
		~Plane();

		virtual bool Intersection(const FRay& r, Scalar* t, HitInfo* hitInfo) const noexcept;

		FVector3f GetCenter() const noexcept;
		FVector3f GetNormal() const noexcept;
		Scalar GetWidth() const noexcept;
		Scalar GetHeight() const noexcept;

		virtual FBoundingBox ObjectBound() const noexcept override;
		virtual FBoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

		std::shared_ptr<TriangleMesh> CreateTessellatedTriangleMesh(uint16_t levels, uint16_t slices) const noexcept;

	private:
		FVector3f mNormal;
		FVector3f mTangent;
		FVector3f mBinormal;
		FVector3f mTopLeft;
		Scalar mWidth;
		Scalar mHeight;
	};
}