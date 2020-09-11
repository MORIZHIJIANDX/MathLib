#pragma once

#include "MathType.h"

namespace Dash
{

	class FTransform
	{
	public:
		FTransform() noexcept;
		FTransform(FIdentity) noexcept;
		template<typename Scalar> FTransform(const TScalarArray<Scalar, 3>& scale, const FQuaternion& rotation, const TScalarArray<Scalar, 3>& position) noexcept;
		template<typename Scalar> FTransform(const TScalarArray<Scalar, 3>& scale, const TScalarArray<Scalar, 3>& rotationEuler, const TScalarArray<Scalar, 3>& position) noexcept;
		explicit FTransform(const FMatrix4x4& mat) noexcept;
		FTransform(const FMatrix4x4& mat, const FMatrix4x4& inverseMat) noexcept;
		FTransform(const FTransform& t) noexcept;

		FTransform& operator=(const FTransform& t) noexcept;
		FTransform& operator*=(const FTransform& t) noexcept;

		FTransform operator*(const FTransform& t) noexcept;

		operator const FMatrix4x4& () const noexcept;
		operator FMatrix4x4& () noexcept;

		FVector3f GetScale() const noexcept;
		void SetScale(const FVector3f& scale) noexcept;

		FQuaternion GetRotation() const;
		void SetRotation(const FQuaternion& rotation) noexcept;

		FVector3f GetPosition() const noexcept;
		void SetPosition(const FVector3f& pos) noexcept;

		FVector3f GetEuler() const noexcept;
		void SetEuler(Scalar pitch, Scalar yaw, Scalar roll) noexcept;
		void SetEuler(const FVector3f& euler) noexcept;

		void SetLookAt(const FVector3f& eye, const FVector3f& lookAt, const FVector3f& up) noexcept;
		void SetLookTo(const FVector3f& eye, const FVector3f& lookTo, const FVector3f& up) noexcept;

		FVector3f GetForwardAxis() const noexcept;
		FVector3f GetUnitForwardAxis() const noexcept;

		FVector3f GetRightAxis() const noexcept;
		FVector3f GetUnitRightAxis() const noexcept;

		FVector3f GetUpAxis() const noexcept;
		FVector3f GetUnitUpAxis() const noexcept;

		FMatrix4x4 GetMatrix() const noexcept;
		FMatrix4x4 GetInverseMatrix() const noexcept;

		void Scale(const FVector3f& r) noexcept;
		void Scale(Scalar x, Scalar y, Scalar z) noexcept;

		void Rotate(const FQuaternion& r) noexcept;
		void RotateAxis(const FVector3f& axis, Scalar angle) noexcept;
		void RotateAround(const FVector3f& point, const FVector3f& axis, Scalar angle) noexcept;

		void Rotate(const FVector3f& euler) noexcept;
		void Rotate(Scalar x, Scalar y, Scalar z) noexcept;

		void RotateLocal(const FVector3f& euler) noexcept;
		void RotateLocal(Scalar x, Scalar y, Scalar z) noexcept;

		void Translate(const FVector3f& t) noexcept;
		void Translate(Scalar x, Scalar y, Scalar z) noexcept;

		void TranslateLocal(const FVector3f& t) noexcept;
		void TranslateLocal(Scalar x, Scalar y, Scalar z) noexcept;

		FVector3f TransformVector(const FVector3f& v) const noexcept;
		FVector3f TransformPoint(const FVector3f& p) const noexcept;
		FVector3f TransformNormal(const FVector3f& n) const noexcept;

		FVector4f TransformVector(const FVector4f& v) const noexcept;
		FVector4f TransformPoint(const FVector4f& p) const noexcept;
		FVector4f TransformNormal(const FVector4f& n) const noexcept;

		FBoundingBox TransformBoundingBox(const FBoundingBox& b) const noexcept;
		FRay TransformRay(const FRay& r) const noexcept;

	private:
		void UpdateMatrix() const;

		void MakeDirty() { mDirty = true; }

		FVector3f mScale;
		FQuaternion mRotation;
		FVector3f mPosition;

