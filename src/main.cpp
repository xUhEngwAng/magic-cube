#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shader.h"
#include "magic_cube.h"
#include "cube.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int NUM_TEXTURES = 7;

MagicCube magicCube;

const glm::vec2 scr_axis[3] = {glm::normalize(glm::vec2(-1.2f, 1.0f)),
							   glm::vec2(1.0f, 0), 
							   glm::normalize(glm::vec2(-1.2f, -1.0f))};
enum {ROTATE_X, ROTATE_Y, ROTATE_Z, ROTATE_NONE} rotate_state;

float fov = 45.0;
double press_xpos, press_ypos;
bool mouse_pressed;

glm::vec3 rotate_axis = glm::vec3(0, 1.0f, 0);
float rotate_angle = 0;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
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
	glm::mat4 model(1.0f), view(1.0f), perspective(1.0f);
	view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0, 0, 0), glm::vec3(0, 2.0f, 0));
	shader.setMat4("view", view);
	// --------------
	glEnable(GL_DEPTH_TEST);
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
		model = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_angle), rotate_axis);
		perspective = glm::perspective(glm::radians(fov), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("perspective", perspective);
		
		magicCube.draw(shader, model);

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
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	double x_offset, y_offset;
	if(button == GLFW_MOUSE_BUTTON_LEFT){
		if(action == GLFW_PRESS){
			glfwGetCursorPos(window, &press_xpos, &press_ypos);
			mouse_pressed = true;
		}
		if(action == GLFW_RELEASE){
			mouse_pressed = false;
			int num_rotates = rotate_angle / 90.0;
			if(fabs(rotate_angle - 90.0 * num_rotates) > 45.0){
				if(rotate_angle < 0) num_rotates -= 1;
				else num_rotates += 1;
			}
			magicCube.rotate(rotate_axis, num_rotates * 90.0);
			rotate_angle = 0;
			rotate_state = ROTATE_NONE;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	const double velocity = 250.0;
	
	if(mouse_pressed){
		glm::vec2 offset(xpos - press_xpos, ypos - press_ypos);
		float xoffset = glm::dot(scr_axis[0], offset);
		float yoffset = glm::dot(scr_axis[1], offset);
		float zoffset = glm::dot(scr_axis[2], offset);
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
				rotate_axis = glm::vec3(1.0f, 0, 0);
			break;
			case ROTATE_Y:
				rotate_angle += velocity * yoffset / (SCR_HEIGHT + SCR_WIDTH);
				rotate_axis = glm::vec3(0, 1.0f, 0);
			break;
			case ROTATE_Z:
				rotate_angle += velocity * zoffset / (SCR_HEIGHT + SCR_WIDTH);
				rotate_axis = glm::vec3(0, 0, 1.0f);
			break;
			default:
				std::cerr << "A fatal error occurred. rotate_state == ROTATE_NONE.";
			break;
		}

		press_xpos = xpos;
		press_ypos = ypos;
	}
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset){
	fov -= y_offset;
	if(fov > 75.0) fov = 75.0;
	if(fov < 10.0) fov = 10.0;
}
