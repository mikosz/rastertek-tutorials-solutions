#include "Renderer.hpp"

using namespace tutorials;
using namespace tutorials::graphics;

void Renderer::initialise(const Device::Properties& properties) {
	device_.initialise(properties);
	vertexShader_.initialise(
		device_.d3dDevice(),
		VertexShader::compileShader(device_.d3dDevice(), "data\\shaders\\vertex-shaders\\basic.vertex.hlsl", "main")
		);
	pixelShader_.initialise(
		device_.d3dDevice(),
		PixelShader::compileShader(device_.d3dDevice(), "data\\shaders\\pixel-shaders\\basic.pixel.hlsl", "main")
		);
}

void Renderer::shutdown() {
	device_.shutdown();
}

void Renderer::renderFrame(const Camera& camera) {
	device_.beginScene();

	std::list<Model*>::iterator it, end = pipeline_.end();
	for (it = pipeline_.begin(); it != end; ++it) {
		VertexShader::MatrixBuffer matrixBuffer;

		matrixBuffer.projection = camera.projectionMatrix();
		matrixBuffer.view = camera.viewMatrix();
		matrixBuffer.world = (*it)->worldMatrix();

		vertexShader_.bind(device_.d3dDeviceContext(), matrixBuffer);

		PixelShader::LightBuffer lightBuffer;
		lightBuffer.diffuseColour = D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f);
		lightBuffer.lightDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		pixelShader_.bind(device_.d3dDeviceContext(), &(*it)->texture(), lightBuffer);

		(*it)->render(&device_, camera);
	}

	device_.endScene();
}
