#include "Camera.h"

namespace Dash
{
	FCamera::FCamera(Scalar nearZ, Scalar farZ, const FViewport& vp)
		: mNear(nearZ)
		, mFar(farZ)
		, mViewPort(vp)
		, mProjectionMatrix(FIdentity{})
		, mViewProjectionMatrix(FIdentity{})
		, mTransform(FIdentity{})
		, mWorldMatrixDirty(true)
		, mProjectionMatrixDirty(true)
	{

	}

	FCamera::~FCamera()
	{
	}

	FMatrix4x4 FCamera::GetViewMatrix() const
	{
		return mTransform.GetInverseMatrix();
	}

	FMatrix4x4 FCamera::GetProjectionMatrix() const
	{
		if (mProjectionMatrixDirty)
		{
			UpdateProjectionMatrix();
		}

		return mProjectionMatrix;
	}

	FMatrix4x4 FCamera::GetViewProjectionMatrix() const
	{
		if (mProjectionMatrixDirty)
		{
			UpdateProjectionMatrix();
		}

		if (mWorldMatrixDirty)
		{
			UpdateViewProjectionMatrix();
		}

		return mViewProjectionMatrix;
	}

	FVector3f FCamera::GetPosition() const
	{
		return mTransform.GetPosition();
	}

	FQuaternion FCamera::GetRotation() const
	{
		return mTransform.GetRotation();
	}

	FVector3f FCamera::GetForward() const
	{
		return mTransform.GetUnitForwardAxis();
	}

	FVector3f FCamera::GetRight() const
	{
		return mTransform.GetUnitRightAxis();
	}

	FVector3f FCamera::GetUp() const
	{
		return mTransform.GetUnitUpAxis();
	}

	Scalar FCamera::GetFar() const
	{
		return mFar;
	}

	Scalar FCamera::GetNear() const
	{
		return mNear;
	}

	FViewport FCamera::GetViewPort() const
	{
		return mViewPort;
	}

	std::size_t FCamera::GetPixelWidth() const
	{
		return static_cast<std::size_t>(mViewPort.Width);
	}

	std::size_t FCamera::GetPixelHeight() const
	{
		return static_cast<std::size_t>(mViewPort.Height);
	}

	void FCamera::SetWorldMatrix(const FMatrix4x4& mat)
	{
		mTransform = FTransform{ mat };
	
		MakeWorldMatrixDirty();
	}

	void FCamera::SetProjectionMatrix(const FMatrix4x4& mat)
	{
		mProjectionMatrix = mat;
		MakeProjectionMatrixDirty();
	}

	void FCamera::SetPosition(const FVector3f& p)
	{
		mTransform.SetPosition(p);
		MakeWorldMatrixDirty();
	}

	void FCamera::SetRotation(const FQuaternion& q)
	{
		mTransform.SetRotation(q);
		MakeWorldMatrixDirty();
	}

	void FCamera::SetRotation(Scalar pitch, Scalar yaw, Scalar roll)
	{
		mTransform.SetEuler(pitch, yaw, roll);
		MakeWorldMatrixDirty();
	}

	void FCamera::SetFarClip(Scalar farZ)
	{
		mFar = farZ;
		MakeProjectionMatrixDirty();
	}

	void FCamera::SetNearClip(Scalar nearZ)
	{
		mNear = nearZ;
		MakeProjectionMatrixDirty();
	}

	void FCamera::SetViewPort(const FViewport& vp)
	{
		mViewPort = vp;
	}

	void FCamera::SetLookAt(const FVector3f& eye, const FVector3f& lookAt, const FVector3f& up)
	{
		SetLookTo(eye, lookAt - eye, up);
	}

	void FCamera::SetLookTo(const FVector3f& eye, const FVector3f& lookTo, const FVector3f& up)
	{
		mTransform.SetLookTo(eye, lookTo, up);
		MakeWorldMatrixDirty();
	}

