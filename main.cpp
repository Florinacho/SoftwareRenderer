#include <stdio.h>

#include "Graphics.h"
#include "Matrix4.h"
#include "CubeMesh.h"
#include "Timer.h"

const Vector2u screenSize(800, 600);
static Matrix4f ModelViewProjection;

struct Mesh3D {
	Image* texture;

	Matrix4f model;
	Matrix4f view;
	Matrix4f perspective;

	vec3 position;
	vec3 rotation;
	vec3 scale;

	bool alphaBlend;

	Mesh3D() {
		texture = NULL;
		scale = vec3(1.0f, 1.0f, 1.0f);
		alphaBlend = false;

		model.setIdentity();
		view.setIdentity();
		perspective.setIdentity();

		// setTransformation(cameraPosition, cameraRotation, cameraScale)
		view.setTransformation(vec3( 0.0f, 0.0f, -100.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));
		view.invert();

		// setPerspective(fovyDegrees, aspectRatio, zNeat, zFar); used for 3d scenes
		static float znear = 200.0f;
		static float zfar = 0.1f;
		perspective.setPerspective(60.0f, (float)screenSize.x / (float)screenSize.y, znear, zfar);
	}

	void draw(Graphics* graphics) {
		model.setTransformation(position, rotation, scale);
		ModelViewProjection = perspective * view * model;

		graphics->setActiveTexture(texture);

		graphics->setVertexShaderCallback(VertexShader);
		graphics->setPixelShaderCallback(PixelShader);

		graphics->setDepthTest(true);
		graphics->setDepthMask(true);
		graphics->setAlphaBlend(alphaBlend);

		graphics->drawPrimitive(Graphics::EPT_TRIANGLES, CubeVertices, CubeVerticesCount);
	}

	static void VertexShader(VertexShaderData& data) {
		data.position = ModelViewProjection * data.position;
	}

	static void PixelShader(PixelShaderData& data) {
		data.color = data.color * data.texture->getPixelByUV(data.uv);
	}
};

int main() {
	FrameBuffer frameBuffer;
	if (frameBuffer.initialize(screenSize.x, screenSize.y, 24) == false) {
		printf("Failed to initialize frame buffer.\n");
		return 1;
	}
	
	Graphics graphics;
	if (graphics.initialize(&frameBuffer) == false) {
		printf("Failed to initialize graphics!\n");
		return 2;
	}

	Image image;
	if (image.load("tex_test.tga") == false) {
		printf("Cannot load image.\n");
		return 3;
	}

	Mesh3D mesh;
	mesh.position = vec3(screenSize.x / 2, screenSize.y / 2, 0.0f);
	mesh.texture = &image;

	unsigned int fps = 0;
	unsigned long long lastPrintTime = os::Timer::GetMilliSeconds();

	while (true) {
		const unsigned long long currentTime = os::Timer::GetMilliSeconds();
		if (currentTime - lastPrintTime >= 1000) {
			printf("FPS: %d\n", fps);
			fps = 0;
			lastPrintTime = currentTime;
		}
		graphics.clear();

		// Draw background
		graphics.setActiveTexture(&image);
		graphics.drawRectangle(vec4(0.0f, 0.0f, (float)screenSize.x, (float)screenSize.y), vec4(1.0f, 1.0f, 1.0f, 1.0f));

		{// Draw static solid cube
		const vec3 rotation = mesh.rotation;
		mesh.rotation = vec3();
		mesh.alphaBlend = false;
		mesh.draw(&graphics);
		mesh.rotation = rotation;
		}

		// Draw transparent rotating cube
		mesh.alphaBlend = true;
		mesh.rotation = mesh.rotation + vec3(0.33f, 0.66f, 1.0f);
		mesh.draw(&graphics);

		// Draw depth buffer
		graphics.setActiveTexture(graphics.getDepthBuffer());
		graphics.drawRectangle(vec4(0.0f, 0.0f, 300.0f, 300.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// Draw X lines
		graphics.drawLine(vec2(0.0f, 0.0f), vec2((float)screenSize.x, (float)screenSize.y), vec4(1.0f, 0.0f, 0.0f, 1.0f));
		graphics.drawLine(vec2((float)screenSize.x, 0.0f), vec2(0.0f, (float)screenSize.y), vec4(0.0f, 1.0f, 0.0f, 1.0f));

		graphics.swap();
		++fps;
	}

	return 0;
}
