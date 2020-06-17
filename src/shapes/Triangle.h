#pragma once

#include "Shape.h"

namespace Dash
{
	struct RayTraceTrianglePoint
	{
		Vector3f Position;
		Vector3f Normal;
		Vector3f Tangent;
		Vector2f TexCoord;
	};

	class Triangle : public Shape
	{
	public:
		Triangle(const Transform& objectToWorld, const Transform& worldToObject, const std::shared_ptr<TriangleMesh>& mesh, std::uint32_t faceId);
		~Triangle();

		virtual bool Intersection(const Ray& r, Scalar* t, HitInfo* hitInfo) const noexcept override;

		virtual BoundingBox ObjectBound() const noexcept override;
		virtual BoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

	private:
		std::shared_ptr<TriangleMesh> mMesh;
		const std::uint32_t* mVertexIndex;
		std::uint32_t mFaceIndex;
	};
}