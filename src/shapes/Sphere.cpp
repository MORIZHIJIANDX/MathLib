#include "Sphere.h"
#include "../math/Intersection.h"

namespace Dash
{
	Sphere::Sphere(const Transform& objectToWorld, const Transform& worldToObject, Scalar radius,
		uint16_t level, uint16_t slice)
		: Shape(objectToWorld, worldToObject)
		, mRadius(radius)
	{
	}

	Sphere::~Sphere()
	{
	}
	
	bool Sphere::Intersection(const Ray& r, Scalar* t, HitInfo* hitInfo) const noexcept
	{
		Scalar t0, t1;
		Vector3f sphereCenter = GetCenter();
		if (Math::RaySphereIntersection(r, sphereCenter, mRadius, t0, t1))
		{
			if (t0 > r.TMax || t1 < r.TMin)
				return false;

			if (t0 < r.TMin)
				t0 = r.TMax;

			if (t != nullptr)
				*t = t0;

			if (hitInfo != nullptr)
			{
				Vector3f hitPos = r(t0);

				hitInfo->Normal = (hitPos - sphereCenter) / mRadius;
				hitInfo->Position = sphereCenter + mRadius * hitInfo->Normal;

				//https://computergraphics.stackexchange.com/questions/5498/compute-sphere-tangent-for-normal-mapping

				Vector2f spherical = Math::CartesianToSpherical(hitInfo->Normal);
				Scalar theta = spherical.x;
				Scalar phi = spherical.y;
				hitInfo->Tangent = Vector3f{-Math::Sin(theta), Math::Cos(theta), 0.0f};
			
				hitInfo->TexCoord = Vector2f{ (theta * ScalarTraits<Scalar>::InvPi() + Scalar{1}) * Scalar{0.5}, phi * ScalarTraits<Scalar>::InvPi() };
			}

			return true;
		}

		return false;
	}

	Vector3f Sphere::GetCenter() const noexcept
	{
		return Vector3f{ ObjectToWorld.GetPosition() };
	}

	Scalar Sphere::GetRadius() const noexcept
	{
		return mRadius;
	}

	BoundingBox Sphere::ObjectBound() const noexcept
	{
		return BoundingBox{ Vector3f{-mRadius, -mRadius, -mRadius} , Vector3f{mRadius, mRadius, mRadius} };
	}

	BoundingBox Sphere::WorldBound() const noexcept
	{
		return BoundingBox{ GetCenter() + Vector3f{-mRadius, -mRadius, -mRadius} ,
			GetCenter() + Vector3f{mRadius, mRadius, mRadius} };
	}

	std::shared_ptr<TriangleMesh> Sphere::ConvertToTriangleMesh() const noexcept
	{
		return CreateTessellatedTriangleMesh(16, 16);
	}

	std::shared_ptr<TriangleMesh> Sphere::CreateTessellatedTriangleMesh(uint16_t levels, uint16_t slices) const noexcept
	{
		std::shared_ptr<TriangleMesh> triangleMesh = std::make_shared<TriangleMesh>();
		triangleMesh->IndexType = DASH_FORMAT::R16_UINT;
		triangleMesh->NumVertices = size_t{ 2 } + (levels - size_t{ 1 }) * (slices + size_t{ 1 });
		triangleMesh->NumIndices = size_t{ 6 } * (levels - size_t{ 1 }) * slices;
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
		triangleMesh->Indices.reserve(GetDashFormatSize(triangleMesh->IndexType) * triangleMesh->NumIndices);

		Scalar theta = 0.0f;
		Scalar phi = 0.0f;

		Scalar phiDelta = ScalarTraits<Scalar>::Pi() / levels;
		Scalar thetaDelta = ScalarTraits<Scalar>::TwoPi() / slices;

		std::size_t vertexIndex = 0;
		std::size_t vertexDataBegin = vertexIndex * triangleMesh->VertexStride;

		//Top point
		WriteData(Vector3f{ 0.0f, mRadius, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(Vector3f{ 0.0f, 1.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(Vector3f{ 1.0f, 0.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(Vector2f{ 0.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);

		++vertexIndex;

		for (std::size_t i = 1; i < levels; ++i)
		{
			phi = phiDelta * i;

			for (std::size_t j = 0; j <= slices; ++j)
			{
				theta = thetaDelta * j;
				Vector3f normal = Math::SphericalToCartesian(theta, phi);
				Vector3f position = normal * mRadius;
				Vector3f tangent{ -Math::Sin(theta), 0.0f, Math::Cos(theta) };
				Vector2f uv{ theta * ScalarTraits<Scalar>::InvTwoPi(), phi * ScalarTraits<Scalar>::InvPi() };

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
		WriteData(Vector3f{ 0.0f, -mRadius, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(Vector3f{ 0.0f, -1.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(Vector3f{ -1.0f, 0.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(Vector2f{ 0.0f, 1.0f }, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);


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
