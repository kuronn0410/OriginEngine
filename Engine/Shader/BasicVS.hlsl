struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

cbuffer Transform : register(b0)
{
    row_major float4x4 world;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    output.position = mul(float4(input.position, 1.0f), world);

    return output;
}