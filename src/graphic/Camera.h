#pragma once

#include "../math/Transform.h"
#include "Viewport.h"

namespace Dash
{
	class FCamera
	{
	public:
		FCamera(Scalar nearZ, Scalar farZ, const FViewport& vp = FViewport{});
		virtual ~FCamera();

		FMatrix4x4 GetViewMatrix() const;
		FMatrix4x4 GetProjectionMatrix() const;
		FMatrix4x4 GetViewProjectionMatrix() const;

		FVector3f GetPosition() const;
		FQuaternion GetRotation() const;

		FVector3f GetForward() const;
		FVector3f GetRight() const;
		FVector3f GetUp() const;

		Scalar GetFar() const;
		Scalar GetNear() const;

		FViewport GetViewPort() const;

		std::size_t GetPixelWidth() const;
		std::size_t GetPixelHeight() const;

		void SetWorldMatrix(const FMatrix4x4& mat);
		void SetProjectionMatrix(const FMatrix4x4& mat);

		void SetPosition(const FVector3f& p);
		void SetRotation(const FQuaternion& q);
		void SetRotation(Scalar pitch, Scalar yaw, Scalar roll);

		void SetFarClip(Scalar farZ);
		void SetNearClip(Scalar nearZ);

		void SetViewPort(const FViewport& vp);

		void SetLookAt(const FVector3f& eye, const FVector3f& lookAt, const FVector3f& up);
		void SetLookTo(const FVector3f& eye, const FVector3f& lookTo, const FVector3f& up);

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

		mutable FMatrix4x4 mProjectionMatrix;
		mutable FMatrix4x4 mViewProjectionMatrix;

		FTransform mTransform;

		Scalar mNear;
		Scalar mFar;

		FViewport mViewPort;

		mutable bool mWorldMatrixDirty;
		mutable bool mProjectionMatrixDirty;
	};


	class OrthographicCamera : public FCamera
	{
	public:
		OrthographicCamera(Scalar minX, Scalar minY, Scalar maxX, Scalar maxY, Scalar nearZ, Scalar farZ, const FViewport& vp = FViewport{});
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

	class PerspectiveCamera : public FCamera
	{
	public:
		PerspectiveCamera(Scalar fov, Scalar aspect, Scalar nearZ, Scalar farZ, const FViewport& vp = FViewport{});
		virtual ~PerspectiveCamera();

		Scalar GetFieldOfView() const { return mFov; }
		Scalar GetAspectRatio() const { return mAspect; }

		void SetFieldOfView(Scalar fov);
		void SetAspectRatio(Scalar aspect);

		FRay GenerateRay(Scalar u, Scalar v) const;

	protected:

		virtual void CreateProjectionMatrix() const override;

		Scalar mFov;
		Scalar mAspect;
	};


}