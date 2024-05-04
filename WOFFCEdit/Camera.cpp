#include "Camera.h"

Camera::Camera()
{
	//functional
	m_movespeed = 0.30;
	m_camRotRate = 3.0;

	//camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;
	m_camOrientation.y = 0;
	m_camOrientation.z = 0;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 1.0f;

	m_camUp.x = 0.0f;
	m_camUp.y = 1.0f;
	m_camUp.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;
}


Camera::~Camera()
{
}

void Camera::Update(InputCommands Input)
{
	//TODO  any more complex than this, and the camera should be abstracted out to somewhere else
	//camera motion is on a plane, so kill the 7 component of the look direction
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;


	if (Input.rotRight)
	{
		m_camOrientation.y -= m_camRotRate;
	}
	if (Input.rotLeft)
	{
		m_camOrientation.y += m_camRotRate;
	}
	if (Input.mouseRightButton) {


		m_camOrientation.y += Input.mouseXDrag;
		m_camOrientation.x -= Input.mouseYDrag;
		//m_camLookAt = Vector3(0, 0, 0);
		//ArcBallMotion(Input);
		//create look direction from Euler angles in m_camOrientation
		m_camLookDirection.x = cos((m_camOrientation.y) * 3.1415 / 180) * cos(m_camOrientation.x * 3.1415 / 180);
		m_camLookDirection.y = sin((m_camOrientation.x) * 3.1415 / 180);
		m_camLookDirection.z = sin((m_camOrientation.y) * 3.1415 / 180) * cos(m_camOrientation.x * 3.1415 / 180);
		m_camLookDirection.Normalize();

		//create right vector from look Direction
		m_camLookDirection.Cross(Vector3::UnitY, m_camRight);
		m_camRight.Normalize();
		m_camLookDirection.Cross(m_camRight, m_camUp);
		m_camUp.Normalize();
		//process input and update stuff
		if (Input.forward)
		{
			//Vector3 direction = m_camLookAt - m_camPosition;
			//direction.Normalize();
			//m_camPosition += direction * m_movespeed;

			m_camPosition += m_camLookDirection * m_movespeed;
		}
		if (Input.back)
		{
		/*	Vector3 direction = m_camLookAt - m_camPosition;
			direction.Normalize();
			m_camPosition -= direction * m_movespeed;*/

			m_camPosition -= m_camLookDirection * m_movespeed;
		}
		if (Input.right)
		{
			Vector3 direction = m_camLookAt - m_camPosition;
			direction.Normalize();
			m_camPosition -= direction * m_movespeed;
			

			m_camPosition += m_camRight * m_movespeed;
		}
		if (Input.left)
		{
			m_camPosition -= m_camRight * m_movespeed;
		}
		if (Input.up) {
			m_camPosition -= m_camUp * m_movespeed;
		}
		if (Input.down) {
			m_camPosition += m_camUp * m_movespeed;
		}

		//update lookat point
		m_camLookAt = m_camPosition + m_camLookDirection;

	}

	//apply camera vectors
	m_camView = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);
}



Matrix Camera::GetViewMatrix()
{
	return m_camView;
}

void Camera::SetUpProjectionMatrix(float aspectRatio)
{
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	m_camProjection = Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		1000.0f
	);
}

Matrix Camera::GetProjectionMatrix()
{
	return m_camProjection;
}

void Camera::ArcBallMotion(InputCommands Input)
{
	// step 1 : Calculate the amount of rotation given the mouse movement.
	float xAngle = m_camOrientation.x + Input.mouseYDrag * 3.1415 / 180;
	float yAngle = m_camOrientation.y + Input.mouseXDrag * 3.1415 / 180;

	// Get the homogenous position of the camera and pivot point
	Vector4 position = Vector4(m_camPosition.x, m_camPosition.y, m_camPosition.z, 1);
	Vector4 pivot = Vector4(m_camLookAt.x, m_camLookAt.y, m_camLookAt.z, 1);

	// step 2: Rotate the camera around the pivot point on the first axis.
	Matrix rotationMatrixX = Matrix::Identity;
	rotationMatrixX *= Matrix::CreateRotationX(xAngle);
	position = Vector4::Transform((position - pivot), rotationMatrixX) + pivot;
	
	
	// step 3: Rotate the camera around the pivot point on the second axis.
	Matrix rotationMatrixY = Matrix::Identity;
	rotationMatrixY *= Matrix::CreateRotationY(yAngle);
	Vector3 finalPosition = Vector3::Transform((Vector3(position.x, position.y, position.z) - Vector3(pivot.x, pivot.y, pivot.z)), rotationMatrixY) + pivot;
	
	m_camView = Matrix::CreateLookAt(finalPosition, m_camLookAt, Vector3::UnitY);

	m_camOrientation.x = xAngle;
	m_camOrientation.y = yAngle;
}
