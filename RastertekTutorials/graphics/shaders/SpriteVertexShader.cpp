#include "SpriteVertexShader.hpp"

using namespace tutorials;
using namespace tutorials::graphics;
using namespace tutorials::graphics::shaders;

void SpriteVertexShader::initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer) {
	VertexShader::initialise(device, shaderBuffer);

	D3D11_INPUT_ELEMENT_DESC inputDesc[2];

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
}

void SpriteVertexShader::reset() {
	inputLayout_.reset();
	matrixBuffer_.reset();

	VertexShader::reset();
}

void SpriteVertexShader::bind(
	ID3D11DeviceContext* deviceContext,
	const MatrixBuffer& matrixBuffer
	) {
	MatrixBuffer transposed;
	D3DXMatrixTranspose(&transposed.world, &matrixBuffer.world);
	D3DXMatrixTranspose(&transposed.view, &matrixBuffer.view);
	D3DXMatrixTranspose(&transposed.projection, &matrixBuffer.projection);

	matrixBuffer_.write(deviceContext, &transposed, sizeof(transposed));
	matrixBuffer_.bind(deviceContext, ShaderConstantsBuffer::VERTEX, 0);

	deviceContext->IASetInputLayout(inputLayout_);

	VertexShader::bind(deviceContext);
}
