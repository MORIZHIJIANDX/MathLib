#include "Sphere.h"
#include "../math/Intersection.h"

namespace Dash
{
	Sphere::Sphere(const FTransform& objectToWorld, const FTransform& worldToObject, Scalar radius,
		uint16_t level, uint16_t slice)
		: Shape(objectToWorld, worldToObject)
		, mRadius(radius)
	{
	}

	Sphere::~Sphere()
	{
	}
	
	bool Sphere::Intersection(const FRay& r, Scalar* t, HitInfo* hitInfo) const noexcept
	{
		Scalar t0, t1;
		FVector3f sphereCenter = GetCenter();
		if (FMath::RaySphereIntersection(r, sphereCenter, mRadius, t0, t1))
		{
			if (t0 > r.TMax || t1 < r.TMin)
				return false;

			if (t0 < r.TMin)
				t0 = r.TMax;

			if (t != nullptr)
				*t = t0;

			if (hitInfo != nullptr)
			{
				FVector3f hitPos = r(t0);

				hitInfo->Normal = (hitPos - sphereCenter) / mRadius;
				hitInfo->Position = sphereCenter + mRadius * hitInfo->Normal;

				//https://computergraphics.stackexchange.com/questions/5498/compute-sphere-tangent-for-normal-mapping

				FVector2f spherical = FMath::CartesianToSpherical(hitInfo->Normal);
				Scalar theta = spherical.x;
				Scalar phi = spherical.y;
				hitInfo->Tangent = FVector3f{-FMath::Sin(theta), FMath::Cos(theta), 0.0f};
			
				hitInfo->TexCoord = FVector2f{ (theta * TScalarTraits<Scalar>::InvPi() + Scalar{1}) * Scalar{0.5}, phi * TScalarTraits<Scalar>::InvPi() };
			}

			return true;
		}

		return false;
	}

	FVector3f Sphere::GetCenter() const noexcept
	{
		return FVector3f{ ObjectToWorld.GetPosition() };
	}

	Scalar Sphere::GetRadius() const noexcept
	{
		return mRadius;
	}

	FBoundingBox Sphere::ObjectBound() const noexcept
	{
		return FBoundingBox{ FVector3f{-mRadius, -mRadius, -mRadius} , FVector3f{mRadius, mRadius, mRadius} };
	}

	FBoundingBox Sphere::WorldBound() const noexcept
	{
		return FBoundingBox{ GetCenter() + FVector3f{-mRadius, -mRadius, -mRadius} ,
			GetCenter() + FVector3f{mRadius, mRadius, mRadius} };
	}

	std::shared_ptr<TriangleMesh> Sphere::ConvertToTriangleMesh() const noexcept
	{
		return CreateTessellatedTriangleMesh(16, 16);
	}

	std::shared_ptr<TriangleMesh> Sphere::CreateTessellatedTriangleMesh(uint16_t levels, uint16_t slices) const noexcept
	{
		std::shared_ptr<TriangleMesh> triangleMesh = std::make_shared<TriangleMesh>();
		triangleMesh->IndexType = EDASH_FORMAT::R16_UINT;
		triangleMesh->NumVertices = size_t{ 2 } + (levels - size_t{ 1 }) * (slices + size_t{ 1 });
		triangleMesh->NumIndices = size_t{ 6 } * (levels - size_t{ 1 }) * slices;
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

		std::size_t positionOffset = triangleMesh->InputElementMap["POSITION"];
		std::size_t normalOffset = triangleMesh->InputElementMap["NORMAL"];
		std::size_t tangentOffset = triangleMesh->InputElementMap["TANGENT"];
		std::size_t texCoordOffset = triangleMesh->InputElementMap["TEXCOORD"];

		triangleMesh->Vertices.reserve((std::size_t)(triangleMesh->VertexStride) * triangleMesh->NumVertices);
		triangleMesh->Indices.reserve(GetByteSizeForFormat(triangleMesh->IndexType) * triangleMesh->NumIndices);

		Scalar theta = 0.0f;
		Scalar phi = 0.0f;

		Scalar phiDelta = TScalarTraits<Scalar>::Pi() / levels;
		Scalar thetaDelta = TScalarTraits<Scalar>::TwoPi() / slices;

		std::size_t vertexIndex = 0;
		std::size_t vertexDataBegin = vertexIndex * triangleMesh->VertexStride;

		//Top point
		WriteData(FVector3f{ 0.0f, mRadius, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(FVector3f{ 0.0f, 1.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(FVector3f{ 1.0f, 0.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(FVector2f{ 0.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);

		++vertexIndex;

		for (std::size_t i = 1; i < levels; ++i)
		{
			phi = phiDelta * i;

			for (std::size_t j = 0; j <= slices; ++j)
			{
				theta = thetaDelta * j;
				FVector3f normal = FMath::SphericalToCartesian(theta, phi);
				FVector3f position = normal * mRadius;
				FVector3f tangent{ -FMath::Sin(theta), 0.0f, FMath::Cos(theta) };
				FVector2f uv{ theta * TScalarTraits<Scalar>::InvTwoPi(), phi * TScalarTraits<Scalar>::InvPi() };

				vertexDataBegin = vertexIndex * triangleMesh->VertexStride;
				WriteData(position, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
				WriteData(normal, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
				WriteData(tangent, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
				WriteData(uv, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);

				++vertexIndex;
			}
		}

		//Bottom point
		vertexDataBegin = vertexIndex * triangleMesh->VertexStride;
		WriteData(FVector3f{ 0.0f, -mRadius, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(FVector3f{ 0.0f, -1.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(FVector3f{ -1.0f, 0.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(FVector2f{ 0.0f, 1.0f }, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);


		//Write Index

		vertexIndex = 0;

		if (levels > 1)
		{
			for (uint16_t j = 1; j < slices; ++j)
			{
				uint16_t index0 = 0;
				uint16_t index1 = j % (slices + 1) + 1;
				uint16_t index2 = j;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);
			}
		}

		for (uint16_t i = 1; i < levels - 1; ++i)
		{
			for (uint16_t j = 1; j <= slices; ++j)
			{
				uint16_t index0 = (i - 1) * (slices + 1) + j;
				uint16_t index1 = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
				uint16_t index2 = i * (slices + 1) + j % (slices + 1) + 1;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);

				index0 = i * (slices + 1) + j % (slices + 1) + 1;
				index1 = i * (slices + 1) + j;
				index2 = (i - 1) * (slices + 1) + j;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);
			}
		}

		if (levels > 1)
		{
			for (uint16_t j = 1; j <= slices; ++j)
			{
				uint16_t index0 = (levels - 2) * (slices + 1) + j;
				uint16_t index1 = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
				uint16_t index2 = (levels - 1) * (slices + 1) + 1;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);
			}
		}

		return triangleMesh;
	}
}