		mutable FMatrix4x4 mMat;
		mutable FMatrix4x4 mInverseMat;

		mutable bool mDirty;
	};









	// Non-member Operators 

	// --Declaration-- //

	bool operator==(const FTransform& a, const FTransform& b) noexcept;








	// Non-member Function

	// --Declaration-- //

	namespace FMath
	{
		FTransform Inverse(const FTransform& t) noexcept;

		FTransform Scale(const FVector3f& s) noexcept;
		FTransform Scale(Scalar x, Scalar y, Scalar z) noexcept;

		FTransform Rotate(const FVector3f& r) noexcept;
		FTransform Rotate(Scalar yaw, Scalar roll, Scalar pitch) noexcept;
		FTransform Rotate(const FQuaternion& r) noexcept;

		FTransform RotateAxis(const FVector3f& axis, Scalar angle) noexcept;
		FTransform RotateAround(const FVector3f& point, const FVector3f& axis, Scalar angle) noexcept;

		FTransform Translate(const FVector3f& t) noexcept;
		FTransform Translate(Scalar x, Scalar y, Scalar z) noexcept;

		FVector3f TransformVector(const FTransform& a, const FVector3f& v) noexcept;
		FVector3f TransformPoint(const FTransform& a, const FVector3f& p) noexcept;
		FVector3f TransformNormal(const FTransform& a, const FVector3f& n) noexcept;

		FVector4f TransformVector(const FTransform& a, const FVector4f& v) noexcept;
		FVector4f TransformPoint(const FTransform& a, const FVector4f& p) noexcept;
		FVector4f TransformNormal(const FTransform& a, const FVector4f& n) noexcept;

		FBoundingBox TransformBoundingBox(const FTransform& a, const FBoundingBox& b) noexcept;
		FRay TransformRay(const FTransform& a, const FRay& r) noexcept;
	}










	// Member Function

	// --Implementation-- //

	FORCEINLINE FTransform::FTransform() noexcept
		: mScale()
		, mRotation()
		, mPosition()
		, mDirty(true)
		, mMat()
		, mInverseMat()
	{
	}

	FORCEINLINE FTransform::FTransform(FIdentity) noexcept
		: mScale(FIdentity{})
		, mRotation(FIdentity{})
		, mPosition(FIdentity{})
		, mDirty(false)
		, mMat(FIdentity{})
		, mInverseMat(FIdentity{})
	{
	}

	template<typename Scalar>
	FORCEINLINE FTransform::FTransform(const TScalarArray<Scalar, 3>& scale, const FQuaternion& rotation, const TScalarArray<Scalar, 3>& position) noexcept
		: mScale(scale)
		, mRotation(rotation)
		, mPosition(position)
		, mDirty(true)
	{
		UpdateMatrix();
	}

	template<typename Scalar>
	FORCEINLINE FTransform::FTransform(const TScalarArray<Scalar, 3>& scale, const TScalarArray<Scalar, 3>& rotationEuler, const TScalarArray<Scalar, 3>& position) noexcept
		: mScale(scale)
		, mRotation(FMath::FromEuler(rotationEuler))
		, mPosition(position)
		, mDirty(true)
	{
		UpdateMatrix();
	}

	FORCEINLINE FTransform::FTransform(const FMatrix4x4& mat) noexcept
		: mMat(mat)
		, mInverseMat(FMath::Inverse(mat))
		, mDirty(false)
	{
		FMath::DecomposeAffineMatrix4x4(mScale, mRotation, mPosition, mMat);
	}

	FORCEINLINE FTransform::FTransform(const FMatrix4x4& mat, const FMatrix4x4& inverseMat) noexcept
		: mMat(mat)
		, mInverseMat(inverseMat)
		, mDirty(false)
	{
		FMath::DecomposeAffineMatrix4x4(mScale, mRotation, mPosition, mMat);
	}

	FORCEINLINE FTransform::FTransform(const FTransform& t) noexcept
		: mScale(t.mScale)
		, mRotation(t.mRotation)
		, mPosition(t.mPosition)
		, mMat(t.mMat)
		, mInverseMat(t.mInverseMat)
		, mDirty(false)
	{
	}

