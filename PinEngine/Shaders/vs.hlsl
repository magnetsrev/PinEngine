#pragma pack_matrix( row_major )

cbuffer transformationBuffer : register(b0)
{
    float4x4 wvpMatrix;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.inPos, 1.0f), wvpMatrix);
    return output;
}