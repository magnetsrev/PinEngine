#pragma pack_matrix( row_major )

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
    output.position = float4(input.inPos, 1.0f);
    return output;
}