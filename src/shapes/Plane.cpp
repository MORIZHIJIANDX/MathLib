#include "Plane.h"
#include "../math/Intersection.h"

namespace Dash
{
	Plane::Plane(const Transform& objectToWorld, const Transform& worldToObject, const Vector3f& normal,
		const Vector3f& topLeft, const Vector3f& topRight, const Vector3f& bottomLeft)
		: Shape(objectToWorld, worldToObject)
		, mNormal(Math::Normalize(normal))
		, mTopLeft(topLeft)
	{
		Vector3f u = topRight - topLeft;
		Vector3f v = bottomLeft - topLeft;

		mWidth = Math::Length(u);
		mHeight = Math::Length(v);

		ASSERT(!Math::IsZero(mWidth));
		ASSERT(!Math::IsZero(mHeight));

		mTangent = u / mWidth;
		mTangent = mTangent - Math::Dot(mTangent, mNormal) * mNormal;
		mBinormal = Math::Cross(mNormal, mTangent);
		mBinormal = mBinormal - Math::Dot(mBinormal, mNormal) * mNormal - Math::Dot(mBinormal, mTangent) * mTangent;
	}

	Plane::~Plane()
	{
	}

	bool Plane::Intersection(const Ray& r, Scalar* t, HitInfo* hitInfo) const noexcept
	{
		Scalar tp;

		Ray objectRay = WorldToObject.TransformRay(r);

		if (Math::RayPlaneIntersection(objectRay, mNormal, mTopLeft, tp))
		{
			if (tp > r.TMax || tp < r.TMin)
				return false;

			Vector3f point = objectRay(tp);

			Vector3f offsetToTopLeft = point - mTopLeft;
			Scalar u = Math::Dot(mTangent, offsetToTopLeft);
			Scalar v = Math::Dot(mBinormal, offsetToTopLeft);

			if ((u >= Scalar{ 0 } && u <= mWidth) && (v >= Scalar{ 0 } && v <= mHeight))
			{
				if (t != nullptr)
					*t = tp;

				if (hitInfo != nullptr)
				{
					hitInfo->Position = ObjectToWorld.TransformPoint(point);
					hitInfo->Normal = ObjectToWorld.TransformNormal(mNormal);
					hitInfo->Tangent = ObjectToWorld.TransformVector(mTangent);
					hitInfo->TexCoord = Vector2f{ u / mWidth, v / mHeight };
				}

				return true;
			}
		}	

		return false;
	}

	Vector3f Plane::GetCenter() const noexcept
	{
		return ObjectToWorld.TransformPoint(mTopLeft) + Scalar{ 0.5 } * (mWidth * ObjectToWorld.TransformVector(mTangent) + mHeight * ObjectToWorld.TransformVector(mBinormal));
	}

	Vector3f Plane::GetNormal() const noexcept
	{
		return ObjectToWorld.TransformNormal(mNormal);
	}

	Scalar Plane::GetWidth() const noexcept
	{
		return mWidth;
	}

	Scalar Plane::GetHeight() const noexcept
	{
		return mHeight;
	}

	BoundingBox Plane::ObjectBound() const noexcept
	{
		Vector3f topLeft = mTopLeft;
		Vector3f topRight = topLeft + mTangent * mWidth;

		Vector3f bottomLeft = topLeft + mBinormal * mHeight;
		Vector3f bottomRight = topLeft + mTangent * mWidth + mBinormal * mHeight;

		BoundingBox bounds = Math::Union(topLeft, topRight);
		bounds = Math::Union(bounds, bottomLeft);
		bounds = Math::Union(bounds, bottomRight);

		Vector3f epsilon = Vector3f{ Identity{} } * ScalarTraits<Scalar>::Epsilon();
		return BoundingBox{ bounds.Lower - epsilon, bounds.Upper + epsilon };
	}

