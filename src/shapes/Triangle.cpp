#include "Triangle.h"

namespace Dash
{
	namespace Math
	{
		Triangle::Triangle(const Transform& objectToWorld, const Transform& worldToObject, const std::shared_ptr<TriangleMesh>& mesh, std::uint32_t faceId)
			: Shape(objectToWorld, worldToObject)
			, mMesh(mesh)
			, mVertexIndex(nullptr)
			, mFaceIndex(faceId)
		{
			mVertexIndex = reinterpret_cast<std::uint32_t*>(&(mMesh->Indices[3 * faceId]));
		}

		Triangle::~Triangle()
		{
		}

		bool Triangle::Intersection(const Ray& r, Scalar* t, HitInfo* hitInfo) const noexcept
		{
			const RayTraceTriangle* data = reinterpret_cast<RayTraceTriangle*>(mMesh->Vertices.data());
			const Vector3f& p0 = data[mVertexIndex[0]].Position;
			const Vector3f& p1 = data[mVertexIndex[1]].Position;
			const Vector3f& p2 = data[mVertexIndex[2]].Position;



			return false;
		}

		BoundingBox Triangle::ObjectBound() const noexcept
		{
			const RayTraceTriangle* data = reinterpret_cast<RayTraceTriangle*>(mMesh->Vertices.data());
			const Vector3f& p0 = data[mVertexIndex[0]].Position;
			const Vector3f& p1 = data[mVertexIndex[1]].Position;
			const Vector3f& p2 = data[mVertexIndex[2]].Position;

			return Union(BoundingBox{ p0, p1 }, p2);
		}

		BoundingBox Triangle::WorldBound() const noexcept
		{
			const RayTraceTriangle* data = reinterpret_cast<RayTraceTriangle*>(mMesh->Vertices.data());
			const Vector3f& p0 = data[mVertexIndex[0]].Position;
			const Vector3f& p1 = data[mVertexIndex[1]].Position;
			const Vector3f& p2 = data[mVertexIndex[2]].Position;

			return Union(BoundingBox{ ObjectToWorld.TransformPoint(p0), ObjectToWorld.TransformPoint(p1) }, ObjectToWorld.TransformPoint(p2));
		}

		std::shared_ptr<TriangleMesh> Triangle::ConvertToTriangleMesh() const noexcept
		{
			return std::shared_ptr<TriangleMesh>();
		}


	}

}