	FORCEINLINE FTransform& FTransform::operator=(const FTransform& t) noexcept
	{
		mScale = t.mScale;
		mRotation = t.mRotation;
		mPosition = t.mPosition;
		mMat = t.mMat;
		mInverseMat = t.mInverseMat;
		mDirty = false;

		return *this;
	}

	FORCEINLINE FTransform& FTransform::operator*=(const FTransform& t) noexcept
	{
		mMat *= t.mMat;
		mInverseMat = FMath::Inverse(mMat);
		mDirty = false;

		FMath::DecomposeAffineMatrix4x4(mScale, mRotation, mPosition, mMat);

		return *this;
	}

	FORCEINLINE FTransform FTransform::operator*(const FTransform& t) noexcept
	{
		return FTransform{
			mMat * t.mMat,
			t.mInverseMat * mInverseMat
		};
	}

	FORCEINLINE FTransform::operator const FMatrix4x4& () const noexcept
	{
		return mMat;
	}

	FORCEINLINE FTransform::operator FMatrix4x4& () noexcept
	{
		return mMat;
	}

	FORCEINLINE FVector3f FTransform::GetScale() const noexcept
	{
		return mScale;
	}

	FORCEINLINE void FTransform::SetScale(const FVector3f& scale) noexcept
	{
		mScale = scale;

		MakeDirty();
	}

	FORCEINLINE FQuaternion FTransform::GetRotation() const
	{
		return mRotation;
	}

	FORCEINLINE void FTransform::SetRotation(const FQuaternion& rotation) noexcept
	{
		mRotation = rotation;

		MakeDirty();
	}

	FORCEINLINE FVector3f FTransform::GetPosition() const noexcept
	{
		return mPosition;
	}

	FORCEINLINE void FTransform::SetPosition(const FVector3f& pos) noexcept
	{
		mPosition = pos;

		MakeDirty();
	}

	FORCEINLINE FVector3f FTransform::GetEuler() const noexcept
	{
		FVector3f euler;
		FMath::ToEuler(euler, mRotation);

		return euler;
	}

	FORCEINLINE void FTransform::SetEuler(Scalar pitch, Scalar yaw, Scalar roll) noexcept
	{
		mRotation = FMath::FromEuler(pitch, yaw, roll);

		MakeDirty();
	}

	FORCEINLINE void FTransform::SetEuler(const FVector3f& euler) noexcept
	{
		mRotation = FMath::FromEuler(euler);

		MakeDirty();
	}

	FORCEINLINE void FTransform::SetLookAt(const FVector3f& eye, const FVector3f& lookAt, const FVector3f& up) noexcept
	{
		SetLookTo(eye, lookAt - eye, up);
	}

	FORCEINLINE void FTransform::SetLookTo(const FVector3f& eye, const FVector3f& lookTo, const FVector3f& up) noexcept
	{
		FVector3f look = FMath::Normalize(lookTo);
		FVector3f right = FMath::Normalize(FMath::Cross(up, look));
		FVector3f newUp = FMath::Cross(look, right);

		mMat.SetRow(0, right);
		mMat.SetRow(1, newUp);
		mMat.SetRow(2, look);
		mMat.SetRow(3, eye);

		mInverseMat = FMath::Inverse(mMat);

		FMath::DecomposeAffineMatrix4x4(mScale, mRotation, mPosition, mMat);

		mDirty = false;
	}

	FORCEINLINE FVector3f FTransform::GetForwardAxis() const noexcept
	{
		return mMat[2].xyz;
	}

	FORCEINLINE FVector3f FTransform::GetUnitForwardAxis() const noexcept
	{
		return FMath::Normalize(mMat[2].xyz);
	}

	FORCEINLINE FVector3f FTransform::GetRightAxis() const noexcept
	{
		return mMat[0].xyz;
	}

	FORCEINLINE FVector3f FTransform::GetUnitRightAxis() const noexcept
	{
		return FMath::Normalize(mMat[0].xyz);
	}

