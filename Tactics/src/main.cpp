#pragma once
//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//#include ""

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

	float positions[] = { -0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.0f
	};

	unsigned int buffer1;
	glGenBuffers(1, &buffer1);
	glBindBuffer(GL_ARRAY_BUFFER, buffer1);
	//specify how big the buffer is
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/*
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.0f);
		glEnd();
		*/
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}