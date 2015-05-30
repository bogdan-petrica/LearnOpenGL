#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "Shader.h"

const double PI = std::atan(1.0) * 4;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
}    

int main()
{
	GLFWwindow* window;
	
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		printf( "Failed to create GLFW window\n" );
		glfwTerminate();
		return -1;
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf( "Failed to initialize GLEW\n" );
		return -1;
	}

	glViewport( 0, 0, 800, 600 );

	glfwSetKeyCallback( window, &key_callback );

	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top Right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // Bottom Left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f   // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	Shader sh;
	if (!sh.load("data/vert.sh", "data/frag.sh"))
	{
		return 0;
	}

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		bool odd = ((int)timeValue % 2) == 1;
		timeValue = timeValue - std::floor(timeValue);
		float y = sin(timeValue * 2 * PI) / 2.0f;
		timeValue -= 0.5;
		if (odd)
		{ 
			timeValue *= -1.0f;
		}

		sh.setUniform2f("offset", timeValue, y);
		sh.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

    return 0;
}