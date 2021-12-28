/******************************************************************************/
/* Source: https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing&src=0 */
/******************************************************************************/
#include <stdio.h>
#include <cmath> 
#include <vector>

#if defined (_WIN32)
#include "Window.h"
#elif defined (__linux__)
#include "FrameBuffer.h"
#else
#error Unsupported platform!
#endif

#include "Timer.h" 

class Camera {
public:
	vec3 position;
	vec3 rotation;
	float aspectRatio;
	float fieldOfView;
	float speed;
	bool keys[4];
	
	float angle;
	vec2 invSize;
	
public:
	Camera(const uvec2& size = uvec2(640, 480), const float fov = 60.0f, const float speed = 1.0f) {
		keys[0] = false;
		keys[1] = false;
		keys[2] = false;
		keys[3] = false;
		//setAspectRatio(size.x / size.y);
		setFieldOfView(fov);
		setSpeed(speed);
	}

	void setPosition(const vec3 &value) {
		position = value;
	}

	const vec3 &getPosition() const {
		return position;
	}

	void setRotation(const vec3 &value) {
		rotation = value;
	}

	const vec3 &getRotation() const {
		return rotation;
	}

	void setAspectRatio(const float value) {
		aspectRatio = value;
	}

	float getAspectRation() const {
		return aspectRatio;
	}

	void setFieldOfView(const float value) {
		fieldOfView = value;
	}

	float getFieldOfView() const {
		return fieldOfView;
	}

	void setSpeed(const float value) {
		speed = value;
	}

	float getSpeed() const {
		return speed;
	}

	bool onEvent(const Event& event) {
		switch (event.type) {
		case Event::KEYBOARD :
			switch (event.key) {
			case KEY_LEFT :
				keys[0] = event.state;
				return true;
			case KEY_DOWN :
				keys[1] = event.state;
				return true;
			case KEY_RIGHT :
				keys[2] = event.state;
				return true;
			case KEY_UP :
				keys[3] = event.state;
				return true;
			}
			break;
		}
	
		return false;
	}

	void update(float deltaTime) {
		static const float DEG2RAD = M_PI / 180.0f;
		const float snX = sinf(rotation.y * DEG2RAD);
		const float csX = cos(rotation.y * DEG2RAD);
		
		float step = speed * deltaTime;
		
		if (keys[0]) {
			rotation.y -= 3.0f * step;
		}
		if (keys[2]) {
			rotation.y += 3.0f * step;
		}		
		if (keys[3]) {
			position.x += snX * step;
			position.z -= csX * step;
		}
		if (keys[1]) {
			position.x -= snX * step;
			position.z += csX * step;
		}
	}
};

struct Sphere { 
    vec3 center;                           /// position of the sphere 
    float radius, radius2;                  /// sphere radius and radius^2 
    vec3 surfaceColor, emissionColor;      /// surface color and emission (light) 
    float transparency, reflection;         /// surface transparency and reflectivity 
    Sphere(const vec3 &c, const float &r, const vec3 &sc, const float &refl = 0, const float &transp = 0, const vec3 &ec = 0) 
		: center(c), radius(r), radius2(r * r), surfaceColor(sc), emissionColor(ec),  transparency(transp), reflection(refl) 
    { } 

    bool intersect(const vec3 &rayorig, const vec3 &raydir, float &t0, float &t1) const  { 
        const vec3 l = center - rayorig; 
        const float tca = l.dot(raydir); 
        if (tca < 0) {
			return false; 
		}
		
        const float d2 = l.dot(l) - tca * tca; 
        if (d2 > radius2) {
			return false; 
		}
		
        float thc = sqrt(radius2 - d2); 
        t0 = tca - thc; 
        t1 = tca + thc; 
        return true; 
    } 
}; 

constexpr float kEpsilon = 1e-8; 
bool rayTriangleIntersect( 
	const vec3 &orig, const vec3 &dir, 
    const vec3 &v0, const vec3 &v1, const vec3 &v2, 
    float &t, float &u, float &v) { 
    vec3 v0v1 = v1 - v0; 
    vec3 v0v2 = v2 - v0; 
    vec3 pvec = dir.cross(v0v2); 
    float det = v0v1.dot(pvec); 

    if (det < kEpsilon) {
		return false; 
	}

    float invDet = 1 / det; 

    vec3 tvec = orig - v0; 
    u = tvec.dot(pvec) * invDet; 
    if (u < 0 || u > 1) {
		return false; 
	}

    vec3 qvec = tvec.cross(v0v1); 
    v = dir.dot(qvec) * invDet; 
    if (v < 0 || u + v > 1) {
		return false; 
	}

    t = v0v2.dot(qvec) * invDet; 
     return true;
} 

float mix(const float &a, const float &b, const float &mix) {
    return b * mix + a * (1.0f - mix); 
} 

