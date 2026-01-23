#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>

// include OpenGL related headers
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>



// --- Global Variables ---
unsigned int gWindowWidth = 800;  // Requirement: 800 width
unsigned int gWindowHeight = 800; // Requirement: 800 height

// Shader Program ID
GLuint shaderProgram;

// VAOs and VBOs for different objects
GLuint vaoGround, vboGround;
GLuint vaoTruck, vboTruck;
GLuint vaoWheel, vboWheel;

// --- Shader Source Code (Modern OpenGL Requirement) ---
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform mat4 transform;\n" // <--- New Uniform Variable
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

// --- Helper: Compile Shaders ---
void setupShaders() {
    // 1. Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // (Check errors omitted for brevity, but recommended)

    // 2. Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 3. Link Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 4. Delete shaders as they are linked now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// --- Helper: Generate Circle Vertices for Wheels ---
void createWheelData(std::vector<float>& vertices, float cx, float cy, float r, float r_color, float g_color, float b_color) {
    // Center point
    vertices.push_back(cx); vertices.push_back(cy);
    vertices.push_back(r_color); vertices.push_back(g_color); vertices.push_back(b_color);

    int segments = 30;
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        
        vertices.push_back(cx + x);
        vertices.push_back(cy + y);
        vertices.push_back(r_color); vertices.push_back(g_color); vertices.push_back(b_color);
    }
}

// --- Global Transformation Variables ---
float gGroundSlope = 0.0f;       // Angle in degrees
float gTruckPosition = 0.0f;     // X offset
float gWheelRotation = 0.0f;     // Wheel angle in degrees
float gLastFrameTime = 0.0f;     // For Frame Time calculation

// --- Matrix Math Helper ---
struct Matrix4 {
    float m[16];

    // Identity Matrix
    static Matrix4 Identity() {
        Matrix4 mat;
        for (int i = 0; i < 16; i++) mat.m[i] = 0.0f;
        mat.m[0] = 1.0f; mat.m[5] = 1.0f; mat.m[10] = 1.0f; mat.m[15] = 1.0f;
        return mat;
    }

    // Translation Matrix
    static Matrix4 Translate(float x, float y, float z) {
        Matrix4 mat = Identity();
        mat.m[12] = x; mat.m[13] = y; mat.m[14] = z;
        return mat;
    }

    // Rotation Z Matrix (for 2D rotation)
    static Matrix4 RotateZ(float angleDegrees) {
        Matrix4 mat = Identity();
        float rad = angleDegrees * 3.14159265f / 180.0f;
        mat.m[0] = cos(rad);  mat.m[1] = sin(rad);
        mat.m[4] = -sin(rad); mat.m[5] = cos(rad);
        return mat;
    }

    // Matrix Multiplication (A * B)
    Matrix4 operator*(const Matrix4& other) {
        Matrix4 result;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                result.m[col * 4 + row] = 0.0f;
                for (int k = 0; k < 4; k++) {
                    result.m[col * 4 + row] += this->m[k * 4 + row] * other.m[col * 4 + k];
                }
            }
        }
        return result;
    }
};

// Helper to send matrix to shader
void setUniformMatrix(GLuint shader, const char* name, Matrix4 mat) {
    unsigned int loc = glGetUniformLocation(shader, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, mat.m);
}

// --- GUI State Variables ---
bool gWireframeMode = false;
float gBackgroundColor[3] = { 0.2f, 0.2f, 0.2f }; // Default Dark Grey

