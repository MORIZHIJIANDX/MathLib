#include "Camera.h"

namespace Dash
{
	Camera::Camera(Math::Scalar nearZ, Math::Scalar farZ, const Viewport& vp)
		: mNear(nearZ)
		, mFar(farZ)
		, mViewPort(vp)
		, mProjectionMatrix(Math::Identity{})
		, mViewProjectionMatrix(Math::Identity{})
		, mTransform(Math::Identity{})
		, mWorldMatrixDirty(true)
		, mProjectionMatrixDirty(true)
	{

	}

	Camera::~Camera()
	{
	}

	Math::Matrix4x4 Camera::GetViewMatrix() const
	{
		return mTransform.GetInverseMatrix();
	}

	Math::Matrix4x4 Camera::GetProjectionMatrix() const
	{
		if (mProjectionMatrixDirty)
		{
			UpdateProjectionMatrix();
		}

		return mProjectionMatrix;
	}

	Math::Matrix4x4 Camera::GetViewProjectionMatrix() const
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

	Math::Vector3f Camera::GetPosition() const
	{
		return mTransform.GetPosition();
	}

	Math::Quaternion Camera::GetRotation() const
	{
		return mTransform.GetRotation();
	}

	Math::Vector3f Camera::GetForward() const
	{
		return mTransform.GetUnitForwardAxis();
	}

	Math::Vector3f Camera::GetRight() const
	{
		return mTransform.GetUnitRightAxis();
	}

	Math::Vector3f Camera::GetUp() const
	{
		return mTransform.GetUnitUpAxis();
	}

	Math::Scalar Camera::GetFar() const
	{
		return mFar;
	}

	Math::Scalar Camera::GetNear() const
	{
		return mNear;
	}

	Viewport Camera::GetViewPort() const
	{
		return mViewPort;
	}

	void Camera::SetWorldMatrix(const Math::Matrix4x4& mat)
	{
		mTransform = Math::Transform{ mat };
	
		MakeWorldMatrixDirty();
	}

	void Camera::SetProjectionMatrix(const Math::Matrix4x4& mat)
	{
		mProjectionMatrix = mat;
		MakeProjectionMatrixDirty();
	}

	void Camera::SetPosition(const Math::Vector3f& p)
	{
		mTransform.SetPosition(p);
		MakeWorldMatrixDirty();
	}

	void Camera::SetRotation(const Math::Quaternion& q)
	{
		mTransform.SetRotation(q);
		MakeWorldMatrixDirty();
	}

	void Camera::SetRotation(Math::Scalar pitch, Math::Scalar yaw, Math::Scalar roll)
	{
		mTransform.SetEuler(pitch, yaw, roll);
		MakeWorldMatrixDirty();
	}

	void Camera::SetFarClip(Math::Scalar farZ)
	{
		mFar = farZ;
		MakeProjectionMatrixDirty();
	}

	void Camera::SetNearClip(Math::Scalar nearZ)
	{
		mNear = nearZ;
		MakeProjectionMatrixDirty();
	}

	void Camera::SetViewPort(const Viewport& vp)
	{
		mViewPort = vp;
	}

	void Camera::SetLookAt(const Math::Vector3f& eye, const Math::Vector3f& lookAt, const Math::Vector3f& up)
	{
		SetLookTo(eye, lookAt - eye, up);
	}