	FORCEINLINE FVector3f FTransform::GetUpAxis() const noexcept
	{
		return mMat[1].xyz;
	}

	FORCEINLINE FVector3f FTransform::GetUnitUpAxis() const noexcept
	{
		return FMath::Normalize(mMat[1].xyz);
	}

	FORCEINLINE FMatrix4x4 FTransform::GetMatrix() const noexcept
	{
		UpdateMatrix();
		return mMat;
	}

	FORCEINLINE FMatrix4x4 FTransform::GetInverseMatrix() const noexcept
	{
		UpdateMatrix();
		return mInverseMat;
	}

	FORCEINLINE void FTransform::Scale(const FVector3f& r) noexcept
	{
		SetScale(mScale * r);
	}

	FORCEINLINE void FTransform::Scale(Scalar x, Scalar y, Scalar z) noexcept
	{
		Scale(FVector3f(x, y, z));
	}

	FORCEINLINE void FTransform::Rotate(const FQuaternion& r) noexcept
	{
		SetRotation(r * mRotation);
	}

	FORCEINLINE void FTransform::RotateAxis(const FVector3f& axis, Scalar angle) noexcept
	{
		SetRotation(FQuaternion{ axis, angle } *mRotation);
	}

	FORCEINLINE void FTransform::RotateAround(const FVector3f& point, const FVector3f& axis, Scalar angle) noexcept
	{
		FVector3f pos = mPosition;
		FQuaternion rot = FMath::FromAxisAngle(axis, angle);
		FVector3f dir = pos - point;
		dir = FMath::Mul(rot, dir);

		mPosition = point + dir;
		mRotation = rot * mRotation;

		MakeDirty();
	}

	FORCEINLINE void FTransform::Rotate(const FVector3f& euler) noexcept
	{
		Rotate(euler.x, euler.y, euler.z);
	}

	FORCEINLINE void FTransform::Rotate(Scalar x, Scalar y, Scalar z) noexcept
	{
		FQuaternion rx = FMath::FromAxisAngle(0, x);
		FQuaternion ry = FMath::FromAxisAngle(1, y);
		FQuaternion rz = FMath::FromAxisAngle(2, z);

		SetRotation(rz * ry * rx * mRotation);
	}

	FORCEINLINE void FTransform::RotateLocal(const FVector3f& euler) noexcept
	{
		RotateLocal(euler.x, euler.y, euler.z);
	}

	FORCEINLINE void FTransform::RotateLocal(Scalar x, Scalar y, Scalar z) noexcept
	{
		FQuaternion rx = FMath::FromAxisAngle(GetUnitRightAxis(), x);
		FQuaternion ry = FMath::FromAxisAngle(GetUnitUpAxis(), y);
		FQuaternion rz = FMath::FromAxisAngle(GetUnitForwardAxis(), z);

		SetRotation(rz * ry * rx * mRotation);
	}

	FORCEINLINE void FTransform::Translate(const FVector3f& t) noexcept
	{
		SetPosition(mPosition + t);
	}

	FORCEINLINE void FTransform::Translate(Scalar x, Scalar y, Scalar z) noexcept
	{
		Translate(FVector3f(x, y, z));
	}

	FORCEINLINE void FTransform::TranslateLocal(const FVector3f& t) noexcept
	{
		TranslateLocal(t.x, t.y, t.z);
	}

	FORCEINLINE void FTransform::TranslateLocal(Scalar x, Scalar y, Scalar z) noexcept
	{
		SetPosition(mPosition + GetUnitRightAxis() * x + GetUnitUpAxis() * y + GetUnitForwardAxis() * z);
	}

	FORCEINLINE FVector3f FTransform::TransformVector(const FVector3f& v) const noexcept
	{
		return FVector3f{
			FMath::Dot(v, FMath::Column(mMat, 0).xyz),
			FMath::Dot(v, FMath::Column(mMat, 1).xyz),
			FMath::Dot(v, FMath::Column(mMat, 2).xyz)
		};
	}

