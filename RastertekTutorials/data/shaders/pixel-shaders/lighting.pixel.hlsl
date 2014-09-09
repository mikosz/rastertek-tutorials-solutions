Texture2D tex;
SamplerState samplerState;

cbuffer LightBuffer {
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float specularPower;
	float4 specularColour;
}

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 textureCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColour = tex.Sample(samplerState, input.textureCoord);

	float4 lightColour = ambientColour;

	float4 specular;

	float lightIntensity = saturate(dot(input.normal, -lightDirection));
	if (lightIntensity > 0.0f) {
		lightColour = saturate(lightColour + saturate(diffuseColour * lightIntensity));

		float3 reflection = normalize(2 * lightIntensity * input.normal - lightDirection);
		specular = specularColour * pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}

	return saturate(lightColour * textureColour + specular);
}
