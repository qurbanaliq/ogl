/*
 * app.cpp
 *
 *  Created on: Jun 24, 2020
 *  Author: 123
 *
 *	This if OpenGL exercise
 *  OpenGL is a state machine
 *  some functions set the state and others use that state
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stbImage/stbImage.h>
#include <iostream>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"

const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
bool firstMouse = true;
float lastX = SCR_WIDTH/2.0f, lastY = SCR_HEIGHT/2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(UPWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.processKeyboard(DOWNWARD, deltaTime);

}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos; lastY = yPos;
		firstMouse = false;
	}
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos; lastY = yPos;

	camera.processMouseMovement(xOffset, yOffset);
}

float fov = 45.0f;

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll((float)yOffset);
}

int main(void)
{
	if (!glfwInit())
		return -1;

	// COMPAT profile contains a vertex array object by default
	// where as CORE profile doesn't
	// we create our own vertex array if we use core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 640, 480);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew Error" << std::endl;
		return -1;
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	// TODO: what is glfwSwapInterval, fps? 1 = 60fps, 5 = 12fps
	glfwSwapInterval(2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);



	float vertices[] = {
	    // positions          // normals           // texture coords
	    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};


	// create and bind vertex array, it will contain all the subsequent
	// vertex and index buffers
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// defines the layout of the data and assign index zero to this data
	// divides data into vertices, each of 3 floats.
	// total 4 vertices in this case indexed 0 to 4 to use in index buffer
	glEnableVertexAttribArray(0);
	// stride can also be zero since we have tightly packed data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

	// unbind everything (don't unbind buffers before vertex array)
	// we can now bind only vertex array when we need to draw this data
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// creating a texture
	//load the texture
	Texture texture1("rc/images/container2.png"),
			texture2("rc/images/container2_specular.png"),
			texture3("rc/images/matrix.jpg");
	texture1.bind(0);
	texture2.bind(1);
	texture3.bind(2);
	// create shaders
	//Shader shader("rc/shaders/v.shader", "rc/shaders/f.shader");
	Shader shader("rc/shaders/v.shader", "rc/shaders/f2.shader");
	Shader lightShader("rc/shaders/v.shader", "rc/shaders/f.shader");

	glm::mat4 transform(1.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::vec3 lightPos(0.2f, 0.0f, 2.0f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	// object shader
	shader.use();
	shader.setUniform1i("material.diffuse", 0);
	shader.setUniform1i("material.specular", 1);
	shader.setUniform1i("material.emission", 2);
	shader.setUniform1f("material.shininess", 64.0f);

	// light shader
	lightShader.use();
	lightShader.setUniformVec3("lightColor", lightColor);

	glm::vec3 cubePositions[] = {
	    glm::vec3( 0.0f,  0.0f,  0.0f),
	    glm::vec3( 2.0f,  5.0f, -15.0f),
	    glm::vec3(-1.5f, -2.2f, -2.5f),
	    glm::vec3(-3.8f, -2.0f, -12.3f),
	    glm::vec3( 2.4f, -0.4f, -3.5f),
	    glm::vec3(-1.7f,  3.0f, -7.5f),
	    glm::vec3( 1.3f, -2.0f, -2.5f),
	    glm::vec3( 1.5f,  2.0f, -2.5f),
	    glm::vec3( 1.5f,  0.2f, -1.5f),
	    glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3( 0.7f,  0.2f,  2.0f),
		glm::vec3( 2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3( 0.0f,  0.0f, -3.0f)
	};

	float currentTime;

	glBindVertexArray(vao);

	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 640.0f/480.0f, 0.1f, 100.0f);

		glEnable(GL_DEPTH_TEST);
		// set the color in the buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state using function
		float radius = 5.0f;
		shader.use();
		//transform the object cube
		shader.setUniformMat4("view", view);
		shader.setUniformMat4("projection", projection);
		shader.setUniformVec3("viewPos", camera.getPosition());
		for (int i = 0; i<4; i++)
		{
			std::stringstream num;
			num << i;
			shader.setUniformVec3(("pointLights[" + num.str() + "].position"), pointLightPositions[i]);
			shader.setUniform1f("pointLights[" + num.str() + "].constant", 1.0f);
			shader.setUniform1f("pointLights[" + num.str() + "].linear", 0.09f);
			shader.setUniform1f("pointLights[" + num.str() + "].quadratic", 0.032f);
			shader.setUniformVec3("pointLights[" + num.str() + "].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			shader.setUniformVec3("pointLights[" + num.str() + "].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
			shader.setUniformVec3("pointLights[" + num.str() + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}
		shader.setUniformVec3("dirLight.direction", glm::vec3(-0.2f, 1.0f, -0.3f));
		shader.setUniformVec3("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setUniformVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.setUniformVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		for (int i=1; i<=10; i++)
		{
			glm::mat4 model = glm::translate(transform, cubePositions[i-1]);
			model = glm::rotate(model, glm::radians(i * 20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setUniformMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightShader.use();
		//transform the light cube
		for (int i = 0; i<4; i++)
		{
			glm::mat4 lightModel = glm::translate(transform, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.1f));
			shader.setUniformMat4("model", lightModel);
			shader.setUniformMat4("view", view);
			shader.setUniformMat4("projection", projection);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(0);
//	glDeleteProgram(shaderProgram);
//	glDeleteProgram(shaderProgramYellow);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return 0;
}
