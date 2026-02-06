// main.cpp
#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

static TwType gTwWrapType;
static TwType gTwMinFilterType;
static TwType gTwMagFilterType;

static void initTextureUIEnums() {
    // Wrap
    TwEnumVal wrapEV[] = {
        { GL_REPEAT, "REPEAT" },
        { GL_MIRRORED_REPEAT, "MIRRORED_REPEAT" },
        { GL_CLAMP_TO_EDGE, "CLAMP_TO_EDGE" },
        { GL_CLAMP_TO_BORDER, "CLAMP_TO_BORDER" },
    };
    gTwWrapType = TwDefineEnum("WrapMode", wrapEV, 4);

    // Min filter (includes mipmaps)
    TwEnumVal minEV[] = {
        { GL_NEAREST, "NEAREST" },
        { GL_LINEAR, "LINEAR" },
        { GL_NEAREST_MIPMAP_NEAREST, "NEAREST_MIPMAP_NEAREST" },
        { GL_LINEAR_MIPMAP_NEAREST, "LINEAR_MIPMAP_NEAREST" },
        { GL_NEAREST_MIPMAP_LINEAR, "NEAREST_MIPMAP_LINEAR" },
        { GL_LINEAR_MIPMAP_LINEAR, "LINEAR_MIPMAP_LINEAR" }
    };
    gTwMinFilterType = TwDefineEnum("MinFilter", minEV, 6);

    // Mag filter (no mipmap variants allowed)
    TwEnumVal magEV[] = {
        { GL_NEAREST, "NEAREST" },
        { GL_LINEAR, "LINEAR" }
    };
    gTwMagFilterType = TwDefineEnum("MagFilter", magEV, 2);
}



// Textures
static GLuint gTexFloor = 0;
static GLuint gTexPainting = 0;

// Texture parameters (like the lab demo)
static int gWrapModeS = GL_REPEAT;
static int gWrapModeT = GL_REPEAT;
static int gMinFilter = GL_LINEAR_MIPMAP_LINEAR;
static int gMagFilter = GL_LINEAR;
static bool gUseTexture = true;

static GLuint loadTexture2D(const char* path, bool flipY = true) {
    stbi_set_flip_vertically_on_load(flipY);

    int w, h, channels;
    unsigned char* data = stbi_load(path, &w, &h, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << "\n";
        return 0;
    }

    GLenum format = GL_RGB;
    if (channels == 1) format = GL_RED;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Upload
    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Default params (will also be overwritten per-frame by UI)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gWrapModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gWrapModeT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gMinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gMagFilter);

    // If you ever use CLAMP_TO_BORDER
    float border[4] = { 1,0,0,1 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return tex;
}

static GLuint vaoPainting = 0, vboPainting = 0;

static void initPaintingQuad() {
    // A small quad on the back wall z=-2, centered, like a framed painting
    std::vector<VertexPNUV> verts;

    auto pushTri = [&](VertexPNUV a, VertexPNUV b, VertexPNUV c) {
        verts.push_back(a); verts.push_back(b); verts.push_back(c);
    };

    glm::vec3 nBack(0, 0, 1);

    // Painting rectangle on the back wall
    // x: -0.8..0.8, y: 0.7..1.7, z=-1.999 so it doesn't Z-fight with wall
    float z = -1.999f;
    VertexPNUV p0{ {-0.8f, 0.7f, z}, nBack, {0,0} };
    VertexPNUV p1{ { 0.8f, 0.7f, z}, nBack, {1,0} };
    VertexPNUV p2{ { 0.8f, 1.7f, z}, nBack, {1,1} };
    VertexPNUV p3{ {-0.8f, 1.7f, z}, nBack, {0,1} };

    pushTri(p0, p1, p2);
    pushTri(p0, p2, p3);

    glGenVertexArrays(1, &vaoPainting);
    glGenBuffers(1, &vboPainting);

    glBindVertexArray(vaoPainting);
    glBindBuffer(GL_ARRAY_BUFFER, vboPainting);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(VertexPNUV), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNUV), (void*)offsetof(VertexPNUV, p));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNUV), (void*)offsetof(VertexPNUV, n));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNUV), (void*)offsetof(VertexPNUV, uv));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}


// -----------------------------
// Window
static unsigned int gWindowWidth = 800;
static unsigned int gWindowHeight = 800;

// -----------------------------
// UI / global params
static bool  gWireframeMode = false;
static float gBackgroundColor[3] = { 0.2f, 0.2f, 0.2f };

static float gFPS = 0.0f;
static float gFrameTimeMs = 0.0f;

