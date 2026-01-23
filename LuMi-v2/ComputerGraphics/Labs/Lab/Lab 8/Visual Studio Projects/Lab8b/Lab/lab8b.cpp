#include "utilities.h"

// global variables
// settings
unsigned int gWindowWidth = 800;
unsigned int gWindowHeight = 800;

// scene content
ShaderProgram gShader;	// shader program object
GLuint gVBO = 0;		// vertex buffer object identifier
GLuint gVAO = 0;		// vertex array object identifier

std::map<std::string, glm::mat4> gModelMatrix;	// object model matrices
glm::mat4 gViewMatrix;			// view matrix
glm::mat4 gProjectionMatrix;	// projection matrix

// controls
bool gBlend = true;
bool gDepth = true;
bool gOrder = true;
float gAlpha = 0.5f;
float gRotation = 90.0f;
float gTranslateZ = 0.1f;
glm::vec3 gBackgroundColour = glm::vec3(0.2f);

// function initialise scene and render settings
static void init(GLFWwindow* window)
{
	// set blending equation
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// compile and link a vertex and fragment shader pair
	gShader.compileAndLink("alphaBlending.vert", "alphaBlending.frag");

	// initialise view matrix
	gViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// initialise projection matrix
	gProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 10.0f);

	// initialise model matrix to the identity matrix
	gModelMatrix["Left"] = glm::mat4(1.0f);
	gModelMatrix["Right"] = glm::mat4(1.0f);

	// vertex positions and colours
	std::vector<GLfloat> vertices = {
		-0.5f, -0.5f, 0.0f,	// vertex 0: position
		1.0f, 0.0f, 0.0f,	// vertex 0: colour
		0.5f, -0.5f, 0.0f,	// vertex 1: position
		0.0f, 1.0f, 0.0f,	// vertex 1: colour
		-0.5f, 0.5f, 0.0f,	// vertex 2: position
		0.0f, 0.0f, 1.0f,	// vertex 2: colour
		0.5f, 0.5f, 0.0f,	// vertex 3: position
		0.0f, 1.0f, 1.0f,	// vertex 3: colour
	};

	// create VBO and buffer the data
	glGenBuffers(1, &gVBO);					// generate unused VBO identifier
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);	// bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// create VAO, specify VBO data and format of the data
	glGenVertexArrays(1, &gVAO);			// generate unused VAO identifier
	glBindVertexArray(gVAO);				// create VAO
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);	// bind the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor),
		reinterpret_cast<void*>(offsetof(VertexColor, position)));	// specify format of position data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor),
		reinterpret_cast<void*>(offsetof(VertexColor, color)));		// specify format of colour data

	glEnableVertexAttribArray(0);	// enable vertex attributes
	glEnableVertexAttribArray(1);
}

// function used to update the scene
static void update_scene()
{
	// set clear background colour
	glClearColor(gBackgroundColour[0], gBackgroundColour[1], gBackgroundColour[2], 1.0f);

	gModelMatrix["Right"] = glm::translate(glm::vec3(0.3f, 0.3f, gTranslateZ))
		* glm::rotate(glm::radians(gRotation), glm::vec3(0.0f, 0.0f, 1.0f));
}

// function to render the scene
static void render_scene()
{
	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gShader.use();						// use the shaders associated with the shader program

	glBindVertexArray(gVAO);			// make VAO active
	glm::mat4 MVP;

	if (gOrder)
	{
		// set uniforms
		MVP = gProjectionMatrix * gViewMatrix * gModelMatrix["Left"];
		gShader.setUniform("uModelViewProjectionMatrix", MVP);
		gShader.setUniform("uAlpha", 0.5f);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// render the vertices

		// set uniforms
		MVP = gProjectionMatrix * gViewMatrix * gModelMatrix["Right"];
		gShader.setUniform("uModelViewProjectionMatrix", MVP);
		gShader.setUniform("uAlpha", gAlpha);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// render the vertices
	}
	else
	{
		// set uniforms
		MVP = gProjectionMatrix * gViewMatrix * gModelMatrix["Right"];
		gShader.setUniform("uModelViewProjectionMatrix", MVP);
		gShader.setUniform("uAlpha", gAlpha);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// render the vertices

		// set uniforms
		MVP = gProjectionMatrix * gViewMatrix * gModelMatrix["Left"];
		gShader.setUniform("uModelViewProjectionMatrix", MVP);
		gShader.setUniform("uAlpha", 0.5f);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// render the vertices
	}


	// flush the graphics pipeline
	glFlush();
}

