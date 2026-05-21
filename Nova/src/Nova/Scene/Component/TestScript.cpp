#include "novapch.h"
#include "TestScript.h"
#include "Components.h"
#include "Nova/Tools/QuaternionExtensions.h"

void Nova::TestScript::Awake()
{
	NOVA_INFO("Test Script Awake!");
}

void Nova::TestScript::Start()
{

}

void Nova::TestScript::Update()
{
	auto& transform = GetComponent<TransformComponent>().Transform;
	XMVECTOR forward = XMVector3Normalize(XMLoadFloat3(&transform.Position) - XMLoadFloat3(&m_Entity.GetScene()->m_FreeLookCamera.m_Position));
	transform.Rotation = XMQuaternionToEulerAngles(XMQuaternionLookRotation(forward));
}
