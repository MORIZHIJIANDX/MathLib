
#include "src/math/MathType.h"
#include "src/math/Transform.h"
#include "src/shapes/Shape.h"
#include "src/shapes/Triangle.h"
#include "src/shapes/Sphere.h"
#include <iostream>

namespace DMath = Dash::Math;

template<typename T, std::size_t N>
void Print(const DMath::ScalarArray<T, N>& v)
{
	for (size_t i = 0; i < v.GetSize(); i++)
	{
		std::cout << v[i] << " ";
	}

	std::cout << std::endl;
}

std::shared_ptr<Dash::TriangleMesh> CreateTriangleMesh()
{
	std::shared_ptr<Dash::TriangleMesh> triangleMesh = std::make_shared<Dash::TriangleMesh>();
	triangleMesh->IndexType = Dash::DASH_FORMAT::R16_UINT;
	triangleMesh->NumVertices = 3;
	triangleMesh->NumIndices = 3;
	triangleMesh->MeshParts.emplace_back(0, 3, 0, 3, 0);

	triangleMesh->InputElements.emplace_back("POSITION", 0, Dash::DASH_FORMAT::R32G32B32_FLOAT, 0);
	triangleMesh->InputElements.emplace_back("NORMAL", 0, Dash::DASH_FORMAT::R32G32B32_FLOAT, 12);
	triangleMesh->InputElements.emplace_back("TANGENT", 0, Dash::DASH_FORMAT::R32G32B32_FLOAT, 24);
	triangleMesh->InputElements.emplace_back("TEXCOORD", 0, Dash::DASH_FORMAT::R32G32_FLOAT, 36);

	triangleMesh->VertexStride = 0;
	for (size_t i = 0; i < triangleMesh->InputElements.size(); i++)
	{
		triangleMesh->InputElementMap.insert(std::pair(triangleMesh->InputElements[i].SemanticName,
			triangleMesh->InputElements[i].AlignedByteOffset));

		triangleMesh->VertexStride += (uint32_t)GetDashFormatSize(triangleMesh->InputElements[i].Format);
	}


	std::size_t positionOffset = triangleMesh->InputElementMap["POSITION"];
	std::size_t normalOffset = triangleMesh->InputElementMap["NORMAL"];
	std::size_t tangentOffset = triangleMesh->InputElementMap["TANGENT"];
	std::size_t texCoordOffset = triangleMesh->InputElementMap["TEXCOORD"];

	DMath::Vector3f pos1{ 0.0f, 0.5f, 0.0f };
	DMath::Vector3f pos2{ 0.5f, 0.0f, 0.0f };
	DMath::Vector3f pos3{ -0.5f, 0.0f, 0.0f };

	DMath::Vector2f uv1{ 0.5f, 0.0f };
	DMath::Vector2f uv2{ 1.0f, 1.0f };
	DMath::Vector2f uv3{ 0.0f, 1.0f };

	DMath::Vector3f normal{ 0.0f, 0.0f, -1.0f };
	DMath::Vector3f tangent{ 1.0f, 0.0f, 0.0f };

	triangleMesh->Vertices.reserve((std::size_t)(triangleMesh->VertexStride) * 3);

	WriteData(pos1, triangleMesh->Vertices.data(), 0 * triangleMesh->VertexStride + positionOffset);
	WriteData(normal, triangleMesh->Vertices.data(), 0 * triangleMesh->VertexStride + normalOffset);
	WriteData(tangent, triangleMesh->Vertices.data(), 0 * triangleMesh->VertexStride + tangentOffset);
	WriteData(uv1, triangleMesh->Vertices.data(), 0 * triangleMesh->VertexStride + texCoordOffset);

	WriteData(pos2, triangleMesh->Vertices.data(), 1 * triangleMesh->VertexStride + positionOffset);
	WriteData(normal, triangleMesh->Vertices.data(), 1 * triangleMesh->VertexStride + normalOffset);
	WriteData(tangent, triangleMesh->Vertices.data(), 1 * triangleMesh->VertexStride + tangentOffset);
	WriteData(uv2, triangleMesh->Vertices.data(), 1 * triangleMesh->VertexStride + texCoordOffset);

	WriteData(pos3, triangleMesh->Vertices.data(), (std::size_t)2 * triangleMesh->VertexStride + positionOffset);
	WriteData(normal, triangleMesh->Vertices.data(), (std::size_t)2 * triangleMesh->VertexStride + normalOffset);
	WriteData(tangent, triangleMesh->Vertices.data(), (std::size_t)2 * triangleMesh->VertexStride + tangentOffset);
	WriteData(uv3, triangleMesh->Vertices.data(), (std::size_t)2 * triangleMesh->VertexStride + texCoordOffset);

	triangleMesh->Indices.resize(sizeof(std::uint32_t) * 3);

	uint16_t indices[3] = { 0, 1, 2 };

	std::memcpy(triangleMesh->Indices.data(), indices, sizeof(indices));

	return triangleMesh;
}

int main()
{
	std::shared_ptr<Dash::TriangleMesh> triangleMesh = CreateTriangleMesh();

	DMath::Transform trans{ DMath::Identity{} };
	Dash::Triangle triangle{ trans , trans, triangleMesh, 0 };
	std::shared_ptr<Dash::TriangleMesh> testTriangle = triangle.ConvertToTriangleMesh();

	DMath::Vector2f uv;


	std::shared_ptr<Dash::Sphere> sphere = std::make_shared<Dash::Sphere>(trans, trans, 1.0f);

	testTriangle = sphere->ConvertToTriangleMesh();

	testTriangle->GetVertexTexCoord(uv, 2);
	std::cout << uv << std::endl;

	float pot = 99.5f;
	float got = 0.0f;

	std::vector<uint8_t> data;
	data.reserve(4);

	WriteData(pot, data.data());
	GetData(got, data.data());

	std::cout << got << std::endl;

	std::cin.get();
}