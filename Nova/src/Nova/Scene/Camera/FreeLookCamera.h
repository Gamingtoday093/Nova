#pragma once
#include "Camera.hpp"

using namespace DirectX;

namespace Nova
{
	class FreeLookCamera : public Camera
	{
	public:
		FreeLookCamera(XMFLOAT3 position, XMFLOAT2 yawPitch, float speed, float rotationSpeed);

		void Update();
		
		XMMATRIX GetViewMatrix() const override;
		
		float m_Speed;
		float m_RotationSpeed;

		XMFLOAT3 m_Position;
		XMFLOAT2 m_YawPitch;

	private:
		void CalculateViewMatrix();
		
		XMMATRIX m_ViewMatrix = {};
	};
}

