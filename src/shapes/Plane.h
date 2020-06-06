#pragma once

#include "Shape.h"

namespace Dash
{
	class Plane : public Shape
	{
	public:
		Plane(const Math::Transform& objectToWorld, const Math::Transform& worldToObject, const Math::Vector3f& normal,
			const Math::Vector3f& topLeft, const Math::Vector3f& topRight, const Math::Vector3f& bottomLeft);
		~Plane();

		virtual bool Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept;

		Math::Vector3f GetCenter() const noexcept;
		Math::Vector3f GetNormal() const noexcept;
		Math::Scalar GetWidth() const noexcept;
		Math::Scalar GetHeight() const noexcept;

		virtual Math::BoundingBox ObjectBound() const noexcept override;
		virtual Math::BoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

	private:
		Math::Vector3f mNormal;
		Math::Vector3f mTangent;
		Math::Vector3f mBinormal;
		Math::Vector3f mTopLeft;
		Math::Scalar mWidth;
		Math::Scalar mHeight;
	};
}