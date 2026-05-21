Texture2D AlbedoTexture : register(t0);
sampler AlbedoSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return AlbedoTexture.Sample(AlbedoSampler, input.texcoord);
}