#pragma once

#include "../math/Transform.h"
#include "Viewport.h"

namespace Dash
{
	class Camera
	{
	public:
		Camera(Scalar nearZ, Scalar farZ, const Viewport& vp = Viewport{});
		virtual ~Camera();

		Matrix4x4 GetViewMatrix() const;
		Matrix4x4 GetProjectionMatrix() const;
		Matrix4x4 GetViewProjectionMatrix() const;

		Vector3f GetPosition() const;
		Quaternion GetRotation() const;

		Vector3f GetForward() const;
		Vector3f GetRight() const;
		Vector3f GetUp() const;

		Scalar GetFar() const;
		Scalar GetNear() const;

		Viewport GetViewPort() const;

		std::size_t GetPixelWidth() const;
		std::size_t GetPixelHeight() const;

		void SetWorldMatrix(const Matrix4x4& mat);
		void SetProjectionMatrix(const Matrix4x4& mat);

		void SetPosition(const Vector3f& p);
		void SetRotation(const Quaternion& q);
		void SetRotation(Scalar pitch, Scalar yaw, Scalar roll);

		void SetFarClip(Scalar farZ);
		void SetNearClip(Scalar nearZ);

		void SetViewPort(const Viewport& vp);

		void SetLookAt(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up);
		void SetLookTo(const Vector3f& eye, const Vector3f& lookTo, const Vector3f& up);

		void TranslateForward(Scalar speed);
		void TranslateRight(Scalar speed);
		void TranslateUp(Scalar speed);
		void TranslateBack(Scalar speed);
		void TranslateLeft(Scalar speed);
		void TranslateDown(Scalar speed);

		void AddPitch(Scalar angle);
		void AddYaw(Scalar angle);
		void AddRoll(Scalar angle);

	protected:

		void MakeWorldMatrixDirty() const { mWorldMatrixDirty = true; }
		void MakeProjectionMatrixDirty() const { mProjectionMatrixDirty = true; }

		void UpdateViewProjectionMatrix() const 
		{ 
			mViewProjectionMatrix = mTransform.GetInverseMatrix() * mProjectionMatrix; 
			mWorldMatrixDirty = false;
		}

		void UpdateProjectionMatrix() const
		{
			CreateProjectionMatrix();
			mProjectionMatrixDirty = false;
		}

		virtual void CreateProjectionMatrix() const = 0;

		mutable Matrix4x4 mProjectionMatrix;
		mutable Matrix4x4 mViewProjectionMatrix;

		Transform mTransform;

		Scalar mNear;
		Scalar mFar;

		Viewport mViewPort;

		mutable bool mWorldMatrixDirty;
		mutable bool mProjectionMatrixDirty;
	};


	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(Scalar minX, Scalar minY, Scalar maxX, Scalar maxY, Scalar nearZ, Scalar farZ, const Viewport& vp = Viewport{});
		virtual ~OrthographicCamera();

		Scalar GetMinX() const { return mXMin; }
		Scalar GetMinY() const { return mYMin; }
		Scalar GetMaxX() const { return mXMax; }
		Scalar GetMaxY() const { return mYMax; }

		void SetMinX(Scalar minX);
		void SetMinY(Scalar minY);
		void SetMaxX(Scalar maxX);
		void SetMaxY(Scalar maxY);

	protected:

		virtual void CreateProjectionMatrix() const override;

		Scalar mXMin;
		Scalar mXMax;
		Scalar mYMin;
		Scalar mYMax;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(Scalar fov, Scalar aspect, Scalar nearZ, Scalar farZ, const Viewport& vp = Viewport{});
		virtual ~PerspectiveCamera();

		Scalar GetFieldOfView() const { return mFov; }
		Scalar GetAspectRatio() const { return mAspect; }

		void SetFieldOfView(Scalar fov);
		void SetAspectRatio(Scalar aspect);

		Ray GenerateRay(Scalar u, Scalar v) const;

	protected:

		virtual void CreateProjectionMatrix() const override;

		Scalar mFov;
		Scalar mAspect;
	};


}