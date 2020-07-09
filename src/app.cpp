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
#include <math.h>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"

void processInput(GLFWwindow* window);

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	float vertices[] = {
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

//	float vertices[]  = {
//			-0.5f, -0.5f, 0.0f,
//			-0.5f, 0.0f, 0.0f,
//			 0.0f, -0.5f, 0.0f
//	};

	// create and bind vertex array, it will contain all the subsequent
	// vertex and index buffers
	unsigned int vao[2], vbo[2];
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// defines the layout of the data and assign index zero to this data
	// divides data into vertices, each of 3 floats.
	// total 4 vertices in this case indexed 0 to 4 to use in index buffer
	glEnableVertexAttribArray(0);
	// stride can also be zero since we have tightly packed data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));

	// unbind everything (don't unbind buffers before vertex array)
	// we can now bind only vertex array when we need to draw this data
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	float vertices2[] = {
			//positions			 //colors			//tex coords
			-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f
	};

	unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
	};

	unsigned int ibo;
	glGenBuffers(1, &ibo);

	// second triangle
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	// for color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(6 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// creating a texture

	//load the texture
	Texture texture1("rc/images/container.jpg"),
			texture2("rc/images/awesomeface.png");

	// create shaders
	//Shader shader("rc/shaders/v.shader", "rc/shaders/f.shader");
	Shader shader2("rc/shaders/v.shader", "rc/shaders/f2.shader");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float visibility = 0.2;

	shader2.use();
	shader2.setUniform1i("texture1", 0);
	shader2.setUniform1i("texture2", 1);
	shader2.setUniform1f("visibility", visibility);

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

	texture1.bind(0);
	texture2.bind(1);

	// TODO: what is glfwSwapInterval, fps? 1 = 60fps, 5 = 12fps
	glfwSwapInterval(2);

	int j = 20;

	glBindVertexArray(vao[0]);

	glm::mat4 transform(1.0f);

	//glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -100.0f, 100.0f);

	float viewX = 0.0f, viewY = 0.0f, viewZ = 0.0f;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float radius = 10.0f;
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			radius += 0.1;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			radius -= 0.1;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			viewX -= 0.1;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			viewX += 0.1;

		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f/480.0f, 0.1f, 100.0f);

		glEnable(GL_DEPTH_TEST);
		// set the color in the buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state using function

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::translate(transform, cubePositions[i]);
			model = glm::translate(model, glm::vec3(viewX, viewY, viewZ));
			model = glm::rotate(model, glm::radians((float)j * (i+1)), glm::vec3(1.0f, 0.3f, 0.5f));
			glm::mat4 finalTransform = projection * view * model;
			shader2.setUniformMat4fv("transform", finalTransform);
	//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		j++;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(0);
//	glDeleteProgram(shaderProgram);
//	glDeleteProgram(shaderProgramYellow);
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
