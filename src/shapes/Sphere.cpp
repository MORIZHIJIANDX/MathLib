#include "Sphere.h"
#include "../math/Intersection.h"

namespace Dash
{
	Sphere::Sphere(const Math::Transform& objectToWorld, const Math::Transform& worldToObject, Math::Scalar radius,
		std::uint16_t level, std::uint16_t slice)
		: Shape(objectToWorld, worldToObject)
		, mRadius(radius)
		, mLevels(level)
		, mSlices(slice)
	{
	}

	Sphere::~Sphere()
	{
	}

	
	bool Sphere::Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept
	{
		Math::Scalar t0, t1;
		Math::Vector3f sphereCenter = GetCenter();
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
				Math::Vector3f hitPos = r(t0);

				hitInfo->Normal = (hitPos - sphereCenter) / mRadius;
				hitInfo->Position = sphereCenter + mRadius * hitInfo->Normal;

				//https://computergraphics.stackexchange.com/questions/5498/compute-sphere-tangent-for-normal-mapping

				Math::Vector2f spherical = Math::CartesianToSpherical(hitInfo->Normal);
				Math::Scalar theta = spherical.x;
				Math::Scalar phi = spherical.y;
				hitInfo->Tangent = Math::Vector3f{-Math::Sin(theta), Math::Cos(theta), 0.0f};
			
				hitInfo->TexCoord = Math::Vector2f{ (theta * Math::ScalarTraits<Math::Scalar>::InvPi() + Math::Scalar{1}) * Math::Scalar{0.5}, phi * Math::ScalarTraits<Math::Scalar>::InvPi() };
			}

			return true;
		}

		return false;
	}

	Math::Vector3f Sphere::GetCenter() const noexcept
	{
		return Math::Vector3f{ ObjectToWorld.GetPosition() };
	}

	Math::Scalar Sphere::GetRadius() const noexcept
	{
		return mRadius;
	}

	Math::BoundingBox Sphere::ObjectBound() const noexcept
	{
		return Math::BoundingBox{ Math::Vector3f{-mRadius, -mRadius, -mRadius} , Math::Vector3f{mRadius, mRadius, mRadius} };
	}

	Math::BoundingBox Sphere::WorldBound() const noexcept
	{
		return Math::BoundingBox{ GetCenter() + Math::Vector3f{-mRadius, -mRadius, -mRadius} ,
			GetCenter() + Math::Vector3f{mRadius, mRadius, mRadius} };
	}

	std::shared_ptr<TriangleMesh> Sphere::ConvertToTriangleMesh() const noexcept
	{
		std::shared_ptr<TriangleMesh> triangleMesh = std::make_shared<TriangleMesh>();
		triangleMesh->IndexType = DASH_FORMAT::R16_UINT;
		triangleMesh->NumVertices = std::uint16_t{ 2 } +(mLevels - std::uint16_t{ 1 }) * (mSlices + std::uint16_t{ 1 });
		triangleMesh->NumIndices = std::uint16_t{ 6 } *(mLevels - std::uint16_t{ 1 }) * mSlices;
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

		Math::Scalar theta = 0.0f;
		Math::Scalar phi = 0.0f;

		Math::Scalar phiDelta = Math::ScalarTraits<Math::Scalar>::Pi() / mLevels;
		Math::Scalar thetaDelta = Math::ScalarTraits<Math::Scalar>::TwoPi() / mSlices;

		std::size_t vertexIndex = 0; 
		std::size_t vertexDataBegin = vertexIndex * triangleMesh->VertexStride;

		//Top point
		WriteData(Math::Vector3f{ 0.0f, mRadius, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(Math::Vector3f{ 0.0f, 1.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(Math::Vector3f{ 1.0f, 0.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(Math::Vector2f{ 0.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);
	
		++vertexIndex;

		for (std::size_t i = 1; i < mLevels; ++i)
		{
			phi = phiDelta * i;

			for (std::size_t j = 0; j <= mSlices; ++j)
			{
				theta = thetaDelta * j;
				Math::Vector3f normal = Math::SphericalToCartesian(theta, phi);
				Math::Vector3f position = normal * mRadius;
				Math::Vector3f tangent{ -Math::Sin(theta), 0.0f, Math::Cos(theta) };
				Math::Vector2f uv{ theta * Math::ScalarTraits<Math::Scalar>::InvTwoPi(), phi * Math::ScalarTraits<Math::Scalar>::InvPi() };

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
		WriteData(Math::Vector3f{ 0.0f, -mRadius, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + positionOffset);
		WriteData(Math::Vector3f{ 0.0f, -1.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + normalOffset);
		WriteData(Math::Vector3f{ -1.0f, 0.0f, 0.0f }, triangleMesh->Vertices.data(), vertexDataBegin + tangentOffset);
		WriteData(Math::Vector2f{ 0.0f, 1.0f }, triangleMesh->Vertices.data(), vertexDataBegin + texCoordOffset);


		vertexIndex = 0;

		if (mLevels > 1)
		{
			for (std::uint16_t j = 1; j < mSlices; ++j)
			{
				std::uint16_t index0 = 0;
				std::uint16_t index1 = j % (mSlices + 1) + 1;
				std::uint16_t index2 = j;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);
			}
		}

		for (std::uint16_t i = 1; i < mLevels - 1; ++i)
		{
			for (std::uint16_t j = 1; j <= mSlices; ++j)
			{
				std::uint16_t index0 = (i - 1) * (mSlices + 1) + j;
				std::uint16_t index1 = (i - 1) * (mSlices + 1) + j % (mSlices + 1) + 1;
				std::uint16_t index2 = i * (mSlices + 1) + j % (mSlices + 1) + 1;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);

				index0 = i * (mSlices + 1) + j % (mSlices + 1) + 1;
				index1 = i * (mSlices + 1) + j ;
				index2 = (i - 1) * (mSlices + 1) + j ;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);
			}
		}

		if (mLevels > 1)
		{
			for (std::uint16_t j = 1; j <= mSlices; ++j)
			{
				std::uint16_t index0 = (mLevels - 2) * (mSlices + 1) + j;
				std::uint16_t index1 = (mLevels - 2) * (mSlices + 1) + j % (mSlices + 1) + 1;
				std::uint16_t index2 = (mLevels - 1) * (mSlices + 1) + 1;
				WriteData(index0, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index1, triangleMesh->Indices.data(), vertexIndex++);
				WriteData(index2, triangleMesh->Indices.data(), vertexIndex++);
			}
		}


		return triangleMesh;
	}
}
