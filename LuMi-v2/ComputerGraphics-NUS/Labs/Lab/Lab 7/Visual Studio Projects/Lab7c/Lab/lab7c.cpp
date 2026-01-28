#include "utilities.h"
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
GLuint gVBO = 0;		// vertex buffer object identifier
GLuint gVAO = 0;		// vertex array object identifier

glm::mat4 gViewMatrix;			// view matrix
glm::mat4 gProjectionMatrix;	// projection matrix
glm::mat4 gModelMatrix;			// object matrix

Light gLight;					// light properties
Material gMaterial;				// material properties
std::map<std::string, Texture> gTexture;	// texture objects

// controls
bool gWireframe = false;	// wireframe control

// function initialise scene and render settings
static void init(GLFWwindow* window)
{
	// set the color the color buffer should be cleared to
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// compile and link a vertex and fragment shader pair
	gShader.compileAndLink("normalMap.vert", "normalMap.frag");

	// initialise view matrix
	gViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// initialise projection matrix
	gProjectionMatrix = glm::perspective(glm::radians(45.0f), 
		static_cast<float>(gWindowWidth) / gWindowHeight, 0.1f, 10.0f);

	// initialise point light properties
	gLight.pos = glm::vec3(-0.5f, 1.0f, 1.0f);
	gLight.La = glm::vec3(0.3f);
	gLight.Ld = glm::vec3(1.0f);
	gLight.Ls = glm::vec3(1.0f);
	gLight.att = glm::vec3(1.0f, 0.0f, 0.0f);

	// initialise material properties
	gMaterial.Ka = glm::vec3(0.2f);
	gMaterial.Kd = glm::vec3(0.2f, 0.7f, 1.0f);
	gMaterial.Ks = glm::vec3(0.2f, 0.7f, 1.0f);
	gMaterial.shininess = 40.0f;

	// initialise model matrices
	gModelMatrix = glm::mat4(1.0f);

	// load texture and normal map
	gTexture["Stone"].generate("./images/Fieldstone.bmp");
	gTexture["StoneNormalMap"].generate("./images/FieldstoneBumpDOT3.bmp");

	// vertex positions, normals, tangents and texture coordinates
	std::vector<GLfloat> vertices =
	{
		-1.0f, -1.0f, 0.0f,	// vertex 0: position
		0.0f, 0.0f, 1.0f,	// vertex 0: normal
		1.0f, 0.0f, 0.0f,	// vertex 0: tangent
		0.0f, 0.0f,			// vertex 0: texture coordinate
		1.0f, -1.0f, 0.0f,	// vertex 1: position
		0.0f, 0.0f, 1.0f,	// vertex 1: normal
		1.0f, 0.0f, 0.0f,	// vertex 1: tangent
		1.0f, 0.0f,			// vertex 1: texture coordinate
		-1.0f, 1.0f, 0.0f,	// vertex 2: position
		0.0f, 0.0f, 1.0f,	// vertex 2: normal
		1.0f, 0.0f, 0.0f,	// vertex 2: tangent
		0.0f, 1.0f,			// vertex 2: texture coordinate
		1.0f, 1.0f, 0.0f,	// vertex 3: position
		0.0f, 0.0f, 1.0f,	// vertex 3: normal
		1.0f, 0.0f, 0.0f,	// vertex 3: tangent
		1.0f, 1.0f,			// vertex 3: texture coordinate
	};

	// create VBO
	glGenBuffers(1, &gVBO);					// generate unused VBO identifier
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// create VAO, specify VBO data and format of the data
	glGenVertexArrays(1, &gVAO);			// generate unused VAO identifier
	glBindVertexArray(gVAO);				// create VAO
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);	// bind the VBO

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTanTex),
		reinterpret_cast<void*>(offsetof(VertexNormTanTex, position)));		// specify format of position data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTanTex),
		reinterpret_cast<void*>(offsetof(VertexNormTanTex, normal)));		// specify format of colour data
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTanTex),
		reinterpret_cast<void*>(offsetof(VertexNormTanTex, tangent)));		// specify format of tangent data
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormTanTex),
		reinterpret_cast<void*>(offsetof(VertexNormTanTex, texCoord)));		// specify format of texture coordinate data

	glEnableVertexAttribArray(0);	// enable vertex attributes
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}

// function used to update the scene
static void update_scene(GLFWwindow* window)
{
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
	gShader.setUniform("uViewpoint", glm::vec3(0.0f, 0.0f, 4.0f));

	// calculate matrices
	glm::mat4 MVP = gProjectionMatrix * gViewMatrix * gModelMatrix;
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(gModelMatrix)));

	// set uniform variables
	gShader.setUniform("uModelViewProjectionMatrix", MVP);
	gShader.setUniform("uModelMatrix", gModelMatrix);
	gShader.setUniform("uNormalMatrix", normalMatrix);

	// set texture and normal map
	gShader.setUniform("uTextureSampler", 0);
	gShader.setUniform("uNormalSampler", 1);

	glActiveTexture(GL_TEXTURE0);
	gTexture["Stone"].bind();

	glActiveTexture(GL_TEXTURE1);
	gTexture["StoneNormalMap"].bind();

	glBindVertexArray(gVAO);				// make VAO active
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// render the vertices

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

	TwDefine(" Main label='User Interface' refresh=0.02 text=light size='250 350' position='10 10' ");

	// create frame stat entries
	TwAddVarRO(twBar, "Frame Rate", TW_TYPE_FLOAT, &gFrameRate, " group='Frame Stats' precision=2 ");
	TwAddVarRO(twBar, "Frame Time", TW_TYPE_FLOAT, &gFrameTime, " group='Frame Stats' ");

	// scene controls
	TwAddVarRW(twBar, "Wireframe", TW_TYPE_BOOLCPP, &gWireframe, " group='Controls' ");

	// light control
	TwAddVarRW(twBar, "Position X", TW_TYPE_FLOAT, &gLight.pos.x, " group='Light' min=-3 max=3 step=0.01 ");
	TwAddVarRW(twBar, "Position Y", TW_TYPE_FLOAT, &gLight.pos.y, " group='Light' min=-3 max=3 step=0.01 ");
	TwAddVarRW(twBar, "Position Z", TW_TYPE_FLOAT, &gLight.pos.z, " group='Light' min=-3 max=3 step=0.01 ");

	// material controls
	TwAddVarRW(twBar, "Ka", TW_TYPE_COLOR3F, &gMaterial.Ka, " group='Material' ");
	TwAddVarRW(twBar, "Kd", TW_TYPE_COLOR3F, &gMaterial.Kd, " group='Material' ");
	TwAddVarRW(twBar, "Ks", TW_TYPE_COLOR3F, &gMaterial.Ks, " group='Material' ");
	TwAddVarRW(twBar, "Shininess", TW_TYPE_FLOAT, &gMaterial.shininess, " group='Material' min=1 max=255 step=1 ");

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