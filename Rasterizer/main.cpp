#include <stdio.h>

#if defined (_WIN32)
#include "Window.h"
#elif defined (__linux__)
#include <linux/input.h>
#include "FrameBuffer.h"
#else
#error Unsupported platform!
#endif

#include "Timer.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

struct TestShader : public Shader {
	TestShader() 
		: Shader() {
	}
	
	void vertexShader(VertexShaderData& vertex) {
		if (uniform != NULL) {
			const ShaderUniform* myUniform = reinterpret_cast<const ShaderUniform*>(uniform);
			vertex.position = myUniform->modelViewProjectionMatrix * vertex.position;
		}
	}
	
	bool pixelShader(PixelShaderData& pixel) {
		if (pixel.texture[0] != NULL) {
			pixel.color *= pixel.texture[0]->sample2D(pixel.uv);
		}
		return true;
	}
};

int main() {
	/*************************************************************************/
	/* Output                                                                */
	/*************************************************************************/
	const uvec2 ScreenSize(640, 480);
#if defined (_WIN32)
	Win32Window output;	
	if (output.initialize(ScreenSize, Image::EPF_R8G8B8, "Software Renderer") == false) {
		printf("Failed to initialize the win32 window.\n");
		return 1;
	}
#elif defined (__linux__)
	FrameBuffer output;
	if (output.initialize(NULL, ScreenSize, Image::EPF_R8G8B8A8) != 0) {
		printf("Failed to initialize the frame buffer.\n");
		return 1;
	}
	output.input.addAllInputs();
#endif

	/*************************************************************************/
	/* Renderer                                                              */
	/*************************************************************************/
	Renderer renderer;
	RenderTarget renderTarget;
	Image colorBuffer;
	Image depthBuffer;
	
	colorBuffer.create(output.getSize(), output.getPixelFormat());
	colorBuffer.wrapping.x = Image::EWT_DISCARD;
	colorBuffer.wrapping.y = Image::EWT_DISCARD;
	depthBuffer.create(output.getSize(), Image::EPF_DEPTH);
	depthBuffer.wrapping.x = Image::EWT_DISCARD;
	depthBuffer.wrapping.y = Image::EWT_DISCARD;
	
	renderTarget.setBuffer(RenderTarget::ERT_COLOR_0, &colorBuffer);
	renderTarget.setBuffer(RenderTarget::ERT_DEPTH, &depthBuffer);

	renderer.setRenderTarget(&renderTarget);
	renderer.setViewport(vec4(0.0f, 0.0f, (float)ScreenSize.x, (float)ScreenSize.y));
	
	/*************************************************************************/
	/* Camera                                                                */
	/*************************************************************************/
	const float FieldOfView = 60.0f;
	const float AspectRatio = (float)ScreenSize.x / (float)ScreenSize.y;
	const vec2 ViewDistance(0.1f, 100.0f);
	
	Camera camera(FieldOfView, AspectRatio, ViewDistance.x, ViewDistance.y);
	camera.position = vec3(0.0f, 50.0f, 30.0f);
	camera.target = vec3(0.0f, 0.0f,-50.0f);
	
	/*************************************************************************/
	/* Textures                                                              */
	/*************************************************************************/
	Image texture[3];
	
	if (texture[0].load("tex_test.tga") == false) {
		printf("Failed to load texture0.\n");
		return 2;
	}

	if (texture[1].load("tex_particle.tga") == false) {
		printf("Failed to load texture1.\n");
		return 3;
	}
	
	if (texture[2].load("tex_suzanne.tga") == false) {
		printf("Failed to load suzanne texture.\n");
		return 3;
	}
	
	/************************************************************************/
	/* Shader                                                               */
	/************************************************************************/
	TestShader shader;

	/*************************************************************************/
	/* Floor mesh                                                            */
	/*************************************************************************/
	Mesh floor;
	floor.camera = &camera;
	floor.position = vec3(0.0f, 0.0f,-50.0f);
	floor.scale = vec3(10.0f, 0.1f, 10.0f);
	floor.texture = &texture[0];
	floor.vertices = CubeVertices;
	floor.vertexCount = CubeVerticesCount;
	floor.shader = &shader;

	/*************************************************************************/
	/* Cube mesh                                                             */
	/*************************************************************************/
	Mesh cube;
	cube.camera = &camera;
	cube.position = vec3(0.0f, 0.0f,-50.0f);
	cube.scale = vec3(2.0f, 2.0f, 2.0f);
	cube.texture = &texture[0];
	cube.vertices = CubeVertices;
	cube.vertexCount = CubeVerticesCount;
	cube.shader = &shader;
	
	/*************************************************************************/
	/* Billboard mesh                                                        */
	/*************************************************************************/
	Mesh billboard;
	billboard.camera = &camera;
	billboard.position = vec3(40.0f, 10.0f,-50.0f);
	billboard.texture = &texture[1];
	billboard.alphaBlend = true;
	billboard.shader = &shader;
	const float half = 10.5f;
	const Vertex billboardVertices[] = {
		Vertex(vec3( half, half, 0.0f), vec2(1.0f, 0.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
		Vertex(vec3(-half,-half, 0.0f), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
		Vertex(vec3(-half, half, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),

		Vertex(vec3(-half,-half, 0.0f), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
		Vertex(vec3( half, half, 0.0f), vec2(1.0f, 0.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
		Vertex(vec3( half,-half, 0.0f), vec2(1.0f, 1.0f), vec4(1.0f, 1.0f, 0.5f, 1.0f)),
	};
	billboard.vertices = billboardVertices;
	billboard.vertexCount = 6;
	
	Mesh suzanne;
	suzanne.loadObj("suzanne.obj");
	suzanne.camera = &camera;
	suzanne.position = vec3(0.0f, 0.0f,-50.0f);
	suzanne.scale = vec3(20.0f, 20.0f, 20.0f);
	suzanne.texture = &texture[2];
	suzanne.shader = &shader;
	
	/*************************************************************************/
	/* Misc variables                                                        */
	/*************************************************************************/
	bool drawObject[] = {true, true, true, true};
	bool keys[] = {false, false, false, false};
	bool running = true;
	Event event;	
	float billAngle = 0.0f;
	unsigned long long lastTime = Timer::GetMilliSeconds();
	unsigned int frameCount = 0;	
	unsigned int totalFPS = 0;
	unsigned int totalSeconds = 0;
	
	/*************************************************************************/
	/* Main loop                                                             */
	/*************************************************************************/
	while (running) {

		// Parse input events
		while (output.getEvent(&event)) {
			switch (event.type) {
			case Event::WINDOW_CLOSE :
				running = false;
				break;

			case Event::KEYBOARD :
				if (event.state == 0) { // Key released
					switch (event.key) {
					case KEY_1 :
						printf("Object 1 visible: %s\n", (drawObject[0] = !drawObject[0]) ? "On" : "Off");
						break;
					case KEY_2 :
						printf("Object 2 visible: %s\n", (drawObject[1] = !drawObject[1]) ? "On" : "Off");
						break;
					case KEY_3 :
						printf("Object 3 visible: %s\n", (drawObject[2] = !drawObject[2]) ? "On" : "Off");
						break;
					case KEY_4 :
						printf("Object 4 visible: %s\n", (drawObject[3] = !drawObject[3]) ? "On" : "Off");
						break;
					case KEY_P :
						printf("Perspective correction: %s\n", renderer.toggleFlag(Renderer::GFX_PERSPECTIVE_CORRECT) ? "On" : "Off");
						break;
					case KEY_W :
						printf("Wireframe: %s\n", renderer.toggleFlag(Renderer::GFX_WIREFRAME) ? "On" : "Off");
						break;
					case KEY_ESCAPE :
						running = false;
						break;
					}
				}

				switch (event.key) {
				case KEY_LEFT :
					keys[0] = event.state;
					break;
				case KEY_DOWN :
					keys[1] = event.state;
					break;
				case KEY_RIGHT :
					keys[2] = event.state;
					break;
				case KEY_UP :
					keys[3] = event.state;
					break;
				}
				break;
			}
		}
		
		// Clear the old frame data
		colorBuffer.clear();
		depthBuffer.clear();

		// Update camera transformations
		if (keys[0]) {
			camera.position.x -= 1.0f;
			camera.target.x -= 1.0f;
			camera.viewDirty = true;
		}
		if (keys[1]) {
			camera.position.z += 1.0f;
			camera.target.z += 1.0f;
			camera.viewDirty = true;
		}
		if (keys[2]) {
			camera.position.x += 1.0f;
			camera.target.x += 1.0f;
			camera.viewDirty = true;
		}
		if (keys[3]) {
			camera.position.z -= 1.0f;
			camera.target.z -= 1.0f;
			camera.viewDirty = true;
		}

		camera.update();

		// Render the meshes
		if (drawObject[0]) {
			floor.draw(&renderer);
		}

		cube.rotation += vec3(0.33f, 0.66f, 0.99f);
		if (drawObject[1]) {
			cube.draw(&renderer);
		}

		suzanne.rotation.y += 0.2f;
		if (drawObject[3]) {
			suzanne.draw(&renderer);
		}

		billboard.position.rotateXZBy(0.5f, vec3(0.0f, 10.0f,-50.0f));
		billboard.setTarget(camera.position);
		if (drawObject[2]) {
			billboard.draw(&renderer);
		}
		
		// Blit the final image to the output
		output.blit(&colorBuffer);
		
		// Calculate the FPS
		++frameCount;
		
		const unsigned long long currentTime = Timer::GetMilliSeconds();
		if ((currentTime - lastTime) >= 1000) {
			char tmp[128];
			
			totalFPS += frameCount;
			totalSeconds += 1;
			
			printf("Software Renderer FPS: %d | %.1f\n", frameCount, (float)totalFPS / (float)totalSeconds);
			frameCount = 0;
			lastTime += 1000;
		}
	}
	
	return 0;
}
