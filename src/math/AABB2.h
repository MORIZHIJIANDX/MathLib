#pragma once

namespace Dash
{
    namespace Math
    {
        template<typename Scalar> Scalar Width(const AABB<Scalar, 2>& b) noexcept;

        template<typename Scalar> Scalar Height(const AABB<Scalar, 2>& b) noexcept;

        template<typename Scalar> Scalar Area(const AABB<Scalar, 2>& b) noexcept;

        template<typename Scalar> std::size_t MaximumExtent(const AABB<Scalar, 2>& b) noexcept;

        template<typename Scalar>
        FORCEINLINE Scalar Width(const AABB<Scalar, 2>& b) noexcept
        {
            return b.Upper.x - b.Lower.x;
        }

        template<typename Scalar>
        FORCEINLINE Scalar Height(const AABB<Scalar, 2>& b) noexcept
        {
            return b.Upper.y - b.Lower.y;
        }

        template<typename Scalar>
        FORCEINLINE Scalar Area(const AABB<Scalar, 2>& b) noexcept
        {
            ScalarArray<Scalar, 2> d = Diagonal(b);
            return (d.x * d.y);
        }

        template<typename Scalar>
        FORCEINLINE std::size_t MaximumExtent(const AABB<Scalar, 2>& b) noexcept
        {
            ScalarArray<Scalar, 2> diag = Diagonal(b);
            if (diag.x > diag.y)
                return 0;
            else
                return 1;
        }

        class AABB2iIterator : public std::forward_iterator_tag {
        public:
            AABB2iIterator(const AABB<int, 2>& b, const ScalarArray<int, 2>& pt)
                : p(pt), AABB(&b) {}
            AABB2iIterator operator++() {
                Advance();
                return *this;
            }
            AABB2iIterator operator++(int) {
                AABB2iIterator old = *this;
                Advance();
                return old;
            }
            bool operator==(const AABB2iIterator& bi) const {
                return p == bi.p && AABB == bi.AABB;
            }
            bool operator!=(const AABB2iIterator& bi) const {
                return p != bi.p || AABB != bi.AABB;
            }

            ScalarArray<int, 2> operator*() const { return p; }

        private:
            void Advance() {
                ++p.x;
                if (p.x == AABB->Upper.x) {
                    p.x = AABB->Lower.x;
                    ++p.y;
                }
            }
            ScalarArray<int, 2> p;
            const AABB<int, 2>* AABB;
        };

    }
}

