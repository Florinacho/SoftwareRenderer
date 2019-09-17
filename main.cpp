#include <stdio.h>

#include "Graphics.h"
#include "Matrix4.h"
#include "CubeMesh.h"

//#define DEBUG_PRINT_FPS

#if defined (DEBUG_PRINT_FPS)
#include "Timer.h"
#endif //DEBUG_PRINT_FPS

#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

const Vector2u screenSize(800, 600);

struct Camera {
	vec3 position;
	vec3 rotation;
	vec3 scale;

	float fieldOfView;
	float aspectRatio;
	float zNear;
	float zFar;

	mat4 view;
	mat4 projection;

	bool viewDirty;
	bool projectionDirty;

	Camera() {
		position = vec3( 0.0f, 0.0f, 0.0f);
		rotation = vec3( 0.0f, 0.0f, 0.0f);
		scale = vec3(1.0f, 1.0f, 1.0f);

		fieldOfView = 60.0f;
		aspectRatio = (float)screenSize.y / (float)screenSize.x;
		zNear = 0.1f;
		zFar = 300.0f;

		view.setIdentity();
		projection.setIdentity();

		viewDirty = true;
		projectionDirty = true;
	}

	void update() {
		if (viewDirty == true) {
//			view.setTransformation(position, rotation, scale);
//			view.invert();
			viewDirty = false;
		}

		if (projectionDirty == true) {
//			projection.setPerspective(fieldOfView, aspectRatio, zNear, zFar);
			projection.setPerspective2(fieldOfView, aspectRatio, zNear, zFar);
			projectionDirty = false;
		}
	}
} camera;

struct Mesh3D {
	Image* texture;

	mat4 model;

	vec3 position;
	vec3 rotation;
	vec3 scale;

	bool alphaBlend;

	Mesh3D() {
		texture = NULL;
		position = vec3( 0.0f, 0.0f, 0.0f);
		rotation = vec3( 0.0f, 0.0f, 0.0f);
		scale = vec3(1.0f, 1.0f, 1.0f);

		alphaBlend = false;

		model.setIdentity();
	}

	void draw(Graphics* graphics) {
		model.setTransformation(position, rotation, scale);

		mat4 ModelViewProjection = camera.projection * camera.view * model;

		graphics->setActiveTexture(NULL);

		graphics->setVertexShaderCallback(VertexShader);
		graphics->setPixelShaderCallback(PixelShader);

		graphics->setDepthTest(true);
		graphics->setDepthMask(true);
		graphics->setAlphaBlend(alphaBlend);

		graphics->setShaderUniform(&ModelViewProjection);

		graphics->drawPrimitive(Graphics::EPT_TRIANGLES, CubeVertices, CubeVerticesCount);
	}

	static void VertexShader(VertexShaderData& vertex, void* uniform) {
		if (uniform == NULL) {
			return;
		}
		mat4 ModelViewProjection = *reinterpret_cast<mat4*>(uniform);
		vertex.position = ModelViewProjection * vertex.position;

		mat4 viewport;
		viewport.setViewport(0.0f, 0.0f, (float)screenSize.x, (float)screenSize.y);
		vertex.position = viewport * vertex.position;
	}

	static void PixelShader(PixelShaderData& pixel, void* uniform) {
		if (pixel.texture) {
			pixel.color *= pixel.texture->getPixelByUV(pixel.uv);
		}
	}
};

int main() {
	FrameBuffer frameBuffer;
	Graphics graphics;
	Mesh3D mesh;
	Image image;
	int status;

	status = frameBuffer.initialize(screenSize.x, screenSize.y, 32);
	if (status != FrameBuffer::ERR_NO_ERROR) {
		printf("Failed to initialize frame buffer.\nError: %s\n", FrameBuffer::ErrorText[status]);
		return 1;
	}
	
	if (graphics.initialize(&frameBuffer) == false) {
		printf("Failed to initialize graphics!\n");
		return 2;
	}

	if (image.load("tex_test.tga") == false) {
		printf("Failed to load image.\n");
		return 3;
	}

	mesh.position.x = 0.0f;
	mesh.position.y = 0.0f;
	mesh.position.z = 50.0f;

	mesh.alphaBlend = false;

//	camera.position = vec3(0.0f, 100.0f, 0.0f);
//	camera.rotation = vec3(-45.0f, 0.0f, 0.0f);
	camera.viewDirty = true;

	while (true) {
		graphics.clear();

		camera.update();

		// Draw background
		graphics.setActiveTexture(&image);
		graphics.drawRectangle(vec4(0.0f, 0.0f, (float)screenSize.x, (float)screenSize.y), vec4(1.0f, 1.0f, 1.0f, 1.0f));

//		camera.position.z += 1.1f;
		camera.viewDirty = true;

		mesh.position.y += 0.5f;
//		mesh.position.z += 1.0f;if (mesh.position.z  > 200.0f) mesh.position.z = 0.0f;
//		mesh.rotation.y += 0.5f;
		mesh.rotation += vec3(0.33f, 0.66f, 0.99f);
//		mesh.scale.y += 0.1f;

		mesh.draw(&graphics);

		graphics.setActiveTexture(graphics.getDepthBuffer());
		graphics.drawRectangle(vec4(0.0f, 0.0f, 200.0f, 150.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));

		graphics.swap();
	}

	return 0;
}
