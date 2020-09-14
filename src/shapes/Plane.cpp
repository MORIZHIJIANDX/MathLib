#include "Plane.h"
#include "../math/Intersection.h"

namespace Dash
{
	Plane::Plane(const FTransform& objectToWorld, const FTransform& worldToObject, const FVector3f& normal,
		const FVector3f& topLeft, const FVector3f& topRight, const FVector3f& bottomLeft)
		: Shape(objectToWorld, worldToObject)
		, mNormal(FMath::Normalize(normal))
		, mTopLeft(topLeft)
	{
		FVector3f u = topRight - topLeft;
		FVector3f v = bottomLeft - topLeft;

		mWidth = FMath::Length(u);
		mHeight = FMath::Length(v);

		ASSERT(!FMath::IsZero(mWidth));
		ASSERT(!FMath::IsZero(mHeight));

		mTangent = u / mWidth;
		mTangent = mTangent - FMath::Dot(mTangent, mNormal) * mNormal;
		mBinormal = FMath::Cross(mNormal, mTangent);
		mBinormal = mBinormal - FMath::Dot(mBinormal, mNormal) * mNormal - FMath::Dot(mBinormal, mTangent) * mTangent;
	}

	Plane::~Plane()
	{
	}

	bool Plane::Intersection(const FRay& r, Scalar* t, HitInfo* hitInfo) const noexcept
	{
		Scalar tp;

		FRay objectRay = WorldToObject.TransformRay(r);

		if (FMath::RayPlaneIntersection(objectRay, mNormal, mTopLeft, tp))
		{
			if (tp > r.TMax || tp < r.TMin)
				return false;

			FVector3f point = objectRay(tp);

			FVector3f offsetToTopLeft = point - mTopLeft;
			Scalar u = FMath::Dot(mTangent, offsetToTopLeft);
			Scalar v = FMath::Dot(mBinormal, offsetToTopLeft);

			if ((u >= Scalar{ 0 } && u <= mWidth) && (v >= Scalar{ 0 } && v <= mHeight))
			{
				if (t != nullptr)
					*t = tp;

				if (hitInfo != nullptr)
				{
					hitInfo->Position = ObjectToWorld.TransformPoint(point);
					hitInfo->Normal = ObjectToWorld.TransformNormal(mNormal);
					hitInfo->Tangent = ObjectToWorld.TransformVector(mTangent);
					hitInfo->TexCoord = FVector2f{ u / mWidth, v / mHeight };
				}

				return true;
			}
		}	

		return false;
	}

	FVector3f Plane::GetCenter() const noexcept
	{
		return ObjectToWorld.TransformPoint(mTopLeft) + Scalar{ 0.5 } * (mWidth * ObjectToWorld.TransformVector(mTangent) + mHeight * ObjectToWorld.TransformVector(mBinormal));
	}

	FVector3f Plane::GetNormal() const noexcept
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

	FBoundingBox Plane::ObjectBound() const noexcept
	{
		FVector3f topLeft = mTopLeft;
		FVector3f topRight = topLeft + mTangent * mWidth;

		FVector3f bottomLeft = topLeft + mBinormal * mHeight;
		FVector3f bottomRight = topLeft + mTangent * mWidth + mBinormal * mHeight;

		FBoundingBox bounds = FMath::Union(topLeft, topRight);
		bounds = FMath::Union(bounds, bottomLeft);
		bounds = FMath::Union(bounds, bottomRight);

		FVector3f epsilon = FVector3f{ FIdentity{} } * TScalarTraits<Scalar>::Epsilon();
		return FBoundingBox{ bounds.Lower - epsilon, bounds.Upper + epsilon };
	}

	FBoundingBox Plane::WorldBound() const noexcept
	{
		FVector3f worldTangent = ObjectToWorld.TransformVector(mTangent);
		FVector3f worldBinrmal = ObjectToWorld.TransformVector(mBinormal);

		FVector3f topLeft = ObjectToWorld.TransformPoint(mTopLeft);
		FVector3f topRight = topLeft + worldTangent * mWidth;

		FVector3f bottomLeft = topLeft + worldBinrmal * mHeight;
		FVector3f bottomRight = topLeft + worldTangent * mWidth + worldBinrmal * mHeight;

		FBoundingBox bounds = FMath::Union(topLeft, topRight);
		bounds = FMath::Union(bounds, bottomLeft);
		bounds = FMath::Union(bounds, bottomRight);

		FVector3f epsilon = FVector3f{ FIdentity{} } * TScalarTraits<Scalar>::Epsilon();
		return FBoundingBox{ bounds.Lower - epsilon, bounds.Upper + epsilon };
	}

