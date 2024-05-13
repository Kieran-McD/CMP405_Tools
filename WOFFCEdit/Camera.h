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
	
	//Getters/Setters
	Matrix GetViewMatrix();

	void SetUpProjectionMatrix(float aspectRatio);
	Matrix GetProjectionMatrix();

	Vector3 GetCamPosition() { return m_camPosition; }

	Vector3 GetCamLookAtDir() { return m_camLookDirection; }

	Vector3 GetCamRightDir() { return m_camRight; }
	
	Vector3 GetCamUpDir() { return m_camUp; }
private:
	
	void ArcBallMotion(InputCommands Input);

	//functionality
	float								m_movespeed;
	float								m_camRotRate;

	//camera
	Vector3		m_camPosition;
	Vector3		m_camOrientation;
	Vector3		m_camLookAt;
	Vector3		m_camLookDirection;
	Vector3		m_camRight;
	Vector3		m_camUp;
	
	float m_fov;
	float m_nearPlane;
	float m_farPlane;

	Matrix		m_camView;
	Matrix		m_camProjection;

};

