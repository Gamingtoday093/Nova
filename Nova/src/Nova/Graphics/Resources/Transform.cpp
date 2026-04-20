#include "novapch.h"
#include "Transform.h"

XMMATRIX Nova::Graphics::Transform::CalculateMatrix() const
{
    return XMMatrixAffineTransformation(
        XMLoadFloat3(&Scale), XMVectorSet(0, 0, 0, 0),
        XMQuaternionRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z),
        XMLoadFloat3(&Position));
}
