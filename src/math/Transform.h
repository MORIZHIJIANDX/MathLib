#pragma once

#include "MathType.h"

namespace Dash
{
	namespace Math
	{
		class Transform
		{
		public:
			Transform() noexcept;
			Transform(Identity) noexcept;
			template<typename Scalar> Transform(const ScalarArray<Scalar, 3>& scale, const Quaternion& rotation, const ScalarArray<Scalar, 3>& position) noexcept;
			template<typename Scalar> Transform(const ScalarArray<Scalar, 3>& scale, const ScalarArray<Scalar, 3>& rotationEuler, const ScalarArray<Scalar, 3>& position) noexcept;
			explicit Transform(const Matrix4x4& mat) noexcept;
			Transform(const Matrix4x4& mat, const Matrix4x4& inverseMat) noexcept;
			Transform(const Transform& t) noexcept;

			Transform& operator=(const Transform& t) noexcept;
			Transform& operator*=(const Transform& t) noexcept;

			Transform operator*(const Transform& t) noexcept;

			operator const Matrix4x4&() const noexcept;
			operator Matrix4x4& () noexcept;

			Vector3f GetScale() const noexcept;
			void SetScale(const Vector3f& scale) noexcept;

			Quaternion GetRotation() const;
			void SetRotation(const Quaternion& rotation) noexcept;

			Vector3f GetPosition() const noexcept;
			void SetPosition(const Vector3f& pos) noexcept;

			Vector3f GetEuler() const noexcept;
			void SetEuler(Scalar pitch, Scalar yaw, Scalar roll) noexcept;
			void SetEuler(const Vector3f& euler) noexcept;

			void SetLookAt(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up) noexcept;
			void SetLookTo(const Vector3f& eye, const Vector3f& lookTo, const Vector3f& up) noexcept;

			Vector3f GetForwardAxis() const noexcept;
			Vector3f GetUnitForwardAxis() const noexcept;

			Vector3f GetRightAxis() const noexcept;
			Vector3f GetUnitRightAxis() const noexcept;

			Vector3f GetUpAxis() const noexcept;
			Vector3f GetUnitUpAxis() const noexcept;

			Matrix4x4 GetMatrix() const noexcept;
			Matrix4x4 GetInverseMatrix() const noexcept;

			void Scale(const Vector3f& r) noexcept;
			void Scale(Scalar x, Scalar y, Scalar z) noexcept;

			void Rotate(const Quaternion& r) noexcept;
			void RotateAxis(const Vector3f& axis, Scalar angle) noexcept;
			void RotateAround(const Vector3f& point, const Vector3f& axis, Scalar angle) noexcept;

			void Rotate(const Vector3f& euler) noexcept;
			void Rotate(Scalar x, Scalar y, Scalar z) noexcept;

			void RotateLocal(const Vector3f& euler) noexcept;
			void RotateLocal(Scalar x, Scalar y, Scalar z) noexcept;

			void Translate(const Vector3f& t) noexcept;
			void Translate(Scalar x, Scalar y, Scalar z) noexcept;

			void TranslateLocal(const Vector3f& t) noexcept;
			void TranslateLocal(Scalar x, Scalar y, Scalar z) noexcept;

		 	Vector3f TransformVector(const Vector3f& v) const noexcept;
			Vector3f TransformPoint(const Vector3f& p) const noexcept;
			Vector3f TransformNormal(const Vector3f& n) const noexcept;

			Vector4f TransformVector(const Vector4f& v) const noexcept;
			Vector4f TransformPoint(const Vector4f& p) const noexcept;
			Vector4f TransformNormal(const Vector4f& n) const noexcept;

			BoundingBox TransformBoundingBox(const BoundingBox& b) const noexcept;
			Ray TransformRay(const Ray& r) const noexcept;

		private:
			void UpdateMatrix() const;

			void MakeDirty() { mDirty = true; }

			Vector3f mScale;
			Quaternion mRotation;
			Vector3f mPosition;
			
			mutable Matrix4x4 mMat;
			mutable Matrix4x4 mInverseMat;

			mutable bool mDirty;
		};

		bool operator==(const Transform& a, const Transform& b) noexcept;

		Transform Inverse(const Transform& t) noexcept;

		Transform Scale(const Vector3f& s) noexcept;
		Transform Scale(Scalar x, Scalar y, Scalar z) noexcept;

		Transform Rotate(const Vector3f& r) noexcept;
		Transform Rotate(Scalar yaw, Scalar roll, Scalar pitch) noexcept;
		Transform Rotate(const Quaternion& r) noexcept;