void render_gui()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create the "Interface" window
    ImGui::Begin("Interface"); 

    // --- Section 1: Frame Stats [cite: 11, 12] ---
    if (ImGui::CollapsingHeader("Frame Stats", ImGuiTreeNodeFlags_DefaultOpen)) {
        // Calculate Frame Rate (FPS)
        float fps = ImGui::GetIO().Framerate;
        // Display Frame Rate
        ImGui::Text("Frame Rate: %.1f", fps);
        // Display Frame Time (in ms) -> 1000.0f / fps
        ImGui::Text("Frame Time: %.4f", 1000.0f / fps);
    }

    // --- Section 2: Display [cite: 13] ---
    if (ImGui::CollapsingHeader("Display", ImGuiTreeNodeFlags_DefaultOpen)) {
        // Wireframe Toggle Button [cite: 14]
        // If clicked, toggle the boolean and set OpenGL mode
        if (ImGui::Checkbox("Wireframe", &gWireframeMode)) {
            if (gWireframeMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    // --- Section 3: Background [cite: 15] ---
    if (ImGui::CollapsingHeader("Background", ImGuiTreeNodeFlags_DefaultOpen)) {
        // Color Picker for Background [cite: 88]
        // Updates the gBackgroundColor array directly
        ImGui::ColorEdit3("Color", gBackgroundColor);
        // Apply the new color immediately
        glClearColor(gBackgroundColor[0], gBackgroundColor[1], gBackgroundColor[2], 1.0f);
    }

    // --- Section 4: Controls [cite: 23] ---
    if (ImGui::CollapsingHeader("Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
        // Ground Slope Slider [cite: 89]
        // Allows mouse control to change gGroundSlope between -15 and 15
        ImGui::SliderFloat("Ground Slope", &gGroundSlope, -15.0f, 15.0f);

        // Position Display [cite: 25, 90]
        // Read-only display of the truck's X position
        ImGui::Text("Position: %.3f", gTruckPosition);
    }

    ImGui::End();

    // Render the GUI data
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// --- Initialization ---
static void init(GLFWwindow* window)
{
    setupShaders();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Dark grey background

    // --- 1. Ground Data (Green) ---
    // Ground is a rectangle from y=-1.0 to y=-0.5
    float groundVertices[] = {
        -1.0f, -1.0f,  0.0f, 0.7f, 0.0f, 
         1.0f, -1.0f,  0.0f, 0.7f, 0.0f, 
        -1.0f, -0.5f,  0.0f, 0.5f, 0.0f, 
        
         1.0f, -1.0f,  0.0f, 0.7f, 0.0f, 
         1.0f, -0.5f,  0.0f, 0.5f, 0.0f, 
        -1.0f, -0.5f,  0.0f, 0.5f, 0.0f, 
    };

    glGenVertexArrays(1, &vaoGround);
    glGenBuffers(1, &vboGround);
    glBindVertexArray(vaoGround);
    glBindBuffer(GL_ARRAY_BUFFER, vboGround);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // --- 2. Truck Body Data ---
    // Constructed using GL_TRIANGLES for maximum compatibility
    // --- 2. Truck Body Data ---
    float truckVertices[] = {
        // --- CHASSIS (Grey Bar) ---
        // Stays the same. This will now be visible in the gap!
        -0.5f, -0.38f,  0.5f, 0.5f, 0.5f, 
         0.4f, -0.38f,  0.5f, 0.5f, 0.5f, 
        -0.5f, -0.25f,  0.5f, 0.5f, 0.5f, 

         0.4f, -0.38f,  0.5f, 0.5f, 0.5f, 
         0.4f, -0.25f,  0.5f, 0.5f, 0.5f, 
        -0.5f, -0.25f,  0.5f, 0.5f, 0.5f, 

        // --- HEAD (Cab) - Modified to create GAP ---
        // Right edge moved from -0.2 to -0.22
        
        // Rectangular part
        -0.4f, -0.25f,  0.5f, 0.5f, 0.0f, 
        -0.22f,-0.25f,  1.0f, 0.0f, 0.0f, // Changed -0.2 to -0.22
        -0.4f,  0.20f,  0.5f, 0.5f, 0.0f, 

        -0.22f,-0.25f,  1.0f, 0.0f, 0.0f, // Changed -0.2 to -0.22
        -0.22f, 0.20f,  1.0f, 0.0f, 0.0f, // Changed -0.2 to -0.22
        -0.4f,  0.20f,  0.5f, 0.5f, 0.0f, 

        // Nose part (Stays the same)
        -0.6f, -0.25f,  0.0f, 1.0f, 0.0f, 
        -0.4f, -0.25f,  0.5f, 0.5f, 0.0f, 
        -0.4f,  0.20f,  0.5f, 0.5f, 0.0f, 

        -0.6f, -0.25f,  0.0f, 1.0f, 0.0f, 
        -0.5f,  0.20f,  0.0f, 1.0f, 0.0f, 
        -0.4f,  0.20f,  0.5f, 0.5f, 0.0f, 

        // --- BODY (Tray) - Modified to create GAP ---
        // Left edge moved from -0.2 to -0.18
        
        // Central Rectangle
        -0.18f,-0.25f,  1.0f, 0.0f, 0.0f, // Changed -0.2 to -0.18
         0.4f, -0.25f,  0.5f, 0.0f, 0.5f, 
        -0.18f, 0.20f,  1.0f, 0.0f, 0.0f, // Changed -0.2 to -0.18

         0.4f, -0.25f,  0.5f, 0.0f, 0.5f, 
         0.4f,  0.20f,  0.5f, 0.0f, 0.5f, 
        -0.18f, 0.20f,  1.0f, 0.0f, 0.0f, // Changed -0.2 to -0.18

        // Hexagon Back Tip (Stays the same)
         0.4f, -0.25f,   0.5f, 0.0f, 0.5f, 
         0.6f,  0.0f,    0.0f, 0.0f, 1.0f, 
         0.4f,  0.20f,   0.5f, 0.0f, 0.5f, 

        // --- WINDOW (Stays the same) ---
        -0.52f, -0.05f, 0.3f, 0.3f, 0.3f, 
        -0.25f, -0.05f, 0.3f, 0.3f, 0.3f, 
        -0.45f,  0.15f, 0.3f, 0.3f, 0.3f, 

        -0.25f, -0.05f, 0.3f, 0.3f, 0.3f, 
        -0.25f,  0.15f, 0.3f, 0.3f, 0.3f, 
        -0.45f,  0.15f, 0.3f, 0.3f, 0.3f, 
    };

    glGenVertexArrays(1, &vaoTruck);
    glGenBuffers(1, &vboTruck);
    glBindVertexArray(vaoTruck);
    glBindBuffer(GL_ARRAY_BUFFER, vboTruck);
    glBufferData(GL_ARRAY_BUFFER, sizeof(truckVertices), truckVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // --- 3. Wheel Data ---
    // Center Y = -0.38. Radius = 0.12.
    // Result: Top = -0.26, Bottom = -0.50 (Touches ground perfectly)
    std::vector<float> wheelData;
    // Left Wheel
    createWheelData(wheelData, -0.35f, -0.38f, 0.12f, 0.1f, 0.1f, 0.1f); 
    createWheelData(wheelData, -0.35f, -0.38f, 0.06f, 0.7f, 0.7f, 0.7f); 
    // Right Wheel
    createWheelData(wheelData, 0.25f, -0.38f, 0.12f, 0.1f, 0.1f, 0.1f);  
    createWheelData(wheelData, 0.25f, -0.38f, 0.06f, 0.7f, 0.7f, 0.7f);  

    glGenVertexArrays(1, &vaoWheel);
    glGenBuffers(1, &vboWheel);
    glBindVertexArray(vaoWheel);
    glBindBuffer(GL_ARRAY_BUFFER, vboWheel);
    glBufferData(GL_ARRAY_BUFFER, wheelData.size() * sizeof(float), wheelData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void processInput(GLFWwindow* window) {
    // Calculate Frame Time 
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - gLastFrameTime;
    gLastFrameTime = currentFrame;

    // --- Ground Slope (Up/Down Arrows) ---
    // Limit between -15 and 15 degrees [cite: 44]
    float slopeSpeed = 20.0f * deltaTime; // Speed of tilting
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        gGroundSlope += slopeSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        gGroundSlope -= slopeSpeed;

    // Clamp slope
    if (gGroundSlope > 15.0f) gGroundSlope = 15.0f;
    if (gGroundSlope < -15.0f) gGroundSlope = -15.0f;

    // --- Truck Movement (Left/Right Arrows) --- [cite: 42]
    float moveSpeed = 1.0f * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        gTruckPosition += moveSpeed;
        // Rotate wheels: Angle = distance / radius. Radius is 0.12.
        // We convert radians to degrees for our matrix function.
        float circumference = 2.0f * 3.14159f * 0.12f;
        float degreesPerUnit = 360.0f / circumference;
        gWheelRotation -= (moveSpeed * degreesPerUnit); // Move Right -> Rotate Clockwise (Negative)
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        gTruckPosition -= moveSpeed;
        float circumference = 2.0f * 3.14159f * 0.12f;
        float degreesPerUnit = 360.0f / circumference;
        gWheelRotation += (moveSpeed * degreesPerUnit); // Move Left -> Rotate Counter-Clockwise
    }
}

static void render_scene(GLFWwindow* window)
{
    // Process Inputs first
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);

    // --- 1. Calculate SCENE Matrix (Ground Rotation) ---
    // Pivot Point is (1.0, -0.5) 
    // Logic: Translate to Origin -> Rotate -> Translate back to Pivot
    Matrix4 pivotTrans = Matrix4::Translate(1.0f, -0.5f, 0.0f);
    Matrix4 pivotInv = Matrix4::Translate(-1.0f, 0.5f, 0.0f);
    Matrix4 groundRot = Matrix4::RotateZ(gGroundSlope);
    
    // M_Scene = Pivot * Rotation * InvPivot
    Matrix4 sceneMatrix = pivotTrans * groundRot * pivotInv;

    // --- Draw Ground ---
    // Ground uses the Scene Matrix directly
    setUniformMatrix(shaderProgram, "transform", sceneMatrix);
    glBindVertexArray(vaoGround);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // --- 2. Calculate TRUCK Matrix ---
    // Truck moves along X, but must stay aligned with the rotated ground.
    // M_Truck = M_Scene * Translate(TruckPos)
    Matrix4 truckMove = Matrix4::Translate(gTruckPosition, 0.0f, 0.0f);
    Matrix4 truckMatrix = sceneMatrix * truckMove;

    // --- Draw Truck Body ---
    setUniformMatrix(shaderProgram, "transform", truckMatrix);
    glBindVertexArray(vaoTruck);
    glDrawArrays(GL_TRIANGLES, 0, 33);

    // --- 3. Draw Wheels ---
    // Wheels rotate locally. 
    // Left Wheel Center: (-0.35, -0.38)
    // Right Wheel Center: (0.25, -0.38)
    // Formula: M_Final = M_Truck * (ToCenter * Rot * ToOrigin)

    Matrix4 wheelRotMat = Matrix4::RotateZ(gWheelRotation);

    glBindVertexArray(vaoWheel);
    int fanSize = 32;

    // -- Left Wheel --
    float lwX = -0.35f; float lwY = -0.38f;
    Matrix4 lwToCenter = Matrix4::Translate(lwX, lwY, 0.0f);
    Matrix4 lwToOrigin = Matrix4::Translate(-lwX, -lwY, 0.0f);
    // Combine: TruckTransform * (MoveToSpot * Spin * MoveFromSpot)
    Matrix4 leftWheelMat = truckMatrix * (lwToCenter * wheelRotMat * lwToOrigin);
    
    setUniformMatrix(shaderProgram, "transform", leftWheelMat);
    // Draw Tire (0) and Hubcap (1)
    glDrawArrays(GL_TRIANGLE_FAN, 0, fanSize);       
    glDrawArrays(GL_TRIANGLE_FAN, fanSize, fanSize); 

    // -- Right Wheel --
    float rwX = 0.25f; float rwY = -0.38f;
    Matrix4 rwToCenter = Matrix4::Translate(rwX, rwY, 0.0f);
    Matrix4 rwToOrigin = Matrix4::Translate(-rwX, -rwY, 0.0f);
    Matrix4 rightWheelMat = truckMatrix * (rwToCenter * wheelRotMat * rwToOrigin);

    setUniformMatrix(shaderProgram, "transform", rightWheelMat);
    // Draw Tire (2) and Hubcap (3)
    glDrawArrays(GL_TRIANGLE_FAN, fanSize * 2, fanSize); 
    glDrawArrays(GL_TRIANGLE_FAN, fanSize * 3, fanSize); 

    glFlush();
}

static void error_callback(int error, const char* description)
{
    std::cerr << description << std::endl;
}

static void render_scene_content() {
    // 1. Calculate Matrices (Same as Part 2)
    Matrix4 pivotTrans = Matrix4::Translate(1.0f, -0.5f, 0.0f);
    Matrix4 pivotInv = Matrix4::Translate(-1.0f, 0.5f, 0.0f);
    Matrix4 groundRot = Matrix4::RotateZ(gGroundSlope);
    Matrix4 sceneMatrix = pivotTrans * groundRot * pivotInv;

    // Draw Ground
    setUniformMatrix(shaderProgram, "transform", sceneMatrix);
    glBindVertexArray(vaoGround);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Truck Matrix
    Matrix4 truckMove = Matrix4::Translate(gTruckPosition, 0.0f, 0.0f);
    Matrix4 truckMatrix = sceneMatrix * truckMove;

    // Draw Truck
    setUniformMatrix(shaderProgram, "transform", truckMatrix);
    glBindVertexArray(vaoTruck);
    glDrawArrays(GL_TRIANGLES, 0, 33);

    // Draw Wheels (Logic from Part 2)
    Matrix4 wheelRotMat = Matrix4::RotateZ(gWheelRotation);
    int fanSize = 32;
    glBindVertexArray(vaoWheel);

    // Left Wheel
    Matrix4 lwToCenter = Matrix4::Translate(-0.35f, -0.38f, 0.0f);
    Matrix4 lwToOrigin = Matrix4::Translate(0.35f, 0.38f, 0.0f);
    Matrix4 leftWheelMat = truckMatrix * (lwToCenter * wheelRotMat * lwToOrigin);
    setUniformMatrix(shaderProgram, "transform", leftWheelMat);
    glDrawArrays(GL_TRIANGLE_FAN, 0, fanSize);       
    glDrawArrays(GL_TRIANGLE_FAN, fanSize, fanSize); 

    // Right Wheel
    Matrix4 rwToCenter = Matrix4::Translate(0.25f, -0.38f, 0.0f);
    Matrix4 rwToOrigin = Matrix4::Translate(-0.25f, 0.38f, 0.0f);
    Matrix4 rightWheelMat = truckMatrix * (rwToCenter * wheelRotMat * rwToOrigin);
    setUniformMatrix(shaderProgram, "transform", rightWheelMat);
    glDrawArrays(GL_TRIANGLE_FAN, fanSize * 2, fanSize); 
    glDrawArrays(GL_TRIANGLE_FAN, fanSize * 3, fanSize); 
}


int main(void)
{
    GLFWwindow* window = nullptr;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Requirement: Window Title with Username and ID
    window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Assignment 1 - MyUsername 1234567", nullptr, nullptr);

    if (window == nullptr)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialisation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // --- Init ImGui ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // Match the dark style in the screenshot
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Initialize Scene (Shaders, Buffers)
    init(window);


    // --- Main Loop ---
    while (!glfwWindowShouldClose(window))
    {
        // 1. Handle Inputs
        processInput(window);

        // 2. Clear Screen
        // (Color is cleared inside render_gui logic if changed, 
        //  but standard clear is needed here for the buffer)
        glClearColor(gBackgroundColor[0], gBackgroundColor[1], gBackgroundColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 3. Render 2D Scene (Truck, Ground)
        // NOTE: We call render_scene WITHOUT clearing again inside it
        // You might need to slightly modify render_scene to remove glClear
        glUseProgram(shaderProgram);
        // ... (Drawing code from Part 2 goes here) ...
        // Re-paste the drawing logic (Ground -> Truck -> Wheel) here
        // OR refactor render_scene to NOT call glClear().
        render_scene_content(); // <--- Logic from Part 2 (minus glClear)

        // 4. Render GUI (Draws ON TOP of the scene)
        render_gui();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- Cleanup ImGui ---
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}