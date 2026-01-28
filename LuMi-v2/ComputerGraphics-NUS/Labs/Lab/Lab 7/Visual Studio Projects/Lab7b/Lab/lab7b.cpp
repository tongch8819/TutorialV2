#include "utilities.h"
#include "SimpleModel.h"
#include "Texture.h"

// global variables
// settings
unsigned int gWindowWidth = 800;
unsigned int gWindowHeight = 600;

// frame stats
float gFrameRate = 60.0f;
float gFrameTime = 1 / gFrameRate;

// scene content
ShaderProgram gShader;	// shader program object

glm::mat4 gViewMatrix;			// view matrix
glm::mat4 gProjectionMatrix;	// projection matrix
glm::mat4 gModelMatrix;			// object matrix

Light gLight;					// light properties
Material gMaterial;				// material properties
SimpleModel gModel;				// object model
std::map<std::string, Texture> gTextures;	// texture object

// controls
bool gWireframe = false;	// wireframe control
float gFactor = 0.5f;
float gRotationY = 0.0f;

// function initialise scene and render settings
static void init(GLFWwindow* window)
{
	// set the color the color buffer should be cleared to
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// compile and link a vertex and fragment shader pair
	gShader.compileAndLink("lightingAndTexture.vert", "pointLightTexture.frag");

	// initialise view matrix
	gViewMatrix = glm::lookAt(glm::vec3(0.0f, 2.0f, 4.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// initialise projection matrix
	gProjectionMatrix = glm::perspective(glm::radians(45.0f), 
		static_cast<float>(gWindowWidth) / gWindowHeight, 0.1f, 10.0f);

	// initialise model matrices
	gModelMatrix = glm::mat4(1.0f);

	// initialise point light properties
	gLight.pos = glm::vec3(2.0f, 2.0f, 3.0f);
	gLight.La = glm::vec3(0.3f);
	gLight.Ld = glm::vec3(1.0f);
	gLight.Ls = glm::vec3(1.0f);
	gLight.att = glm::vec3(1.0f, 0.0f, 0.0f);

	// initialise material properties
	gMaterial.Ka = glm::vec3(0.25f, 0.21f, 0.21f);
	gMaterial.Kd = glm::vec3(1.0f, 0.83f, 0.83f);
	gMaterial.Ks = glm::vec3(0.3f, 0.3f, 0.3f);
	gMaterial.shininess = 11.3f;

	// load textures
	gTextures["Check"].generate("./images/check.bmp");
	gTextures["Smile"].generate("./images/smile.bmp");

	// load model
	gModel.loadModel("./models/cube.obj", true);
}

// function used to update the scene
static void update_scene(GLFWwindow* window)
{
	gModelMatrix = glm::rotate(glm::radians(gRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
}

// function to render the scene
static void render_scene()
{
	// clear colour buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the shaders associated with the shader program
	gShader.use();

	// set light properties
	gShader.setUniform("uLight.pos", gLight.pos);
	gShader.setUniform("uLight.La", gLight.La);
	gShader.setUniform("uLight.Ld", gLight.Ld);
	gShader.setUniform("uLight.Ls", gLight.Ls);
	gShader.setUniform("uLight.att", gLight.att);

	// set material properties
	gShader.setUniform("uMaterial.Ka", gMaterial.Ka);
	gShader.setUniform("uMaterial.Kd", gMaterial.Kd);
	gShader.setUniform("uMaterial.Ks", gMaterial.Ks);
	gShader.setUniform("uMaterial.shininess", gMaterial.shininess);

	// set viewing position
	gShader.setUniform("uViewpoint", glm::vec3(0.0f, 2.0f, 4.0f));

	// calculate matrices
	glm::mat4 MVP = gProjectionMatrix * gViewMatrix * gModelMatrix;
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(gModelMatrix)));

	// set uniform variables
	gShader.setUniform("uModelViewProjectionMatrix", MVP);
	gShader.setUniform("uModelMatrix", gModelMatrix);
	gShader.setUniform("uNormalMatrix", normalMatrix);

	// set textures
	gShader.setUniform("uTextureSampler", 0);
	gShader.setUniform("uTextureSampler2", 1);
	gShader.setUniform("uFactor", gFactor);

	glActiveTexture(GL_TEXTURE0);
	gTextures["Check"].bind();

	glActiveTexture(GL_TEXTURE1);
	gTextures["Smile"].bind();

	// draw model
	gModel.drawModel();

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

	TwDefine(" Main label='User Interface' refresh=0.02 text=light size='250 320' position='10 10' ");

	// create frame stat entries
	TwAddVarRO(twBar, "Frame Rate", TW_TYPE_FLOAT, &gFrameRate, " group='Frame Stats' precision=2 ");
	TwAddVarRO(twBar, "Frame Time", TW_TYPE_FLOAT, &gFrameTime, " group='Frame Stats' ");

	// scene controls
	TwAddVarRW(twBar, "Wireframe", TW_TYPE_BOOLCPP, &gWireframe, " group='Controls' ");

	// light controls
	TwAddVarRW(twBar, "Position X", TW_TYPE_FLOAT, &gLight.pos.x, " group='Light' min=-3.0 max=3.0 step=0.01 ");
	TwAddVarRW(twBar, "Position Y", TW_TYPE_FLOAT, &gLight.pos.y, " group='Light' min=-3.0 max=3.0 step=0.01 ");
	TwAddVarRW(twBar, "Position Z", TW_TYPE_FLOAT, &gLight.pos.z, " group='Light' min=-3.0 max=3.0 step=0.01 ");

	// texture blending
	TwAddVarRW(twBar, "Blend", TW_TYPE_FLOAT, &gFactor, " group='Texture' min=0.0 max=1.0 step=0.01 ");

	// rotation
	TwAddVarRW(twBar, "RotationY", TW_TYPE_FLOAT, &gRotationY, " group='Transformation' min=-180 max=180 step=1 ");

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

	// timing data
	double lastUpdateTime = glfwGetTime();	// last update time
	double elapsedTime = lastUpdateTime;	// time since last update
	int frameCount = 0;						// number of frames since last update

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(window);	// update the scene

		// if wireframe set polygon render mode to wireframe
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		render_scene();			// render the scene

		// set polygon render mode to fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		TwDraw();				// draw tweak bar

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events

		frameCount++;
		elapsedTime = glfwGetTime() - lastUpdateTime;	// time since last update

		// if elapsed time since last update > 1 second
		if (elapsedTime > 1.0)
		{
			gFrameTime = elapsedTime / frameCount;	// average time per frame
			gFrameRate = 1 / gFrameTime;			// frames per second
			lastUpdateTime = glfwGetTime();			// set last update time to current time
			frameCount = 0;							// reset frame counter
		}
	}

	// uninitialise tweak bar
	TwDeleteBar(tweakBar);
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}