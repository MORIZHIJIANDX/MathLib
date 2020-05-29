#pragma once

namespace Dash
{
	namespace Math
	{
		template<typename Scalar> Scalar Area(const Bounds<Scalar, 2>& b) noexcept;

		template<typename Scalar> std::size_t MaximumExtent(const Bounds<Scalar, 2>& b) noexcept;

		template<typename Scalar>
		FORCEINLINE Scalar Area(const Bounds<Scalar, 2>& b) noexcept
		{
			ScalarArray<Scalar, 2> d = Diagonal(b);
			return (d.x * d.y);
		}

        template<typename Scalar>
		FORCEINLINE std::size_t MaximumExtent(const Bounds<Scalar, 2>& b) noexcept
		{
			ScalarArray<Scalar, 2> diag = Diagonal(b);
			if (diag.x > diag.y)
				return 0;
			else
				return 1;
		}

        class Bounds2iIterator : public std::forward_iterator_tag {
        public:
            Bounds2iIterator(const Bounds<int, 2>& b, const ScalarArray<int, 2>& pt)
                : p(pt), bounds(&b) {}
            Bounds2iIterator operator++() {
                Advance();
                return *this;
            }
            Bounds2iIterator operator++(int) {
                Bounds2iIterator old = *this;
                Advance();
                return old;
            }
            bool operator==(const Bounds2iIterator& bi) const {
                return p == bi.p && bounds == bi.bounds;
            }
            bool operator!=(const Bounds2iIterator& bi) const {
                return p != bi.p || bounds != bi.bounds;
            }

            ScalarArray<int, 2> operator*() const { return p; }

        private:
            void Advance() {
                ++p.x;
                if (p.x == bounds->Upper.x) {
                    p.x = bounds->Lower.x;
                    ++p.y;
                }
            }
            ScalarArray<int, 2> p;
            const Bounds<int, 2>* bounds;
        };

	}
}