	void FCamera::TranslateForward(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() + speed * mTransform.GetUnitForwardAxis());
		MakeWorldMatrixDirty();
	}

	void FCamera::TranslateRight(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() + speed * mTransform.GetUnitRightAxis());
		MakeWorldMatrixDirty();
	}

	void FCamera::TranslateUp(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() + speed * mTransform.GetUnitUpAxis());
		MakeWorldMatrixDirty();
	}

	void FCamera::TranslateBack(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() - speed * mTransform.GetUnitForwardAxis());
		MakeWorldMatrixDirty();
	}

	void FCamera::TranslateLeft(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() - speed * mTransform.GetUnitRightAxis());
		MakeWorldMatrixDirty();
	}

	void FCamera::TranslateDown(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() - speed * mTransform.GetUnitUpAxis());
		MakeWorldMatrixDirty();
	}

	void FCamera::AddPitch(Scalar angle)
	{
		FQuaternion rotation = FMath::FromAxisAngle(FMath::Mul(mTransform.GetRotation(), FVector3f(FUnit<0>{})), FMath::Radians(angle)) * mTransform.GetRotation();
		mTransform.SetRotation(rotation);
		MakeWorldMatrixDirty();
	}

	void FCamera::AddYaw(Scalar angle)
	{
		FQuaternion rotation = FMath::FromAxisAngle(FMath::Mul(mTransform.GetRotation(), FVector3f(FUnit<1>{})), FMath::Radians(angle))* mTransform.GetRotation();
		mTransform.SetRotation(rotation);
		MakeWorldMatrixDirty();
	}

	void FCamera::AddRoll(Scalar angle)
	{
		FQuaternion rotation = FMath::FromAxisAngle(FMath::Mul(mTransform.GetRotation(), FVector3f(FUnit<2>{})), FMath::Radians(angle))* mTransform.GetRotation();
		mTransform.SetRotation(rotation);
		MakeWorldMatrixDirty();
	}




	//OrthographicCamera

	OrthographicCamera::OrthographicCamera(Scalar minX, Scalar minY, Scalar maxX, Scalar maxY, Scalar nearZ, Scalar farZ, const FViewport& vp)
		: FCamera(nearZ, farZ, vp)
		, mXMin(minX)
		, mXMax(maxX)
		, mYMin(minY)
		, mYMax(maxY)
	{
		UpdateProjectionMatrix();
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::SetMinX(Scalar minX)
	{
		mXMin = minX;
		MakeProjectionMatrixDirty();
	}

	void OrthographicCamera::SetMinY(Scalar minY)
	{
		mYMin = minY;
		MakeProjectionMatrixDirty();
	}

	void OrthographicCamera::SetMaxX(Scalar maxX)
	{
		mXMax = maxX;
		MakeProjectionMatrixDirty();
	}

	void OrthographicCamera::SetMaxY(Scalar maxY)
	{
		mYMax = maxY;
		MakeProjectionMatrixDirty();
	}

	void OrthographicCamera::CreateProjectionMatrix() const
	{
		Scalar oneOverWidth = Scalar{ 1 } / mXMax - mXMin;
		Scalar oneOverHeight = Scalar{ 1 }/ mYMax - mYMin;
		Scalar oneOverDepth = Scalar{ 1 } / mFar - mNear;

		mProjectionMatrix.SetRow(0, FVector4f{ 2 * oneOverWidth, Scalar{0}, Scalar{0}, -(mXMax + mXMin) * oneOverWidth });
		mProjectionMatrix.SetRow(1, FVector4f{ Scalar{0}, 2 * oneOverHeight, Scalar{0}, -(mYMax + mYMin) * oneOverHeight });
		mProjectionMatrix.SetRow(2, FVector4f{ Scalar{0}, Scalar{0}, oneOverDepth, -mNear * oneOverDepth });
		mProjectionMatrix.SetRow(3, FVector4f{ Scalar{0}, Scalar{0}, Scalar{0}, Scalar{1} });
	}




	//PerspectiveCamera

	PerspectiveCamera::PerspectiveCamera(Scalar fov, Scalar aspect, Scalar nearZ, Scalar farZ, const FViewport& vp)
		: FCamera(nearZ, farZ, vp)
		, mFov(fov)
		, mAspect(aspect)
	{
		UpdateProjectionMatrix();
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::SetFieldOfView(Scalar fov)
	{
		mFov = fov;
		MakeProjectionMatrixDirty();
	}

	void PerspectiveCamera::SetAspectRatio(Scalar aspect)
	{
		mAspect = aspect;
		MakeProjectionMatrixDirty();
	}

	FRay PerspectiveCamera::GenerateRay(Scalar u, Scalar v) const
	{
		FVector3f camPos = GetPosition();
		FVector3f forward = GetForward();
		FVector3f right = GetRight();
		FVector3f up = GetUp();
		
		Scalar nearPlaneHeight = mNear * FMath::Tan(FMath::Radians(mFov) * Scalar{0.5});
		Scalar nearPlaneWidth = nearPlaneHeight * mAspect;

		const FVector3f horizon = right * nearPlaneWidth;
		const FVector3f vertical = up * nearPlaneHeight;

		const FVector3f topRightCorner = camPos + forward * mNear - horizon * 0.5f + vertical * 0.5f;

		return FRay{ camPos, FMath::Normalize(topRightCorner + u * horizon - v * vertical - camPos), 0.0f, 1000.0f };
	}

	void PerspectiveCamera::CreateProjectionMatrix() const
	{
		mProjectionMatrix = FMath::Frustum(mFov, mAspect, mNear, mFar);
	}

}
