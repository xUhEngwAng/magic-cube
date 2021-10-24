#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "magic_cube.h"
#include "cube.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void loadTextures(GLuint n, const std::string* paths, GLuint* textures);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int NUM_TEXTURES = 4;

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
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// load and configure cube
	Cube cube(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_0, FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_3);
	// load shader programs
	// --------------------
	Shader shader("./shader/vertex.glsl", "./shader/fragment.glsl");
	shader.Use();
	// configure MVP matrices
	// ----------------------
	glm::mat4 model(1.0f), view(1.0f), perspective(1.0f);
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0, 1.0f, 0));
	view = glm::lookAt(glm::vec3(0, 1.0f, 3.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1.0f, 0));
	perspective = glm::perspective(glm::radians(45.0f), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("perspective", perspective);
	// load textures
	// -------------
	GLuint* textures = new GLuint[NUM_TEXTURES];
	std::string texPaths[NUM_TEXTURES] = {"./images/black.png", "./images/green.png", "./images/orange.png", "./images/skyblue.png"};
	loadTextures(NUM_TEXTURES, texPaths, textures);
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

		cube.draw(textures);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFWresources.
	//---------------------------------------------------------------
	cube.finishDrawing();
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

/*
 * Load Texture images in batch mode.
 * Note that the images should be in .png format.
 * 
 * @param n: # textures to be loaded
 * @param paths: corresponding texture paths
 * @param textures: pre-allocated array to hold the resulting texture handlers
 */
void loadTextures(GLuint n, const std::string* paths, GLuint* textures){
	for(int ix = 0; ix != n; ++ix){
		glGenTextures(1, textures + ix);
		// glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[ix]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		int width, height, nrchannels;
		unsigned char *data = stbi_load(paths[ix].c_str(), &width, &height, &nrchannels, 0);
		if(!data){
			std::cerr << "Failed to load texture image " << paths[ix] << std::endl;
		}
		else{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);
	}
}
