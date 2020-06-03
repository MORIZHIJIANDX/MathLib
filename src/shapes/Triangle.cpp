#include "Triangle.h"
#include "../math/Intersection.h"

namespace Dash
{
	Triangle::Triangle(const Math::Transform& objectToWorld, const Math::Transform& worldToObject, const std::shared_ptr<TriangleMesh>& mesh, std::uint32_t faceId)
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

	bool Triangle::Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept
	{
		const RayTraceTrianglePoint* data = reinterpret_cast<RayTraceTrianglePoint*>(mMesh->Vertices.data());
		const Math::Vector3f& p0 = ObjectToWorld.TransformPoint(data[mVertexIndex[0]].Position);
		const Math::Vector3f& p1 = ObjectToWorld.TransformPoint(data[mVertexIndex[1]].Position);
		const Math::Vector3f& p2 = ObjectToWorld.TransformPoint(data[mVertexIndex[2]].Position);

		Math::Scalar u, v, tp;
		if (RayTriangleIntersection(r, p0, p1, p2, u, v, tp))
		{
			if (t != nullptr)
				*t = tp;

			if (hitInfo)
			{
				const Math::Vector3f& n0 = ObjectToWorld.TransformNormal(data[mVertexIndex[0]].Normal);
				const Math::Vector3f& n1 = ObjectToWorld.TransformNormal(data[mVertexIndex[1]].Normal);
				const Math::Vector3f& n2 = ObjectToWorld.TransformNormal(data[mVertexIndex[2]].Normal);

				const Math::Vector3f& t0 = ObjectToWorld.TransformVector(data[mVertexIndex[0]].Tangent);
				const Math::Vector3f& t1 = ObjectToWorld.TransformVector(data[mVertexIndex[1]].Tangent);
				const Math::Vector3f& t2 = ObjectToWorld.TransformVector(data[mVertexIndex[2]].Tangent);

				const Math::Vector2f& uv0 = data[mVertexIndex[0]].TexCoord;
				const Math::Vector2f& uv1 = data[mVertexIndex[1]].TexCoord;
				const Math::Vector2f& uv2 = data[mVertexIndex[2]].TexCoord;

				Math::Scalar w = 1 - u - v;
				hitInfo->Position = u * p0 + v * p1 + w * p2;
				hitInfo->Normal = Math::Normalize(u * n0 + v * n1 + w * n2);
				hitInfo->Tangent = Math::Normalize(u * t0 + v * t1 + w * t2);
				hitInfo->TexCoord = u * uv0 + v * uv1 + w * uv2;
			}

			return true;
		}

		return false;
	}

	Math::BoundingBox Triangle::ObjectBound() const noexcept
	{
		const RayTraceTrianglePoint* data = reinterpret_cast<RayTraceTrianglePoint*>(mMesh->Vertices.data());
		const Math::Vector3f& p0 = data[mVertexIndex[0]].Position;
		const Math::Vector3f& p1 = data[mVertexIndex[1]].Position;
		const Math::Vector3f& p2 = data[mVertexIndex[2]].Position;

		return Math::Union(Math::BoundingBox{ p0, p1 }, p2);
	}

	Math::BoundingBox Triangle::WorldBound() const noexcept
	{
		const RayTraceTrianglePoint* data = reinterpret_cast<RayTraceTrianglePoint*>(mMesh->Vertices.data());
		const Math::Vector3f& p0 = data[mVertexIndex[0]].Position;
		const Math::Vector3f& p1 = data[mVertexIndex[1]].Position;
		const Math::Vector3f& p2 = data[mVertexIndex[2]].Position;

		return Math::Union(Math::BoundingBox{ ObjectToWorld.TransformPoint(p0), ObjectToWorld.TransformPoint(p1) }, ObjectToWorld.TransformPoint(p2));
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
		triangleMesh->Indices.reserve((std::size_t)(GetDashFormatSize(triangleMesh->IndexType)) * 3);

		std::memcpy(triangleMesh->Vertices.data(), mMesh->Vertices.data() + mMesh->VertexStride * 3 * mFaceIndex, mMesh->VertexStride * 3);

		if (triangleMesh->IndexType == DASH_FORMAT::R16_UINT)
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
