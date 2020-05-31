
#include "src/math/MathType.h"
#include "src/math/Transform.h"
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

int main()
{
	DMath::Vector4f data = DMath::Vector4f{};
	data.Fill(4);


	DMath::Vector4f data2{ DMath::Unit<1>{} };

	float Comot = DMath::Dot(data, data2);

	float m = static_cast<float>(DMath::Zero{});

	Print(DMath::Pow(data, 1 / 2.2f));

	DMath::ScalarArray<float, 4> tm{1, 2, 5, 1};
	DMath::ScalarArray<float, 4> rm{ 0, 1, 1, 1 };

	DMath::ScalarArray<float, 4> mulre{};

	auto mat = DMath::ScaleMatrix4x4(DMath::Vector3f{ 1.0f, 2.0f, 3.0f });

	std::cout <<  DMath::RotateMatrix4x4<float>(DMath::Quaternion(DMath::Identity{})) * DMath::RotateMatrix4x4<float>(DMath::Quaternion(DMath::Identity{})) << std::endl;

	DMath::Vector3f scale;
	DMath::Vector3f translation;
	DMath::Quaternion rotation;

	DecomposeAffineMatrix4x4(scale, rotation, translation, mat);

	std::cout << scale << std::endl;
	std::cout << translation << std::endl;
	std::cout << rotation << std::endl;


	std::cout << mat << std::endl;

	struct POI
	{
		char p;
		DMath::Vector3f c;
	};

	std::cout << "P offset : " << offsetof(POI, p) << std::endl;
	std::cout << "C offset : " << offsetof(POI, c) << std::endl;
	
	std::cin.get();
}