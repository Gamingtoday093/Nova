#include "novapch.h"
#include "FreeLookCamera.h"
#include "Nova/Input/Input.h"
#include "Nova/Time/Time.h"

Nova::FreeLookCamera::FreeLookCamera(XMFLOAT3 position, XMFLOAT2 yawPitch, float speed, float rotationSpeed)
{
	m_Position = position;
	m_YawPitch = yawPitch;

	m_Speed = speed;
	m_RotationSpeed = rotationSpeed;

	CalculateViewMatrix();
}

void Nova::FreeLookCamera::Update()
{
	bool flying = Input::KeyHeld(EMouseButton::RIGHT);
	bool drag = Input::KeyHeld(EMouseButton::MIDDLE);
	short scroll = Input::GetScrollDelta();
	if (!drag && !flying && scroll == 0) return;
	XMFLOAT3 setDelta{};
	if (flying)
	{
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
	}
	else if (drag)
	{
		POINT mouseDelta = Input::GetMouseDelta();
		setDelta.x = -float(mouseDelta.x) * 10.f;
		setDelta.y = float(mouseDelta.y) * 10.f;
	}
	else
	{
		if (scroll > 1) setDelta.z -= 1.f;
		else setDelta.z += 1.f;

		setDelta.z *= 1000.f;
	}
	XMVECTOR moveDelta = XMLoadFloat3(&setDelta);
	if (flying) moveDelta = XMVector3Normalize(moveDelta);
	if (XMVector3LengthSq(moveDelta).m128_f32[0] > 0.5f)
	{
		moveDelta = XMVector3TransformCoord(moveDelta, XMMatrixRotationRollPitchYaw(m_YawPitch.y, m_YawPitch.x, 0));
		moveDelta *= m_Speed * Time::GetDeltaTime();
		if (flying && Input::KeyHeld(EKey::SHIFT))
		{
			moveDelta *= 2.f;
		}
	}
	XMStoreFloat3(&m_Position, XMLoadFloat3(&m_Position) + moveDelta);

	if (flying)
	{
		POINT mouseDelta = Input::GetMouseDelta();
		m_YawPitch.x += mouseDelta.x * m_RotationSpeed * Time::GetDeltaTime();
		m_YawPitch.y += mouseDelta.y * m_RotationSpeed * Time::GetDeltaTime();
	}

	CalculateViewMatrix();
}

XMMATRIX Nova::FreeLookCamera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

void Nova::FreeLookCamera::CalculateViewMatrix()
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(m_YawPitch.y, m_YawPitch.x, 0);
	m_ViewMatrix = XMMatrixLookToLH(
		XMLoadFloat3(&m_Position),
		XMVector3Transform(XMVectorSet(0, 0, 1, 0), rotationMatrix),
		XMVector3Transform(XMVectorSet(0, 1, 0, 0), rotationMatrix));
}
