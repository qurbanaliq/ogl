/*
 * app.cpp
 *
 *  Created on: Jun 24, 2020
 *      Author: 123
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	 // COMPAT profile contains a vertex array object by default
	 // where as CORE profile doesn't
	 // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	 // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	 // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


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
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 0.0f, 1.0f, 0.0f
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	unsigned int shaderProgram = glCreateProgram();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vs = "#version 330\n"
			"layout(location=0) in vec3 position;\n"
			"void main(void)\n"
			"{\n"
			"gl_Position = vec4(0.5 * position, 1.0);\n"
			"}\n";

	const char* src;
	src = vs.c_str();
	glShaderSource(vertexShader, 1, &src, nullptr);
	glCompileShader(vertexShader);

	std::string fs = "#version 330\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"color = vec4(0.3, 0.2, 0.5, 1.0);\n"
			"}\n";

	src = fs.c_str();
	glShaderSource(fragmentShader, 1, &src, nullptr);
	glCompileShader(fragmentShader);

	//TODO: validate shader here

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);
	glUseProgram(shaderProgram);

	std::cout << glGetString(GL_VERSION) << std::endl;

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_POINTS, 0, 1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

