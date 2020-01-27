#include <stdio.h>

#include "Renderer.h"
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
	mat4 viewProjection;

	bool viewDirty;
	bool projectionDirty;
	bool viewProjectionDirty;

	Camera() {
		position = vec3( 0.0f, 0.0f, 0.0f);
		rotation = vec3( 0.0f, 0.0f, 0.0f);
		scale    = vec3( 1.0f, 1.0f, 1.0f);
		target   = vec3( 0.0f, 0.0f,-50.0f);
		up       = vec3( 0.0f, 1.0f, 0.0f);

		fieldOfView = 75.0f;
		zNear = 0.1f;
		zFar = 300.0f;

		aspectRatio = (float)screenSize.x / (float)screenSize.y;

		viewDirty = true;
		projectionDirty = true;
		viewProjectionDirty = false;
	}

	void update() {
		if (viewDirty == true) {
			view.setCameraLookAtTransformation(position, target, up);
			viewDirty = false;
			viewProjectionDirty = true;
		}

		if (projectionDirty == true) {
			projection.setPerspective(fieldOfView, aspectRatio, zNear, zFar);
			projectionDirty = false;
			viewProjectionDirty = true;
		}

		if (viewProjectionDirty == true) {
			viewProjection = projection * view;
			viewProjectionDirty = false;
		}
	}
} camera;

struct Mesh {
	Image* texture;
	mat4 model;

	vec3 position;
	vec3 rotation;
	vec3 scale;

	bool alphaBlend;

	Mesh() {
		texture = NULL;

		model.setIdentity();

		position = vec3(  0.0f,  0.0f,-50.0f);
		rotation = vec3(  0.0f,  0.0f,  0.0f);
		scale    = vec3(  1.0f,  1.0f,  1.0f);

		alphaBlend = false;
	}

	void draw(Renderer* renderer) {
		renderer->setActiveTexture(0, texture);

		renderer->setFlag(Renderer::ERF_DEPTH_TEST, true);
		renderer->setFlag(Renderer::ERF_DEPTH_MASK, true);
		renderer->setFlag(Renderer::ERF_ALPHA_BLEND, alphaBlend);

		renderer->setVertexShaderCallback(VertexShader);
		renderer->setPixelShaderCallback(PixelShader);

		model.setTransformation(position, rotation, scale);

		const mat4 modelViewProjection = camera.viewProjection * model;

		renderer->setShaderUniform(&modelViewProjection);

		renderer->render(Renderer::EPT_TRIANGLES, CubeVertices, CubeVerticesCount);
	}

	static void VertexShader(VertexShaderData& vertex) {
		if (vertex.uniform == NULL) {
			return;
		}

		const mat4 modelViewProjection = *reinterpret_cast<const mat4*>(vertex.uniform);

		vertex.position = modelViewProjection * vertex.position;
	}

	static void PixelShader(PixelShaderData& pixel) {
		if (pixel.texture[0] == NULL) {
			return;
		}

		pixel.color *= pixel.texture[0]->getPixelByUV(pixel.uv);
	}
};

class FBRenderer : public Renderer {
	FrameBuffer frameBuffer;
	RenderTarget renderTarget;
	Image colorBuffer;
	Image depthBuffer;

public:
	int initialize(const char* fb_path, const Vector2u& size, const Image::PixelFormat& pixelFormat) {
		int status = 0;
		const vec4 viewport(0.0f, 0.0f, (float)size.x, (float)size.y);

		status = frameBuffer.initialize(fb_path, size, pixelFormat);
		if (status != FrameBuffer::ERR_NO_ERROR) {
			printf("Failed to initialize frame buffer.\nError: %s\n", 
				FrameBuffer::ErrorText[status]);
			return 1;
		}

		colorBuffer.create(size, frameBuffer.getPixelFormat());
		depthBuffer.create(size, Image::EPF_DEPTH);

		renderTarget.setBuffer(RenderTarget::ERT_COLOR_0, &colorBuffer);
		renderTarget.setBuffer(RenderTarget::ERT_DEPTH, &depthBuffer);

		setRenderTarget(&renderTarget);
		setViewport(viewport);

		return status;
	}

	void clear(bool color, bool depth) {
		if (color) {
			colorBuffer.clearColor();
		}
		if (depth) {
			depthBuffer.clearColor();
		}
	}

	void swap() {
		frameBuffer.draw(&colorBuffer);
	}
};

int main() {
	FBRenderer renderer;
	Image image;
	Mesh mesh;
	int status;

	status = renderer.initialize(NULL, screenSize, Image::EPF_R8G8B8A8);
	if (status != 0) {
		printf("Failed to initialize frame buffer renderer.\n");
		return status;
	}

	if (image.load("tex_test.tga") == false) {
		printf("Failed to load image.\n");
		return 3;
	}

	mesh.texture = &image;

	while (true) {
		renderer.clear(false, true);

		renderer.draw2DImage(&image, viewport);

		camera.update();

		mesh.rotation += vec3(0.33f, 0.66f, 0.99f);

		mesh.draw(&renderer);

		renderer.swap();
	}

	return 0;
}