	void Camera::SetLookTo(const Math::Vector3f& eye, const Math::Vector3f& lookTo, const Math::Vector3f& up)
	{
		mTransform.SetLookTo(eye, lookTo, up);
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateForward(Math::Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() + speed * mTransform.GetUnitForwardAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateRight(Math::Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() + speed * mTransform.GetUnitRightAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateUp(Math::Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() + speed * mTransform.GetUnitUpAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateBack(Math::Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() - speed * mTransform.GetUnitForwardAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateLeft(Math::Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() - speed * mTransform.GetUnitRightAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::TranslateDown(Math::Scalar speed)
	{
		mTransform.SetPosition(mTransform.GetPosition() - speed * mTransform.GetUnitUpAxis());
		MakeWorldMatrixDirty();
	}

	void Camera::AddPitch(Math::Scalar angle)
	{
		Math::Quaternion rotation = Math::FromAxisAngle(Math::Mul(mTransform.GetRotation(), Math::Vector3f(Math::Unit<0>{})), Math::Radians(angle)) * mTransform.GetRotation();
		mTransform.SetRotation(rotation);
		MakeWorldMatrixDirty();
	}

	void Camera::AddYaw(Math::Scalar angle)
	{
		Math::Quaternion rotation = Math::FromAxisAngle(Math::Mul(mTransform.GetRotation(), Math::Vector3f(Math::Unit<1>{})), Math::Radians(angle))* mTransform.GetRotation();
		mTransform.SetRotation(rotation);
		MakeWorldMatrixDirty();
	}

	void Camera::AddRoll(Math::Scalar angle)
	{
		Math::Quaternion rotation = Math::FromAxisAngle(Math::Mul(mTransform.GetRotation(), Math::Vector3f(Math::Unit<2>{})), Math::Radians(angle))* mTransform.GetRotation();
		mTransform.SetRotation(rotation);
		MakeWorldMatrixDirty();
	}




	//OrthographicCamera

	OrthographicCamera::OrthographicCamera(Math::Scalar minX, Math::Scalar minY, Math::Scalar maxX, Math::Scalar maxY, Math::Scalar nearZ, Math::Scalar farZ, const Viewport& vp)
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

	void OrthographicCamera::SetMinX(Math::Scalar minX)
	{
		mXMin = minX;
		MakeProjectionMatrixDirty();
	}

	void OrthographicCamera::SetMinY(Math::Scalar minY)
	{
		mYMin = minY;
		MakeProjectionMatrixDirty();
	}

	void OrthographicCamera::SetMaxX(Math::Scalar maxX)
	{
		mXMax = maxX;
		MakeProjectionMatrixDirty();
	}

	void OrthographicCamera::SetMaxY(Math::Scalar maxY)
	{
		mYMax = maxY;
		MakeProjectionMatrixDirty();
	}

	void OrthographicCamera::CreateProjectionMatrix() const
	{
		Math::Scalar oneOverWidth = Math::Scalar{ 1 } / mXMax - mXMin;
		Math::Scalar oneOverHeight = Math::Scalar{ 1 }/ mYMax - mYMin;
		Math::Scalar oneOverDepth = Math::Scalar{ 1 } / mFar - mNear;

		mProjectionMatrix.SetRow(0, Math::Vector4f{ 2 * oneOverWidth, Math::Scalar{0}, Math::Scalar{0}, -(mXMax + mXMin) * oneOverWidth });
		mProjectionMatrix.SetRow(1, Math::Vector4f{ Math::Scalar{0}, 2 * oneOverHeight, Math::Scalar{0}, -(mYMax + mYMin) * oneOverHeight });
		mProjectionMatrix.SetRow(2, Math::Vector4f{ Math::Scalar{0}, Math::Scalar{0}, oneOverDepth, -mNear * oneOverDepth });
		mProjectionMatrix.SetRow(3, Math::Vector4f{ Math::Scalar{0}, Math::Scalar{0}, Math::Scalar{0}, Math::Scalar{1} });
	}




	//PerspectiveCamera

	PerspectiveCamera::PerspectiveCamera(Math::Scalar fov, Math::Scalar aspect, Math::Scalar nearZ, Math::Scalar farZ, const Viewport& vp)
		: Camera(nearZ, farZ, vp)
		, mFov(fov)
		, mAspect(aspect)
	{
		UpdateProjectionMatrix();
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::SetFieldOfView(Math::Scalar fov)
	{
		mFov = fov;
		MakeProjectionMatrixDirty();
	}

	void PerspectiveCamera::SetAspectRatio(Math::Scalar aspect)
	{
		mAspect = aspect;
		MakeProjectionMatrixDirty();
	}

	Math::Ray PerspectiveCamera::GenerateRay(Math::Scalar u, Math::Scalar v) const
	{
		Math::Vector3f camPos = GetPosition();
		Math::Vector3f forward = GetForward();
		Math::Vector3f right = GetRight();
		Math::Vector3f up = GetUp();
		
		Math::Scalar nearPlaneHeight = mNear * Math::Tan(Math::Radians(mFov) * Math::Scalar{0.5});
		Math::Scalar nearPlaneWidth = nearPlaneHeight * mAspect;

		const Math::Vector3f horizon = right * nearPlaneWidth;
		const Math::Vector3f vertical = up * nearPlaneHeight;

		const Math::Vector3f topRightCorner = camPos + forward * mNear - horizon * 0.5f + vertical * 0.5f;

		return Math::Ray{ camPos, Math::Normalize(topRightCorner + u * horizon - v * vertical - camPos), 0.0f, 1000.0f };
	}

	void PerspectiveCamera::CreateProjectionMatrix() const
	{
		mProjectionMatrix = Math::Frustum(mFov, mAspect, mNear, mFar);
	}

}
