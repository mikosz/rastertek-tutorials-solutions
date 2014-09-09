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

struct VertexInputType
{
    float4 position : POSITION;
    float2 textureCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 textureCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection: TEXCOORD1;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	float4 worldPosition;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
	worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);

    // Store the input color for the pixel shader to use.
    output.textureCoord = input.textureCoord;
    
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

    return output;
}
