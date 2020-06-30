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
#include <iostream>
#include <string>
#include <math.h>

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	// COMPAT profile contains a vertex array object by default
	// where as CORE profile doesn't
	// we create our own vertex array if we use core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
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

	float vertices2[] = {
			0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
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

	// second triangle
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	// for color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// create shaders
	unsigned int shaderProgram = glCreateProgram();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* src = "#version 330\n"
			"layout(location=0) in vec3 position;\n"
			"layout(location=1) in vec3 aColor;\n"
			"out vec3 outColor;\n"
			"void main(void)\n"
			"{\n"
			"gl_Position = vec4(position, 1.0);\n"
			"outColor = aColor;\n"
			"}\n";

	glShaderSource(vertexShader, 1, &src, nullptr);
	glCompileShader(vertexShader);

	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int length;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
		char* info = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(vertexShader, sizeof(info), NULL, info);
		std::cout << "Error: could not compile vertex shader: " << info << std::endl;
		return -1;
	}


	src = "#version 330\n"
			"out vec4 color;\n"
			"uniform vec4 ourColor;\n"
			"void main()\n"
			"{\n"
			"color = ourColor;\n"
			"}\n";

	glShaderSource(fragmentShader, 1, &src, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int length;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
		char* info = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(fragmentShader, sizeof(info), NULL, info);
		std::cout << "Error: Could not compile fragment shader: " << info << std::endl;
		return -1;
	}

	// Yellow
	unsigned int shaderProgramYellow = glCreateProgram();
	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);

	src = "#version 330\n"
			"out vec4 color;\n"
			"in vec3 outColor;\n"
			"void main()\n"
			"{\n"
			"color = vec4(outColor, 1.0);\n"
			"}\n";
	glShaderSource(fragmentShaderYellow, 1, &src, nullptr);
	glCompileShader(fragmentShaderYellow);

	glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int length;
		glGetShaderiv(fragmentShaderYellow, GL_INFO_LOG_LENGTH, &length);
		char* info = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(fragmentShaderYellow, sizeof(info), NULL, info);
		std::cout << "Error: Could not compile fragment shader yellow: " << info << std::endl;
		return -1;
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		int length;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
		char* info = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(shaderProgram, sizeof(info), NULL, info);
		std::cout << "could not link shader program" << std::endl;
		return -1;
	}

	glValidateProgram(shaderProgram);

	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
	if (!success)
	{
		int length;
		glGetProgramiv(shaderProgramYellow, GL_INFO_LOG_LENGTH, &length);
		char* info = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(shaderProgramYellow, sizeof(info), NULL, info);
		std::cout << "Could not link shader program yellow: " << info << std::endl;
		return -1;
	}

	glValidateProgram(shaderProgramYellow);

	// delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShaderYellow);

	int colorLocation = glGetUniformLocation(shaderProgram, "ourColor");


	// enable vertex array we want to draw
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float color = 0;
	while(!glfwWindowShouldClose(window))
	{
		// set the color in the buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function
		glClear(GL_COLOR_BUFFER_BIT); // state using function

		glUseProgram(shaderProgram);
		glUniform4f(colorLocation, 0.0, sin(color), 0.0, 1.0);
		color += 0.001;
		glBindVertexArray(vao[0]);
		// draw the elements in currently enabled vertex array
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgramYellow);
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(0);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgramYellow);
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
	glfwTerminate();
	return 0;
}

