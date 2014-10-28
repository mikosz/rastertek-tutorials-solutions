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

	{
		A3DCamera::Properties properties;
		properties.aspectRatio = 800.0f / 600.0f;
		properties.nearPlane = 0.1f;
		properties.farPlane = 1000.0f;
		properties.fieldOfView = static_cast<float>(D3DXToRadian(40.0f));
		properties.lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		properties.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		properties.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		worldCamera_.initialise(properties);
	}

	hudVertexShader_.initialise(
		device_.d3dDevice(),
		shaders::VertexShader::compileShader(device_.d3dDevice(), "data\\shaders\\vertex-shaders\\sprite.vertex.hlsl", "main")
		);

	hudPixelShader_.initialise(
		device_.d3dDevice(),
		shaders::PixelShader::compileShader(device_.d3dDevice(), "data\\shaders\\pixel-shaders\\sprite.pixel.hlsl", "main")
		);

	{
		A2DCamera::Properties properties;
		properties.farPlane = 1.0f;
		properties.nearPlane = 0.0f;
		properties.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		properties.viewHeight = 600;
		properties.viewWidth = 800;

		hudCamera_.initialise(properties);
	}
}

void Renderer::shutdown() {
	hudPixelShader_.reset();
	hudVertexShader_.reset();
	worldPixelShader_.reset();
	worldVertexShader_.reset();
	device_.shutdown();
}

void Renderer::renderFrame() {
	device_.beginScene();

	{
		std::list<Model*>::iterator it, end = worldPipeline_.end();
		for (it = worldPipeline_.begin(); it != end; ++it) {
			shaders::LightingVertexShader::MatrixBuffer matrixBuffer;
			matrixBuffer.projection = worldCamera_.projectionMatrix();
			matrixBuffer.view = worldCamera_.viewMatrix();
			matrixBuffer.world = (*it)->worldMatrix();

			shaders::LightingVertexShader::CameraBuffer cameraBuffer;
			cameraBuffer.cameraPosition = worldCamera_.position();

			worldVertexShader_.bind(device_.d3dDeviceContext(), matrixBuffer, cameraBuffer);

			shaders::LightingPixelShader::LightBuffer lightBuffer;
			lightBuffer.ambientColour = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
			lightBuffer.diffuseColour = D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f);
			lightBuffer.lightDirection = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
			lightBuffer.specularColour = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
			lightBuffer.specularPower = 32.0f;
			lightBuffer.gamma = 2.0f;

			worldPixelShader_.bind(
				device_.d3dDeviceContext(),
				&(*it)->baseTexture(),
				&(*it)->detailTexture(),
				&(*it)->bumpMap(),
				&(*it)->specularHighlightsMap(),
				lightBuffer,
				2.0f
				);

			(*it)->render(&device_, worldCamera_);
		}
	}

	{
		/* std::list<Model*>::iterator it, end = hudPipeline_.end();
		for (it = hudPipeline_.begin(); it != end; ++it) {
			shaders::LightingVertexShader::MatrixBuffer matrixBuffer;
			matrixBuffer.projection = worldCamera_.projectionMatrix();
			matrixBuffer.view = worldCamera_.viewMatrix();
			matrixBuffer.world = (*it)->worldMatrix();

			shaders::LightingVertexShader::CameraBuffer cameraBuffer;
			cameraBuffer.cameraPosition = worldCamera_.position();

			worldVertexShader_.bind(device_.d3dDeviceContext(), matrixBuffer, cameraBuffer);

			shaders::LightingPixelShader::LightBuffer lightBuffer;
			lightBuffer.ambientColour = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
			lightBuffer.diffuseColour = D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f);
			lightBuffer.lightDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			lightBuffer.specularColour = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			lightBuffer.specularPower = 32.0f;
			lightBuffer.gamma = 2.0f;

			worldPixelShader_.bind(device_.d3dDeviceContext(), &(*it)->texture(), lightBuffer);

			(*it)->render(&device_, worldCamera_);
		} */
	}

	device_.endScene();
}