static bool  gAnimate = true;

// Light position (A2 requirement)
static glm::vec3 gLightPos(0.0f, 1.5f, 1.5f);

// Perspective camera controls (A2 requirement)
static float gYawDeg   = 0.0f;
static float gPitchDeg = -20.0f;

// -----------------------------
// AntTweakBar
static TwBar* gBar = nullptr;

// -----------------------------
// Shaders
static ShaderProgram gBasicShader;
static ShaderProgram gLineShader;

// -----------------------------
// Geometry
static GLuint vaoRoom = 0, vboRoom = 0;   // placeholder room geometry
static GLuint vaoLines = 0, vboLines = 0; // separator lines

// Simple vertex format for now: position + normal + uv
// (normal/uv included now so you can keep the same format when adding textures)
struct VertexPNUV {
    glm::vec3 p;
    glm::vec3 n;
    glm::vec2 uv;
};

// -----------------------------
// FPS helper (borrowed from A1)
static void updateFPS() {
    static double lastTime = glfwGetTime();
    static int frames = 0;

    double currentTime = glfwGetTime();
    frames++;
    if (currentTime - lastTime >= 1.0) {
        gFPS = (float)frames;
        gFrameTimeMs = 1000.0f / (gFPS > 0.0f ? gFPS : 1.0f);
        frames = 0;
        lastTime = currentTime;
    }
}

// -----------------------------
// AntTweakBar + GLFW callback forwarding (borrowed from A1)
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void)window; (void)mods;
    TwEventMouseButtonGLFW(button, action);
}
static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    (void)window;
    TwEventMousePosGLFW((int)xpos, (int)ypos);
}
static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    (void)window; (void)xoffset;
    TwEventMouseWheelGLFW((int)yoffset);
}
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode; (void)mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }
    TwEventKeyGLFW(key, action);

    // Optional: quick toggle animate with Space
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        gAnimate = !gAnimate;
    }
}
static void charCallback(GLFWwindow* window, unsigned int codepoint) {
    (void)window;
    TwEventCharGLFW(codepoint, GLFW_PRESS);
}

static void framebufferSizeCallback(GLFWwindow* window, int w, int h) {
    (void)window;
    gWindowWidth = (unsigned int)w;
    gWindowHeight = (unsigned int)h;
    TwWindowSize(w, h);
}

// -----------------------------
// Build simple “room” placeholder geometry
// For now: just a floor quad (y=0) and a back wall quad (z=-2)
static void initRoomPlaceholder() {
    std::vector<VertexPNUV> verts;

    // Floor (two triangles), size 4x4 centered at origin on y=0
    // UVs tiled (0..4) so later you can see wrap repeat on floor texture
    auto pushTri = [&](VertexPNUV a, VertexPNUV b, VertexPNUV c) {
        verts.push_back(a); verts.push_back(b); verts.push_back(c);
    };

    glm::vec3 nFloor(0, 1, 0);
    VertexPNUV f0{ {-2,0, 2}, nFloor, {0,0} };
    VertexPNUV f1{ { 2,0, 2}, nFloor, {4,0} };
    VertexPNUV f2{ { 2,0,-2}, nFloor, {4,4} };
    VertexPNUV f3{ {-2,0,-2}, nFloor, {0,4} };
    pushTri(f0, f1, f2);
    pushTri(f0, f2, f3);

    // Back wall (z=-2), from y=0..2, x=-2..2
    glm::vec3 nBack(0, 0, 1);
    VertexPNUV w0{ {-2,0,-2}, nBack, {0,0} };
    VertexPNUV w1{ { 2,0,-2}, nBack, {1,0} };
    VertexPNUV w2{ { 2,2,-2}, nBack, {1,1} };
    VertexPNUV w3{ {-2,2,-2}, nBack, {0,1} };
    pushTri(w0, w1, w2);
    pushTri(w0, w2, w3);

    glGenVertexArrays(1, &vaoRoom);
    glGenBuffers(1, &vboRoom);

    glBindVertexArray(vaoRoom);
    glBindBuffer(GL_ARRAY_BUFFER, vboRoom);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(VertexPNUV), verts.data(), GL_STATIC_DRAW);

    // layout(location=0) vec3 pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNUV), (void*)offsetof(VertexPNUV, p));
    glEnableVertexAttribArray(0);

    // layout(location=1) vec3 normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNUV), (void*)offsetof(VertexPNUV, n));
    glEnableVertexAttribArray(1);

    // layout(location=2) vec2 uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNUV), (void*)offsetof(VertexPNUV, uv));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

