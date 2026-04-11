TextureCube skyboxTexure : register(t0);
SamplerState skyboxSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return skyboxTexure.Sample(skyboxSampler, input.worldPosition);
}