int MAX_RAY_DEPTH = 0;

vec3 trace(const vec3 &rayorig, const vec3 &raydir, const std::vector<Sphere> &spheres, const int &depth)  { 
    //if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
    float tnear = INFINITY; 
    const Sphere* sphere = NULL; 

    // find intersection of this ray with the sphere in the scene
    for (unsigned i = 0; i < spheres.size(); ++i) { 
        float t0 = INFINITY, t1 = INFINITY; 
        if (spheres[i].intersect(rayorig, raydir, t0, t1)) { 
            if (t0 < 0) {
				t0 = t1; 
			}

            if (t0 < tnear) { 
                tnear = t0; 
                sphere = &spheres[i]; 
            } 
        } 
    } 

    // if there's no intersection return black or background color
    if (!sphere) {
		return vec3(1.0f); 
	}

    vec3 surfaceColor = 0.0f; // color of the ray/surfaceof the object intersected by the ray 
    vec3 phit = rayorig + raydir * tnear; // point of intersection 
    vec3 nhit = phit - sphere->center; // normal at the intersection point 
    nhit.normalize(); // normalize normal direction 
    // If the normal and the view direction are not opposite to each other
    // reverse the normal direction. That also means we are inside the sphere so set
    // the inside bool to true. Finally reverse the sign of IdotN which we want
    // positive.
    float bias = 1e-4; // add some bias to the point from which we will be tracing 
    bool inside = false; 
    if (raydir.dot(nhit) > 0.0f) {
		nhit = -nhit;
		inside = true; 
	}

    if ((sphere->transparency > 0.0f || sphere->reflection > 0.0f) && depth < MAX_RAY_DEPTH) { 
        float facingratio = -raydir.dot(nhit); 
        // change the mix value to tweak the effect
        float fresneleffect = mix(pow(1 - facingratio, 3.0f), 1.0f, 0.1f); 
        // compute reflection direction (not need to normalize because all vectors
        // are already normalized)
        vec3 refldir = raydir - nhit * 2.0f * raydir.dot(nhit); 
        refldir.normalize(); 
        vec3 reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1); 
        vec3 refraction;

        // if the sphere is also transparent compute refraction ray (transmission)
        if (sphere->transparency) { 
            float ior = 1.1f;
			float eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface? 
            float cosi = -nhit.dot(raydir); 
            float k = 1.0f - eta * eta * (1.0f - cosi * cosi); 
            vec3 refrdir = raydir * eta + nhit * (eta *  cosi - sqrt(k)); 
            refrdir.normalize(); 
            refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1); 
        } 
        // the result is a mix of reflection and refraction (if the sphere is transparent)
        surfaceColor = (reflection * fresneleffect + refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor; 
    } else { 
        // it's a diffuse object, no need to raytrace any further
        for (unsigned i = 0; i < spheres.size(); ++i) { 
            if (spheres[i].emissionColor.x > 0.0f) { 
                // this is a light
                vec3 transmission = 1; 
                vec3 lightDirection = spheres[i].center - phit; 
                lightDirection.normalize(); 
                for (unsigned j = 0; j < spheres.size(); ++j) { 
                    if (i != j) { 
                        float t0, t1; 
                        if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) { 
                            transmission = 0; 
                            break; 
                        } 
                    } 
                } 
                surfaceColor += sphere->surfaceColor * transmission * 
                std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor; 
            } 
        } 
    } 

    return surfaceColor + sphere->emissionColor; 
} 

bool RayIntersectsTriangle(const vec3 &rayOrigin, 
                           const vec3 &rayVector, 
                           const vec3* triangle,
                           vec3& outIntersectionPoint,
						   vec3& bcc) {
    const float EPSILON = 0.0000001;
    vec3 edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = triangle[1] - triangle[0];
    edge2 = triangle[2] - triangle[0];
    h = rayVector.cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON) {
        return false;    // This ray is parallel to this triangle.
	}
    f = 1.0 / a;
    s = rayOrigin - triangle[0];
    u = f * s.dot(h);
    if (u < 0.0 || u > 1.0) {
        return false;
	}
    q = s.cross(edge1);
    v = f * rayVector.dot(q);
    if (v < 0.0 || u + v > 1.0) {
        return false;
	}
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * edge2.dot(q);
    if (t > EPSILON) { // ray intersection
        outIntersectionPoint = rayOrigin + rayVector * t;
		bcc.x = u;
		bcc.y = v;
		bcc.z = 1.0f -u - v;
        return true;
    } 
	// This means that there is a line intersection but not a ray intersection.
	return false;
}

