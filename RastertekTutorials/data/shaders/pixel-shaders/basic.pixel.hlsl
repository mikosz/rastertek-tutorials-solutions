Texture2D tex;
SamplerState samplerState;

cbuffer LightBuffer {
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
}

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 textureCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColour = tex.Sample(samplerState, input.textureCoord);
	float lightIntensity = saturate(dot(input.normal, -lightDirection));
	float4 lightColour = saturate(diffuseColour * lightIntensity);

	return lightColour * textureColour;
}
