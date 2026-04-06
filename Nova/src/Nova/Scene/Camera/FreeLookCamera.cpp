#include "pch.h"
#include "FreeLookCamera.h"
#include "Nova/Input/Input.h"

Nova::FreeLookCamera::FreeLookCamera(XMFLOAT3 position, XMFLOAT2 yawPitch, float speed, float rotationSpeed)
{
	m_Position = position;
	m_YawPitch = yawPitch;

	m_Speed = speed;
	m_RotationSpeed = rotationSpeed;
}

void Nova::FreeLookCamera::Update()
{
	m_CalculatedViewMatrix = false;

	XMFLOAT3 setDelta{};
	if (Input::KeyHeld(EKey::W))
	{
		setDelta.z += 1.f;
	}
	if (Input::KeyHeld(EKey::A))
	{
		setDelta.x += -1.f;
	}
	if (Input::KeyHeld(EKey::S))
	{
		setDelta.z += -1.f;
	}
	if (Input::KeyHeld(EKey::D))
	{
		setDelta.x += 1.f;
	}
	if (Input::KeyHeld(EKey::Q))
	{
		setDelta.y += 1.f;
	}
	if (Input::KeyHeld(EKey::E))
	{
		setDelta.y += -1.f;
	}
	XMVECTOR moveDelta = XMVector3Normalize(XMLoadFloat3(&setDelta));
	if (XMVector3LengthSq(moveDelta).m128_f32[0] > 0.5f)
	{
		moveDelta = XMVector3TransformCoord(moveDelta, XMMatrixRotationRollPitchYaw(m_YawPitch.y, m_YawPitch.x, 0));

		moveDelta *= m_Speed;
		if (Input::KeyHeld(EKey::SHIFT))
		{
			moveDelta *= 2.f;
		}
	}
	XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + moveDelta);

	if (Input::KeyHeld(EMouseButton::RIGHT))
	{
		POINT mouseDelta = Input::GetMouseDelta();
		m_YawPitch.x += mouseDelta.x * m_RotationSpeed;
		m_YawPitch.y += mouseDelta.y * m_RotationSpeed;
	}
}

XMMATRIX Nova::FreeLookCamera::CalculateViewMatrix()
{
	if (m_CalculatedViewMatrix) return m_ViewMatrix;

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(m_YawPitch.y, m_YawPitch.x, 0);
	m_ViewMatrix = XMMatrixLookToLH(
		XMLoadFloat3(&m_Position),
		XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), rotationMatrix),
		XMVector3TransformCoord(XMVectorSet(0, 1, 0, 0), rotationMatrix));

	m_CalculatedViewMatrix = true;
	return m_ViewMatrix;
}
