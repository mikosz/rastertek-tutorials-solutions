Texture2D tex[2];
SamplerState samplerState;

cbuffer LightBuffer {
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float specularPower;
	float4 specularColour;
	float gamma;

	float3 padding_;
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
    float4 texture0Colour = tex[0].Sample(samplerState, input.textureCoord);
    float4 texture1Colour = tex[1].Sample(samplerState, input.textureCoord);
	float4 textureColour = saturate(texture0Colour * texture1Colour * gamma);

	float4 lightColour = ambientColour;

	float4 specular = 0;

	float lightIntensity = saturate(dot(input.normal, -lightDirection));
	if (lightIntensity > 0.0f) {
		lightColour = saturate(lightColour + saturate(diffuseColour * lightIntensity));

		float3 reflection = normalize(2 * lightIntensity * input.normal - lightDirection);
		specular = specularColour * pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}

	return saturate(lightColour * textureColour + specular);
}
