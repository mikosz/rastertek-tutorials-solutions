Texture2D tex[3];
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
	float3 viewDirection : TEXCOORD1;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 baseTextureColour = tex[0].Sample(samplerState, input.textureCoord);
    float4 detailTextureColour = tex[1].Sample(samplerState, input.textureCoord);
	float4 textureColour = saturate(baseTextureColour * detailTextureColour * gamma);

	float4 bumpMapValue = tex[2].Sample(samplerState, input.textureCoord);
	// Expand the range of the normal value from (0, +1) to (-1, +1)
	bumpMapValue = (bumpMapValue * 2.0f) - 1.0f;

	float3 bumpNormal = (bumpMapValue.x * input.tangent) +
		(bumpMapValue.y * input.binormal) +
		(bumpMapValue.z * input.normal);
	bumpNormal = normalize(bumpNormal);

	float4 lightColour = ambientColour;

	float4 specular = 0;

	float lightIntensity = saturate(dot(bumpNormal, -lightDirection));
	if (lightIntensity > 0.0f) {
		lightColour = saturate(lightColour + saturate(diffuseColour * lightIntensity));

		float3 reflection = normalize(2 * lightIntensity * bumpNormal - lightDirection);
		specular = specularColour * pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}

	return saturate(lightColour * textureColour + specular);
}
