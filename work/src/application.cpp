
// std
#include <iostream>
#include <string>
#include <chrono>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// project
#include "application.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_image.hpp"
#include "cgra/cgra_shader.hpp"
#include "cgra/cgra_wavefront.hpp"


using namespace std;
using namespace cgra;
using namespace glm;


void basic_model::draw(const glm::mat4 &view, const glm::mat4 proj) {
	mat4 modelview = view * modelTransform;
	
	glUseProgram(shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));

	mesh.draw(); // draw
}


Application::Application(GLFWwindow *window) : m_window(window) {
	
	shader_builder sb;
    sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_vert.glsl"));
	sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_frag.glsl"));
	GLuint shader = sb.build();

	m_model.shader = shader;
	m_model.mesh = load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//teapot.obj")).build();
	m_model.color = vec3(1, 0, 0);
}

void Application::load_sphere_lat_long(float radius, int latDiv, int longDiv ) {
	mesh_builder mb;
	
	for(int j = 0; j < longDiv + 1; j++) {
		float phi = j * glm::pi<GLfloat>()/longDiv;
		
		for(int i = 0; i < latDiv; i++ ) {
			float theta =  i * 2 * glm::pi<GLfloat>()/latDiv;
			
			glm::vec3 pos(glm::sin(theta) * glm::sin(phi), glm::cos(theta)* glm::sin(phi), glm::cos(phi));
			mb.push_vertex(mesh_vertex{radius * pos, pos, glm::vec2(0)});
			
		}
	}
	for (int j = 0; j < longDiv ; j++) {
		for(int i = 0; i < latDiv - 1; i++){
			mb.push_index(j * latDiv + i);
			mb.push_index(j * latDiv + i + 1);
			mb.push_index((j + 1) * latDiv + i);
			mb.push_index(j * latDiv + i + 1);
			mb.push_index((j + 1) * latDiv + i);
			mb.push_index((j + 1) * latDiv + i + 1);
		}
			mb.push_index(j * latDiv + latDiv - 1);
			mb.push_index(j * latDiv + 0);
			mb.push_index((j + 1) * latDiv + latDiv - 1);
			mb.push_index(j * latDiv + 0);
			mb.push_index((j + 1) * latDiv + latDiv - 1);
			mb.push_index((j + 1) * latDiv );

	}
	m_model.mesh = mb.build();


}

