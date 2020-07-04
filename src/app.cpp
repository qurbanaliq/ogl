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

void processInput(GLFWwindow* window);

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

	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew Error" << std::endl;
		return -1;
	}

	float vertices[]  = {
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.0f, 0.0f,
			 0.0f, -0.5f, 0.0f
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

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
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("rc/images/container.jpg", &width, &height, &nrChannels, 0);

	// generate the texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the wrapping options (on the currently bound texture)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//float colorr[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colorr);

	// set the minifying and magnifying options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// free the buffer
	stbi_image_free(data);

	// 2nd texture
	data = stbi_load("rc/images/awesomeface.png", &width, &height, &nrChannels, 0);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);


	// bind textures to texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// create shaders
	//Shader shader("rc/shaders/v.shader", "rc/shaders/f.shader");
	Shader shader2("rc/shaders/v.shader", "rc/shaders/f2.shader");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float visibility = 0.2;

	shader2.use();
	shader2.setUniform1i("texture1", 0);
	shader2.setUniform1i("texture2", 1);
	glBindVertexArray(vao[1]);

	while(!glfwWindowShouldClose(window))
	{
		processInput(window);
		// set the color in the buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function
		glClear(GL_COLOR_BUFFER_BIT); // state using function

		shader2.setUniform1f("visibility", visibility);

		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			visibility += 0.001;
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			visibility -= 0.001;

		if (visibility < 0) visibility = 0;
		if (visibility > 1) visibility = 1;

		//glUseProgram(shaderProgram);
		//glUniform4f(colorLocation, 0.0, sin(color), 0.0, 1.0);
//		shader.use();
//		shader.setUniform4f("ourColor", 0.0, sin(color), 0.0, 1.0);

//		glBindVertexArray(vao[0]);
		// draw the elements in currently enabled vertex array
//		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glUseProgram(shaderProgramYellow);
//		shader2.use();
//		//shader2.setUniform1f("offset", 0.0);
//		//glBindTexture(GL_TEXTURE_2D, texture);
//		shader2.setUniform1i("texture1", 0);
//		shader2.setUniform1i("texture2", 1);
//		glBindVertexArray(vao[1]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
