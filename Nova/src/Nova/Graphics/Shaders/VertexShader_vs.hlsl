struct VS_INPUT
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float2 texcoord : TEXCOORD;
};

cbuffer transformBuffer : register(b0)
{
    float4x4 ProjectionViewMatrix;
    float4x4 ModelMatrix;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.worldPosition = mul(ModelMatrix, float4(input.position, 1.f));
    output.position = mul(ProjectionViewMatrix, output.worldPosition);
    output.texcoord = input.texcoord;
    
	return output;
}
