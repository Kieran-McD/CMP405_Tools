#pragma once
#include "pch.h"
#include "InputCommands.h"
#include <vector>

using namespace DirectX::SimpleMath;

class Camera
{
	
public:
	Camera();
	~Camera();
	void Update(InputCommands Input);
	
	Matrix GetViewMatrix();
private:
	
	//functionality
	float								m_movespeed;
	float								m_camRotRate;

	//camera
	Vector3		m_camPosition;
	Vector3		m_camOrientation;
	Vector3		m_camLookAt;
	Vector3		m_camLookDirection;
	Vector3		m_camRight;
	Vector3 m_camUp;


	Matrix                                             m_view;

};

