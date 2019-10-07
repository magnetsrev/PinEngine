struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    //return float4(1, 1, 1, 1);
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    return float4(sampleColor, 1.0f);
}