	FORCEINLINE FVector3f FTransform::TransformPoint(const FVector3f& p) const noexcept
	{
		FVector4f hp{ p, Scalar{1} };

		FVector3f result{
			FMath::Dot(hp, FMath::Column(mMat, 0)),
			FMath::Dot(hp, FMath::Column(mMat, 1)),
			FMath::Dot(hp, FMath::Column(mMat, 2))
		};

		Scalar w = FMath::Dot(hp, FMath::Column(mMat, 3));

		ASSERT(!FMath::IsZero(w));

		if (w == 1)
			return result;
		else
			return result / w;
	}

	FORCEINLINE FVector3f FTransform::TransformNormal(const FVector3f& n) const noexcept
	{
		return FVector3f{
			FMath::Dot(n, FMath::Row(mInverseMat, 0).xyz),
			FMath::Dot(n, FMath::Row(mInverseMat, 1).xyz),
			FMath::Dot(n, FMath::Row(mInverseMat, 2).xyz)
		};
	}

	FORCEINLINE FVector4f FTransform::TransformVector(const FVector4f& v) const noexcept
	{
		return FVector4f{
			FMath::Dot(v.xyz, FMath::Column(mMat, 0).xyz),
			FMath::Dot(v.xyz, FMath::Column(mMat, 1).xyz),
			FMath::Dot(v.xyz, FMath::Column(mMat, 2).xyz),
			Scalar{}
		};
	}

	FORCEINLINE FVector4f FTransform::TransformPoint(const FVector4f& p) const noexcept
	{
		FVector4f hp = FMath::Mul(p, mMat);

		ASSERT(!FMath::IsZero(hp.w));

		if (hp.w == 1)
			return hp;
		else
			return hp / hp.w;
	}

	FORCEINLINE FVector4f FTransform::TransformNormal(const FVector4f& n) const noexcept
	{
		return FVector4f{
			FMath::Dot(n.xyz, FMath::Row(mInverseMat, 0).xyz),
			FMath::Dot(n.xyz, FMath::Row(mInverseMat, 1).xyz),
			FMath::Dot(n.xyz, FMath::Row(mInverseMat, 2).xyz),
			Scalar{}
		};
	}

	FORCEINLINE FBoundingBox FTransform::TransformBoundingBox(const FBoundingBox& b) const noexcept
	{
		const FTransform& M = *this;
		FBoundingBox ret(M.TransformPoint(FVector3f(b.Lower.x, b.Lower.y, b.Lower.z)));
		ret = FMath::Union(ret, (M.TransformPoint(FVector3f{ b.Upper.x, b.Lower.y, b.Lower.z })));
		ret = FMath::Union(ret, (M.TransformPoint(FVector3f{ b.Lower.x, b.Upper.y, b.Lower.z })));
		ret = FMath::Union(ret, (M.TransformPoint(FVector3f{ b.Lower.x, b.Lower.y, b.Upper.z })));
		ret = FMath::Union(ret, (M.TransformPoint(FVector3f{ b.Lower.x, b.Upper.y, b.Upper.z })));
		ret = FMath::Union(ret, (M.TransformPoint(FVector3f{ b.Upper.x, b.Upper.y, b.Lower.z })));
		ret = FMath::Union(ret, (M.TransformPoint(FVector3f{ b.Upper.x, b.Lower.y, b.Upper.z })));
		ret = FMath::Union(ret, (M.TransformPoint(FVector3f{ b.Upper.x, b.Upper.y, b.Upper.z })));
		return ret;
	}

	FORCEINLINE FRay FTransform::TransformRay(const FRay& r) const noexcept
	{
		return FRay{ TransformPoint(r.Origin), FMath::Normalize(TransformVector(r.Direction)), r.TMin, r.TMax };
	}

	FORCEINLINE void FTransform::UpdateMatrix() const
	{
		if (mDirty)
		{
			mMat = FMath::ScaleMatrix4x4<Scalar>(mScale) * FMath::RotateMatrix4x4<Scalar>(mRotation) * FMath::TranslateMatrix4x4<Scalar>(mPosition);
			mInverseMat = FMath::Inverse(mMat);
			mDirty = false;
		}
	}







	// Non-member Operators 