// -----------------------------
// Separator lines geometry (two lines)
// We'll draw them in NDC using the line shader and glViewport full screen.
static void initSeparatorLines() {
    // Two lines: vertical x=0 and horizontal y=0 in NDC
    // Each line as 2 points (GL_LINES)
    // Format: vec2 position
    float lineVerts[] = {
        // vertical line (x=0, y=-1..1)
        0.0f, -1.0f,
        0.0f,  1.0f,

        // horizontal line (y=0, x=-1..1)
        -1.0f, 0.0f,
         1.0f, 0.0f
    };

    glGenVertexArrays(1, &vaoLines);
    glGenBuffers(1, &vboLines);

    glBindVertexArray(vaoLines);
    glBindBuffer(GL_ARRAY_BUFFER, vboLines);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerts), lineVerts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

// -----------------------------
// Camera helpers
static glm::mat4 makeView_TopDown() {
    // Look down -Y onto origin; Z points "up" in view for stability
    return glm::lookAt(glm::vec3(0.0f, 5.0f, 0.001f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 0.0f, -1.0f));
}

static glm::mat4 makeView_Front() {
    // Look from +Z to origin
    return glm::lookAt(glm::vec3(0.0f, 1.0f, 5.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
}

static glm::mat4 makeView_PerspectiveYawPitch(float yawDeg, float pitchDeg) {
    // Orbit-ish camera about origin
    float yaw   = glm::radians(yawDeg);
    float pitch = glm::radians(pitchDeg);

    float r = 6.0f;
    glm::vec3 eye;
    eye.x = r * cosf(pitch) * sinf(yaw);
    eye.y = r * sinf(pitch) + 1.0f;  // lift a bit
    eye.z = r * cosf(pitch) * cosf(yaw);

    glm::vec3 center(0.0f, 1.0f, 0.0f);
    return glm::lookAt(eye, center, glm::vec3(0,1,0));
}

// -----------------------------
static void renderViewport(int x, int y, int w, int h,
                           const glm::mat4& view,
                           const glm::mat4& proj)
{
    glViewport(x, y, w, h);

    gBasicShader.use();

    glm::mat4 model(1.0f);
    glm::mat4 mvp = proj * view * model;

    gBasicShader.setUniform("uMVP", mvp);
    gBasicShader.setUniform("uModel", model);
    gBasicShader.setUniform("uLightPos", gLightPos);

    // Tell the shader: sample from texture unit 0
    gBasicShader.setUniform("uTex", 0);
    gBasicShader.setUniform("uUseTexture", gUseTexture);

    // Helper to apply UI-selected wrap/filter to whichever texture is bound
    auto applyTexParams2D = [&](GLuint tex) {
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gWrapModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gWrapModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gMinFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gMagFilter);
    };

    // -------------------------
    // 1) Draw FLOOR with floor texture
    glActiveTexture(GL_TEXTURE0);
    applyTexParams2D(gTexFloor);

    glBindVertexArray(vaoRoom);
    glDrawArrays(GL_TRIANGLES, 0, 6);   // floor = first 6 verts

    // -------------------------
    // 2) Draw BACK WALL (for now using floor texture too; later you'll use wall diffuse/normal)
    // If you already have a wall diffuse texture, bind that here instead.
    applyTexParams2D(gTexFloor);
    glDrawArrays(GL_TRIANGLES, 6, 6);   // wall = next 6 verts
    glBindVertexArray(0);

    // -------------------------
    // 3) Draw PAINTING with painting texture
    applyTexParams2D(gTexPainting);

    glBindVertexArray(vaoPainting);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}


static void renderSeparators() {
    glViewport(0, 0, (int)gWindowWidth, (int)gWindowHeight);
    gLineShader.use();
    glBindVertexArray(vaoLines);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);
}

