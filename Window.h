#pragma once

#include<stdio.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>


class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat getBufferWidth() { return (GLfloat)bufferWidth; }
	GLfloat getBufferHeight() { return (GLfloat)bufferHeight;  }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }
	void ChangeTitle(const char* title);

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double x, double y);
};

