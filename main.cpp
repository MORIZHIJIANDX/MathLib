
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
	DMath::Float4 data = DMath::Float4{};
	data.Fill(4);

	DMath::Float4 data2{ DMath::Unit<1>{} };

	float Comot = DMath::Dot(data, data2);

	float m = static_cast<float>(DMath::Zero{});

	Print(DMath::Pow(data, 1 / 2.2f));

	//Print(Comot);

	DMath::ScalarArray<float, 4> tm{1, 2, 5, 1};
	DMath::ScalarArray<float, 4> rm{ 0, 1, 1, 1 };

	DMath::ScalarArray<float, 4> mulre{};

	std::cout << DMath::Frac(-3.8);
	
	
	std::cin.get();
}