		Transform RotateAxis(const Vector3f& axis, Scalar angle) noexcept;
		Transform RotateAround(const Vector3f& point, const Vector3f& axis, Scalar angle) noexcept;

		Transform Translate(const Vector3f& t) noexcept;
		Transform Translate(Scalar x, Scalar y, Scalar z) noexcept;

		Vector3f TransformVector(const Transform& a, const Vector3f& v) noexcept;
		Vector3f TransformPoint(const Transform& a, const Vector3f& p) noexcept;
		Vector3f TransformNormal(const Transform& a, const Vector3f& n) noexcept;

		Vector4f TransformVector(const Transform& a, const Vector4f& v) noexcept;
		Vector4f TransformPoint(const Transform& a, const Vector4f& p) noexcept;
		Vector4f TransformNormal(const Transform& a, const Vector4f& n) noexcept;

		BoundingBox TransformBoundingBox(const Transform& a, const BoundingBox& b) noexcept;
		Ray TransformRay(const Transform& a, const Ray& r) noexcept;




		//Member Function

		FORCEINLINE Transform::Transform() noexcept
			: mScale()
			, mRotation()
			, mPosition()
			, mDirty(true)
			, mMat()
			, mInverseMat()
		{
		}

		FORCEINLINE Transform::Transform(Identity) noexcept
			: mScale(Identity{})
			, mRotation(Identity{})
			, mPosition(Identity{})
			, mDirty(false)
			, mMat(Identity{})
			, mInverseMat(Identity{})
		{
		}

		template<typename Scalar>
		FORCEINLINE Transform::Transform(const ScalarArray<Scalar, 3>& scale, const Quaternion& rotation, const ScalarArray<Scalar, 3>& position) noexcept
			: mScale(scale)
			, mRotation(rotation)
			, mPosition(position)
			, mDirty(true)
		{
			UpdateMatrix();
		}

		template<typename Scalar>
		FORCEINLINE Transform::Transform(const ScalarArray<Scalar, 3>& scale, const ScalarArray<Scalar, 3>& rotationEuler, const ScalarArray<Scalar, 3>& position) noexcept
			: mScale(scale)
			, mRotation(FromEuler(rotationEuler))
			, mPosition(position)
			, mDirty(true)
		{
			UpdateMatrix();
		}

		FORCEINLINE Transform::Transform(const Matrix4x4& mat) noexcept
			: mMat(mat)
			, mInverseMat(Inverse(mat))
			, mDirty(false)
		{
			DecomposeAffineMatrix4x4(mScale, mRotation, mPosition, mMat);
		}

		FORCEINLINE Transform::Transform(const Matrix4x4& mat, const Matrix4x4& inverseMat) noexcept
			: mMat(mat)
			, mInverseMat(inverseMat)
			, mDirty(false)
		{
			DecomposeAffineMatrix4x4(mScale, mRotation, mPosition, mMat);
		}

		FORCEINLINE Transform::Transform(const Transform& t) noexcept
			: mScale(t.mScale)
			, mRotation(t.mRotation)
			, mPosition(t.mPosition)
			, mMat(t.mMat)
			, mInverseMat(t.mInverseMat)
			, mDirty(false)
		{
		}

		FORCEINLINE Transform& Transform::operator=(const Transform& t) noexcept
		{
			mScale = t.mScale;
			mRotation = t.mRotation;
			mPosition = t.mPosition;
			mMat = t.mMat;
			mInverseMat = t.mInverseMat;
			mDirty = false;

			return *this;
		}

		FORCEINLINE Transform& Transform::operator*=(const Transform& t) noexcept
		{
			mMat *= t.mMat;
			mInverseMat = Inverse(mMat);
			mDirty = false;

			DecomposeAffineMatrix4x4(mScale, mRotation, mPosition, mMat);
			
			return *this;
		}

		FORCEINLINE Transform Transform::operator*(const Transform& t) noexcept
		{
			return Transform{
				mMat * t.mMat,
				t.mInverseMat* mInverseMat
			};
		}

		FORCEINLINE Transform::operator const Matrix4x4& () const noexcept
		{
			return mMat;
		}

		FORCEINLINE Transform::operator Matrix4x4& () noexcept
		{
			return mMat;
		}

		FORCEINLINE Vector3f Transform::GetScale() const noexcept
		{
			return mScale;
		}

		FORCEINLINE void Transform::SetScale(const Vector3f& scale) noexcept
		{
			mScale = scale;

			MakeDirty();
		}

		FORCEINLINE Quaternion Transform::GetRotation() const
		{
			return mRotation;
		}