	// --Implementation-- //

	FORCEINLINE bool operator==(const FTransform& a, const FTransform& b) noexcept
	{
		return a.GetScale() == b.GetScale() && a.GetRotation() == b.GetRotation() && a.GetPosition() == b.GetPosition();
	}










	// Non-member Function

	// --Implementation-- //

	namespace FMath
	{
		FORCEINLINE FTransform Inverse(const FTransform& t) noexcept
		{
			return FTransform{ t.GetInverseMatrix(), t.GetMatrix() };
		}

		FORCEINLINE FTransform Scale(const FVector3f& s) noexcept
		{
			return FTransform{ s, FQuaternion{FIdentity{}}, FVector3f{FZero{}} };
		}

		FORCEINLINE FTransform Scale(Scalar x, Scalar y, Scalar z) noexcept
		{
			return FTransform{ FVector3f{x,y,z}, FQuaternion{FIdentity{}}, FVector3f{FZero{}} };
		}

		FORCEINLINE FTransform Rotate(const FVector3f& r) noexcept
		{
			return FTransform{ FVector3f{FIdentity{}}, r, FVector3f{FZero{}} };
		}

		FORCEINLINE FTransform Rotate(Scalar yaw, Scalar roll, Scalar pitch) noexcept
		{
			return FTransform{ FVector3f{FIdentity{}}, FVector3f{yaw, roll, pitch}, FVector3f{FZero{}} };
		}

		FORCEINLINE FTransform Rotate(const FQuaternion& r) noexcept
		{
			return FTransform{ FVector3f{FIdentity{}}, r, FVector3f{FZero{}} };
		}

		FORCEINLINE FTransform RotateAxis(const FVector3f& axis, Scalar angle) noexcept
		{
			return FTransform{ FVector3f{FIdentity{}}, FMath::FromAxisAngle(axis, angle), FVector3f{FZero{}} };
		}

		FORCEINLINE FTransform RotateAround(const FVector3f& point, const FVector3f& axis, Scalar angle) noexcept
		{
			FVector3f pos = FVector3f{ FZero{} };
			FQuaternion rot = FMath::FromAxisAngle(axis, angle);
			FVector3f dir = pos - point;
			dir = FMath::Mul(rot, dir);

			pos = point + dir;

			return FTransform{ FVector3f{FIdentity{}}, rot, pos };
		}

		FORCEINLINE FTransform Translate(const FVector3f& t) noexcept
		{
			return FTransform{ FVector3f{FIdentity{}}, FQuaternion{FIdentity{}}, t };
		}

		FORCEINLINE FTransform Translate(Scalar x, Scalar y, Scalar z) noexcept
		{
			return FTransform{ FVector3f{FIdentity{}}, FQuaternion{FIdentity{}}, FVector3f{x, y, z} };
		}

		FORCEINLINE FVector3f TransformVector(const FTransform& a, const FVector3f& v) noexcept
		{
			return a.TransformVector(v);
		}

		FORCEINLINE FVector3f TransformPoint(const FTransform& a, const FVector3f& p) noexcept
		{
			return a.TransformPoint(p);
		}

		FORCEINLINE FVector3f TransformNormal(const FTransform& a, const FVector3f& n) noexcept
		{
			return a.TransformNormal(n);
		}

		FORCEINLINE FVector4f TransformVector(const FTransform& a, const FVector4f& v) noexcept
		{
			return a.TransformVector(v);
		}

		FORCEINLINE FVector4f TransformPoint(const FTransform& a, const FVector4f& p) noexcept
		{
			return a.TransformPoint(p);
		}

		FORCEINLINE FVector4f TransformNormal(const FTransform& a, const FVector4f& n) noexcept
		{
			return a.TransformNormal(n);
		}

		FORCEINLINE FBoundingBox TransformBoundingBox(const FTransform& a, const FBoundingBox& b) noexcept
		{
			return a.TransformBoundingBox(b);
		}

		FORCEINLINE FRay TransformRay(const FTransform& a, const FRay& r) noexcept
		{
			return a.TransformRay(r);
		}
	}
}