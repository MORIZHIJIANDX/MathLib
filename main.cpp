
#include "src/math/MathType.h"
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


	std::cout << DMath::Mul(DMath::Vector3f{1,2,3}, DMath::Matrix3x3{ DMath::Identity{} }) << std::endl;

	std::cout << DMath::Normalize(DMath::Vector3f{ 1,2,3 }) << std::endl;

	DMath::Bounds<float, 3> bound{ DMath::Vector3f{0, 0, 0}, DMath::Vector3f{2, 2, 2} };
	
	std::cout << bound.Lower << bound.Upper << std::endl;
	
	std::cin.get();
}