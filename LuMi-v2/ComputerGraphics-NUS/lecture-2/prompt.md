# First Round 

I am working on the assignment for undergraduate computer graphics homework. 
My development environment is windows 11 and visual studio. 
We use UI library: AntTweakBar.dll.
For the OpenGL library, we use glew32.dll and glfw3.dll.
We also use the file assimp-vc142-mt.dll, which is a dynamic link library (DLL) file associated with the Open Asset Import Library (Assimp), a popular library used by developers to import 3D model formats into various applications.

Here is a code demo for assignment 1, you learn its library usage and OpenGL version to write the correct version of future assignment.
```cpp
// include C++ headers
#include <cstdio>
#include <iostream>
#include <vector>

// include OpenGL related headers
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Shader
#include "ShaderProgram.h"

// Global variables for window
unsigned int gWindowWidth = 800;
unsigned int gWindowHeight = 800;

// Shader set up
ShaderProgram gShader;

GLuint vaoGround, vboGround;
GLuint vaoTruck, vboTruck;
GLuint vaoWheel, vboWheel;

// Global variables
float gGroundSlope = 0.0f;
float gTruckPosition = 0.0f;
float gWheelRotation = 0.0f;

bool gWireframeMode = false;
float gBackgroundColor[3] = { 0.2f, 0.2f, 0.2f };
float gFPS = 0.0f;
float gFrameTimeMs = 0.0f;

TwBar* gBar;

// --- Initialize scene ---
void initScene() {
    // Use shader program
    gShader.compileAndLink("window.vert", "window.frag");

    // Ground vertices
    float groundVertices[] = {
        -1.0f, -1.0f, 0.0f, 0.7f, 0.0f, 1.0f, -1.0f, 0.0f, 0.7f, 0.0f, -1.0f, -0.5f, 0.0f, 0.5f, 0.0f,
         1.0f, -1.0f, 0.0f, 0.7f, 0.0f, 1.0f, -0.5f, 0.0f, 0.5f, 0.0f, -1.0f, -0.5f, 0.0f, 0.5f, 0.0f
    };
    glGenVertexArrays(1, &vaoGround); glGenBuffers(1, &vboGround);
    glBindVertexArray(vaoGround); glBindBuffer(GL_ARRAY_BUFFER, vboGround);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))); glEnableVertexAttribArray(1);

    // Truck vertices
    float truckVertices[] = {
        -0.5f, -0.38f, 0.5f, 0.5f, 0.5f, 0.4f, -0.38f, 0.5f, 0.5f, 0.5f, -0.5f, -0.25f, 0.5f, 0.5f, 0.5f,
         0.4f, -0.38f, 0.5f, 0.5f, 0.5f, 0.4f, -0.25f, 0.5f, 0.5f, 0.5f, -0.5f, -0.25f, 0.5f, 0.5f, 0.5f,
         -0.4f, -0.25f, 0.75f, 0.25f, 0.0f, -0.22f,-0.25f, 1.00f, 0.00f, 0.0f, -0.4f,  0.20f, 0.25f, 0.75f, 0.0f,
         -0.22f,-0.25f, 1.00f, 0.00f, 0.0f, -0.22f, 0.20f, 0.50f, 0.50f, 0.0f, -0.4f,  0.20f, 0.25f, 0.75f, 0.0f,
         -0.6f, -0.25f, 0.50f, 0.50f, 0.0f, -0.4f, -0.25f, 0.75f, 0.25f, 0.0f, -0.4f,  0.20f, 0.25f, 0.75f, 0.0f,
         -0.6f, -0.25f, 0.50f, 0.50f, 0.0f, -0.5f,  0.20f, 0.00f, 1.00f, 0.0f, -0.4f,  0.20f, 0.25f, 0.75f, 0.0f,
         -0.18f,-0.25f, 0.5f, 0.0f, 0.5f,  0.4f, -0.25f, 0.0f, 0.0f, 1.0f, -0.18f, 0.20f, 1.0f, 0.0f, 0.0f,
          0.4f, -0.25f, 0.0f, 0.0f, 1.0f,  0.4f,  0.20f, 0.5f, 0.0f, 0.5f, -0.18f, 0.20f, 1.0f, 0.0f, 0.0f,
          0.4f, -0.25f, 0.0f, 0.0f, 1.0f,  0.6f,  0.0f,  0.1f, 0.0f, 0.9f,  0.4f,  0.20f, 0.5f, 0.0f, 0.5f,
          -0.52f, -0.05f, 0.3f, 0.3f, 0.3f, -0.25f, -0.05f, 0.3f, 0.3f, 0.3f, -0.45f, 0.15f, 0.3f, 0.3f, 0.3f,
          -0.25f, -0.05f, 0.3f, 0.3f, 0.3f, -0.25f,  0.15f, 0.3f, 0.3f, 0.3f, -0.45f, 0.15f, 0.3f, 0.3f, 0.3f
    };
    glGenVertexArrays(1, &vaoTruck); glGenBuffers(1, &vboTruck);
    glBindVertexArray(vaoTruck); glBindBuffer(GL_ARRAY_BUFFER, vboTruck);
    glBufferData(GL_ARRAY_BUFFER, sizeof(truckVertices), truckVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))); glEnableVertexAttribArray(1);

    // Truck wheel data
    std::vector<float> wheelData;
    auto createWheel = [&](float cx, float cy, float r, float r_c, float g_c, float b_c) {
        wheelData.push_back(cx); wheelData.push_back(cy);
        wheelData.push_back(r_c); wheelData.push_back(g_c); wheelData.push_back(b_c);
        for (int i = 0; i <= 30; i++) {
            float th = 2.0f * 3.14159f * i / 30.0f;
            wheelData.push_back(cx + r * cosf(th)); wheelData.push_back(cy + r * sinf(th));
            wheelData.push_back(r_c); wheelData.push_back(g_c); wheelData.push_back(b_c);
        }
        };
    createWheel(-0.35f, -0.38f, 0.12f, 0.1f, 0.1f, 0.1f); createWheel(-0.35f, -0.38f, 0.06f, 0.7f, 0.7f, 0.7f);
    createWheel(0.25f, -0.38f, 0.12f, 0.1f, 0.1f, 0.1f);  createWheel(0.25f, -0.38f, 0.06f, 0.7f, 0.7f, 0.7f);
    glGenVertexArrays(1, &vaoWheel); glGenBuffers(1, &vboWheel);
    glBindVertexArray(vaoWheel); glBindBuffer(GL_ARRAY_BUFFER, vboWheel);
    glBufferData(GL_ARRAY_BUFFER, wheelData.size() * sizeof(float), wheelData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))); glEnableVertexAttribArray(1);
}

void updateFPS() {
    static double lastTime = glfwGetTime();
    static int frames = 0;
    double currentTime = glfwGetTime();
    frames++;
    if (currentTime - lastTime >= 1.0) {
        gFPS = (float)frames;
        gFrameTimeMs = 1000.0f / gFPS;
        frames = 0;
        lastTime = currentTime;
    }
}

void processInput(GLFWwindow* window, float deltaTime) {
    float slopeSpeed = 20.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) gGroundSlope += slopeSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) gGroundSlope -= slopeSpeed;
    if (gGroundSlope > 15.0f) gGroundSlope = 15.0f;
    if (gGroundSlope < -15.0f) gGroundSlope = -15.0f;

    float moveSpeed = 1.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        gTruckPosition += moveSpeed;
        gWheelRotation -= (moveSpeed * (360.0f / (2.0f * 3.14159f * 0.12f)));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        gTruckPosition -= moveSpeed;
        gWheelRotation += (moveSpeed * (360.0f / (2.0f * 3.14159f * 0.12f)));
    }
}

// GLFW settings
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) { TwEventMouseButtonGLFW(button, action); }
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) { TwEventMousePosGLFW((int)xpos, (int)ypos); }
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) { TwEventMouseWheelGLFW((int)yoffset); }

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }
    TwEventKeyGLFW(key, action);
}

void charCallback(GLFWwindow* window, unsigned int codepoint) { TwEventCharGLFW(codepoint, GLFW_PRESS); }

int main(void) {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Name and ID", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewInit();

    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(gWindowWidth, gWindowHeight);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, charCallback);

    gBar = TwNewBar("Interface");
    TwDefine(" GLOBAL fontsize=3 ");
    TwDefine(" TW_HELP visible=false ");
    // AntTweakBar UI settings
    TwDefine(" Interface label='User Interface' text=light size='250 400' alpha=200 refresh=0.02 ");

    TwAddVarRO(gBar, "FPS", TW_TYPE_FLOAT, &gFPS, " group='Frame Stats' precision=1 ");
    TwAddVarRO(gBar, "Frame Time (ms)", TW_TYPE_FLOAT, &gFrameTimeMs, " group='Frame Stats' precision=3 ");
    TwAddVarRW(gBar, "Wireframe", TW_TYPE_BOOL32, &gWireframeMode, " group='Display' ");
    TwAddVarRW(gBar, "BG Color", TW_TYPE_COLOR3F, &gBackgroundColor, " group='Display' label='Background Color' ");
    TwAddVarRW(gBar, "Ground Slope", TW_TYPE_FLOAT, &gGroundSlope, " group='Controls' min=-15 max=15 step=0.1 ");
    TwAddVarRO(gBar, "Position", TW_TYPE_FLOAT, &gTruckPosition, " group='Controls' precision=3 ");

    TwDefine(" Interface/'Display' opened=true ");
    TwDefine(" Interface/'BG Color' opened=true ");

    initScene();

    double lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        updateFPS();
        processInput(window, deltaTime);

        glClearColor(gBackgroundColor[0], gBackgroundColor[1], gBackgroundColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (gWireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        gShader.use(); // User shader

        // 1. Ground
        glm::mat4 sceneMat = glm::translate(glm::vec3(1.0f, -0.5f, 0.0f)) * glm::rotate(glm::radians(-gGroundSlope), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::vec3(-1.0f, 0.5f, 0.0f));
        gShader.setUniform("transform", sceneMat);
        glBindVertexArray(vaoGround);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 2. Truck
        glm::mat4 truckMat = sceneMat * glm::translate(glm::vec3(gTruckPosition, 0.0f, 0.0f));
        gShader.setUniform("transform", truckMat);
        glBindVertexArray(vaoTruck);
        glDrawArrays(GL_TRIANGLES, 0, 33);

        // 3. Wheels
        glBindVertexArray(vaoWheel);
        auto drawW = [&](float x, float y, int offset) {
            glm::mat4 m = truckMat * glm::translate(glm::vec3(x, y, 0.0f)) * glm::rotate(glm::radians(gWheelRotation), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::vec3(-x, -y, 0.0f));
            gShader.setUniform("transform", m);
            glDrawArrays(GL_TRIANGLE_FAN, offset, 32);
            glDrawArrays(GL_TRIANGLE_FAN, offset + 32, 32);
            };
        drawW(-0.35f, -0.38f, 0); drawW(0.25f, -0.38f, 64);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        TwDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    TwTerminate();
    glfwTerminate();
    return 0;
}
```


# Second 

Attached file `Assignment2.pdf` is the requirements for assignment 2. Let's analyze its requirements, what we could borrow from assignment 1 into this new assignment 2, and summarize the content.


# Thrid

Attached zip file `DemoCode.zip` is a lab demo for texture parameters. I know it is somehow related to our assignment 2. But I don't know the details. Please investigate and elaborate.