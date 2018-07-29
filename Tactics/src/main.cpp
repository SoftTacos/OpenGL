#pragma once
//#define GLEW_STATIC

#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//#include ""

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	//todo: check if string is valid
	const char* src = source.c_str();//&source[0]//make sure source code is still in scope
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {//GL_FALSE is 0, but oh well
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));//TODO refresh stack vs heap in c++
		glGetShaderInfoLog(id, length, &length, message);//reread docs
		std::cout << "Failed to compile shader: \n" << message << std::endl;//probably should add shader type later
		return 0;
	}
	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
	//for simplicity's sake, just gonna write the shader in line, will write shader externally later
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);//TODO:re-read documentation on these two
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
    GLFWwindow* window;

	/* Initialize the library */
    if (!glfwInit())
        return -1;

	/* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 960, "", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	if (GLEW_OK != glewInit())
		std::cout << "ERR";

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = { 
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f, 0.5f
	};

	unsigned int buffer1;
	glGenBuffers(1, &buffer1);
	glBindBuffer(GL_ARRAY_BUFFER, buffer1);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	//http://docs.gl/gl4/glVertexAttribPointer
	//stride is number of bytes between the vertices, going to have to know how many bytes are in each vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);
	
	//shader stuff
	std::string vsString =
		"#version 330 core\n"//probably use 450 later
		"\n"
		"layout(location = 0) in vec4 position;\n"//the 0 should match up with the 0 in vertexAttribPointer
		"void main(){\n"
		"	gl_Position = position;\n"
		"}\n";
	std::string fsString =
		"#version 330 core\n"//probably use 450 later
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"void main(){\n"
		"	color = vec4(1.0, 0.1, 0.1, 1.0);\n"//RGBA
		"}\n";
	unsigned int shader1 = CreateShader(vsString,fsString);
	glUseProgram(shader1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 3, ) //leaving this hook to work with and learn later
		//vertex in openGL can contain way more than just position data, position, texture coords, normals, etc.
		

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	glDeleteProgram(shader1);

    glfwTerminate();
    return 0;
}