// key press or release callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// close the window when the ESCAPE key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		// set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

// mouse movement callback function
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// pass cursor position to tweak bar
	TwEventMousePosGLFW(static_cast<int>(xpos), static_cast<int>(ypos));
}

// mouse button callback function
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// pass mouse button status to tweak bar
	TwEventMouseButtonGLFW(button, action);
}

// error callback function
static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;	// output error description
}

// create and populate tweak bar elements
TwBar* create_UI(const std::string name)
{
	// create a tweak bar
	TwBar* twBar = TwNewBar(name.c_str());

	// give tweak bar the size of graphics window
	TwWindowSize(gWindowWidth, gWindowHeight);
	TwDefine(" TW_HELP visible=false ");	// disable help menu
	TwDefine(" GLOBAL fontsize=3 ");		// set large font size

	TwDefine(" Main label='User Interface' refresh=0.02 text=light size='250 300' ");

	// scene controls
	TwAddVarRW(twBar, "Blend", TW_TYPE_BOOLCPP, &gBlend, " group='Controls' ");
	TwAddVarRW(twBar, "Depth", TW_TYPE_BOOLCPP, &gDepth, " group='Controls' ");
	TwAddVarRW(twBar, "Order", TW_TYPE_BOOLCPP, &gOrder, " group='Controls' ");

	TwAddVarRW(twBar, "Alpha", TW_TYPE_FLOAT, &gAlpha, " group='Blending' min=0.0 max=1.0 step=0.01 ");

	TwAddVarRW(twBar, "Rotation", TW_TYPE_FLOAT, &gRotation, " group='Transformation' min=-180.0 max=180.0 step=1.0 ");
	TwAddVarRW(twBar, "TranslateZ", TW_TYPE_FLOAT, &gTranslateZ, " group='Transformation' min=-0.1 max=0.1 step=0.2 ");

	TwAddVarRW(twBar, "Colour", TW_TYPE_COLOR3F, &gBackgroundColour[0], " group='Background' opened=true");

	return twBar;
}

int main(void)
{
	GLFWwindow* window = nullptr;	// GLFW window handle

	glfwSetErrorCallback(error_callback);	// set GLFW error callback function

	// initialise GLFW
	if (!glfwInit())
	{
		// if failed to initialise GLFW
		exit(EXIT_FAILURE);
	}

	// minimum OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window and its OpenGL context
	window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Lab", nullptr, nullptr);

	// check if window created successfully
	if (window == nullptr)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);	// set window context as the current context
	glfwSwapInterval(1);			// swap buffer interval

	// initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		// if failed to initialise GLEW
		std::cerr << "GLEW initialisation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// set GLFW callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// initialise scene and render settings
	init(window);

	// initialise AntTweakBar
	TwInit(TW_OPENGL_CORE, nullptr);
	TwBar* tweakBar = create_UI("Main");		// create and populate tweak bar elements

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene();		// update the scene

		if (gBlend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		if (gDepth)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		render_scene();		// render the scene

		TwDraw();			// draw tweak bar

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events
	}

	// clean up
	glDeleteBuffers(1, &gVBO);
	glDeleteVertexArrays(1, &gVAO);

	// uninitialise tweak bar
	TwDeleteBar(tweakBar);
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}