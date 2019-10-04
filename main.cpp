#include <stdio.h>

#include "Graphics.h"
#include "Matrix4.h"
#include "CubeMesh.h"

const Vector2u screenSize(800, 600);
const Vector4f viewport(0.0f, 0.0f, (float)screenSize.x, (float)screenSize.y);

struct Camera {
	vec3 position;
	vec3 rotation;
	vec3 scale;

	vec3 target;
	vec3 up;

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
		scale    = vec3( 1.0f, 1.0f, 1.0f);
		target   = vec3( 0.0f, 0.0f,-1.0f);
		up       = vec3( 0.0f, 1.0f, 0.0f);

		fieldOfView = 75.0f;
		aspectRatio = (float)screenSize.x / (float)screenSize.y;
		zNear = 10.0f;
		zFar = 300.0f;

		viewDirty = true;
		projectionDirty = true;
	}

	void update() {
		if (viewDirty == true) {
			view.setCameraLookAtTransformation(position, target, up);
			viewDirty = false;
		}

		if (projectionDirty == true) {
			projection.setPerspective(fieldOfView, aspectRatio, zNear, zFar);
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

		model.setIdentity();

		position = vec3(  0.0f,  0.0f,-50.0f);
		rotation = vec3(  0.0f,  0.0f,  0.0f);
		scale    = vec3(  1.0f,  1.0f,  1.0f);

		alphaBlend = false;
	}

	void draw(Graphics* graphics) {
		mat4 translationMatrix;
		mat4 rotationXMatrix, rotationYMatrix, rotationZMatrix;
		mat4 scaleMatrix;
		mat4 modelViewProjection;

		translationMatrix.setTranslation(vec4(position.x, position.y, position.z, 1.0f));
		rotationXMatrix.setRotationX(deg2rad(rotation.x));
		rotationYMatrix.setRotationY(deg2rad(rotation.y));
		rotationZMatrix.setRotationZ(deg2rad(rotation.z));
		scaleMatrix.setScale(scale);

		model = translationMatrix * rotationZMatrix * rotationYMatrix * rotationXMatrix * scaleMatrix;
		modelViewProjection = camera.projection * camera.view * model;

		graphics->setActiveTexture(texture);

		graphics->setVertexShaderCallback(VertexShader);
		graphics->setPixelShaderCallback(PixelShader);

		graphics->setDepthTest(true);
		graphics->setDepthMask(true);
		graphics->setAlphaBlend(alphaBlend);

		graphics->setShaderUniform(&modelViewProjection);

		graphics->drawPrimitive(Graphics::EPT_TRIANGLES, CubeVertices, CubeVerticesCount);
	}

	static void VertexShader(VertexShaderData& vertex, void* uniform) {
		if (uniform == NULL) {
			return;
		}

		mat4 modelViewProjection = *reinterpret_cast<mat4*>(uniform);

		vertex.position = modelViewProjection * vertex.position;

		vertex.position /= vertex.position.w;

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
		printf("Failed to initialize frame buffer.\nError: %s\n", 
			FrameBuffer::ErrorText[status]);
		return 1;
	}
	
	if (graphics.initialize(&frameBuffer, 32) == false) {
		printf("Failed to initialize graphics!\n");
		return 2;
	}

	if (image.load("tex_test.tga") == false) {
		printf("Failed to load image.\n");
		return 3;
	}

	mesh.texture = &image;

	while (true) 
	{
		graphics.clear();

		camera.update();

		// Draw background
		graphics.setActiveTexture(&image);
		graphics.drawRectangle(viewport, vec4(1.0f, 1.0f, 1.0f, 1.0f));

		mesh.rotation += vec3(0.33f, 0.66f, 0.99f);

		mesh.draw(&graphics);

		graphics.swap();
	}

	return 0;
}
