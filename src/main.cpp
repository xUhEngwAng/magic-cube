#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shader.h"
#include "camera.h"
#include "magic_cube.h"
#include "cube.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
const unsigned int NUM_TEXTURES = 7;

Camera cam(glm::vec3(2.0f, 2.0f, 2.0f), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT);
HitRecord rec;
MagicCube magicCube;

const glm::vec2 scr_axis[3] = {glm::normalize(glm::vec2(-1.2f, 1.0f)),
							   glm::vec2(1.0f, 0), 
							   glm::normalize(glm::vec2(-1.2f, -1.0f))};
RotateState rotate_state = ROTATE_NONE;
RotateMode  rotate_mode;
RotateLayer rotate_layer;

double press_xpos, press_ypos;
bool mouse_pressed;

float rotate_angle = 0;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //uncomment this statement to fix compilation on OS X
#endif
	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// load textures and configure magicCube
	// -------------------------------------
	std::string texPaths[NUM_TEXTURES] = {"./images/black.png",   "./images/green.png",  "./images/orange.png", "./images/red.png", 
										  "./images/skyblue.png", "./images/yellow.png", "./images/white.png" };
	magicCube.loadTextures(NUM_TEXTURES, texPaths);
	// load shader programs
	// --------------------
	Shader shader("./shader/vertex.glsl", "./shader/fragment.glsl");
	shader.Use();
	// configure MVP matrices
	// ----------------------
	glm::mat4 view, perspective;
	view = cam.getView();
	GLfloat u, v;
	shader.setMat4("view", view);
	// --------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//magicCube.rotate(glm::vec3(0, 1.0f, 0), glm::radians(0.3f));
		//magicCube.rotateUpperLayerY(glm::radians(0.3f));
		perspective = cam.getPerspective();
		shader.setMat4("perspective", perspective);
		
		magicCube.draw(shader, rotate_state, rotate_layer, rotate_angle);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFWresources.
	//---------------------------------------------------------------
	//cube.finishDrawing();
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	float aspect_ratio = (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT;
	cam.setAspectRatio(aspect_ratio);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	double x_offset, y_offset;
	float u, v;
	if(button == GLFW_MOUSE_BUTTON_LEFT){
		if(action == GLFW_PRESS){
			glfwGetCursorPos(window, &press_xpos, &press_ypos);
			u = press_xpos / SCR_WIDTH;
			v = 1 - press_ypos / SCR_HEIGHT;
			glm::vec3 target = cam.at(u, v);
			glm::vec3 cam_pos = cam.getPosition();
			Ray ray(cam_pos, glm::normalize(target - cam_pos));

			if(magicCube.hit(ray, 1e-5, 100.0f, rec)) 
				rotate_mode = ROTATE_LOCAL;
			else 
				rotate_mode = ROTATE_GLOBAL;

			mouse_pressed = true;
		}
		if(action == GLFW_RELEASE){
			mouse_pressed = false;
			int num_rotates = rotate_angle / 90.0;
			if(fabs(rotate_angle - 90.0 * num_rotates) > 45.0){
				if(rotate_angle < 0) num_rotates -= 1;
				else num_rotates += 1;
			}
			rotate_angle = 0;
			rotate_state = ROTATE_NONE;
		}
	}
}

void global_rotate(glm::vec2 mouse_offset){
	const double velocity = 50.0;
	float xoffset = glm::dot(scr_axis[0], mouse_offset);
	float yoffset = glm::dot(scr_axis[1], mouse_offset);
	float zoffset = glm::dot(scr_axis[2], mouse_offset);
	if(rotate_state == ROTATE_NONE){
		if(fabs(xoffset) > fabs(yoffset)){
			if(fabs(xoffset) > fabs(zoffset)) rotate_state = ROTATE_X;
			else rotate_state = ROTATE_Z;
		}
		else{
			if(fabs(yoffset) > fabs(zoffset)) rotate_state = ROTATE_Y;
			else rotate_state = ROTATE_Z;
		}
	}
	switch(rotate_state){
		case ROTATE_X:
			rotate_angle += velocity * xoffset / (SCR_HEIGHT + SCR_WIDTH);
		break;
		case ROTATE_Y:
			rotate_angle += velocity * yoffset / (SCR_HEIGHT + SCR_WIDTH);
		break;
		case ROTATE_Z:
			rotate_angle += velocity * zoffset / (SCR_HEIGHT + SCR_WIDTH);
		break;
		default:
			std::cerr << "A fatal error occurred. rotate_state == ROTATE_NONE." << std::endl;;
		break;
	}
	rotate_layer = LAYER_ALL;
}

