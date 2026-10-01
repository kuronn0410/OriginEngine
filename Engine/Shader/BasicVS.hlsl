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
    row_major float4x4 view;
    row_major float4x4 projection;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    float4 position = float4(input.position, 1.0f);

    position = mul(position, world);
    position = mul(position, view);
    position = mul(position, projection);
    
    output.position = position;

    return output;
}