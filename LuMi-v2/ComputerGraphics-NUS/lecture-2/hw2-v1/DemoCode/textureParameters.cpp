// Assignment2.cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>

// OpenGL and GLFW headers
#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

// AntTweakBar for GUI
#include <AntTweakBar.h>

// GLM for mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// SOIL for texture loading
#include <SOIL/SOIL.h>

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(std::vector<std::string> faces);

// Shader sources
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;
    
    // TBN matrix for normal mapping
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));
    
    TangentLightPos = TBN * lightPos;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * FragPos;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

struct Material {
    sampler2D diffuse;
    sampler2D normal;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform bool useNormalMap;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // Diffuse 
    vec3 norm;
    if(useNormalMap) {
        norm = texture(material.normal, TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);  // Convert from [0,1] to [-1,1]
    } else {
        norm = normalize(Normal);
    }
    
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    
    // Specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.diffuse, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
)";

const char* skyboxVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
)";

const char* skyboxFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}
)";

// Settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// Camera for perspective view
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float frameTime = 0.0f;
int frameCount = 0;
float fps = 0.0f;
float lastFPSUpdate = 0.0f;

// Animation control
bool animationPaused = false;
float ornamentRotation = 0.0f;

// Light position
glm::vec3 lightPos = glm::vec3(0.0f, 3.0f, 0.0f);

// AntTweakBar
TwBar *bar;

// Function to compile shader
unsigned int compileShader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    return shader;
}

// Function to create shader program
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    unsigned int vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

// Cube vertices for room (with normals and texture coordinates)
float roomVertices[] = {
    // Positions          // Normals           // Texture Coords  // Tangent      // Bitangent
    // Floor
    -5.0f, 0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     5.0f, 0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     5.0f, 0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     5.0f, 0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -5.0f, 0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -5.0f, 0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    
    // Wall 1 (Front)
    -5.0f, 0.0f,  5.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 0.0f,  5.0f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f,        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 5.0f,  5.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 5.0f,  5.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, 5.0f,  5.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, 0.0f,  5.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    
    // Wall 2 (Back)
    -5.0f, 0.0f, -5.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,       -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, 5.0f, -5.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,       -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 5.0f, -5.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,       -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 5.0f, -5.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,       -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 0.0f, -5.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,       -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, 0.0f, -5.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,       -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    
    // Wall 3 (Left)
    -5.0f, 0.0f, -5.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,       0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, 5.0f, -5.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,       0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, 5.0f,  5.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,       0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, 5.0f,  5.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,       0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, 0.0f,  5.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,       0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -5.0f, 0.0f, -5.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,       0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    
    // Wall 4 (Right)
     5.0f, 0.0f, -5.0f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,       0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 5.0f, -5.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,       0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 5.0f,  5.0f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,       0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 5.0f,  5.0f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,       0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 0.0f,  5.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,       0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     5.0f, 0.0f, -5.0f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,       0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f
};

// Painting vertices
float paintingVertices[] = {
    // Positions          // Texture Coords
    -1.0f,  2.0f,  4.99f, 0.0f, 1.0f,
     1.0f,  2.0f,  4.99f, 1.0f, 1.0f,
     1.0f,  4.0f,  4.99f, 1.0f, 0.0f,
     1.0f,  4.0f,  4.99f, 1.0f, 0.0f,
    -1.0f,  4.0f,  4.99f, 0.0f, 0.0f,
    -1.0f,  2.0f,  4.99f, 0.0f, 1.0f
};

