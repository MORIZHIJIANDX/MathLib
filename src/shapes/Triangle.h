#pragma once

#include "Shape.h"

namespace Dash
{
	struct RayTraceTrianglePoint
	{
		FVector3f Position;
		FVector3f Normal;
		FVector3f Tangent;
		FVector2f TexCoord;
	};

	class Triangle : public Shape
	{
	public:
		Triangle(const FTransform& objectToWorld, const FTransform& worldToObject, const std::shared_ptr<TriangleMesh>& mesh, std::uint32_t faceId);
		~Triangle();

		virtual bool Intersection(const FRay& r, Scalar* t, HitInfo* hitInfo) const noexcept override;

		virtual FBoundingBox ObjectBound() const noexcept override;
		virtual FBoundingBox WorldBound() const noexcept override;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept override;

	private:
		std::shared_ptr<TriangleMesh> mMesh;
		const std::uint32_t* mVertexIndex;
		std::uint32_t mFaceIndex;
	};
}