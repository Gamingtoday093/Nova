#include "novapch.h"
#include "QuaternionExtensions.h"

DirectX::XMFLOAT3 XM_CALLCONV DirectX::XMQuaternionToEulerAngles(XMVECTOR rotation)
{
    XMFLOAT4X4 matrix;
    XMStoreFloat4x4(&matrix, XMMatrixRotationQuaternion(rotation));
    
    XMFLOAT3 eulerAngles;
    eulerAngles.x = asinf(-matrix._32);
    if (cosf(eulerAngles.x) > 16.f * FLT_EPSILON)
    {
        eulerAngles.y = atan2f(matrix._31, matrix._33);
        eulerAngles.z = atan2f(matrix._12, matrix._22);
    }
    else
    {
        eulerAngles.y = 0.f;
        eulerAngles.z = atan2f(-matrix._21, matrix._11);
    }
    
    return eulerAngles;
}

DirectX::XMVECTOR XM_CALLCONV DirectX::XMQuaternionFromToRotation(XMVECTOR from, XMVECTOR to)
{
    from = XMVector3Normalize(from);
    to = XMVector3Normalize(to);

    float dot = XMVectorGetX(XMVector3Dot(from, to));
    if (dot >= 1.f - FLT_EPSILON)
        return XMQuaternionIdentity();

    if (dot <= -1.f + FLT_EPSILON)
    {
        XMVECTOR axis = XMVector3Cross(from, XMVectorSet(1, 0, 0, 0));
        if (XMVectorGetX(XMVector3LengthSq(axis)) <= FLT_EPSILON)
            axis = XMVector3Cross(from, XMVectorSet(0, 1, 0, 0));

        return XMQuaternionRotationNormal(axis, XM_PI);
    }

    // Create a Quaternion that describes the rotation from -> to along the axis that is cross both of them, Ex: Forward -> Right = Axis: Up, Angle: Pi/2
    XMVECTOR axis = XMVector3Cross(from, to);
    return XMQuaternionNormalize(XMVectorSetW(axis, 1.0f + dot));
}

DirectX::XMVECTOR XM_CALLCONV DirectX::XMQuaternionLookRotation(XMVECTOR forward)
{
    return XMQuaternionFromToRotation(XMVectorSet(0, 0, 1, 0), forward);
}

DirectX::XMVECTOR XM_CALLCONV DirectX::XMQuaternionLookRotation(XMVECTOR forward, XMVECTOR up)
{
    forward = XMVector3Normalize(forward);
    up = XMVector3Normalize(up);

    XMVECTOR forwardRot = XMQuaternionFromToRotation(XMVectorSet(0, 0, 1, 0), forward);

    if (XMVectorGetX(XMVector3Dot(forward, up)) >= 1.f - FLT_EPSILON)
        return forwardRot;

    XMVECTOR upRot = XMQuaternionFromToRotation(XMVectorSet(0, 1, 0, 0), up);

    return XMQuaternionMultiply(forwardRot, upRot);
}
