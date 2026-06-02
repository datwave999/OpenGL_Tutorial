#include "Window.h"

Window::Window()
	: mainWindow(nullptr),
	width(800), height(600),
	bufferWidth(0), bufferHeight(0),
	xChange(0.0f), yChange(0.0f),
	mouseFirstMoved(true)
{
	for (size_t i = 0; i < 1024; i++) keys[i] = 0;
}

Window::Window(GLint windowWidth, GLint windowHeight)
	: mainWindow(nullptr),
	  width(windowWidth), height(windowHeight),
	  bufferWidth(0), bufferHeight(0),
	xChange(0.0f), yChange(0.0f),
	mouseFirstMoved(true)
{
	for (size_t i = 0; i < 1024; i++) keys[i] = 0;
}

int Window::Initialise() {
	//Initialise GLFW & Error handleing if failed
	if (!glfwInit()) {
		printf("GLFW initialization failed!\n");
		glfwTerminate();
		return 1;
	}

	//Setup GLFW window properties
	//OpenGL version 3.3 with modern profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow Forward Compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Creating window using a function and error handleing
	//function takes width, height, name, monitor, shared across systems 
	mainWindow = glfwCreateWindow(width, height, "OpenGL Graphics Engine", NULL, NULL);
	if (mainWindow == nullptr) {
		printf("GLFW window creation failed!\n");
		glfwTerminate();
		return 1;
	}

	//This function gets the buffer info and puts it into the respective variables
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Turning off V-Sync
	glfwSwapInterval(0);

	//handle key and mouse inputs
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Allow modern extension features (Not used in this course)
	glewExperimental = GL_TRUE;

	//Initialize glew, error handleing, destroy both window and terminate glfw if error
	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed!\n");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//Setup Viewport size (top left corner to bottom right corner)
	//glfw gave us the actual buffer width and height so we use it here
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);


	//Enabling depth test
	glEnable(GL_DEPTH_TEST);

	return 0;
}

void Window::createCallbacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange() {
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange() {
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}


void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double x, double y) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	GLfloat xPos = static_cast<GLfloat>(x);
	GLfloat yPos = static_cast<GLfloat>(y);

	if (theWindow->mouseFirstMoved) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

}

void Window::ChangeTitle(const char* title) {
	glfwSetWindowTitle(mainWindow, title);
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}