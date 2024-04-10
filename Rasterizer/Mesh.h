#ifndef __MESH_H__
#define __MESH_H__

#include "CubeMesh.h"
#include "Renderer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

struct ShaderUniform {
	mat4 modelViewProjectionMatrix;
	vec4 ambientLight;
};

struct Mesh {
	Image* texture;
	Camera* camera;
	Shader* shader;
	
	const Vertex* vertices;
	unsigned int vertexCount;
	std::vector<Vertex> vertexBuffer;
	
	mat4 model;

	vec3 position;
	vec3 rotation;
	vec3 scale;

	bool alphaBlend;
	
	Mesh() {
		texture = NULL;
		vertices = NULL;
		vertexCount = 0;
		shader = NULL;

		model.setIdentity();

		position = vec3( 0.0f, 0.0f, 0.0f);
		rotation = vec3( 0.0f, 0.0f, 0.0f);
		scale    = vec3( 1.0f, 1.0f, 1.0f);

		alphaBlend = false;
		
	}

	void setTarget(vec3 target) {
		vec3 dis = target - position;
		const float xzdis = sqrt(dis.x * dis.x + dis.z * dis.z);
		rotation.x = rad2deg(-atan2f(dis.y, xzdis));
		rotation.y = rad2deg(-(atan2f(-dis.x, dis.z)));
		rotation.z = 0.0f;
	}

	void draw(Renderer* renderer) {
		if ((vertices == NULL) || (vertexCount == 0)) {
		//	return;
		}
		
		renderer->setActiveTexture(0, texture);

		renderer->setFlag(Renderer::ERF_DEPTH_TEST, true);
		renderer->setFlag(Renderer::ERF_DEPTH_MASK, true);
		renderer->setFlag(Renderer::ERF_ALPHA_BLEND, alphaBlend);

		model.setTransformation(position, rotation, scale);

		ShaderUniform uniform;
		uniform.modelViewProjectionMatrix = camera->viewProjection * model;
		shader->uniform = &uniform;
		
		renderer->setShader(shader);

		if (vertices != NULL) {
			renderer->render(Renderer::EPT_TRIANGLES, vertices, vertexCount);
		} else {
			renderer->render(Renderer::EPT_TRIANGLES, &vertexBuffer[0], vertexBuffer.size());
		}
	}
	
	int loadObj(const char* filename) {
		FILE* file = fopen(filename, "r");
		if (file == NULL) {
			printf("Mesh::load(%s): Failed to open file.\n", filename);
			return 1;
		}
		
		char line[256];
		char objectName[64];
		bool switchVertexOrder = true;
		std::vector<vec3> positions;
		std::vector<vec3> normals;
		std::vector<vec2> uvs;
		
		vec3 tmp3;
		
		int indices[9];
		
		while (fgets(line, sizeof(line), file) != NULL) {
			if (sscanf(line, "o %s", objectName) == 1) {
				printf("Found object name: %s\n", objectName);
			} else if (sscanf(line, "v %f %f %f", &tmp3.x, &tmp3.y, &tmp3.z) == 3) {
				positions.push_back(tmp3);
			} else if (sscanf(line, "vn %f %f %f", &tmp3.x, &tmp3.y, &tmp3.z) == 3) {
				normals.push_back(tmp3);
			} else if (sscanf(line, "vt %f %f", &tmp3.x, &tmp3.y) == 2) {
				uvs.push_back(vec2(tmp3.x, tmp3.y));
			} else if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
								&indices[0], &indices[1], &indices[2],
								&indices[3], &indices[4], &indices[5],
								&indices[6], &indices[7], &indices[8]) == 9) {
				Vertex vertex;
				vertex.color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
				
				for (unsigned int index = 0; index < 9; ++index) {
					indices[index] -= 1;
				}
				
				if ((indices[0] >= positions.size()) || (indices[3] >= positions.size()) || (indices[6] >= positions.size())) {
					printf("Mesh::load(%s): Found an out of bounds position index\n", filename);
					continue;
				}
				if ((indices[1] >= uvs.size()) || (indices[4] >= uvs.size()) || (indices[7] >= uvs.size())) {
					printf("Mesh::load(%s): Found an out of bounds uv index\n", filename);
					continue;
				}
				if ((indices[2] >= normals.size()) || (indices[5] >= normals.size()) || (indices[8] >= normals.size())) {
					printf("Mesh::load(%s): Found an out of bounds normal index\n", filename);
					continue;
				}

				
				if (switchVertexOrder) {
					vertex.position = positions[indices[3]];
					vertex.textureCoords = uvs[indices[4]];
					vertex.normal = normals[indices[5]];
					vertexBuffer.push_back(vertex);
					
					vertex.position = positions[indices[0]];
					vertex.textureCoords = uvs[indices[1]];
					vertex.normal = normals[indices[2]];
					vertexBuffer.push_back(vertex);
					
					vertex.position = positions[indices[6]];
					vertex.textureCoords = uvs[indices[7]];
					vertex.normal = normals[indices[8]];
					vertexBuffer.push_back(vertex);
				} else {
					vertex.position = positions[indices[0]];
					vertex.textureCoords = uvs[indices[1]];
					vertex.normal = normals[indices[2]];
					vertexBuffer.push_back(vertex);
					
					vertex.position = positions[indices[3]];
					vertex.textureCoords = uvs[indices[4]];
					vertex.normal = normals[indices[5]];
					vertexBuffer.push_back(vertex);
					
					vertex.position = positions[indices[6]];
					vertex.textureCoords = uvs[indices[7]];
					vertex.normal = normals[indices[8]];
					vertexBuffer.push_back(vertex);
				}
			}
		}
		
		printf("Mesh::load(%s): Loaded %lu vertices\n", filename, vertexBuffer.size());
		
		fclose(file);
		
		return 0;
	}
};

#endif // __MESH_H__