// Cube vertices for ornament (with normals for environment mapping)
float cubeVertices[] = {
    // Positions          
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

// Skybox vertices
float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


void renderView(unsigned int shaderProgram, unsigned int simpleShader, unsigned int skyboxShader,
                unsigned int roomVAO, unsigned int paintingVAO, unsigned int ornamentVAO, unsigned int skyboxVAO,
                unsigned int floorTexture, unsigned int wallTexture, unsigned int wallNormalTexture,
                unsigned int paintingTexture, unsigned int cubemapTexture,
                glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos, bool useNormalMap,
                float ornamentRotation, bool renderSkybox) {
    
    // Render room
    glUseProgram(shaderProgram);
    
    // Set uniforms
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 2.0f, 5.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1i(glGetUniformLocation(shaderProgram, "useNormalMap"), useNormalMap);
    
    // Floor
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);  // No normal map for floor
    
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(roomVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);  // Draw floor
    
    // Walls with normal mapping
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, wallNormalTexture);
    
    glDrawArrays(GL_TRIANGLES, 6, 24);  // Draw walls
    
    // Render painting
    glUseProgram(simpleShader);
    glUniformMatrix4fv(glGetUniformLocation(simpleShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(simpleShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(simpleShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, paintingTexture);
    
    glBindVertexArray(paintingVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // Render ornament with environment mapping
    if (renderSkybox) {
        // First render skybox
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShader);
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));  // Remove translation
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }
    
    // Render ornament cube
    glUseProgram(shaderProgram);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
    model = glm::rotate(model, glm::radians(ornamentRotation), glm::vec3(0.5f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    // Use cubemap for ornament
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useNormalMap"), false);
    
    glBindVertexArray(ornamentVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// Callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    TwWindowSize(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (TwEventMousePosGLFW(xpos, ypos))
        return;
    
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    cameraYaw += xoffset;
    cameraPitch += yoffset;
    
    // Constrain pitch
    if (cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if (cameraPitch < -89.0f)
        cameraPitch = -89.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (TwEventMouseWheelGLFW(yoffset))
        return;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// Utility function to load texture
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char* data = SOIL_load_image(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        SOIL_free_image_data(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        SOIL_free_image_data(data);
    }
    
    return textureID;
}

// Utility function to load cubemap
unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            SOIL_free_image_data(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            SOIL_free_image_data(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 2 - Multiple Viewpoints", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    
    // Initialize AntTweakBar
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(SCR_WIDTH, SCR_HEIGHT);
    
    // Create AntTweakBar
    bar = TwNewBar("Control Panel");
    TwDefine(" GLOBAL help='This is a control panel for the 3D scene' ");
    TwDefine(" ControlPanel size='300 400' color='96 216 224' ");
    
    // Add variables to AntTweakBar
    TwAddVarRW(bar, "FPS", TW_TYPE_FLOAT, &fps, " label='Frames Per Second' readonly=true ");
    TwAddVarRW(bar, "Frame Time", TW_TYPE_FLOAT, &frameTime, " label='Frame Time (ms)' readonly=true ");
    TwAddButton(bar, "Toggle Animation", [](void* clientData) { 
        animationPaused = !animationPaused; 
    }, NULL, " label='Toggle Animation (Pause/Play)' ");
    TwAddVarRW(bar, "Light X", TW_TYPE_FLOAT, &lightPos.x, " label='Light X' min=-5.0 max=5.0 step=0.1 ");
    TwAddVarRW(bar, "Light Y", TW_TYPE_FLOAT, &lightPos.y, " label='Light Y' min=0.0 max=5.0 step=0.1 ");
    TwAddVarRW(bar, "Light Z", TW_TYPE_FLOAT, &lightPos.z, " label='Light Z' min=-5.0 max=5.0 step=0.1 ");
    TwAddVarRW(bar, "Camera Yaw", TW_TYPE_FLOAT, &cameraYaw, " label='Camera Yaw' min=-180.0 max=180.0 step=1.0 ");
    TwAddVarRW(bar, "Camera Pitch", TW_TYPE_FLOAT, &cameraPitch, " label='Camera Pitch' min=-89.0 max=89.0 step=1.0 ");
    bool useNormalMap = true;
    TwAddVarRW(bar, "Normal Mapping", TW_TYPE_BOOLCPP, &useNormalMap, " label='Use Normal Mapping' ");
    
    // Compile shaders
    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    unsigned int skyboxShader = createShaderProgram(skyboxVertexShaderSource, skyboxFragmentShaderSource);
    unsigned int simpleShader = createShaderProgram(R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoords;
        uniform mat4 projection;
        uniform mat4 view;
        uniform mat4 model;
        out vec2 TexCoords;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoords = aTexCoords;
        }
    )", R"(
        #version 330 core
        out vec4 FragColor;
        in vec2 TexCoords;
        uniform sampler2D texture1;
        void main() {
            FragColor = texture(texture1, TexCoords);
        }
    )");
    
    // Room VAO, VBO
    unsigned int roomVAO, roomVBO;
    glGenVertexArrays(1, &roomVAO);
    glGenBuffers(1, &roomVBO);
    glBindVertexArray(roomVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roomVertices), roomVertices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // Tangent attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
    // Bitangent attribute
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(4);
    
    // Painting VAO, VBO
    unsigned int paintingVAO, paintingVBO;
    glGenVertexArrays(1, &paintingVAO);
    glGenBuffers(1, &paintingVBO);
    glBindVertexArray(paintingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, paintingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(paintingVertices), paintingVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Ornament VAO, VBO
    unsigned int ornamentVAO, ornamentVBO;
    glGenVertexArrays(1, &ornamentVAO);
    glGenBuffers(1, &ornamentVBO);
    glBindVertexArray(ornamentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ornamentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Skybox VAO, VBO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Load textures
    unsigned int floorTexture = loadTexture("textures/floor.jpg");  // You need to provide floor texture
    unsigned int wallTexture = loadTexture("textures/wall.jpg");    // You need to provide wall texture
    unsigned int wallNormalTexture = loadTexture("textures/wall_normal.jpg");  // You need to provide wall normal map
    unsigned int paintingTexture = loadTexture("textures/painting.jpg");  // You need to provide painting texture
    
    // Load cubemap for environment mapping
    std::vector<std::string> faces = {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    
    // Set up shader uniforms
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "material.normal"), 1);
    glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 32.0f);
    
    // Light properties
    glUniform3f(glGetUniformLocation(shaderProgram, "light.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shaderProgram, "light.diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(shaderProgram, "light.specular"), 1.0f, 1.0f, 1.0f);
    
    // Skybox shader
    glUseProgram(skyboxShader);
    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate frame time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Calculate FPS
        frameCount++;
        if (currentFrame - lastFPSUpdate >= 1.0f) {
            fps = frameCount / (currentFrame - lastFPSUpdate);
            frameTime = 1000.0f / fps;
            frameCount = 0;
            lastFPSUpdate = currentFrame;
        }
        
        // Process input
        processInput(window);
        
        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Update camera direction based on yaw and pitch
        glm::vec3 front;
        front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        front.y = sin(glm::radians(cameraPitch));
        front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraFront = glm::normalize(front);
        
        // Update ornament rotation if animation is not paused
        if (!animationPaused) {
            ornamentRotation += 50.0f * deltaTime;
            if (ornamentRotation > 360.0f) ornamentRotation -= 360.0f;
        }
        
        // Define viewports for multiple viewpoints
        int viewportWidth = SCR_WIDTH / 3;
        int viewportHeight = SCR_HEIGHT;
        
        // 1. Top-down orthographic view (left viewport)
        glViewport(0, 0, viewportWidth, viewportHeight);
        renderView(shaderProgram, simpleShader, skyboxShader, roomVAO, paintingVAO, ornamentVAO, skyboxVAO,
                  floorTexture, wallTexture, wallNormalTexture, paintingTexture, cubemapTexture,
                  glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, 0.1f, 100.0f),  // Orthographic projection
                  glm::lookAt(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                  lightPos, useNormalMap, ornamentRotation, false);
        
        // Draw separation line
        glViewport(viewportWidth - 2, 0, 4, viewportHeight);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 2. Front orthographic view (middle viewport)
        glViewport(viewportWidth, 0, viewportWidth, viewportHeight);
        renderView(shaderProgram, simpleShader, skyboxShader, roomVAO, paintingVAO, ornamentVAO, skyboxVAO,
                  floorTexture, wallTexture, wallNormalTexture, paintingTexture, cubemapTexture,
                  glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, 0.1f, 100.0f),  // Orthographic projection
                  glm::lookAt(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
                  lightPos, useNormalMap, ornamentRotation, false);
        
        // Draw separation line
        glViewport(2 * viewportWidth - 2, 0, 4, viewportHeight);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 3. Perspective view (right viewport)
        glViewport(2 * viewportWidth, 0, viewportWidth, viewportHeight);
        renderView(shaderProgram, simpleShader, skyboxShader, roomVAO, paintingVAO, ornamentVAO, skyboxVAO,
                  floorTexture, wallTexture, wallNormalTexture, paintingTexture, cubemapTexture,
                  glm::perspective(glm::radians(45.0f), (float)viewportWidth / (float)viewportHeight, 0.1f, 100.0f),
                  glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp),
                  lightPos, useNormalMap, ornamentRotation, true);
        
        // Reset viewport for AntTweakBar
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        
        // Draw AntTweakBar
        TwDraw();
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    glDeleteVertexArrays(1, &roomVAO);
    glDeleteBuffers(1, &roomVBO);
    glDeleteVertexArrays(1, &paintingVAO);
    glDeleteBuffers(1, &paintingVBO);
    glDeleteVertexArrays(1, &ornamentVAO);
    glDeleteBuffers(1, &ornamentVBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    
    TwTerminate();
    glfwTerminate();
    
    return 0;
}
