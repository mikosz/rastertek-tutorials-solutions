cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

cbuffer FogBuffer
{
	float fogStart;
	float fogEnd;

	float _[2]; // padding
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 textureCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 textureCoord : TEXCOORD0;
	float3 viewDirection: TEXCOORD1;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float fogFactor: FOG;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	float4 worldPosition, cameraPosition;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
	worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
	cameraPosition = output.position;
    output.position = mul(output.position, projectionMatrix);
    
	output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);

    // Store the input color for the pixel shader to use.
    output.textureCoord = input.textureCoord;
    
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.tangent = mul(input.binormal, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	output.fogFactor = saturate((fogEnd - cameraPosition.z) / (fogEnd - fogStart));

	return output;
}
