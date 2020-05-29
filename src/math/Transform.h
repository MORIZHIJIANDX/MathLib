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
			template<typename Scalar> Transform(const ScalarArray<Scalar, 3>& scale, const Quaternion& rotation = Quaternion{ Identity{} }, const ScalarArray<Scalar, 3>& translation = ScalarArray<Scalar, 3>{ Zero{} }) noexcept;
			explicit Transform(const Matrix4x4& mat) noexcept;
			Transform(const Matrix4x4& mat, const Matrix4x4& inverseMat) noexcept;
			Transform(const Transform& t) noexcept;

			Transform& operator=(const Transform& t) noexcept;
			Transform& operator*=(const Transform& t) noexcept;

			operator const Matrix4x4&() const noexcept;
			operator Matrix4x4& () noexcept;

			Vector3f GetScale() const noexcept;
			void SetScale(const Vector3f& scale) noexcept;

			Quaternion GetRotation() const;
			void SetRotation(const Quaternion& rotation) noexcept;

			Vector3f GetPosition() const noexcept;
			void SetPosition(const Vector3f& pos) noexcept;

			Vector3f GetEuler() const noexcept;
			void SetEuler(const Vector3f& euler) noexcept;

			Vector3f GetXAxis() const noexcept;
			Vector3f GetUnitXAxis() const noexcept;

			Vector3f GetYAxis() const noexcept;
			Vector3f GetUnitYAxis() const noexcept;

			Vector3f GetZAxis() const noexcept;
			Vector3f GetUnitZAxis() const noexcept;

			Matrix4x4 GetMatrix() const noexcept;
			Matrix4x4 GetInverseMatrix() const noexcept;
			
			void SetMatrix(const Matrix4x4& mat) noexcept;

			void Scale(const Vector3f& r) noexcept;
			void Rotate(const Quaternion& r) noexcept;
			void Translate(const Vector3f& t) noexcept;

		 	Vector3f TransformVector(const Vector3f& v) const noexcept;
			Vector3f TransformPoint(const Vector3f& v) const noexcept;
			Vector3f TransformNormal(const Vector3f& v) const noexcept;

			Vector4f TransformVector(const Vector4f& v) const noexcept;
			Vector4f TransformPoint(const Vector4f& v) const noexcept;
			Vector4f TransformNormal(const Vector4f& v) const noexcept;

		private:
			Vector3f mScale;
			Quaternion mRotation;
			Vector3f mPosition;
			
			Matrix4x4 mMat;
			Matrix4x4 mInverseMat;

			bool mDirty;
		};

		Transform operator*(const Transform& t1, const Transform& t2) noexcept;

		Transform Scale(const Vector3f& r);
		Transform Rotate(const Vector3f& r);
		Transform Rotate(Scalar yaw, Scalar roll, Scalar pitch);
		Transform Rotate(const Quaternion& r);
		Transform Translate(const Vector3f& r);

		Transform RotateAxis(const Vector3f& axis, Scalar angle);

	}
}