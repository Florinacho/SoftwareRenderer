#include <stdio.h>
#include <vector>

#include "Shader.h"
#include "FBRenderer.h"
#include "Light.h"
#include "Camera.h"
#include "CubeMesh.h"

const vec2u screenSize(800, 600);
const vec4 viewport(0.0f, 0.0f, (float)screenSize.x, (float)screenSize.y);
const vec4 AmbientLight(0.2f, 0.2f, 0.2, 1.0f);
std::vector<Light> lightList;
Camera camera(60.0f, (float)screenSize.x / (float)screenSize.y);

#include <string.h>

/*****************************************************************************/
/* Shaders                                                                   */
/*****************************************************************************/
template<typename T>T max(T a, T b) {return ((a < b) ? b : a);}
template<typename T>T min(T a, T b) {return ((a < b) ? a : b);}

struct ShaderUniform {
	mat4 modelViewMatrix;
	mat4 modelViewProjectionMatrix;
	mat4 normalMatrix;
	Light* lights;
	unsigned int lightCount;
	vec4 ambientLight;
};

// Default shaders
void VertexShader(VertexShaderData& vertex) {
	if (vertex.uniform == NULL) {
		return;
	}

	const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(vertex.uniform);

	vertex.position = uniform->modelViewProjectionMatrix * vertex.position;
}

// Light shaders
void PixelShader(PixelShaderData& pixel) {
	const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(pixel.uniform);

	if (pixel.texture[0] != NULL) {
		pixel.color *= pixel.texture[0]->getPixelByUV(pixel.uv);
	}
}

void LightVertexShader(VertexShaderData& vertex) {
	if (vertex.uniform == NULL) {
		return;
	}

	const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(vertex.uniform);

	vertex.position = uniform->modelViewProjectionMatrix * vertex.position;
#if 1
	vertex.normal = uniform->normalMatrix * vertex.normal;
	vertex.normal.normalize();
#else
	Matrix3f nmat3 = uniform->normalMatrix.getMatrix3();
	vec3 nvec3 = nmat3 * vertex.normal.xyz();
	vertex.normal = vec4(nvec3, 0.0f);
#endif

#define POINT_LIGHT
#if defined (POINT_LIGHT)
	const vec4 pos = uniform->modelViewMatrix * vertex.position;
	vertex.lightDirection = uniform->lights[1].position - pos.getXYZ();
	vertex.eye = -pos.getXYZ();
/**
	for (unsigned int index = 0; index < uniform->lightCount; ++index) {
		switch (uniform->lights[index].type) {
		case Light::ELT_POINT :
			uniform->lights[index].direction = uniform->lights[index].position - vertex.position;
			break;
		}
	}
/**/
#endif
}

void LightPixelShader(PixelShaderData& pixel) {
	const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(pixel.uniform);

	vec4 ambient;
	vec4 diffuse;

	if (pixel.texture[0] != NULL) {
			pixel.color *= pixel.texture[0]->getPixelByUV(pixel.uv);
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
			case Light::ELT_POINT :
				{
	//			vec3
//				float intensity = 
				}
				break;
			}
		}
	}
	pixel.color *= (ambient + diffuse);
	pixel.color.clamp(0.0f, 1.0f);
	pixel.color.w = 1.0f;
}

int k = 0;
struct LightShader : public Shader {
	int testLocation;

	LightShader() {
		testLocation = addUniform("testVarying", ShaderVariable::EDT_VEC4);
	}