int main(int argc, char **argv) {
	/*************************************************************************/
	/* Output                                                                */
	/*************************************************************************/
	const uvec2 ScreenSize(640, 480);
#if defined (_WIN32)
	Win32Window output;	
	if (output.initialize(ScreenSize, Image::EPF_R8G8B8, "Raytracer") == false) {
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
	Image colorBuffer;
	colorBuffer.create(output.getSize(), output.getPixelFormat());
	colorBuffer.wrapping.x = Image::EWT_DISCARD;
	colorBuffer.wrapping.y = Image::EWT_DISCARD;

    std::vector<Sphere> spheres; 
    // position, radius, surface color, reflectivity, transparency, emission color
    spheres.push_back(Sphere(vec3( 0.0f, -10004, -20.0f), 10000.0f, vec3(0.2f, 0.2f, 0.2f), 0.0f, 0.0f)); 
    spheres.push_back(Sphere(vec3( 0.0f,      0, -20.0f),     4.0f, vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f)); 
    spheres.push_back(Sphere(vec3( 5.0f,      0, -25.0f),     3.0f, vec3(0.0f, 0.0f, 1.0f), 0.1f, 0.0f)); 
    spheres.push_back(Sphere(vec3( 5.0f,     -1, -15.0f),     2.0f, vec3(0.0f, 1.0f, 0.0f), 0.9f, 0.0f)); 
    spheres.push_back(Sphere(vec3(-5.5f,      0, -15.0f),     3.0f, vec3(1.0f, 0.0f, 1.0f), 0.1f, 0.0f)); 
    // light
    spheres.push_back(Sphere(vec3( 0.0f,  20.0f, -30.0f),     3.0f, vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, vec3(3))); 

 	/*************************************************************************/
	/* Camera                                                                */
	/*************************************************************************/
	Camera camera;

	/*************************************************************************/
	/* Main loop                                                             */
	/*************************************************************************/
	const float invWidth  = 1 / float(ScreenSize.x);
	const float invHeight = 1 / float(ScreenSize.y); 
	const float fov = 30.0f;
	const float aspectratio = (float)ScreenSize.x / float(ScreenSize.y); 
	const float angle = tan(M_PI * 0.5 * fov / 180.); 

	Event event;
	bool running = true;

	while (running) {
		// Parse input events
		while (output.getEvent(&event)) {
			if (camera.onEvent(event) == false) {
				switch (event.type) {
				case Event::WINDOW_CLOSE :
					running = false;
					break;

				case Event::KEYBOARD :
					if (event.state == 0) { // Key released
						switch (event.key) {
						case KEY_ESCAPE :
							running = false;
							break;
						case KEY_2 :
							if (++MAX_RAY_DEPTH > 5) MAX_RAY_DEPTH = 5;
							break;
						case KEY_1 :
							if (--MAX_RAY_DEPTH < 0) MAX_RAY_DEPTH = 0;
							break;
						case KEY_3 :
							spheres[1].transparency -= 0.1f;
							if (spheres[1].transparency < 0.0f) spheres[1].transparency = 0.0f;
							printf("spheres[1].transparency = %f\n", spheres[1].transparency);
							break;
						case KEY_4 :
							spheres[1].transparency += 0.1f;
							if (spheres[1].transparency > 1.0f) spheres[1].transparency = 1.0f;
							printf("spheres[1].transparency = %f\n", spheres[1].transparency);
							break;
						case KEY_5 :
							spheres[1].reflection -= 0.1f;
							if (spheres[1].reflection < 0.0f) spheres[1].reflection = 0.0f;
							printf("spheres[1].reflection = %f\n", spheres[1].reflection);
							break;
						case KEY_6 :
							spheres[1].reflection += 0.1f;
							if (spheres[1].reflection > 1.0f) spheres[1].reflection = 1.0f;
							printf("spheres[1].reflection = %f\n", spheres[1].reflection);
							break;
						}
					}
				}
			}
		}		

		camera.update(1.0f);

		// Trace rays
		for (unsigned y = 0; y < ScreenSize.y; ++y) { 
			for (unsigned x = 0; x < ScreenSize.x; ++x) { 
				vec3 rayDirection;
				rayDirection.x = (2.0f * ((x + 0.5f) * invWidth) - 1.0f) * angle * aspectratio; 
				rayDirection.y = (1.0f - 2.0f * ((y + 0.5f) * invHeight)) * angle;
				rayDirection.z = -1.0f;
				rayDirection.rotateXZBy(camera.rotation.y);
				rayDirection.normalize(); 

				vec3 pixel = trace(camera.position, rayDirection, spheres, 0); 

				if (pixel.x > 1.0f) {
					pixel.x = 1.0f;
				}
				if (pixel.y > 1.0f) {
					pixel.y = 1.0f;
				}
				if (pixel.z > 1.0f) {
					pixel.z = 1.0f;
				}

				colorBuffer.setPixelf(x, y, vec4(pixel, 1.0f));
			}
		}
		// Blit the final image to the output
		output.blit(&colorBuffer);
	}

    return 0; 
} 