void local_rotate(glm::vec2 mouse_offset, const HitRecord& rec){
	const double velocity = 50.0;
	glm::vec3 hit_point = rec.p;
	float xoffset = 0, yoffset = 0, zoffset = 0;
	if(fabs(hit_point.x - 0.45) < 1e-5){ // Hit right face 
		yoffset = glm::dot(glm::normalize(glm::vec2(1.5f, -1.0f)), mouse_offset);
		zoffset = glm::dot(glm::normalize(glm::vec2(0, -1.0f)), mouse_offset);
		if(rotate_state == ROTATE_NONE){
			if(fabs(yoffset) > fabs(zoffset)){ 
				// Rotate along y-axis
				rotate_state = ROTATE_Y;
				if(fabs(hit_point.y + 0.3f) < 0.15f) rotate_layer = LAYER_ONE;
				if(fabs(hit_point.y) < 0.15f) rotate_layer = LAYER_TWO;
				if(fabs(hit_point.y - 0.3f) < 0.15f) rotate_layer = LAYER_THREE;
			}
			else{ 
				// Rotate along z-axis
				rotate_state = ROTATE_Z;
				if(fabs(hit_point.z + 0.3f) < 0.15f) rotate_layer = LAYER_ONE;
				if(fabs(hit_point.z) < 0.15f) rotate_layer = LAYER_TWO;
				if(fabs(hit_point.z - 0.3f) < 0.15f) rotate_layer = LAYER_THREE;
			}
		}
	}else
	if(fabs(hit_point.y - 0.45) < 1e-5){ // Hit Upper face
		xoffset = glm::dot(glm::normalize(glm::vec2(-1.5f, 1.0f)), mouse_offset);
		zoffset = glm::dot(glm::normalize(glm::vec2(-1.5f, -1.0f)), mouse_offset);
		if(rotate_state == ROTATE_NONE){
			if(fabs(xoffset) > fabs(zoffset)){ 
				// Rotate along x-axis
				rotate_state = ROTATE_X;
				if(fabs(hit_point.x + 0.3f) < 0.15f) rotate_layer = LAYER_ONE;
				if(fabs(hit_point.x) < 0.15f) rotate_layer = LAYER_TWO;
				if(fabs(hit_point.x - 0.3f) < 0.15f) rotate_layer = LAYER_THREE;
			}
			else{ 
				// Rotate along z-axis
				rotate_state = ROTATE_Z;
				if(fabs(hit_point.z + 0.3f) < 0.15f) rotate_layer = LAYER_ONE;
				if(fabs(hit_point.z) < 0.15f) rotate_layer = LAYER_TWO;
				if(fabs(hit_point.z - 0.3f) < 0.15f) rotate_layer = LAYER_THREE;
			}
		}
	}
	else
	if(fabs(hit_point.z - 0.45) < 1e-5){ // Hit Front face
		xoffset = glm::dot(glm::normalize(glm::vec2(0, 1.0f)), mouse_offset);
		yoffset = glm::dot(glm::normalize(glm::vec2(1.5f, 1.0f)), mouse_offset);
		if(rotate_state == ROTATE_NONE){
			if(fabs(xoffset) > fabs(yoffset)){ 
				// Rotate along x-axis
				rotate_state = ROTATE_X;
				if(fabs(hit_point.x + 0.3f) < 0.15f) rotate_layer = LAYER_ONE;
				if(fabs(hit_point.x) < 0.15f) rotate_layer = LAYER_TWO;
				if(fabs(hit_point.x - 0.3f) < 0.15f) rotate_layer = LAYER_THREE;
			}
			else{ 
				// Rotate along y-axis
				rotate_state = ROTATE_Y;
				if(fabs(hit_point.y + 0.3f) < 0.15f) rotate_layer = LAYER_ONE;
				if(fabs(hit_point.y) < 0.15f) rotate_layer = LAYER_TWO;
				if(fabs(hit_point.y - 0.3f) < 0.15f) rotate_layer = LAYER_THREE;
			}
		}
	}
	else{
		std::cerr << "Fatal error. Ray doesn't hit any surface." << std::endl;
	}
	switch(rotate_state){
		case ROTATE_X:
			rotate_angle += velocity * xoffset / (SCR_HEIGHT + SCR_WIDTH);
		break;
		case ROTATE_Y:
			rotate_angle += velocity * yoffset / (SCR_HEIGHT + SCR_WIDTH);
		break;
		case ROTATE_Z:
			rotate_angle += velocity * zoffset / (SCR_HEIGHT + SCR_WIDTH);
		break;
		default:
			std::cerr << "A fatal error occurred. rotate_state == ROTATE_NONE." << std::endl;;
		break;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){	
	if(!mouse_pressed) return;
	
	glm::vec2 offset(xpos - press_xpos, ypos - press_ypos);
	if(rotate_mode == ROTATE_GLOBAL) global_rotate(offset);
	else if(rotate_mode == ROTATE_LOCAL) local_rotate(offset, rec);
	else std::cerr << "Fatal error. Unknown value " << rotate_mode << " for rotate_mode." << std::endl;

	press_xpos = xpos;
	press_ypos = ypos;
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset){
	cam.onZooming(y_offset);
}