// -----------------------------
// UI init (borrowed pattern from A1)
static void initUI() {
    gBar = TwNewBar("Interface");
    TwDefine(" GLOBAL fontsize=3 ");
    TwDefine(" TW_HELP visible=false ");
    TwDefine(" Interface label='User Interface' text=light size='260 420' alpha=200 refresh=0.02 ");

    TwAddVarRO(gBar, "FPS", TW_TYPE_FLOAT, &gFPS, " group='Frame Stats' precision=1 ");
    TwAddVarRO(gBar, "Frame Time (ms)", TW_TYPE_FLOAT, &gFrameTimeMs, " group='Frame Stats' precision=3 ");

    TwAddVarRW(gBar, "Wireframe", TW_TYPE_BOOL32, &gWireframeMode, " group='Display' ");
    TwAddVarRW(gBar, "BG Color", TW_TYPE_COLOR3F, &gBackgroundColor, " group='Display' label='Background Color' ");

    TwAddVarRW(gBar, "Animate", TW_TYPE_BOOL32, &gAnimate, " group='Controls' label='Pause/Unpause' ");

    // Light position sliders
    TwAddVarRW(gBar, "Light X", TW_TYPE_FLOAT, &gLightPos.x, " group='Light' step=0.05 ");
    TwAddVarRW(gBar, "Light Y", TW_TYPE_FLOAT, &gLightPos.y, " group='Light' step=0.05 ");
    TwAddVarRW(gBar, "Light Z", TW_TYPE_FLOAT, &gLightPos.z, " group='Light' step=0.05 ");

    // Perspective camera yaw/pitch
    TwAddVarRW(gBar, "Yaw",   TW_TYPE_FLOAT, &gYawDeg,   " group='Camera' min=-180 max=180 step=0.5 ");
    TwAddVarRW(gBar, "Pitch", TW_TYPE_FLOAT, &gPitchDeg, " group='Camera' min=-89  max=89  step=0.5 ");

    TwDefine(" Interface/'Frame Stats' opened=true ");
    TwDefine(" Interface/'Display' opened=true ");
    TwDefine(" Interface/'Controls' opened=true ");
    TwDefine(" Interface/'Light' opened=true ");
    TwDefine(" Interface/'Camera' opened=true ");


	TwAddVarRW(gBar, "Use Texture", TW_TYPE_BOOL32, &gUseTexture, " group='Texture' ");

	TwAddVarRW(gBar, "Wrap S", gTwWrapType, &gWrapModeS, " group='Texture' ");
	TwAddVarRW(gBar, "Wrap T", gTwWrapType, &gWrapModeT, " group='Texture' ");
	TwAddVarRW(gBar, "Min Filter", gTwMinFilterType, &gMinFilter, " group='Texture' ");
	TwAddVarRW(gBar, "Mag Filter", gTwMagFilterType, &gMagFilter, " group='Texture' ");

	TwDefine(" Interface/'Texture' opened=true ");

}

// -----------------------------
// Init
static void initScene() {
    // Enable depth (you will want it for A2)
    glEnable(GL_DEPTH_TEST);

    // Shaders: put these files in /shaders
    gBasicShader.compileAndLink("shaders/basic.vert", "shaders/basic.frag");
    gLineShader.compileAndLink("shaders/line.vert", "shaders/line.frag");

    initRoomPlaceholder();
    initSeparatorLines();

	gTexFloor = loadTexture2D("images/check.bmp");      // change to your real path
	gTexPainting = loadTexture2D("images/check.bmp");// change to your real path

}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow((int)gWindowWidth, (int)gWindowHeight, "Assignment 2", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed\n";
        return -1;
    }

    // AntTweakBar
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize((int)gWindowWidth, (int)gWindowHeight);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    initUI();
    initScene();

    double lastFrameTime = glfwGetTime();
    float t = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        updateFPS();

        if (gAnimate) t += deltaTime;

        // Global clear
        glClearColor(gBackgroundColor[0], gBackgroundColor[1], gBackgroundColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (gWireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Compute viewport rectangles: 2x2 split
        int halfW = (int)gWindowWidth / 2;
        int halfH = (int)gWindowHeight / 2;

        // Projections
        // Ortho extents chosen to comfortably frame the placeholder room
        glm::mat4 orthoProj = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 50.0f);

        float aspect = (halfW > 0 && halfH > 0) ? (float)halfW / (float)halfH : 1.0f;
        glm::mat4 perspProj = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

        // Views
        glm::mat4 viewTop  = makeView_TopDown();
        glm::mat4 viewFront = makeView_Front();
        glm::mat4 viewPersp = makeView_PerspectiveYawPitch(gYawDeg, gPitchDeg);

        // Render viewports (choose a layout; here: TL=Top, BL=Front, BR=Perspective)
        // Top-left
        renderViewport(0, halfH, halfW, halfH, viewTop, orthoProj);
        // Bottom-left
        renderViewport(0, 0, halfW, halfH, viewFront, orthoProj);
        // Bottom-right
        renderViewport(halfW, 0, halfW, halfH, viewPersp, perspProj);

        // Separator lines on top
        renderSeparators();

        // UI on top
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        TwDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    TwTerminate();
    glfwTerminate();
    return 0;
}
