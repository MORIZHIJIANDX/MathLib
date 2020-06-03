#pragma once

#include "Shape.h"

namespace Dash
{
	struct RayTraceTrianglePoint
	{
		Math::Vector3f Position;
		Math::Vector3f Normal;
		Math::Vector3f Tangent;
		Math::Vector2f TexCoord;
	};

	class Triangle : public Shape
	{
	public:
		Triangle(const Math::Transform& objectToWorld, const Math::Transform& worldToObject, const std::shared_ptr<TriangleMesh>& mesh, std::uint32_t faceId);
		~Triangle();

		virtual bool Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept override;

		virtual Math::BoundingBox ObjectBound() const noexcept override;
		virtual Math::BoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

	private:
		std::shared_ptr<TriangleMesh> mMesh;
		const std::uint32_t* mVertexIndex;
		std::uint32_t mFaceIndex;
	};
}