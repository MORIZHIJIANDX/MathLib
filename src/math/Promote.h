#pragma once

namespace Dash {
	namespace Math {

		template<typename Scalar1, typename Scalar2>
		struct Promote
		{
			using RT = Scalar1;
		};

		template<>
		struct Promote<int, float>
		{
			using RT = float;
		};

		template<>
		struct Promote<int, double>
		{
			using RT = double;
		};

		template<>
		struct Promote<int, long double>
		{
			using RT = long double;
		};

		template<>
		struct Promote<int, long long>
		{
			using RT = long long;
		};

		template<>
		struct Promote<float, double>
		{
			using RT = double;
		};

		template<>
		struct Promote<float, long double>
		{
			using RT = long double;
		};

		template<>
		struct Promote<double, long double>
		{
			using RT = long double;
		};

	}
}
