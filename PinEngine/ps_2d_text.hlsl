struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float4 inColor : COLOR;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	
    //return float4(1, 1, 1, 1);
    float4 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    if (sampleColor.r  == 0)
        discard;

    float4 newColor = input.inColor * sampleColor.r;

    return float4(newColor);
}