		FORCEINLINE void Transform::SetRotation(const Quaternion& rotation) noexcept
		{
			mRotation = rotation;

			MakeDirty();
		}

		FORCEINLINE Vector3f Transform::GetPosition() const noexcept
		{
			return mPosition;
		}

		FORCEINLINE void Transform::SetPosition(const Vector3f& pos) noexcept
		{
			mPosition = pos;

			MakeDirty();
		}

		FORCEINLINE Vector3f Transform::GetEuler() const noexcept
		{
			Vector3f euler;
			ToEuler(euler, mRotation);

			return euler;
		}

		FORCEINLINE void Transform::SetEuler(Scalar pitch, Scalar yaw, Scalar roll) noexcept
		{
			mRotation = FromEuler(pitch, yaw, roll);

			MakeDirty();
		}

		FORCEINLINE void Transform::SetEuler(const Vector3f& euler) noexcept
		{
			mRotation = FromEuler(euler);

			MakeDirty();
		}

		FORCEINLINE void Transform::SetLookAt(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up) noexcept
		{
			SetLookTo(eye, lookAt - eye, up);
		}

		FORCEINLINE void Transform::SetLookTo(const Vector3f& eye, const Vector3f& lookTo, const Vector3f& up) noexcept
		{
			Math::Vector3<Scalar> look = Math::Normalize(lookTo);
			Math::Vector3<Scalar> right = Math::Normalize(Math::Cross(up, look));
			Math::Vector3<Scalar> newUp = Math::Cross(look, right);

			mMat.SetRow(0, right);
			mMat.SetRow(1, newUp);
			mMat.SetRow(2, look);
			mMat.SetRow(3, eye);

			mInverseMat = Inverse(mMat);

			DecomposeAffineMatrix4x4(mScale, mRotation, mPosition, mMat);

			mDirty = false;
		}

		FORCEINLINE Vector3f Transform::GetForwardAxis() const noexcept
		{
			return mMat[2].xyz;
		}

		FORCEINLINE Vector3f Transform::GetUnitForwardAxis() const noexcept
		{
			return Normalize(mMat[2].xyz);
		}

		FORCEINLINE Vector3f Transform::GetRightAxis() const noexcept
		{
			return mMat[0].xyz;
		}

		FORCEINLINE Vector3f Transform::GetUnitRightAxis() const noexcept
		{
			return Normalize(mMat[0].xyz);
		}

		FORCEINLINE Vector3f Transform::GetUpAxis() const noexcept
		{
			return mMat[1].xyz;
		}

		FORCEINLINE Vector3f Transform::GetUnitUpAxis() const noexcept
		{
			return Normalize(mMat[1].xyz);
		}

		FORCEINLINE Matrix4x4 Transform::GetMatrix() const noexcept
		{
			UpdateMatrix();
			return mMat;
		}

		FORCEINLINE Matrix4x4 Transform::GetInverseMatrix() const noexcept
		{
			UpdateMatrix();
			return mInverseMat;
		}

		FORCEINLINE void Transform::Scale(const Vector3f& r) noexcept
		{
			SetScale(mScale * r);
		}

		FORCEINLINE void Transform::Scale(Scalar x, Scalar y, Scalar z) noexcept
		{		
			Scale(Vector3f(x, y, z));
		}

		FORCEINLINE void Transform::Rotate(const Quaternion& r) noexcept
		{
			SetRotation(r * mRotation);
		}

		FORCEINLINE void Transform::RotateAxis(const Vector3f& axis, Scalar angle) noexcept
		{
			SetRotation(Quaternion{ axis, angle} *mRotation);
		}

		FORCEINLINE void Transform::RotateAround(const Vector3f& point, const Vector3f& axis, Scalar angle) noexcept
		{
			Vector3f pos = mPosition;
			Quaternion rot = FromAxisAngle(axis, angle);
			Vector3f dir = pos - point;
			dir = Mul(rot, dir);
		
			mPosition = point + dir;
			mRotation = rot * mRotation;

			MakeDirty();
		}

		FORCEINLINE void Transform::Rotate(const Vector3f& euler) noexcept
		{
			Rotate(euler.x, euler.y, euler.z);
		}

		FORCEINLINE void Transform::Rotate(Scalar x, Scalar y, Scalar z) noexcept
		{
			Quaternion rx = FromAxisAngle(0, x);
			Quaternion ry = FromAxisAngle(1, y);
			Quaternion rz = FromAxisAngle(2, z);

			SetRotation(rz * ry * rx * mRotation);
		}

