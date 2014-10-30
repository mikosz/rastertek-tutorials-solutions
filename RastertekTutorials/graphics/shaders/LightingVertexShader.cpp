#include "LightingVertexShader.hpp"

using namespace tutorials;
using namespace tutorials::graphics;
using namespace tutorials::graphics::shaders;

void LightingVertexShader::initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer) {
	VertexShader::initialise(device, shaderBuffer);

	D3D11_INPUT_ELEMENT_DESC inputDesc[5];

	inputDesc[0].SemanticName = "POSITION";
	inputDesc[0].SemanticIndex = 0;
	inputDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[0].InputSlot = 0;
	inputDesc[0].AlignedByteOffset = 0;
	inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[0].InstanceDataStepRate = 0;

	inputDesc[1].SemanticName = "TEXCOORD";
	inputDesc[1].SemanticIndex = 0;
	inputDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputDesc[1].InputSlot = 0;
	inputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[1].InstanceDataStepRate = 0;

	inputDesc[2].SemanticName = "NORMAL";
	inputDesc[2].SemanticIndex = 0;
	inputDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[2].InputSlot = 0;
	inputDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[2].InstanceDataStepRate = 0;

	inputDesc[3].SemanticName = "TANGENT";
	inputDesc[3].SemanticIndex = 0;
	inputDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[3].InputSlot = 0;
	inputDesc[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[3].InstanceDataStepRate = 0;

	inputDesc[4].SemanticName = "BINORMAL";
	inputDesc[4].SemanticIndex = 0;
	inputDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[4].InputSlot = 0;
	inputDesc[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[4].InstanceDataStepRate = 0;

	size_t inputDescSize = sizeof(inputDesc) / sizeof(inputDesc[0]);

	HRESULT result = device->CreateInputLayout(
		inputDesc,
		inputDescSize,
		shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(),
		&inputLayout_.get()
		);
	if (FAILED(result)) {
		throw std::runtime_error("Failed to create an input layout");
	}

	matrixBuffer_.initialise(device, sizeof(MatrixBuffer));
	cameraBuffer_.initialise(device, sizeof(CameraBuffer));
	fogBuffer_.initialise(device, sizeof(FogBuffer));
	reflectionBuffer_.initialise(device, sizeof(ReflectionBuffer));
}

void LightingVertexShader::reset() {
	inputLayout_.reset();
	matrixBuffer_.reset();
	cameraBuffer_.reset();
	fogBuffer_.reset();
	reflectionBuffer_.reset();

	VertexShader::reset();
}

void LightingVertexShader::bind(
	ID3D11DeviceContext* deviceContext,
	const MatrixBuffer& matrixBuffer,
	const CameraBuffer& cameraBuffer,
	const FogBuffer& fogBuffer,
	const ReflectionBuffer& reflectionBuffer
	) {
	MatrixBuffer transposed;
	D3DXMatrixTranspose(&transposed.world, &matrixBuffer.world);
	D3DXMatrixTranspose(&transposed.view, &matrixBuffer.view);
	D3DXMatrixTranspose(&transposed.projection, &matrixBuffer.projection);

	matrixBuffer_.write(deviceContext, &transposed, sizeof(transposed));
	matrixBuffer_.bind(deviceContext, ShaderConstantsBuffer::VERTEX, 0);

	cameraBuffer_.write(deviceContext, &cameraBuffer, sizeof(cameraBuffer));
	cameraBuffer_.bind(deviceContext, ShaderConstantsBuffer::VERTEX, 1);

	fogBuffer_.write(deviceContext, &fogBuffer, sizeof(fogBuffer));
	fogBuffer_.bind(deviceContext, ShaderConstantsBuffer::VERTEX, 2);

	ReflectionBuffer transposedReflection;
	D3DXMatrixTranspose(&transposedReflection.reflectionMatrix, &transposedReflection.reflectionMatrix);
	reflectionBuffer_.write(deviceContext, &transposedReflection, sizeof(transposedReflection));
	reflectionBuffer_.bind(deviceContext, ShaderConstantsBuffer::VERTEX, 3);

	deviceContext->IASetInputLayout(inputLayout_);

	VertexShader::bind(deviceContext);
}