	std::shared_ptr<TriangleMesh> Plane::ConvertToTriangleMesh() const noexcept
	{
		return CreateTessellatedTriangleMesh(1, 1);
	}

	std::shared_ptr<TriangleMesh> Plane::CreateTessellatedTriangleMesh(uint16_t levels, uint16_t slices) const noexcept
	{
		FVector3f topLeft = mTopLeft;
		FVector3f topRight = topLeft + mTangent * mWidth;

		FVector3f bottomLeft = topLeft + mBinormal * mHeight;
		FVector3f bottomRight = topLeft + mTangent * mWidth + mBinormal * mHeight;

		std::shared_ptr<TriangleMesh> triangleMesh = std::make_shared<TriangleMesh>();
		triangleMesh->IndexType = EDASH_FORMAT::R16_UINT;
		triangleMesh->NumVertices = (static_cast<size_t>(levels) + 1) * (static_cast<size_t>(slices) + 1);
		triangleMesh->NumIndices = static_cast<size_t>(levels) * static_cast<size_t>(slices) * 2;
		triangleMesh->MeshParts.emplace_back(0, triangleMesh->NumVertices, 0, triangleMesh->NumIndices, 0);

		triangleMesh->InputElements.emplace_back("POSITION", 0, Dash::EDASH_FORMAT::R32G32B32_FLOAT, 0);
		triangleMesh->InputElements.emplace_back("NORMAL", 0, Dash::EDASH_FORMAT::R32G32B32_FLOAT, 12);
		triangleMesh->InputElements.emplace_back("TANGENT", 0, Dash::EDASH_FORMAT::R32G32B32_FLOAT, 24);
		triangleMesh->InputElements.emplace_back("TEXCOORD", 0, Dash::EDASH_FORMAT::R32G32_FLOAT, 36);

		triangleMesh->VertexStride = 0;
		for (size_t i = 0; i < triangleMesh->InputElements.size(); i++)
		{
			triangleMesh->InputElementMap.insert(std::pair(triangleMesh->InputElements[i].SemanticName,
				triangleMesh->InputElements[i].AlignedByteOffset));

			triangleMesh->VertexStride += GetByteSizeForFormat(triangleMesh->InputElements[i].Format);
		}

		size_t positionOffset = triangleMesh->InputElementMap["POSITION"];
		size_t normalOffset = triangleMesh->InputElementMap["NORMAL"];
		size_t tangentOffset = triangleMesh->InputElementMap["TANGENT"];
		size_t texCoordOffset = triangleMesh->InputElementMap["TEXCOORD"];

		triangleMesh->Vertices.reserve((size_t)(triangleMesh->VertexStride) * triangleMesh->NumVertices);
		triangleMesh->Indices.reserve((size_t)(GetByteSizeForFormat(triangleMesh->IndexType)) * triangleMesh->NumIndices);

		//Write Vertex Attribute
		size_t vertexIndex = 0;
		size_t vertexDataBegin = vertexIndex * triangleMesh->VertexStride;

		for (uint16_t j = 0; j < (levels + uint16_t{ 1 }); j++)
		{
			for (uint16_t i = 0; i < (slices + uint16_t{ 1 }); i++)
			{
				FVector2f uv{ i / static_cast<Scalar>(slices) , j / static_cast<Scalar>(levels) };
				FVector3f position = topLeft + mTangent * mWidth * uv.x + mBinormal * mHeight * uv.y;
				WriteData(position, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
				WriteData(mNormal, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
				WriteData(mTangent, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
				WriteData(uv, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);

				++vertexIndex;
				vertexDataBegin = vertexIndex * triangleMesh->VertexStride;
			}
		}


		vertexIndex = 0;

		//Write Index
		for (uint16_t j = 0; j < levels; j++)
		{
			for (uint16_t i = 0; i < slices; i++)
			{
				uint16_t index0 = j * (slices + 1) + i;
				uint16_t index1 = j * (slices + 1) + i + 1;
				uint16_t index2 = (j + 1) * (slices + 1) + i;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);

				index0 = j * (slices + 1) + i + 1;
				index1 = (j + 1) * (slices + 1) + i + 1;
				index2 = (j + 1) * (slices + 1) + i;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);
			}
		}

		return triangleMesh;
	}


}

