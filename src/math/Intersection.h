#pragma once

#include "MathType.h"

namespace Dash
{
	namespace Math
	{
		bool RayTriangleIntersection(const Ray& r, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, Scalar& u, Scalar& v, Scalar& t) noexcept;
		bool RayTriangleIntersection(const Ray& r, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2) noexcept;

		bool RayBoundingBoxIntersection(const Ray& r, const BoundingBox& b, Scalar& t0, Scalar& t1) noexcept;

		bool RaySphereIntersection(const Ray& r, const Vector3f& center, Scalar radius,  Scalar& t) noexcept;

		bool Quadratic(Scalar a, Scalar b, Scalar c, Scalar& t0, Scalar& t1) noexcept;
	
		FORCEINLINE bool RayTriangleIntersection(const Ray& r, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2) noexcept
		{
			Scalar u, v, t;
			return RayTriangleIntersection(r, v0, v1, v2, u, v, t);
		}

		FORCEINLINE bool RayTriangleIntersection(const Ray& r, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2,
			Scalar& u, Scalar& v, Scalar& t) noexcept
		{
			Vector3f v0v1 = v1 - v0;
			Vector3f v0v2 = v2 - v0;
			Vector3f pvec = Cross(r.Direction, v0v2);
			Scalar det = Dot(v0v1, pvec);

			//Ray triangle parallel
			if (Abs(det) < ScalarTraits<Scalar>::Epsilon())
				return false;

			Scalar invDet = Scalar{ 1 } / det;

			Vector3f tvec = r.Origin - v0;
			u = Dot(tvec, pvec) * invDet;
			if (u < 0 || u > 1) return false;

			Vector3f qvec = Cross(tvec, v0v1);
			v = Dot(r.Direction, qvec) * invDet;
			if (v < 0 || v > 1) return false;

			t = Dot(v0v2, qvec) * invDet;

			return true;
		}

		FORCEINLINE Scalar ErrorGamma(int n) {
			return (n * ScalarTraits<Scalar>::Epsilon()) / (1 - n * ScalarTraits<Scalar>::Epsilon());
		}

		FORCEINLINE bool RayBoundingBoxIntersection(const Ray& r, const BoundingBox& b, Scalar& t0, Scalar& t1) noexcept
		{
			Scalar tMin = 0, tMax = r.TMax;
			for (std::size_t i = 0; i < 3; i++)
			{
				Scalar invRayDir = Scalar{ 1 } / r.Direction[i];
				Scalar tNear = (b.Lower[i] - r.Origin[i]) * invRayDir;
				Scalar tFar = (b.Upper[i] - r.Origin[i]) * invRayDir;

				if (tNear > tFar)
					Swap(tNear, tFar);

				tFar *= 1 + 2 * ErrorGamma(3);

				tMin = tNear > tMin ? tNear : tMin;
				tMax = tFar < tMax ? tFar : tMax;

				if (tMin > tMax)
					return false;
			}

			t0 = tMin;
			t1 = tMax;

			return true;
		}

		FORCEINLINE bool RaySphereIntersection(const Ray& r, const Vector3f& center, Scalar radius, Scalar& t) noexcept
		{
			Vector3f oc = r.Origin - center;
			Scalar a = Dot(r.Direction, r.Direction);
			Scalar halfB = Dot(oc, r.Direction);
			Scalar c = Dot(oc, oc) - radius * radius;

			Scalar discrim = halfB * halfB - a * c;

			if (discrim < 0) return false;
			
			t = (-halfB - Sqrt(discrim)) / a;
			return true;
		}

		bool Quadratic(Scalar a, Scalar b, Scalar c, Scalar& t0, Scalar& t1) noexcept
		{
			return false;
		}
	}
}