		FORCEINLINE void Transform::RotateLocal(const Vector3f& euler) noexcept
		{
			RotateLocal(euler.x, euler.y, euler.z);
		}

		FORCEINLINE void Transform::RotateLocal(Scalar x, Scalar y, Scalar z) noexcept
		{
			Quaternion rx = FromAxisAngle(GetUnitRightAxis(), x);
			Quaternion ry = FromAxisAngle(GetUnitUpAxis(), y);
			Quaternion rz = FromAxisAngle(GetUnitForwardAxis(), z);

			SetRotation(rz * ry * rx * mRotation);
		}

		FORCEINLINE void Transform::Translate(const Vector3f& t) noexcept
		{
			SetPosition(mPosition + t);
		}

		FORCEINLINE void Transform::Translate(Scalar x, Scalar y, Scalar z) noexcept
		{
			Translate(Vector3f(x, y, z));
		}

		FORCEINLINE void Transform::TranslateLocal(const Vector3f& t) noexcept
		{
			TranslateLocal(t.x, t.y, t.z);
		}

		FORCEINLINE void Transform::TranslateLocal(Scalar x, Scalar y, Scalar z) noexcept
		{
			SetPosition(mPosition + GetUnitRightAxis() * x + GetUnitUpAxis() * y + GetUnitForwardAxis() * z);
		}

		FORCEINLINE Vector3f Transform::TransformVector(const Vector3f& v) const noexcept
		{
			return Vector3f{
				Dot(v, Column(mMat, 0).xyz),
				Dot(v, Column(mMat, 1).xyz),
				Dot(v, Column(mMat, 2).xyz)
			};
		}

		FORCEINLINE Vector3f Transform::TransformPoint(const Vector3f& p) const noexcept
		{
			Vector4f hp{ p, Scalar{1} };

			Vector3f result{
				Dot(hp, Column(mMat, 0)),
				Dot(hp, Column(mMat, 1)),
				Dot(hp, Column(mMat, 2))
			};

			Scalar w = Dot(hp, Column(mMat, 3));

			ASSERT(!IsZero(w));

			if (w == 1)
				return result;
			else
				return result / w;
		}

		FORCEINLINE Vector3f Transform::TransformNormal(const Vector3f& n) const noexcept
		{
			return Vector3f{
				Dot(n, Row(mInverseMat, 0).xyz),
				Dot(n, Row(mInverseMat, 1).xyz),
				Dot(n, Row(mInverseMat, 2).xyz)
			};
		}

		FORCEINLINE Vector4f Transform::TransformVector(const Vector4f& v) const noexcept
		{
			return Vector4f{
				Dot(v.xyz, Column(mMat, 0).xyz),
				Dot(v.xyz, Column(mMat, 1).xyz),
				Dot(v.xyz, Column(mMat, 2).xyz),
				Scalar{}
			};
		}

		FORCEINLINE Vector4f Transform::TransformPoint(const Vector4f& p) const noexcept
		{
			Vector4f hp = Mul(p, mMat);

			ASSERT(!IsZero(hp.w));

			if (hp.w == 1)
				return hp;
			else
				return hp / hp.w;
		}

		FORCEINLINE Vector4f Transform::TransformNormal(const Vector4f& n) const noexcept
		{
			return Vector4f{
				Dot(n.xyz, Row(mInverseMat, 0).xyz),
				Dot(n.xyz, Row(mInverseMat, 1).xyz),
				Dot(n.xyz, Row(mInverseMat, 2).xyz),
				Scalar{}
			};
		}

		FORCEINLINE BoundingBox Transform::TransformBoundingBox(const BoundingBox& b) const noexcept
		{
			const Transform& M = *this;
			BoundingBox ret(M.TransformPoint(Vector3f(b.Lower.x, b.Lower.y, b.Lower.z)));
			ret = Union(ret, (M.TransformPoint(Vector3f{ b.Upper.x, b.Lower.y, b.Lower.z })));
			ret = Union(ret, (M.TransformPoint(Vector3f{ b.Lower.x, b.Upper.y, b.Lower.z })));
			ret = Union(ret, (M.TransformPoint(Vector3f{ b.Lower.x, b.Lower.y, b.Upper.z })));
			ret = Union(ret, (M.TransformPoint(Vector3f{ b.Lower.x, b.Upper.y, b.Upper.z })));
			ret = Union(ret, (M.TransformPoint(Vector3f{ b.Upper.x, b.Upper.y, b.Lower.z })));
			ret = Union(ret, (M.TransformPoint(Vector3f{ b.Upper.x, b.Lower.y, b.Upper.z })));
			ret = Union(ret, (M.TransformPoint(Vector3f{ b.Upper.x, b.Upper.y, b.Upper.z })));
			return ret;
		}