void Application::load_sphere_cube(float radius, float explodeDistance, int divisions) {
	mesh_builder mb;
	float offset = 2.0f / (divisions - 1);
	// Face 0
	for (int j = 0; j < divisions; j++){
		for (int i = 0; i < divisions; i++){
			glm::vec3 pos(-1, -1 + j * offset, -1 + i * offset);
			pos = radius * glm::normalize(pos);
			if(m_expand){
				pos = pos + glm::vec3(-explodeDistance, 0, 0);
			}
			mb.push_vertex(mesh_vertex{pos, pos, glm::vec2(0,0)});
		}
	}
	for (int j = 0; j < divisions - 1; j++){
		for (int i = 0; i < divisions - 1; i++){
			mb.push_index(j * divisions + i);
			mb.push_index(j * divisions + i + 1);
			mb.push_index((j + 1) * divisions + i);
			mb.push_index(j * divisions + i + 1);
			mb.push_index((j + 1) * divisions + i);
			mb.push_index((j + 1) * divisions + i + 1);
		}
	}
	// Face 1
	for (int j = 0; j < divisions; j++){
		for (int i = 0; i < divisions; i++){
			glm::vec3 pos(1, -1 + j * offset, -1 + i * offset);
			pos = radius * glm::normalize(pos);
			if(m_expand){
				pos = pos + glm::vec3(explodeDistance, 0, 0);
			}
			mb.push_vertex(mesh_vertex{pos, pos, glm::vec2(0,0)});
		}
	}
	for (int j = 0; j < divisions - 1; j++){
		for (int i = 0; i < divisions - 1; i++){
			mb.push_index(divisions*divisions + j * divisions + i);
			mb.push_index(divisions*divisions + j * divisions + i + 1);
			mb.push_index(divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(divisions*divisions + j * divisions + i + 1);
			mb.push_index(divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(divisions*divisions + (j + 1) * divisions + i + 1);
		}
	}
	// Face 2
	for (int j = 0; j < divisions; j++){
		for (int i = 0; i < divisions; i++){
			glm::vec3 pos(-1 + j * offset, -1,  -1 + i * offset);
			pos = radius * glm::normalize(pos);
			if(m_expand){
				pos = pos + glm::vec3(0, -explodeDistance, 0);
			}
			mb.push_vertex(mesh_vertex{pos, pos, glm::vec2(0,0)});
		}
	}
	for (int j = 0; j < divisions - 1; j++){
		for (int i = 0; i < divisions - 1; i++){
			mb.push_index(2 * divisions*divisions + j * divisions + i);
			mb.push_index(2 * divisions*divisions + j * divisions + i + 1);
			mb.push_index(2 * divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(2 * divisions*divisions + j * divisions + i + 1);
			mb.push_index(2 * divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(2 * divisions*divisions + (j + 1) * divisions + i + 1);
		}
	}
	// Face 3
	for (int j = 0; j < divisions; j++){
		for (int i = 0; i < divisions; i++){
			glm::vec3 pos(-1 + j * offset, 1, -1 + i * offset);
			pos = radius * glm::normalize(pos);
			if(m_expand){
				pos = pos + glm::vec3(0, explodeDistance, 0);
			}
			mb.push_vertex(mesh_vertex{pos, pos, glm::vec2(0,0)});
		}
	}
	for (int j = 0; j < divisions - 1; j++){
		for (int i = 0; i < divisions - 1; i++){
			mb.push_index(3 * divisions*divisions + j * divisions + i);
			mb.push_index(3 * divisions*divisions + j * divisions + i + 1);
			mb.push_index(3 * divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(3 * divisions*divisions + j * divisions + i + 1);
			mb.push_index(3 * divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(3 * divisions*divisions + (j + 1) * divisions + i + 1);
		}
	}
	// Face 4
	for (int j = 0; j < divisions; j++){
		for (int i = 0; i < divisions; i++){
			glm::vec3 pos(-1 + j * offset,  -1 + i * offset, -1);
			pos = radius * glm::normalize(pos);
			if(m_expand){
				pos = pos + glm::vec3(0, 0, -explodeDistance);
			}
			mb.push_vertex(mesh_vertex{pos, pos, glm::vec2(0,0)});
		}
	}
	for (int j = 0; j < divisions - 1; j++){
		for (int i = 0; i < divisions - 1; i++){
			mb.push_index(4 * divisions*divisions + j * divisions + i);
			mb.push_index(4 * divisions*divisions + j * divisions + i + 1);
			mb.push_index(4 * divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(4 * divisions*divisions + j * divisions + i + 1);
			mb.push_index(4 * divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(4 * divisions*divisions + (j + 1) * divisions + i + 1);
		}
	}
	// Face 5
	for (int j = 0; j < divisions; j++){
		for (int i = 0; i < divisions; i++){
			glm::vec3 pos(-1 + j * offset,  -1 + i * offset, 1);
			pos = radius * glm::normalize(pos);
			if(m_expand){
				pos = pos + glm::vec3(0, 0, explodeDistance);
			}
			mb.push_vertex(mesh_vertex{pos, pos, glm::vec2(0,0)});
		}
	}
	for (int j = 0; j < divisions - 1; j++){
		for (int i = 0; i < divisions - 1; i++){
			mb.push_index(5 * divisions*divisions + j * divisions + i);
			mb.push_index(5 * divisions*divisions + j * divisions + i + 1);
			mb.push_index(5 * divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(5 * divisions*divisions + j * divisions + i + 1);
			mb.push_index(5 * divisions*divisions + (j + 1) * divisions + i);
			mb.push_index(5 * divisions*divisions + (j + 1) * divisions + i + 1);
		}
	}
	m_model.mesh = mb.build();
}

void Application::load_tor_lat_long(float radius, float thickness, int latDiv, int longDiv) {
	mesh_builder mb;
	for(int j = 0; j < longDiv + 1; j++) {
		float phi = j * 2 * glm::pi<GLfloat>()/longDiv;
		
		for(int i = 0; i < latDiv; i++ ) {
			float theta =  i * 2 * glm::pi<GLfloat>()/latDiv;
			
			glm::vec3 pos(thickness * glm::cos(theta), thickness * glm::sin(theta), 0);
			glm::vec3 norm = glm::normalize(pos);
			glm::vec3 axis( 1, 0, 0);
			glm::vec3 transVec(0, radius, 0);
			
			glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), phi, axis);
			
			// pos = glm::vec3(rotationMat * glm::vec4(pos, 1.0f));
			transVec = glm::vec3(rotationMat * glm::vec4(transVec, 1.0f));
			norm = glm::vec3(rotationMat * glm::vec4(norm, 1.0f));

			glm::mat4 transMat = glm::translate(glm::mat4(1.0f), transVec);
			pos = glm::vec3(transMat * rotationMat * glm::vec4(pos, 1.0f)); 

			mb.push_vertex(mesh_vertex{pos, norm, glm::vec2(0)});
			
		}
	}
	for (int j = 0; j < longDiv; j++) {
		for(int i = 0; i < latDiv - 1; i++){
			mb.push_index(j * latDiv + i);
			mb.push_index(j * latDiv + i + 1);
			mb.push_index((j + 1) * latDiv + i);
			mb.push_index(j * latDiv + i + 1);
			mb.push_index((j + 1) * latDiv + i);
			mb.push_index((j + 1) * latDiv + i + 1);
		}
			mb.push_index(j * latDiv + latDiv - 1);
			mb.push_index(j * latDiv + 0);
			mb.push_index((j + 1) * latDiv + latDiv - 1);
			mb.push_index(j * latDiv + 0);
			mb.push_index((j + 1) * latDiv + latDiv - 1);
			mb.push_index((j + 1) * latDiv );

	}
	m_model.mesh = mb.build();

}

void Application::render() {
	
	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);

	// projection matrix
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);

	// view matrix
	mat4 view = translate(mat4(1), vec3(0, 0, -m_distance))
		* rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0));


	// helpful draw options
	if (m_show_grid) drawGrid(view, proj);
	if (m_show_axis) drawAxis(view, proj);
	glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);


	// draw the model
	m_model.draw(view, proj);
}


void Application::renderGUI() {

	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);
	ImGui::Begin("Options", 0);

	// display current camera parameters
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("Pitch", &m_pitch, -pi<float>() / 2, pi<float>() / 2, "%.2f");
	ImGui::SliderFloat("Yaw", &m_yaw, -pi<float>(), pi<float>(), "%.2f");
	ImGui::SliderFloat("Distance", &m_distance, 0, 100, "%.2f", 2.0f);

	// Select model to be drawn
	if (ImGui::Button("Sphere LatLong")) {
		m_shape = SP_LATLONG;
		load_sphere_lat_long(m_sp_rad, m_latDivision, m_longDivision);
	}
	if (ImGui::Button("Sphere Cube")){
		m_shape = SP_CUBE;
		load_sphere_cube(m_sp_rad, m_sp_cube_exp_dis, m_cubeDivisions);
	}
	if (ImGui::Button("Torus")){
		m_shape = TOR_LATLONG;
		load_tor_lat_long(m_tor_radius, m_tor_thickness, m_latDivision, m_longDivision);

	}
	if (m_shape == SP_LATLONG) {
		int prevLatDiv = m_latDivision;
		int prevLongDiv = m_longDivision;
		float prevRad = m_sp_rad;
		ImGui::SliderFloat("Radius", &m_sp_rad, 0.5f, 5.0f);
		ImGui::SliderInt("Lat Divisions", &m_latDivision, 2, 30);
		ImGui::SliderInt("Long Divisions", &m_longDivision, 2, 30);
		if (prevLatDiv != m_latDivision || prevLongDiv != m_longDivision || prevRad != m_sp_rad){
			load_sphere_lat_long(m_sp_rad, m_latDivision, m_longDivision);
		}
	}

	if (m_shape == SP_CUBE) {
		int prevDivisions = m_cubeDivisions;
		float prevRad = m_sp_rad;
		
		if(ImGui::Button("Explode")) {
			m_expand = !m_expand;
			load_sphere_cube(m_sp_rad, m_sp_cube_exp_dis, m_cubeDivisions);
		}
		if(m_expand) {
			float prevExpDis = m_sp_cube_exp_dis;
			ImGui::SliderFloat("Explode Distance", &m_sp_cube_exp_dis, 0.0f, 5.0f);
			if (prevExpDis != m_sp_cube_exp_dis) {
				load_sphere_cube(m_sp_rad, m_sp_cube_exp_dis, m_cubeDivisions);
			}
		}
		ImGui::SliderFloat("Radius", &m_sp_rad, 0.5f, 5.0f);
		ImGui::SliderInt("Cube Divisions", &m_cubeDivisions, 2, 30);
		if (prevDivisions != m_cubeDivisions || prevRad != m_sp_rad){
			load_sphere_cube(m_sp_rad, m_sp_cube_exp_dis, m_cubeDivisions);
		}
	}
	if (m_shape == TOR_LATLONG) {
		int prevLatDiv = m_latDivision;
		int prevLongDiv = m_longDivision;
		float prevRad = m_tor_radius;
		float prevThick = m_tor_thickness;
		ImGui::SliderFloat("Radius", &m_tor_radius, 2.0f, 10.0f);
		ImGui::SliderFloat("Thickness", &m_tor_thickness, 0.5f, 3.0f);		
		ImGui::SliderInt("Lat Divisions", &m_latDivision, 2, 30);
		ImGui::SliderInt("Long Divisions", &m_longDivision, 2, 30);
		if (prevLatDiv != m_latDivision || prevLongDiv != m_longDivision || prevRad != m_tor_radius || prevThick != m_tor_thickness){
			load_tor_lat_long(m_tor_radius, m_tor_thickness, m_latDivision, m_longDivision);

		}
	}

	// helpful drawing options
	ImGui::Checkbox("Show axis", &m_show_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &m_show_grid);
	ImGui::Checkbox("Wireframe", &m_showWireframe);
	ImGui::SameLine();
	if (ImGui::Button("Screenshot")) rgba_image::screenshot(true);

	
	ImGui::Separator();

	// example of how to use input boxes
	static float exampleInput;
	if (ImGui::InputFloat("example input", &exampleInput)) {
		cout << "example input changed to " << exampleInput << endl;
	}

	// finish creating window
	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	if (m_leftMouseDown) {
		vec2 whsize = m_windowsize / 2.0f;

		// clamp the pitch to [-pi/2, pi/2]
		m_pitch += float(acos(glm::clamp((m_mousePosition.y - whsize.y) / whsize.y, -1.0f, 1.0f))
			- acos(glm::clamp((float(ypos) - whsize.y) / whsize.y, -1.0f, 1.0f)));
		m_pitch = float(glm::clamp(m_pitch, -pi<float>() / 2, pi<float>() / 2));

		// wrap the yaw to [-pi, pi]
		m_yaw += float(acos(glm::clamp((m_mousePosition.x - whsize.x) / whsize.x, -1.0f, 1.0f))
			- acos(glm::clamp((float(xpos) - whsize.x) / whsize.x, -1.0f, 1.0f)));
		if (m_yaw > pi<float>()) m_yaw -= float(2 * pi<float>());
		else if (m_yaw < -pi<float>()) m_yaw += float(2 * pi<float>());
	}

	// updated mouse position
	m_mousePosition = vec2(xpos, ypos);
}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)mods; // currently un-used

	// capture is left-mouse down
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		m_leftMouseDown = (action == GLFW_PRESS); // only other option is GLFW_RELEASE
}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset; // currently un-used
	m_distance *= pow(1.1f, -yoffset);
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}
