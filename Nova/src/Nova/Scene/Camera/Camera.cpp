#include "novapch.h"
#include "Camera.h"
#include "Nova/Graphics/DX11.h"

XMVECTOR XM_CALLCONV Nova::Camera::ScreenToWorld(XMVECTOR position) const
{
	position -= XMVectorSet(0.5f, 0.5f, 0.f, 0.f);
	position *= XMVectorSet(2.f, -2.f, 0.f, 0.f);

	XMMATRIX matrix =
		GetViewMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(FovAngle, Graphics::DX11::GetAspectRatio_s(), NearClipPlane, FarClipPlane);
	matrix = DirectX::XMMatrixInverse(nullptr, matrix);

	return XMVector3TransformCoord(position, matrix);
}

XMVECTOR XM_CALLCONV Nova::Camera::WorldToScreen(XMVECTOR position) const
{
	XMMATRIX matrix =
		GetViewMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(FovAngle, Graphics::DX11::GetAspectRatio_s(), NearClipPlane, FarClipPlane);

	position = XMVector3TransformCoord(position, matrix);
	position *= XMVectorSet(0.5f, -0.5f, 0.f, 0.f);
	position += XMVectorSet(0.5f, 0.5f, 0.f, 0.f);
	return position;
}
