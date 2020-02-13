#include <stdio.h>
#include <vector>

#include "FBRenderer.h"
#include "CubeMesh.h"

const Vector2u screenSize(800, 600);
const Vector4f viewport(0.0f, 0.0f, (float)screenSize.x, (float)screenSize.y);
vec4 AmbientLight(0.2f, 0.2f, 0.2, 1.0f);

struct Light {
	enum Type {
		ELT_POINT,
		ELT_SPOT,
		ELT_DIRECTIONAL
	};

	Type type;
	vec3 position;
	vec3 direction;
	vec4 diffuse;

	Light(Light::Type ntype, const vec4& color)
		: type(ntype), diffuse(color) {
	}
};
std::vector<Light> lightList;

struct ShaderUniform {
	mat4 modelViewMatrix;
	mat4 modelViewProjectionMatrix;
	mat4 normalMatrix;
	Light* lights;
	unsigned int lightCount;
	vec4 ambientLight;
};

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

		fieldOfView = 60.0f;
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

template<typename T>
T max (T a, T b) {
	return ((a < b) ? b : a);
}
template<typename T>
T min (T a, T b) {
	return ((a < b) ? a : b);
}

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

		renderer->setVertexShaderCallback(LightVertexShader);
		renderer->setPixelShaderCallback(LightPixelShader);

		model.setTransformation(position, rotation, scale);

		ShaderUniform uniform;

		uniform.modelViewProjectionMatrix = camera.viewProjection * model;
		uniform.modelViewMatrix = camera.view * model;
		uniform.normalMatrix = model;
		uniform.normalMatrix.transpose();
		uniform.normalMatrix.invert();

		uniform.lights = &lightList[0];
		uniform.lightCount = lightList.size();
		uniform.ambientLight = AmbientLight;

		renderer->setShaderUniform(&uniform);

		renderer->render(Renderer::EPT_TRIANGLES, CubeVertices, CubeVerticesCount);
	}

	void drawNormals(Renderer* renderer) {
		renderer->setActiveTexture(0, NULL);

		renderer->setFlag(Renderer::ERF_DEPTH_TEST, true);
		renderer->setFlag(Renderer::ERF_DEPTH_MASK, true);
		renderer->setFlag(Renderer::ERF_ALPHA_BLEND, false);

		renderer->setVertexShaderCallback(VertexShader);
		renderer->setPixelShaderCallback(PixelShader);

		model.setTransformation(position, rotation, scale);

		ShaderUniform uniform;
		uniform.modelViewProjectionMatrix = camera.viewProjection * model;

		renderer->setShaderUniform(&uniform);

		for (unsigned int index = 0; index < CubeVerticesCount; ++index) {
			Vertex vertices[] = {
				Vertex(CubeVertices[index].position, vec3(), vec2(), CubeVertices[index].color),
				Vertex(CubeVertices[index].position + CubeVertices[index].normal, vec3(), vec2(), CubeVertices[index].color)
			};
			renderer->render(Renderer::EPT_LINES, vertices, 2);
		}
	}

	static void VertexShader(VertexShaderData& vertex) {
		if (vertex.uniform == NULL) {
			return;
		}

		const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(vertex.uniform);

		vertex.position = uniform->modelViewProjectionMatrix * vertex.position;
	}

	static void PixelShader(PixelShaderData& pixel) {
		const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(pixel.uniform);

		if (pixel.texture[0] != NULL) {
			pixel.color *= pixel.texture[0]->getPixelByUV(pixel.uv);
		}
	}

	static void LightVertexShader(VertexShaderData& vertex) {
		if (vertex.uniform == NULL) {
			return;
		}

		const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(vertex.uniform);

		vertex.position = uniform->modelViewProjectionMatrix * vertex.position;
		vertex.normal = uniform->normalMatrix * vertex.normal;
		vertex.normal.normalize();
	}

	static void LightPixelShader(PixelShaderData& pixel) {
		const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(pixel.uniform);

		vec4 ambient;
		vec4 diffuse;

		pixel.color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

		if (pixel.texture[0] != NULL) {
//			pixel.color *= pixel.texture[0]->getPixelByUV(pixel.uv);
//			pixel.color = pixel.texture[0]->getPixelByUV(pixel.uv);
		}

		if (pixel.uniform != NULL) {
			ambient += uniform->ambientLight;

			float diffuseProc = 0.0f;
			for (unsigned int index = 0; index < uniform->lightCount; ++index) {
				switch (uniform->lights[0].type) {
				case Light::ELT_DIRECTIONAL :
					diffuseProc = max(pixel.normal.dot(uniform->lights[0].direction), 0.0f);
					diffuse = uniform->lights[0].diffuse * diffuseProc;
					break;
				}
			}
		}
		pixel.color *= (ambient + diffuse);
		pixel.color.clamp(0.0f, 1.0f);
		pixel.color.w = 1.0f;
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

	Light directionalLight(Light::ELT_DIRECTIONAL, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	directionalLight.direction = vec3(0.0f, 1.0f, 0.0f);
	directionalLight.direction.normalize();
	lightList.push_back(directionalLight);

	mesh.texture = &image;

	mesh.scale.x = 10.0f;
	mesh.scale.y = 0.1f;
	mesh.scale.z = 10.0f;

	camera.position.z += 30.0f;
	camera.position.y += 50.0f;
	camera.viewDirty = true;

	while (true) {
		renderer.clear(true, true);

		camera.update();

		{
		vec3 position = mesh.position;
		vec3 rotation = mesh.rotation;
		vec3 scale = mesh.scale;

		mesh.rotation = vec3( 0.0f, 0.0f, 0.0f);
		mesh.rotation = vec3( 0.0f, 0.0f, 0.0f);
		mesh.scale = vec3(10.0f, 0.1f,10.0f);
		mesh.draw(&renderer);

		mesh.position = position;
		mesh.rotation = rotation;
		mesh.scale = scale;
		}

		{
		mesh.scale = vec3(2.0f, 2.0f, 2.0f);
		mesh.rotation += vec3(0.33f, 0.66f, 0.99f);
		mesh.draw(&renderer);
		mesh.drawNormals(&renderer);
		}

		renderer.swap();
	}

	return 0;
}
