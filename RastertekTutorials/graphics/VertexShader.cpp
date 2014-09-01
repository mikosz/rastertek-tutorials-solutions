#include "VertexShader.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

utils::COMWrapper<ID3D10Blob> VertexShader::compileShader(
	ID3D11Device* device,
	const boost::filesystem::path& path,
	const std::string& function
	) {
	utils::COMWrapper<ID3D10Blob> shaderBuffer;
	utils::COMWrapper<ID3D10Blob> errorMessageBuffer;

	HRESULT result = D3DX11CompileFromFile(
		path.string().c_str(),
		0,
		0,
		function.c_str(),
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		0,
		&shaderBuffer.get(),
		&errorMessageBuffer.get(),
		0
		);
	if (FAILED(result)) {
		if (errorMessageBuffer.get()) {
			std::string errorMessage(
				reinterpret_cast<const char*>(errorMessageBuffer->GetBufferPointer()),
				errorMessageBuffer->GetBufferSize()
				);
			throw std::runtime_error("Failed to compile shader file " + path.string() + ". Errors:\n" + errorMessage);
		} else {
			throw std::runtime_error("Failed to compile shader file " + path.string());
		}
	}

	return shaderBuffer;
}

void VertexShader::initialise(ID3D11Device* device, utils::COMWrapper<ID3D10Blob> shaderBuffer) {
	if (FAILED(device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), 0, &shader_.get()))) {
		throw std::runtime_error("Failed to create a vertex shader");
	}

	D3D11_INPUT_ELEMENT_DESC inputDesc[3];

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

void VertexShader::reset() {
	shader_.reset();
	inputLayout_.reset();
	matrixBuffer_.reset();
}

void VertexShader::bind(ID3D11DeviceContext* deviceContext, const MatrixBuffer& matrixBuffer) {
	MatrixBuffer transposed;
	D3DXMatrixTranspose(&transposed.world, &matrixBuffer.world);
	D3DXMatrixTranspose(&transposed.view, &matrixBuffer.view);
	D3DXMatrixTranspose(&transposed.projection, &matrixBuffer.projection);

	matrixBuffer_.write(deviceContext, &transposed, sizeof(transposed));
	matrixBuffer_.bind(deviceContext, ShaderConstantsBuffer::VERTEX, 0);

	deviceContext->IASetInputLayout(inputLayout_);
	deviceContext->VSSetShader(shader_, 0, 0);
}
