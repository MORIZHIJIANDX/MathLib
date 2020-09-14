#include "Triangle.h"
#include "../math/Intersection.h"

namespace Dash
{
	Triangle::Triangle(const FTransform& objectToWorld, const FTransform& worldToObject, const std::shared_ptr<TriangleMesh>& mesh, std::uint32_t faceId)
		: Shape(objectToWorld, worldToObject)
		, mMesh(mesh)
		, mVertexIndex(nullptr)
		, mFaceIndex(faceId)
	{
		mVertexIndex = reinterpret_cast<std::uint32_t*>(&(mMesh->Indices[3 * (std::size_t)faceId]));
	}

	Triangle::~Triangle()
	{
	}

	bool Triangle::Intersection(const FRay& r, Scalar* t, HitInfo* hitInfo) const noexcept
	{
		const RayTraceTrianglePoint* data = reinterpret_cast<RayTraceTrianglePoint*>(mMesh->Vertices.data());
		const FVector3f& p0 = ObjectToWorld.TransformPoint(data[mVertexIndex[0]].Position);
		const FVector3f& p1 = ObjectToWorld.TransformPoint(data[mVertexIndex[1]].Position);
		const FVector3f& p2 = ObjectToWorld.TransformPoint(data[mVertexIndex[2]].Position);

		Scalar u, v, tp;
		if (FMath::RayTriangleIntersection(r, p0, p1, p2, u, v, tp))
		{
			if (tp > r.TMax || tp < r.TMin)
				return false;

			if (t != nullptr)
				*t = tp;

			if (hitInfo != nullptr)
			{
				const FVector3f& n0 = ObjectToWorld.TransformNormal(data[mVertexIndex[0]].Normal);
				const FVector3f& n1 = ObjectToWorld.TransformNormal(data[mVertexIndex[1]].Normal);
				const FVector3f& n2 = ObjectToWorld.TransformNormal(data[mVertexIndex[2]].Normal);

				const FVector3f& t0 = ObjectToWorld.TransformVector(data[mVertexIndex[0]].Tangent);
				const FVector3f& t1 = ObjectToWorld.TransformVector(data[mVertexIndex[1]].Tangent);
				const FVector3f& t2 = ObjectToWorld.TransformVector(data[mVertexIndex[2]].Tangent);

				const FVector2f& uv0 = data[mVertexIndex[0]].TexCoord;
				const FVector2f& uv1 = data[mVertexIndex[1]].TexCoord;
				const FVector2f& uv2 = data[mVertexIndex[2]].TexCoord;

				Scalar w = 1 - u - v;
				hitInfo->Position = u * p0 + v * p1 + w * p2;
				hitInfo->Normal = FMath::Normalize(u * n0 + v * n1 + w * n2);
				hitInfo->Tangent = FMath::Normalize(u * t0 + v * t1 + w * t2);
				hitInfo->TexCoord = u * uv0 + v * uv1 + w * uv2;
			}

			return true;
		}

		return false;
	}

	FBoundingBox Triangle::ObjectBound() const noexcept
	{
		const RayTraceTrianglePoint* data = reinterpret_cast<RayTraceTrianglePoint*>(mMesh->Vertices.data());
		const FVector3f& p0 = data[mVertexIndex[0]].Position;
		const FVector3f& p1 = data[mVertexIndex[1]].Position;
		const FVector3f& p2 = data[mVertexIndex[2]].Position;

		return FMath::Union(FBoundingBox{ p0, p1 }, p2);
	}

	FBoundingBox Triangle::WorldBound() const noexcept
	{
		const RayTraceTrianglePoint* data = reinterpret_cast<RayTraceTrianglePoint*>(mMesh->Vertices.data());
		const FVector3f& p0 = data[mVertexIndex[0]].Position;
		const FVector3f& p1 = data[mVertexIndex[1]].Position;
		const FVector3f& p2 = data[mVertexIndex[2]].Position;

		return FMath::Union(FBoundingBox{ ObjectToWorld.TransformPoint(p0), ObjectToWorld.TransformPoint(p1) }, ObjectToWorld.TransformPoint(p2));
	}

	std::shared_ptr<TriangleMesh> Triangle::ConvertToTriangleMesh() const noexcept
	{
		std::shared_ptr<TriangleMesh> triangleMesh = std::make_shared<TriangleMesh>();
		triangleMesh->IndexType = mMesh->IndexType;
		triangleMesh->NumVertices = 3;
		triangleMesh->NumIndices = 3;
		triangleMesh->MeshParts.emplace_back(0, 3, 0, 3, 0);

		triangleMesh->InputElements = mMesh->InputElements;
		triangleMesh->InputElementMap = mMesh->InputElementMap;
		triangleMesh->VertexStride = mMesh->VertexStride;


		triangleMesh->Vertices.reserve((std::size_t)(triangleMesh->VertexStride) * 3);
		triangleMesh->Indices.reserve(GetByteSizeForFormat(triangleMesh->IndexType) * 3);

		std::memcpy(triangleMesh->Vertices.data(), mMesh->Vertices.data() + mMesh->VertexStride * 3 * mFaceIndex, mMesh->VertexStride * 3);

		if (triangleMesh->IndexType == EDASH_FORMAT::R16_UINT)
		{
			uint16_t indices[3] = { 0, 1, 2 };
			std::memcpy(triangleMesh->Indices.data(), indices, sizeof(indices));
		}
		else
		{
			uint32_t indices[3] = { 0, 1, 2 };
			std::memcpy(triangleMesh->Indices.data(), indices, sizeof(indices));
		}

		return triangleMesh;
	}
}
