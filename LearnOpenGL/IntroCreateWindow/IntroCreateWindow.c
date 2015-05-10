#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint InstallShader(GLenum shaderType, const GLchar * const source)
{
    GLuint shaderId;
    GLint success;
    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &source, NULL);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar msg[512];
        glGetShaderInfoLog(shaderId, 512, NULL, msg);
        printf("Shader compilation failed. Msg: \"%s\".");
        exit(1);
    }

    return shaderId;
}

GLuint InstallShaders()
{
    GLint success;

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    vertexShader = InstallShader(GL_VERTEX_SHADER,
        "#version 330 core\n"
        "layout(location = 0) in vec3 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\n");

    fragmentShader = InstallShader(GL_FRAGMENT_SHADER,
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n");

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar msg[512];
        glGetProgramInfoLog(program, 512, NULL, msg);
        printf("Shader compilation failed. Msg: \"%s\".");
        exit(1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

GLuint GenerateVAO()
{
    GLfloat vertices[][3] = {
        { 0.5f, 0.5f, 0.0f }, // Top Right
        { 0.5f, -0.5f, 0.0f }, // Bottom Right
        { -0.5f, -0.5f, 0.0f }, // Bottom Left
        { -0.5f, 0.5f, 0.0f }, // Top Left
    };

    GLuint indices[][3] = {
        { 0, 1, 3 },
        { 1, 2, 3 }
    };

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Don not unbind GL_ELEMENT_ARRAY_BUFFER as the VAO remembers the call
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vao;         
}

int main()
{
	GLFWwindow* window;
    GLuint program;
    GLuint vao;
	
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

    program = InstallShaders();
    vao = GenerateVAO();

	glViewport( 0, 0, 800, 600 );
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glfwSetKeyCallback( window, &KeyCallback );

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


		glfwSwapBuffers(window);
	}

	glfwTerminate();

    return 0;
}