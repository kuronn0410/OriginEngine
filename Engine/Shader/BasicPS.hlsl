struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    return texture0.Sample(sampler0, input.uv);
    //return float4(input.normal.x, input.normal.y, input.normal.z, 1.0f);
}