		FORCEINLINE Ray Transform::TransformRay(const Ray& r) const noexcept
		{
			return Ray{ TransformPoint(r.Origin), Normalize(TransformVector(r.Direction)), r.TMax, r.Time };
		}

		FORCEINLINE void Transform::UpdateMatrix() const
		{
			if (mDirty)
			{
				mMat = ScaleMatrix4x4<Scalar>(mScale) * RotateMatrix4x4<Scalar>(mRotation) * TranslateMatrix4x4<Scalar>(mPosition);
				mInverseMat = Inverse(mMat);
				mDirty = false;
			}
		}

		FORCEINLINE bool operator==(const Transform& a, const Transform& b) noexcept
		{
			return a.GetScale() == b.GetScale() && a.GetRotation() == b.GetRotation() && a.GetPosition() == b.GetPosition();
		}

		FORCEINLINE Transform Inverse(const Transform& t) noexcept
		{
			return Transform{ t.GetInverseMatrix(), t.GetMatrix() };
		}

		FORCEINLINE Transform Scale(const Vector3f& s) noexcept
		{
			return Transform{ s, Quaternion{Identity{}}, Vector3f{Zero{}} };
		}

		FORCEINLINE Transform Scale(Scalar x, Scalar y, Scalar z) noexcept
		{
			return Transform{ Vector3f{x,y,z}, Quaternion{Identity{}}, Vector3f{Zero{}} };
		}

		FORCEINLINE Transform Rotate(const Vector3f& r) noexcept
		{
			return Transform{ Vector3f{Identity{}}, r, Vector3f{Zero{}} };
		}

		FORCEINLINE Transform Rotate(Scalar yaw, Scalar roll, Scalar pitch) noexcept
		{
			return Transform{ Vector3f{Identity{}}, Vector3f{yaw, roll, pitch}, Vector3f{Zero{}} };
		}

		FORCEINLINE Transform Rotate(const Quaternion& r) noexcept
		{
			return Transform{ Vector3f{Identity{}}, r, Vector3f{Zero{}} };
		}

		FORCEINLINE Transform RotateAxis(const Vector3f& axis, Scalar angle) noexcept
		{
			return Transform{ Vector3f{Identity{}}, FromAxisAngle(axis, angle), Vector3f{Zero{}} };
		}

		FORCEINLINE Transform RotateAround(const Vector3f& point, const Vector3f& axis, Scalar angle) noexcept
		{
			Vector3f pos = Vector3f{ Zero{} };
			Quaternion rot = FromAxisAngle(axis, angle);
			Vector3f dir = pos - point;
			dir = Mul(rot, dir);

			pos = point + dir;

			return Transform{ Vector3f{Identity{}}, rot, pos };
		}

		FORCEINLINE Transform Translate(const Vector3f& t) noexcept
		{
			return Transform{ Vector3f{Identity{}}, Quaternion{Identity{}}, t };
		}

		FORCEINLINE Transform Translate(Scalar x, Scalar y, Scalar z) noexcept
		{
			return Transform{ Vector3f{Identity{}}, Quaternion{Identity{}}, Vector3f{x, y, z} };
		}

		FORCEINLINE Vector3f TransformVector(const Transform& a, const Vector3f& v) noexcept
		{
			return a.TransformVector(v);
		}

		FORCEINLINE Vector3f TransformPoint(const Transform& a, const Vector3f& p) noexcept
		{
			return a.TransformPoint(p);
		}

		FORCEINLINE Vector3f TransformNormal(const Transform& a, const Vector3f& n) noexcept
		{
			return a.TransformNormal(n);
		}

		FORCEINLINE Vector4f TransformVector(const Transform& a, const Vector4f& v) noexcept
		{
			return a.TransformVector(v);
		}

		FORCEINLINE Vector4f TransformPoint(const Transform& a, const Vector4f& p) noexcept
		{
			return a.TransformPoint(p);
		}

		FORCEINLINE Vector4f TransformNormal(const Transform& a, const Vector4f& n) noexcept
		{
			return a.TransformNormal(n);
		}

		FORCEINLINE BoundingBox TransformBoundingBox(const Transform& a, const BoundingBox& b) noexcept
		{
			return a.TransformBoundingBox(b);
		}

		FORCEINLINE Ray TransformRay(const Transform& a, const Ray& r) noexcept
		{
			return a.TransformRay(r);
		}




	}
}