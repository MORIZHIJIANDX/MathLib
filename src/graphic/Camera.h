#pragma once

#include "../math/Transform.h"
#include "Viewport.h"

namespace Dash
{
	class Camera
	{
	public:
		Camera(Math::Scalar nearZ, Math::Scalar farZ, const Viewport& vp = Viewport{});
		virtual ~Camera();

		Math::Matrix4x4 GetViewMatrix() const;
		Math::Matrix4x4 GetProjectionMatrix() const;
		Math::Matrix4x4 GetViewProjectionMatrix() const;

		Math::Vector3f GetPosition() const;
		Math::Quaternion GetRotation() const;

		Math::Vector3f GetForward() const;
		Math::Vector3f GetRight() const;
		Math::Vector3f GetUp() const;

		Math::Scalar GetFar() const;
		Math::Scalar GetNear() const;

		Viewport GetViewPort() const;

		void SetWorldMatrix(const Math::Matrix4x4& mat);
		void SetProjectionMatrix(const Math::Matrix4x4& mat);

		void SetPosition(const Math::Vector3f& p);
		void SetRotation(const Math::Quaternion& q);
		void SetRotation(Math::Scalar pitch, Math::Scalar yaw, Math::Scalar roll);

		void SetFarClip(Math::Scalar farZ);
		void SetNearClip(Math::Scalar nearZ);

		void SetViewPort(const Viewport& vp);

		void SetLookAt(const Math::Vector3f& eye, const Math::Vector3f& lookAt, const Math::Vector3f& up);
		void SetLookTo(const Math::Vector3f& eye, const Math::Vector3f& lookTo, const Math::Vector3f& up);

		void TranslateForward(Math::Scalar speed);
		void TranslateRight(Math::Scalar speed);
		void TranslateUp(Math::Scalar speed);
		void TranslateBack(Math::Scalar speed);
		void TranslateLeft(Math::Scalar speed);
		void TranslateDown(Math::Scalar speed);

		void AddPitch(Math::Scalar angle);
		void AddYaw(Math::Scalar angle);
		void AddRoll(Math::Scalar angle);

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

		mutable Math::Matrix4x4 mProjectionMatrix;
		mutable Math::Matrix4x4 mViewProjectionMatrix;

		Math::Transform mTransform;

		Math::Scalar mNear;
		Math::Scalar mFar;

		Viewport mViewPort;

		mutable bool mWorldMatrixDirty;
		mutable bool mProjectionMatrixDirty;
	};


	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(Math::Scalar minX, Math::Scalar minY, Math::Scalar maxX, Math::Scalar maxY, Math::Scalar nearZ, Math::Scalar farZ, const Viewport& vp = Viewport{});
		virtual ~OrthographicCamera();

		Math::Scalar GetMinX() const { return mXMin; }
		Math::Scalar GetMinY() const { return mYMin; }
		Math::Scalar GetMaxX() const { return mXMax; }
		Math::Scalar GetMaxY() const { return mYMax; }

		void SetMinX(Math::Scalar minX);
		void SetMinY(Math::Scalar minY);
		void SetMaxX(Math::Scalar maxX);
		void SetMaxY(Math::Scalar maxY);

	protected:

		virtual void CreateProjectionMatrix() const override;

		Math::Scalar mXMin;
		Math::Scalar mXMax;
		Math::Scalar mYMin;
		Math::Scalar mYMax;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(Math::Scalar fov, Math::Scalar aspect, Math::Scalar nearZ, Math::Scalar farZ, const Viewport& vp = Viewport{});
		virtual ~PerspectiveCamera();

		Math::Scalar GetFieldOfView() const { return mFov; }
		Math::Scalar GetAspectRatio() const { return mAspect; }

		void SetFieldOfView(Math::Scalar fov);
		void SetAspectRatio(Math::Scalar aspect);

		Math::Ray GenerateRay(Math::Scalar u, Math::Scalar v) const;

	protected:

		virtual void CreateProjectionMatrix() const override;

		Math::Scalar mFov;
		Math::Scalar mAspect;
	};


}