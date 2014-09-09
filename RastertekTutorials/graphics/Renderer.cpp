#include "Renderer.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

void Renderer::initialise(const Device::Properties& properties) {
	device_.initialise(properties);
	worldVertexShader_.initialise(
		device_.d3dDevice(),
		shaders::VertexShader::compileShader(device_.d3dDevice(), "data\\shaders\\vertex-shaders\\lighting.vertex.hlsl", "main")
		);
	worldPixelShader_.initialise(
		device_.d3dDevice(),
		shaders::PixelShader::compileShader(device_.d3dDevice(), "data\\shaders\\pixel-shaders\\lighting.pixel.hlsl", "main")
		);
	hudVertexShader_.initialise(
		device_.d3dDevice(),
		shaders::VertexShader::compileShader(device_.d3dDevice(), "data\\shaders\\vertex-shaders\\sprite.vertex.hlsl", "main")
		);
	hudPixelShader_.initialise(
		device_.d3dDevice(),
		shaders::PixelShader::compileShader(device_.d3dDevice(), "data\\shaders\\pixel-shaders\\sprite.pixel.hlsl", "main")
		);
}

void Renderer::shutdown() {
	hudPixelShader_.reset();
	hudVertexShader_.reset();
	worldPixelShader_.reset();
	worldVertexShader_.reset();
	device_.shutdown();
}

void Renderer::renderFrame(const Camera& camera) {
	device_.beginScene();

	std::list<Model*>::iterator it, end = pipeline_.end();
	for (it = pipeline_.begin(); it != end; ++it) {
		shaders::LightingVertexShader::MatrixBuffer matrixBuffer;
		matrixBuffer.projection = camera.projectionMatrix();
		matrixBuffer.view = camera.viewMatrix();
		matrixBuffer.world = (*it)->worldMatrix();

		shaders::LightingVertexShader::CameraBuffer cameraBuffer;
		cameraBuffer.cameraPosition = camera.position();

		worldVertexShader_.bind(device_.d3dDeviceContext(), matrixBuffer, cameraBuffer);

		shaders::LightingPixelShader::LightBuffer lightBuffer;
		lightBuffer.ambientColour = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
		lightBuffer.diffuseColour = D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f);
		lightBuffer.lightDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		lightBuffer.specularColour = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		lightBuffer.specularPower = 32.0f;

		worldPixelShader_.bind(device_.d3dDeviceContext(), &(*it)->texture(), lightBuffer);

		(*it)->render(&device_, camera);
	}

	device_.endScene();
}
