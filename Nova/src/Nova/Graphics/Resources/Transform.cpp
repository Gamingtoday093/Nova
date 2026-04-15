#include "novapch.h"
#include "Transform.h"

XMFLOAT3 Nova::Graphics::Transform::GetEulerAngles() const
{
    // FIX! LITTLE BROKEN RIGHT NOW!!

    //XMFLOAT4X4 matrix;
    //XMStoreFloat4x4(&matrix, XMMatrixRotationQuaternion(XMLoadFloat4(&Rotation)));
    //
    //XMFLOAT3 eulerAngles;
    //eulerAngles.x = asinf(-matrix._32);
    //if (cosf(eulerAngles.x) > 16.f * FLT_EPSILON)
    //{
    //    eulerAngles.y = atan2f(matrix._31, matrix._33);
    //    eulerAngles.z = atan2f(matrix._12, matrix._22);
    //}
    //else
    //{
    //    eulerAngles.y = 0.f;
    //    eulerAngles.z = atan2f(-matrix._21, matrix._11);
    //}
    //
    //return eulerAngles;

    float xx = Rotation.x * Rotation.x;
    float yy = Rotation.y * Rotation.y;
    float zz = Rotation.z * Rotation.z;

    float m31 = 2.f * Rotation.x * Rotation.z + 2.f * Rotation.y * Rotation.w;
    float m32 = 2.f * Rotation.y * Rotation.z - 2.f * Rotation.x * Rotation.w;
    float m33 = 1.f - 2.f * xx - 2.f * yy;

    float cy = sqrtf(m33 * m33 + m31 * m31);
    float cx = atan2f(-m32, cy);
    if (cy > 16.f * FLT_EPSILON)
    {
        float m12 = 2.f * Rotation.x * Rotation.y + 2.f * Rotation.z * Rotation.w;
        float m22 = 1.f - 2.f * xx - 2.f * zz;

        return { cx, atan2f(m31, m33), atan2f(m12, m22) };
    }
    else
    {
        float m11 = 1.f - 2.f * yy - 2.f * zz;
        float m21 = 2.f * Rotation.x * Rotation.y - 2.f * Rotation.z * Rotation.w;

        return { cx, 0.f, atan2f(-m21, m11) };
    }
}

void Nova::Graphics::Transform::SetEulerAngles(const XMFLOAT3& rotation)
{
    XMStoreFloat4(&Rotation, XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));
}

XMMATRIX Nova::Graphics::Transform::CalculateMatrix() const
{
    XMVECTOR position = XMLoadFloat3(&Position);
    return XMMatrixAffineTransformation(XMLoadFloat3(&Scale), position, XMLoadFloat4(&Rotation), position);
}