	BoundingBox Plane::WorldBound() const noexcept
	{
		Vector3f worldTangent = ObjectToWorld.TransformVector(mTangent);
		Vector3f worldBinrmal = ObjectToWorld.TransformVector(mBinormal);

		Vector3f topLeft = ObjectToWorld.TransformPoint(mTopLeft);
		Vector3f topRight = topLeft + worldTangent * mWidth;

		Vector3f bottomLeft = topLeft + worldBinrmal * mHeight;
		Vector3f bottomRight = topLeft + worldTangent * mWidth + worldBinrmal * mHeight;

		BoundingBox bounds = Math::Union(topLeft, topRight);
		bounds = Math::Union(bounds, bottomLeft);
		bounds = Math::Union(bounds, bottomRight);

		Vector3f epsilon = Vector3f{ Identity{} } * ScalarTraits<Scalar>::Epsilon();
		return BoundingBox{ bounds.Lower - epsilon, bounds.Upper + epsilon };
	}

	std::shared_ptr<TriangleMesh> Plane::ConvertToTriangleMesh() const noexcept
	{
		Vector3f topLeft = mTopLeft;
		Vector3f topRight = topLeft + mTangent * mWidth;

		Vector3f bottomLeft = topLeft + mBinormal * mHeight;
		Vector3f bottomRight = topLeft + mTangent * mWidth + mBinormal * mHeight;

		std::shared_ptr<TriangleMesh> triangleMesh = std::make_shared<TriangleMesh>();
		triangleMesh->IndexType = DASH_FORMAT::R16_UINT;
		triangleMesh->NumVertices = 4;
		triangleMesh->NumIndices = 6;
		triangleMesh->MeshParts.emplace_back(0, triangleMesh->NumVertices, 0, triangleMesh->NumIndices, 0);

		triangleMesh->InputElements.emplace_back("POSITION", 0, Dash::DASH_FORMAT::R32G32B32_FLOAT, 0);
		triangleMesh->InputElements.emplace_back("NORMAL", 0, Dash::DASH_FORMAT::R32G32B32_FLOAT, 12);
		triangleMesh->InputElements.emplace_back("TANGENT", 0, Dash::DASH_FORMAT::R32G32B32_FLOAT, 24);
		triangleMesh->InputElements.emplace_back("TEXCOORD", 0, Dash::DASH_FORMAT::R32G32_FLOAT, 36);

		triangleMesh->VertexStride = 0;
		for (size_t i = 0; i < triangleMesh->InputElements.size(); i++)
		{
			triangleMesh->InputElementMap.insert(std::pair(triangleMesh->InputElements[i].SemanticName,
				triangleMesh->InputElements[i].AlignedByteOffset));

			triangleMesh->VertexStride += GetDashFormatSize(triangleMesh->InputElements[i].Format);
		}

		std::size_t positionOffset = triangleMesh->InputElementMap["POSITION"];
		std::size_t normalOffset = triangleMesh->InputElementMap["NORMAL"];
		std::size_t tangentOffset = triangleMesh->InputElementMap["TANGENT"];
		std::size_t texCoordOffset = triangleMesh->InputElementMap["TEXCOORD"];

		triangleMesh->Vertices.reserve((std::size_t)(triangleMesh->VertexStride) * triangleMesh->NumVertices);
		triangleMesh->Indices.reserve((std::size_t)(GetDashFormatSize(triangleMesh->IndexType)) * triangleMesh->NumIndices);

		std::size_t vertexIndex = 0;
		std::size_t vertexDataBegin = vertexIndex * triangleMesh->VertexStride;

		//Top left
		WriteData(topLeft, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(mNormal, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(mTangent, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(Vector2f{0, 0}, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);

		++vertexIndex;

		//Top right
		vertexDataBegin = vertexIndex * triangleMesh->VertexStride;
		WriteData(topRight, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(mNormal, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(mTangent, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(Vector2f{ 1, 0 }, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);

		++vertexIndex;
		
		//bottom left
		vertexDataBegin = vertexIndex * triangleMesh->VertexStride;
		WriteData(bottomLeft, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(mNormal, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(mTangent, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(Vector2f{ 0, 1 }, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);

		++vertexIndex;

		//bottom right
		vertexDataBegin = vertexIndex * triangleMesh->VertexStride;
		WriteData(bottomRight, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(mNormal, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(mTangent, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(Vector2f{ 1, 1 }, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);

		uint16_t indices[6] = { 0, 1, 2, 1, 3, 2 };
		std::memcpy(triangleMesh->Indices.data(), indices, sizeof(indices));

		return triangleMesh;
	}


}

