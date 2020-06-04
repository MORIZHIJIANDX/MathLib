#include "Sphere.h"
#include "../math/Intersection.h"

namespace Dash
{
	Sphere::Sphere(const Math::Transform& objectToWorld, const Math::Transform& worldToObject, Math::Scalar radius)
		: Shape(objectToWorld, worldToObject)
	{
	}

	Sphere::~Sphere()
	{
	}

	
	bool Sphere::Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept
	{
		Math::Scalar tp;
		Math::Vector3f sphereCenter = GetCenter();
		if (Math::RaySphereIntersection(r, sphereCenter, mRadius, tp))
		{
			if (tp > r.TMax)
				return false;

			if (t != nullptr)
				*t = tp;

			if (hitInfo != nullptr)
			{
				Math::Vector3f hitPos = r(tp);

				hitInfo->Position = sphereCenter - mRadius * r.Direction;
				hitInfo->Normal = -r.Direction;
				
				//https://computergraphics.stackexchange.com/questions/5498/compute-sphere-tangent-for-normal-mapping

				
			}

		}

		return false;
	}

	Math::Vector3f Sphere::GetCenter() const noexcept
	{
		return Math::Vector3f{ ObjectToWorld.GetPosition() };
	}

	Math::Scalar Sphere::GetRadius() const noexcept
	{
		return mRadius;
	}

	Math::BoundingBox Sphere::ObjectBound() const noexcept
	{
		return Math::BoundingBox{ Math::Vector3f{-mRadius, -mRadius, -mRadius} , Math::Vector3f{mRadius, mRadius, mRadius} };
	}

	Math::BoundingBox Sphere::WorldBound() const noexcept
	{
		return Math::BoundingBox{ GetCenter() + Math::Vector3f{-mRadius, -mRadius, -mRadius} ,
			GetCenter() + Math::Vector3f{mRadius, mRadius, mRadius} };
	}
}