	void VertexShader(VertexShaderData& vertex) {
		if (vertex.uniform == NULL) {
			return;
		}

		const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(vertex.uniform);

		vertex.position = uniform->modelViewProjectionMatrix * vertex.position;
#if 1
		vertex.normal = uniform->normalMatrix * vertex.normal;
		vertex.normal.normalize();
#else
		Matrix3f nmat3 = uniform->normalMatrix.getMatrix3();
		vec3 nvec3 = nmat3 * vertex.normal.xyz();
		vertex.normal = vec4(nvec3, 0.0f);
#endif

		switch (k % 3) {
		case 0 :
			this->uniform[testLocation] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case 1 :
			this->uniform[testLocation] = vec4(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case 2 :
			this->uniform[testLocation] = vec4(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		default :
			this->uniform[testLocation] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
		++k;
	}

	void FragmentShader(PixelShaderData& pixel) {
		const ShaderUniform* uniform = reinterpret_cast<const ShaderUniform*>(pixel.uniform);

		vec4 ambient;
		vec4 diffuse;

		if (pixel.texture[0] != NULL) {
				pixel.color *= pixel.texture[0]->getPixelByUV(pixel.uv);
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

		pixel.color = this->uniform[testLocation].get<vec4>();
	}
} lightShader;

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

		position = vec3( 0.0f, 0.0f, 0.0f);
		rotation = vec3( 0.0f, 0.0f, 0.0f);
		scale    = vec3( 1.0f, 1.0f, 1.0f);

		alphaBlend = false;
	}

	void draw(Renderer* renderer, bool drawNormals = false) {
		renderer->setActiveTexture(0, texture);

		renderer->setFlag(Renderer::ERF_DEPTH_TEST, true);
		renderer->setFlag(Renderer::ERF_DEPTH_MASK, true);
		renderer->setFlag(Renderer::ERF_ALPHA_BLEND, alphaBlend);

		renderer->setVertexShaderCallback(LightVertexShader);
		renderer->setPixelShaderCallback(LightPixelShader);
		renderer->setShader(&lightShader);

		model.setTransformation(position, rotation, scale);

		ShaderUniform uniform;

		uniform.modelViewProjectionMatrix = camera.viewProjection * model;
		uniform.modelViewMatrix = camera.view * model;
		uniform.normalMatrix = model;

		uniform.lights = &lightList[0];
		uniform.lightCount = lightList.size();
		uniform.ambientLight = AmbientLight;

		renderer->setShaderUniform(&uniform);

		renderer->render(Renderer::EPT_TRIANGLES, CubeVertices, CubeVerticesCount);

		if (drawNormals) {
			renderer->setActiveTexture(0, NULL);

			renderer->setFlag(Renderer::ERF_DEPTH_TEST, true);
			renderer->setFlag(Renderer::ERF_DEPTH_MASK, true);
			renderer->setFlag(Renderer::ERF_ALPHA_BLEND, false);

			renderer->setVertexShaderCallback(VertexShader);
			renderer->setPixelShaderCallback(PixelShader);

			for (unsigned int index = 0; index < CubeVerticesCount; ++index) {
				const Vertex vertices[] = {
					Vertex(CubeVertices[index].position, vec3(), vec2(), CubeVertices[index].color),
					Vertex(CubeVertices[index].position + CubeVertices[index].normal, vec3(), vec2(), CubeVertices[index].color)
				};
				renderer->render(Renderer::EPT_LINES, vertices, 2);
			}
		}
	}
};

struct Billboard : public Mesh {
	void setTarget(vec3 target) {
		vec3 dis = target - position;
		const float xzdis = sqrt(dis.x * dis.x + dis.z * dis.z);
		rotation.x = rad2deg(-atan2f(dis.y, xzdis));
		rotation.y = rad2deg(-(atan2f(-dis.x, dis.z)));
		rotation.z = 0.0f;
	}

	void draw(Renderer* renderer) {
		renderer->setActiveTexture(0, texture);

		renderer->setFlag(Renderer::ERF_DEPTH_TEST, true);
		renderer->setFlag(Renderer::ERF_DEPTH_MASK, true);
		renderer->setFlag(Renderer::ERF_ALPHA_BLEND, alphaBlend);

		renderer->setVertexShaderCallback(VertexShader);
		renderer->setPixelShaderCallback(PixelShader);
		renderer->setShader(NULL);

		model.setTransformation(position, rotation, scale);

		ShaderUniform uniform;
		uniform.modelViewProjectionMatrix = camera.viewProjection * model;
		renderer->setShaderUniform(&uniform);

		static const float half = 10.5f;
		static const Vertex vertices[] = {
			Vertex(vec3( half, half, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
			Vertex(vec3(-half,-half, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
			Vertex(vec3(-half, half, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),

			Vertex(vec3(-half,-half, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
			Vertex(vec3( half, half, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
			Vertex(vec3( half,-half, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
		};

		renderer->render(Renderer::EPT_TRIANGLES, vertices, sizeof(vertices) / sizeof(vertices[0]));
	}
};

int main() {
#if 0
	LightShader shader;

	vec2 inUV(4.0f, 8.0);
	vec2 outUV(-1.0f, -2.0f);
	vec3 inNormal(1.0f, 2.0f, 3.0f);
	vec3 outNormal(-1, -2, -3);
	int uvLocation;
	int normalLocation;

	uvLocation = shader.addUniform("uv", ShaderVariable::EDT_VEC2);
	normalLocation = shader.addUniform("normal", ShaderVariable::EDT_VEC3);

	if (normalLocation != -1) {
		shader.uniform[normalLocation] = inNormal;
		shader.uniform[uvLocation] = inUV;

		for (unsigned int index = 0; index < shader.uniform.size(); ++index) {
			switch (shader.uniform[index].dataType) {
			case ShaderVariable::EDT_VEC2 :
				{
				UniformVariable tmp[3];
				tmp[0] = shader.uniform[uvLocation].get<vec2>() * 0.3f;
				tmp[1] = shader.uniform[uvLocation].get<vec2>() * 0.3f;
				tmp[2] = shader.uniform[uvLocation].get<vec2>() * 0.3f;
				shader.uniform[uvLocation] = tmp[0].get<vec2>() + tmp[1].get<vec2>() + tmp[2].get<vec2>();
				}
				break;
			case ShaderVariable::EDT_VEC3 :
				shader.uniform[normalLocation] = shader.uniform[normalLocation].get<vec3>() * 0.5f;
				break;
			}
		}
		outUV = shader.uniform[uvLocation].get<vec2>();
		outNormal = shader.uniform[normalLocation].get<vec3>();
	}

	printf("outUv: %f, %f\n", outUV.x, outUV.y);
	printf("outNormal: %f, %f, %f\n", outNormal.x, outNormal.y, outNormal.z);

	printf("sizeof(Shader) = %lu\n", sizeof(Shader));
	printf("sizeof(UniformVariable) = %lu\n", sizeof(UniformVariable));

	return 0;
#endif
	FBRenderer renderer;
	Image image;
	Image image2;
	Mesh floor, cube;
	Billboard billboard;
	float billAngle = 0.0f;
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

	if (image2.load("particle.tga") == false) {
		printf("Failed to load bill image.\n");
		return 3;
	}

	camera.position = vec3(0.0f, 50.0f, 30.0f);
	camera.target = vec3(0.0f, 0.0f,-50.0f);
	camera.viewDirty = true;	

	Light directionalLight(Light::ELT_DIRECTIONAL, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	directionalLight.direction = vec3(0.0f, 1.0f, 0.0f);
	directionalLight.direction.normalize();
	lightList.push_back(directionalLight);

	lightList.push_back(Light(Light::ELT_POINT, vec4(0.0f, 0.0f, 1.0f, 1.0f)));

	floor.position = vec3(0.0f, 0.0f,-50.0f);
	floor.scale = vec3(10.0f, 0.1f, 10.0f);
	floor.texture = &image;

	cube.position = vec3(0.0f, 0.0f,-50.0f);
	cube.scale = vec3(2.0f, 2.0f, 2.0f);
	cube.texture = &image;
	
	billboard.position = vec3(20.0f, 10.0f,-50.0f);
	billboard.texture = &image2;
	billboard.alphaBlend = true;

	while (true) {
		renderer.clear(true, true);

		camera.update();

		floor.draw(&renderer);

		cube.rotation += vec3(0.33f, 0.66f, 0.99f);
		cube.draw(&renderer, true);

		billboard.position.rotateXZBy(1.0f, vec3(0.0f, 10.0f,-50.0f));
		billboard.setTarget(camera.position);
		billboard.draw(&renderer);

		lightList[1].position = billboard.position;

		renderer.swap();
	}

	return 0;
}
