#pragma once
//#define GLEW_STATIC

#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "headers/GameMap.h"
#include "headers/Tile.h"

std::string LoadTextFile(std::string filepath) {
	std::ifstream stream(filepath);
	if (!stream) {
		std::cout << "Unable to open file";
		exit(1); // terminate with error
	}
	std::string content((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	stream.close();
	return content;
}

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type = ShaderType::NONE;
	
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("#shader_vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("#shader_fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}

	return {ss[0].str(), ss[1].str() };
}

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
	//just doing object testing here as I add new stuff
	GameMap testMap = GameMap();
	Tile testTile = Tile();
	//want to read in a map file, using rapidxml
	
	std::string tileTypes = LoadTextFile("res/types/tileTypes");

	std::cout << tileTypes << std::endl;
	
	GLFWwindow* window;

	/* Initialize the library */
    if (!glfwInit())
        return -1;

	/* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "", NULL, NULL);
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
		-1.0f, -1.0f,//-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		
//		0.5f, 0.5f,
//		-0.5f, 0.5f,
		-0.5f, 0.5f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int buffer1;
	glGenBuffers(1, &buffer1);
	glBindBuffer(GL_ARRAY_BUFFER, buffer1);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	//http://docs.gl/gl4/glVertexAttribPointer
	//stride is number of bytes between the vertices, going to have to know how many bytes are in each vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);
	
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//shader stuff
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shader1 = CreateShader(source.VertexSource,source.FragmentSource);
	glUseProgram(shader1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 6);//used this without index buffering
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);//&ibo);
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