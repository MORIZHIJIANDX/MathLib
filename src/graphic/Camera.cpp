#include "Camera.h"

namespace Dash
{
	Camera::Camera(Scalar nearZ, Scalar farZ, const Viewport& vp)
		: mNear(nearZ)
		, mFar(farZ)
		, mViewPort(vp)
		, mProjectionMatrix(Identity{})
		, mViewProjectionMatrix(Identity{})
		, mTransform(Identity{})
		, mWorldMatrixDirty(true)
		, mProjectionMatrixDirty(true)
	{

	}

	Camera::~Camera()
	{
	}

	Matrix4x4 Camera::GetViewMatrix() const
	{
		return mTransform.GetInverseMatrix();
	}

	Matrix4x4 Camera::GetProjectionMatrix() const
	{
		if (mProjectionMatrixDirty)
		{
			UpdateProjectionMatrix();
		}

		return mProjectionMatrix;
	}

	Matrix4x4 Camera::GetViewProjectionMatrix() const
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

	Vector3f Camera::GetPosition() const
	{
		return mTransform.GetPosition();
	}

	Quaternion Camera::GetRotation() const
	{
		return mTransform.GetRotation();
	}

	Vector3f Camera::GetForward() const
	{
		return mTransform.GetUnitForwardAxis();
	}

	Vector3f Camera::GetRight() const
	{
		return mTransform.GetUnitRightAxis();
	}

	Vector3f Camera::GetUp() const
	{
		return mTransform.GetUnitUpAxis();
	}

	Scalar Camera::GetFar() const
	{
		return mFar;
	}

	Scalar Camera::GetNear() const
	{
		return mNear;
	}

	Viewport Camera::GetViewPort() const
	{
		return mViewPort;
	}

	void Camera::SetWorldMatrix(const Matrix4x4& mat)
	{
		mTransform = Transform{ mat };
	
		MakeWorldMatrixDirty();
	}

	void Camera::SetProjectionMatrix(const Matrix4x4& mat)
	{
		mProjectionMatrix = mat;
		MakeProjectionMatrixDirty();
	}

	void Camera::SetPosition(const Vector3f& p)
	{
		mTransform.SetPosition(p);
		MakeWorldMatrixDirty();
	}

	void Camera::SetRotation(const Quaternion& q)
	{
		mTransform.SetRotation(q);
		MakeWorldMatrixDirty();
	}

	void Camera::SetRotation(Scalar pitch, Scalar yaw, Scalar roll)
	{
		mTransform.SetEuler(pitch, yaw, roll);
		MakeWorldMatrixDirty();
	}

	void Camera::SetFarClip(Scalar farZ)
	{
		mFar = farZ;
		MakeProjectionMatrixDirty();
	}

	void Camera::SetNearClip(Scalar nearZ)
	{
		mNear = nearZ;
		MakeProjectionMatrixDirty();
	}

	void Camera::SetViewPort(const Viewport& vp)
	{
		mViewPort = vp;
	}

	void Camera::SetLookAt(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up)
	{
		SetLookTo(eye, lookAt - eye, up);
	}

	void Camera::SetLookTo(const Vector3f& eye, const Vector3f& lookTo, const Vector3f& up)
	{
		mTransform.SetLookTo(eye, lookTo, up);
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateForward(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() + speed * mTransform.GetUnitForwardAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateRight(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() + speed * mTransform.GetUnitRightAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateUp(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() + speed * mTransform.GetUnitUpAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateBack(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() - speed * mTransform.GetUnitForwardAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateLeft(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() - speed * mTransform.GetUnitRightAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateDown(Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() - speed * mTransform.GetUnitUpAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::AddPitch(Scalar angle)
	{
		Quaternion rotation = Math::FromAxisAngle(Math::Mul(mTransform.GetRotation(), Vector3f(Unit<0>{})), Math::Radians(angle)) * mTransform.GetRotation();
		mTransform.SetRotation(rotation);
		MakeWorldMatrixDirty();
	}

	void Camera::AddYaw(Scalar angle)
	{
		Quaternion rotation = Math::FromAxisAngle(Math::Mul(mTransform.GetRotation(), Vector3f(Unit<1>{})), Math::Radians(angle))* mTransform.GetRotation();
		mTransform.SetRotation(rotation);
		MakeWorldMatrixDirty();
	}

	void Camera::AddRoll(Scalar angle)
	{
		Quaternion rotation = Math::FromAxisAngle(Math::Mul(mTransform.GetRotation(), Vector3f(Unit<2>{})), Math::Radians(angle))* mTransform.GetRotation();
		mTransform.SetRotation(rotation);
		MakeWorldMatrixDirty();
	}




	//OrthographicCamera

	OrthographicCamera::OrthographicCamera(Scalar minX, Scalar minY, Scalar maxX, Scalar maxY, Scalar nearZ, Scalar farZ, const Viewport& vp)
		: Camera(nearZ, farZ, vp)
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

		mProjectionMatrix.SetRow(0, Vector4f{ 2 * oneOverWidth, Scalar{0}, Scalar{0}, -(mXMax + mXMin) * oneOverWidth });
		mProjectionMatrix.SetRow(1, Vector4f{ Scalar{0}, 2 * oneOverHeight, Scalar{0}, -(mYMax + mYMin) * oneOverHeight });
		mProjectionMatrix.SetRow(2, Vector4f{ Scalar{0}, Scalar{0}, oneOverDepth, -mNear * oneOverDepth });
		mProjectionMatrix.SetRow(3, Vector4f{ Scalar{0}, Scalar{0}, Scalar{0}, Scalar{1} });
	}




	//PerspectiveCamera

	PerspectiveCamera::PerspectiveCamera(Scalar fov, Scalar aspect, Scalar nearZ, Scalar farZ, const Viewport& vp)
		: Camera(nearZ, farZ, vp)
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

	Ray PerspectiveCamera::GenerateRay(Scalar u, Scalar v) const
	{
		Vector3f camPos = GetPosition();
		Vector3f forward = GetForward();
		Vector3f right = GetRight();
		Vector3f up = GetUp();
		
		Scalar nearPlaneHeight = mNear * Math::Tan(Math::Radians(mFov) * Scalar{0.5});
		Scalar nearPlaneWidth = nearPlaneHeight * mAspect;

		const Vector3f horizon = right * nearPlaneWidth;
		const Vector3f vertical = up * nearPlaneHeight;

		const Vector3f topRightCorner = camPos + forward * mNear - horizon * 0.5f + vertical * 0.5f;

		return Ray{ camPos, Math::Normalize(topRightCorner + u * horizon - v * vertical - camPos), 0.0f, 1000.0f };
	}

	void PerspectiveCamera::CreateProjectionMatrix() const
	{
		mProjectionMatrix = Math::Frustum(mFov, mAspect, mNear, mFar);
	}

}
