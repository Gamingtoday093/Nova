struct VS_INPUT
{
    float3 position : POSITION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION;
};

cbuffer transformBuffer : register(b0)
{
    float4x4 ProjectionViewMatrix;
    float4x4 ModelMatrix;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.worldPosition = input.position;
    output.position = mul(ProjectionViewMatrix, float4(output.worldPosition, 0.f));
    // make sure that the depth after w divide will be 1.0 (so that the z-buffering will work)
    output.position.z = output.position.w;
    
	return output;
}
