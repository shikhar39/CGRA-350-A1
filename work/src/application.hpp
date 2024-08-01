
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "skeleton_model.hpp"


// Basic model that holds the shader, mesh and transform for drawing.
// Can be copied and modified for adding in extra information for drawing
// including textures for texture mapping etc.
struct basic_model {
	GLuint shader = 0;
	cgra::gl_mesh mesh;
	glm::vec3 color{0.7};
	glm::mat4 modelTransform{1.0};
	GLuint texture;

	void draw(const glm::mat4 &view, const glm::mat4 proj);
};


// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	// oribital camera
	float m_pitch = .86;
	float m_yaw = -.86;
	float m_distance = 20;

	// last input
	bool m_leftMouseDown = false;
	glm::vec2 m_mousePosition;

	// drawing flags
	bool m_show_axis = false;
	bool m_show_grid = false;
	bool m_showWireframe = false;

	// geometry
	enum Shape{
		NONE, 
		SP_LATLONG,
		SP_CUBE,
		TOR_LATLONG
	};

	Shape m_shape = NONE;
	basic_model m_model;
	//Sphere_LatLong and TORUS_LatLong Properties
	int m_latDivision = 10;
	int m_longDivision = 10;

	float m_tor_radius = 3.0f;
	float m_tor_thickness = 1.0f;

	// Sphere From cube Properties
	int m_cubeDivisions = 5;
	bool m_expand = false;
public:
	// setup
	Application(GLFWwindow *);

	// disable copy constructors (for safety)
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;



	//Create Models
	void load_sphere_lat_long(int latDiv, int longDiv);
	void load_sphere_cube(int divisions);
	void load_tor_lat_long(float rad, float thickness, int latDivisions, int longDivisions);

	// rendering callbacks (every frame)
	void render();
	void renderGUI();

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
};