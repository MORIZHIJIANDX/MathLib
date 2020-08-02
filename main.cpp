
#include "src/math/MathType.h"
#include "src/math/Transform.h"
#include "src/shapes/Shape.h"
#include "src/shapes/Triangle.h"
#include "src/shapes/Sphere.h"

#include "src/shapes/Plane.h"

#include "src/graphic/Camera.h"

#include "Image.h"

#include "src/graphic/Window.h"

#include <iostream>
#include <chrono>

#include "src/utility/Keyboard.h"
#include "src/graphic/Application.h"

namespace DMath = Dash::Math;

template<typename T, std::size_t N>
void Print(const Dash::ScalarArray<T, N>& v)
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

	Dash::Vector3f pos1{ 0.0f, 0.5f, 0.0f };
	Dash::Vector3f pos2{ 0.5f, 0.0f, 0.0f };
	Dash::Vector3f pos3{ -0.5f, 0.0f, 0.0f };

	Dash::Vector2f uv1{ 0.5f, 0.0f };
	Dash::Vector2f uv2{ 1.0f, 1.0f };
	Dash::Vector2f uv3{ 0.0f, 1.0f };

	Dash::Vector3f normal{ 0.0f, 0.0f, -1.0f };
	Dash::Vector3f tangent{ 1.0f, 0.0f, 0.0f };

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

void GeometryTest()
{
	std::shared_ptr<Dash::TriangleMesh> triangleMesh = CreateTriangleMesh();

	Dash::Transform trans{ Dash::Identity{} };
	Dash::Triangle triangle{ trans , trans, triangleMesh, 0 };
	std::shared_ptr<Dash::TriangleMesh> testTriangle = triangle.ConvertToTriangleMesh();

	Dash::Vector2f uv;


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
}

Dash::Vector3f GetRayColor(const Dash::Ray& r, std::shared_ptr<Dash::Shape> shape)
{
	Dash::Scalar t;
	Dash::HitInfo hitInfo;

	if (shape->Intersection(r, &t, &hitInfo))
	{
		//return DMath::Vector3f{ hitInfo.TexCoord, 0.0f };
		return Dash::Vector3f{ hitInfo.Normal * 0.5f + Dash::Vector3f{0.5f, 0.5f, 0.5f} };
	}

	Dash::Vector3f normDir = DMath::Normalize(r.Direction);
	Dash::Scalar lerpVal = 0.5f * (normDir.y + 1.0f);
	return DMath::Lerp(Dash::Vector3f{1.0f, 1.0f, 1.0f}, Dash::Vector3f{ 0.5f, 0.7f, 1.0f }, lerpVal);
}

//int main()
//{
//	const Dash::Scalar aspectRatio = 16.0f / 9.0f;
//	const std::size_t imageWidth = 512;
//	const std::size_t imageHeight = static_cast<std::size_t>(imageWidth / aspectRatio);
//
//	Dash::Viewport vp{0.0f, 0.0f, imageWidth, imageHeight, 0.0f, 1.0f};
//
//	Dash::PerspectiveCamera camera{ 90.0f, aspectRatio, 1.0f, 1000.0f, vp };
//
//	camera.SetLookAt(Dash::Vector3f{0.0f, 0.0f, 0.0f}, Dash::Vector3f{ 0.0f, 0.0f, 1.0f }, Dash::Vector3f{ 0.0f, 1.0f, 0.0f });
//
//	std::shared_ptr<Image> tempImage = std::make_shared<Image>(imageWidth, imageHeight, Dash::DASH_FORMAT::R32G32B32_FLOAT);
//	tempImage->ClearImage(Dash::Vector3f{0.5f, 0.5f, 0.5f});
//
//	//DMath::Transform trans{ DMath::Vector3f{1.0f, 1.0f, 1.0f},  DMath::Quaternion{DMath::Identity{}},  DMath::Vector3f{0.0f, 0.0f, 5.0f} };
//	//std::shared_ptr<Dash::Sphere> sphere = std::make_shared<Dash::Sphere>(trans, DMath::Inverse(trans), 1.0f);
//
//	int planeSize = 10;
//
//	Dash::Transform trans{ Dash::Vector3f{1.0f, 1.0f, 1.0f},  Dash::Quaternion{Dash::Identity{}},  Dash::Vector3f{0.0f, -2.0f, 0.0f} };
//	std::shared_ptr<Dash::Plane> plane = std::make_shared<Dash::Plane>(trans, Dash::Math::Inverse(trans), Dash::Vector3f{ 0, 1, 0 }, Dash::Vector3f{ -planeSize, 0, planeSize },
//		Dash::Vector3f{ planeSize, 0, planeSize }, Dash::Vector3f{ -planeSize, 0, -planeSize });
//
//	for (std::size_t i = 0; i < imageHeight; i++)
//	{
//		for (std::size_t j = 0; j < imageWidth; j++)
//		{
//			Dash::Scalar u = j / (Dash::Scalar)(imageWidth - 1);
//			Dash::Scalar v = i / (Dash::Scalar)(imageHeight - 1);
//
//			Dash::Ray r = camera.GenerateRay(u, v);
//
//			tempImage->SetPixel(GetRayColor(r, plane), j, i);
//		}
//	}
//
//	SavePPMImage(tempImage, "render_target.ppm");
//
//	std::cout << "Program End" << std::endl;
//
//	std::cin.get();
//}

//int main()
//{
//
//}

int CALLBACK WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	//Dash::Window window{ hInstance, "TestWindow", "RenderProject" };
	//window.ProcessMessage();

	//using namespace std::chrono_literals;

	//window.CloseWindow();


	//Dash::Keyboard::Get().IsKeyPressed(KeyCode::A);

	Dash::Application app;

	app.Run();
}