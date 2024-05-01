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


	//if (Input.rotRight)
	//{
	//	m_camOrientation.y -= m_camRotRate;
	//}
	//if (Input.rotLeft)
	//{
	//	m_camOrientation.y += m_camRotRate;
	//}
	if (Input.mouseRightButton) {


		m_camOrientation.y += Input.mouseXDrag;
		m_camOrientation.x -= Input.mouseYDrag;

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
			m_camPosition += m_camLookDirection * m_movespeed;
		}
		if (Input.back)
		{
			m_camPosition -= m_camLookDirection * m_movespeed;
		}
		if (Input.right)
		{
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
	m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);
}

Matrix Camera::GetViewMatrix()
{
	return m_view;
}
