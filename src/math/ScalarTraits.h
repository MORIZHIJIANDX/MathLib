#pragma once

#include <limits>

namespace Dash
{
	namespace Math
	{
		template<typename Scalar>
		struct ScalarTraits{};

        template<>
        struct ScalarTraits<std::size_t>
        {
            static std::size_t Pi()
            {
                return 3;
            }

            static std::size_t Infinity()
            {
                return std::numeric_limits<std::size_t>::infinity();
            }

            static std::size_t Epsilon()
            {
                return std::numeric_limits<std::size_t>::epsilon();
            }

            static std::size_t Max()
            {
                return std::numeric_limits<std::size_t>::max();
            }

            static std::size_t Lowest()
            {
                return std::numeric_limits<std::size_t>::lowest();
            }
        };

		template<>
		struct ScalarTraits<float> 
		{
            static float Pi()
            {
                return 3.14159265358979323846f;
            }

            static float Infinity()
            {
                return std::numeric_limits<float>::infinity();
            }

            static float Epsilon()
            {
                return std::numeric_limits<float>::epsilon();
            }

            static float Max()
            {
                return std::numeric_limits<float>::max();
            }

            static float Lowest()
            {
                return std::numeric_limits<float>::lowest();
            }
		};


        template<>
        struct ScalarTraits<double>
        {
            static double Pi()
            {
                return 3.14159265358979323846;
            }

            static double Infinity()
            {
                return std::numeric_limits<double>::infinity();
            }

            static double Epsilon()
            {
                return std::numeric_limits<double>::epsilon();
            }

            static double Max()
            {
                return std::numeric_limits<double>::max();
            }

            static double Lowest()
            {
                return std::numeric_limits<double>::lowest();
            }
        };


        template<>
        struct ScalarTraits<long double>
        {
            static long double Pi()
            {
                return 3.1415926535897932384626433832795029L;
            }

            static long double Infinity()
            {
                return std::numeric_limits<long double>::infinity();
            }

            static long double Epsilon()
            {
                return std::numeric_limits<long double>::epsilon();
            }

            static long double Max()
            {
                return std::numeric_limits<long double>::max();
            }

            static long double Lowest()
            {
                return std::numeric_limits<long double>::lowest();
            }
        };

	}
}
