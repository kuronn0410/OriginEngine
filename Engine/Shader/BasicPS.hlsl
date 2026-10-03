struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};


float4 main(VSOutput input) : SV_TARGET
{
    return float4(input.uv.x, input.uv.y, 0.0f, 1.0f);
    //return float4(input.normal.x, input.normal.y, input.normal.z, 1.0f);
}