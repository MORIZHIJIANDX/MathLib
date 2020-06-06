#include "Plane.h"
#include "../math/Intersection.h"

namespace Dash
{
	Plane::Plane(const Math::Transform& objectToWorld, const Math::Transform& worldToObject, const Math::Vector3f& normal,
		const Math::Vector3f& topLeft, const Math::Vector3f& topRight, const Math::Vector3f& bottomLeft)
		: Shape(objectToWorld, worldToObject)
		, mNormal(Math::Normalize(normal))
		, mTopLeft(topLeft)
	{
		Math::Vector3f u = topRight - topLeft;
		Math::Vector3f v = bottomLeft - topLeft;

		mWidth = Math::Length(u);
		mHeight = Math::Length(v);

		ASSERT(!Math::IsZero(mWidth));
		ASSERT(!Math::IsZero(mHeight));

		mTangent = u / mWidth;
		mTangent = mTangent - Math::Dot(mTangent, mNormal) * mNormal;
		mBinormal = Math::Cross(mNormal, mTangent);
		mBinormal = mBinormal - Math::Dot(mBinormal, mNormal) * mNormal - Math::Dot(mBinormal, mTangent) * mTangent;
	}

	Plane::~Plane()
	{
	}

	bool Plane::Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept
	{
		Math::Scalar tp;

		Math::Ray objectRay = WorldToObject.TransformRay(r);

		if (Math::RayPlaneIntersection(objectRay, mNormal, mTopLeft, tp))
		{
			Math::Vector3f point = objectRay(tp);

			Math::Vector3f offsetToTopLeft = point - mTopLeft;
			Math::Scalar u = Dot(mTangent, offsetToTopLeft);
			Math::Scalar v = Dot(mBinormal, offsetToTopLeft);

			if ((u >= Math::Scalar{ 0 } && u <= mWidth) && (v >= Math::Scalar{ 0 } && v <= mHeight))
			{
				if (t != nullptr)
					*t = tp;

				if (hitInfo != nullptr)
				{
					hitInfo->Position = ObjectToWorld.TransformPoint(point);
					hitInfo->Normal = ObjectToWorld.TransformNormal(mNormal);
					hitInfo->Tangent = ObjectToWorld.TransformVector(mTangent);
					hitInfo->TexCoord = Math::Vector2f{ u / mWidth, v / mHeight };
				}
			}
		}	

		return false;
	}

	Math::Vector3f Plane::GetCenter() const noexcept
	{
		return ObjectToWorld.TransformPoint(mTopLeft) + Math::Scalar{ 0.5 } * (mWidth * ObjectToWorld.TransformVector(mTangent) + mHeight * ObjectToWorld.TransformVector(mBinormal));
	}

	Math::Vector3f Plane::GetNormal() const noexcept
	{
		return ObjectToWorld.TransformNormal(mNormal);
	}

	Math::Scalar Plane::GetWidth() const noexcept
	{
		return mWidth;
	}

	Math::Scalar Plane::GetHeight() const noexcept
	{
		return mHeight;
	}


}

