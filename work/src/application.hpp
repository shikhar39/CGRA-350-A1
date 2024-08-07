
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
	float ka = 0.1f;
	float F0 = 0.56f;
	float roughness = 0.23f;
	float rho = 0.65f;
	float albedo = 0.85f;
	glm::mat4 modelTransform{1.0};
	GLuint normalTexture;
	GLuint colorTexture;


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

	// Geometry
	enum Shape{
		NONE, 
		SP_LATLONG,
		SP_CUBE,
		TOR_LATLONG
	};

	// Shader
	enum Shader{
		PHONG,
		COOK_TORRENCE,
		OREN_NAYAR, 
		COMBO,
		NORMAL_MAP
	};

	Shape m_shape = NONE;
	Shader m_shader = PHONG;
	basic_model m_model;

	
	
	//Shape properties
	float m_sp_rad = 1.0f;
	int m_latDivision = 10;
	int m_longDivision = 10;

	float m_tor_radius = 3.0f;
	float m_tor_thickness = 1.0f;

	// Sphere From cube Properties
	int m_cubeDivisions = 5;
	bool m_expand = false;
	float m_sp_cube_exp_dis = 1.0f;
public:
	// setup
	Application(GLFWwindow *);

	// disable copy constructors (for safety)
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	//Change Shaders
	void load_phong();
	void load_cook_torrence();
	void load_oren_nayar();
	void load_combo_shader();
	void load_normal_map_shader();


	//Create Models
	void load_sphere_lat_long(float radius, int latDiv, int longDiv);
	void load_sphere_cube(float radius, float explodeDistance, int divisions);
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