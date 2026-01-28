// main.cpp - OpenGL 3.3 Core rewrite of your GLUT assignment
// Dependencies:
//   - freeglut
//   - glad (OpenGL 3.3 core loader)  -> include glad/glad.h and compile glad.c
//   - stb_easy_font.h (single-header simple text drawing)

#include <glad/glad.h>
#include <GL/freeglut.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

// ---------------- stb_easy_font ----------------
#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font.h"

// ---------------- Window ----------------
const int WIDTH  = 800;
const int HEIGHT = 800;

// ---------------- Timing ----------------
int   prevTime   = 0;
float deltaTime  = 0.0f;
float fps        = 60.0f;
float frameTime  = 16.67f;

// ---------------- Scene state ----------------
float truckX      = 0.0f;
float truckSpeed  = 2.0f;

float wheelAngle  = 0.0f;
float wheelRadius = 0.07f;

float groundAngle = 0.0f;
const float GROUND_MIN = -15.0f;
const float GROUND_MAX =  15.0f;

bool  wireframe = false;
float bgColor[3] = { 0.2f, 0.2f, 0.2f };

static float clampf(float x, float a, float b) {
    return (x < a) ? a : (x > b) ? b : x;
}
// ---------------- UI State ----------------
struct UIInteractionState {
    int  activeItemID = -1; // ID of element being dragged
    bool isDragging   = false;
} uiState;

// We'll draw UI in PIXEL space (much nicer for text).
// Projection will be ortho(0..WIDTH, HEIGHT..0) => origin top-left, y down.
const float UI_X_START_PX     = 8.0f;
const float UI_Y_START_PX     = 20.0f;
const float UI_LINE_HEIGHT_PX = 32.0f;
const float UI_INDENT_PX      = 20.0f;
const float UI_PANEL_WIDTH_PX = 280.0f;
const float UI_PANEL_HEIGHT_PX= 480.0f;
const float UI_HEADER_H_PX    = 20.0f;
const float UI_HEADER_OFFSET_PX = 40.0f; // similar to your "-0.1f" offset in NDC

// column layout inside each row
const float UI_LABEL_PAD_PX   = 10.0f;
const float UI_VALUE_COL_PX   = 140.0f; // distance from xPos to value area
const float UI_BAR_W_PX       = 80.0f;
const float UI_BAR_H_PX       = 6.0f;
const float UI_BAR_OFFSET_PX  = 60.0f;  // from value text to slider bar

// ---------------- Minimal math (mat4) ----------------
struct Mat4 {
    float m[16]; // column-major

    static Mat4 identity() {
        Mat4 r{};
        r.m[0]=1; r.m[5]=1; r.m[10]=1; r.m[15]=1;
        return r;
    }

    static Mat4 ortho(float l, float r, float b, float t, float n, float f) {
        Mat4 M{};
        M.m[0]  =  2.0f / (r - l);
        M.m[5]  =  2.0f / (t - b);
        M.m[10] = -2.0f / (f - n);
        M.m[12] = -(r + l) / (r - l);
        M.m[13] = -(t + b) / (t - b);
        M.m[14] = -(f + n) / (f - n);
        M.m[15] =  1.0f;
        return M;
    }

    static Mat4 translate(float x, float y, float z=0.0f) {
        Mat4 M = identity();
        M.m[12] = x;
        M.m[13] = y;
        M.m[14] = z;
        return M;
    }

    static Mat4 scale(float x, float y, float z=1.0f) {
        Mat4 M{};
        M.m[0]  = x;
        M.m[5]  = y;
        M.m[10] = z;
        M.m[15] = 1.0f;
        return M;
    }

    static Mat4 rotateZ(float deg) {
        float a = deg * 3.1415926535f / 180.0f;
        float c = std::cos(a);
        float s = std::sin(a);
        Mat4 M = identity();
        M.m[0] = c;  M.m[4] = -s;
        M.m[1] = s;  M.m[5] =  c;
        return M;
    }
};

static Mat4 mul(const Mat4& A, const Mat4& B) {
    Mat4 R{};
    for (int c=0;c<4;c++) {
        for (int r=0;r<4;r++) {
            R.m[c*4 + r] =
                A.m[0*4 + r] * B.m[c*4 + 0] +
                A.m[1*4 + r] * B.m[c*4 + 1] +
                A.m[2*4 + r] * B.m[c*4 + 2] +
                A.m[3*4 + r] * B.m[c*4 + 3];
        }
    }
    return R;
}

// ---------------- Shader helpers ----------------
static GLuint compileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);

    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0');
        glGetShaderInfoLog(s, len, nullptr, &log[0]);
        std::fprintf(stderr, "Shader compile error:\n%s\n", log.c_str());
        std::exit(1);
    }
    return s;
}

static GLuint linkProgram(GLuint vs, GLuint fs) {
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);

    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0');
        glGetProgramInfoLog(p, len, nullptr, &log[0]);
        std::fprintf(stderr, "Program link error:\n%s\n", log.c_str());
        std::exit(1);
    }
    return p;
}

// One simple shader for both triangles and lines.
static GLuint gProgram = 0;
static GLint  uMVP_loc = -1;
static GLint  uColor_loc = -1;

// ---------------- Meshes ----------------
static GLuint quadVAO=0, quadVBO=0;     // unit quad [0..1]x[0..1] as 2 triangles
static GLuint circleVAO=0, circleVBO=0; // unit circle fan (center + ring)
static int    circleVertexCount = 0;

static GLuint dynLineVAO=0, dynLineVBO=0;   // dynamic lines for UI (dashes, checkbox)
static GLuint dynTriVAO=0, dynTriVBO=0;     // dynamic triangles for text (stb_easy_font)

// ---------------- Common draw ----------------
static void useProgram() {
    glUseProgram(gProgram);
}

static void setColor(float r,float g,float b,float a=1.0f) {
    glUniform4f(uColor_loc, r,g,b,a);
}

static void setMVP(const Mat4& mvp) {
    glUniformMatrix4fv(uMVP_loc, 1, GL_FALSE, mvp.m);
}

static void drawQuad(const Mat4& mvp) {
    setMVP(mvp);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void drawCircle(const Mat4& mvp) {
    setMVP(mvp);
    glBindVertexArray(circleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, circleVertexCount);
}

static void drawLines(const Mat4& mvp, const std::vector<float>& xyPairs) {
    // xyPairs = {x0,y0,x1,y1, x2,y2,x3,y3, ...} interpreted as GL_LINES
    setMVP(mvp);
    glBindVertexArray(dynLineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, dynLineVBO);
    glBufferData(GL_ARRAY_BUFFER, xyPairs.size() * sizeof(float), xyPairs.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, (GLsizei)(xyPairs.size()/2));
}

static void drawTriangles(const Mat4& mvp, const std::vector<float>& xyPairs) {
    // xyPairs interpreted as GL_TRIANGLES (each 3 verts = 6 floats)
    setMVP(mvp);
    glBindVertexArray(dynTriVAO);
    glBindBuffer(GL_ARRAY_BUFFER, dynTriVBO);
    glBufferData(GL_ARRAY_BUFFER, xyPairs.size() * sizeof(float), xyPairs.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(xyPairs.size()/2));
}

// ---------------- Scene utilities (NDC space) ----------------
static Mat4 sceneMVP(const Mat4& model) {
    // We draw in NDC directly => MVP = model
    return model;
}

static Mat4 rectModelNDC(float x, float y, float w, float h) {
    return mul(Mat4::translate(x,y,0), Mat4::scale(w,h,1));
}

static Mat4 circleModelNDC(float cx, float cy, float r) {
    return mul(Mat4::translate(cx,cy,0), Mat4::scale(r,r,1));
}

// ---------------- UI utilities (pixel space) ----------------
static Mat4 gUIProj;

static Mat4 uiMVP(const Mat4& model) {
    return mul(gUIProj, model);
}

static Mat4 rectModelPX(float x, float y, float w, float h) {
    return mul(Mat4::translate(x,y,0), Mat4::scale(w,h,1));
}

// dashed lines in pixel space (replace glLineStipple)
static void addDashedLine(std::vector<float>& out, float x0,float y0,float x1,float y1, float dash=6.0f, float gap=6.0f) {
    float dx = x1 - x0, dy = y1 - y0;
    float len = std::sqrt(dx*dx + dy*dy);
    if (len < 0.0001f) return;
    float ux = dx/len, uy = dy/len;

    float t = 0.0f;
    while (t < len) {
        float t0 = t;
        float t1 = std::min(t + dash, len);
        out.push_back(x0 + ux*t0); out.push_back(y0 + uy*t0);
        out.push_back(x0 + ux*t1); out.push_back(y0 + uy*t1);
        t += dash + gap;
    }
}

static void addTextTriangles(std::vector<float>& outXY, float x, float y, const char* text) {
    // stb_easy_font_print writes QUADS (4 vertices each) into the buffer.
    // Each vertex is 3 floats: x, y, z.
    // We'll convert each quad into 2 triangles (6 vertices) using only x,y.

    char buffer[99999];
    int quads = stb_easy_font_print(x, y, (char*)text, nullptr, buffer, sizeof(buffer));

    float* v = (float*)buffer; // interpret as float array

    for (int q = 0; q < quads; q++) {
        int base = q * 4 * 3; // 4 verts * 3 floats per vert

        float x0 = v[base + 0],  y0 = v[base + 1];
        float x1 = v[base + 3],  y1 = v[base + 4];
        float x2 = v[base + 6],  y2 = v[base + 7];
        float x3 = v[base + 9],  y3 = v[base +10];

        // tri 1: 0,1,2
        outXY.push_back(x0); outXY.push_back(y0);
        outXY.push_back(x1); outXY.push_back(y1);
        outXY.push_back(x2); outXY.push_back(y2);

        // tri 2: 0,2,3
        outXY.push_back(x0); outXY.push_back(y0);
        outXY.push_back(x2); outXY.push_back(y2);
        outXY.push_back(x3); outXY.push_back(y3);
    }
}


// ---------------- UI drawing ----------------
static void drawTreeLinesPX(float x, float yTop, float height, float width) {
    // mimic your tree connector: vertical then horizontal mid
    std::vector<float> lines;
    float yBot = yTop + height;
    float yMid = yTop + height * 0.5f;

    // dashed vertical
    addDashedLine(lines, x, yTop, x, yBot, 4.0f, 4.0f);
    // dashed horizontal
    addDashedLine(lines, x, yMid, x + width, yMid, 4.0f, 4.0f);

    setColor(0.5f,0.5f,0.5f,1.0f);
    drawLines(uiMVP(Mat4::identity()), lines);
}

static void drawTreeRowPX(
    int row_index, int level,
    const char* label,
    const char* valueStr,
    bool isSlider, float* valueRef, float minV, float maxV,
    bool isToggle, bool* toggleRef
) {
    float yPos = UI_Y_START_PX + UI_HEADER_OFFSET_PX + row_index * UI_LINE_HEIGHT_PX;
    float xPos = UI_X_START_PX + level * UI_INDENT_PX;

    // Tree structure lines (if not root)
    if (level > 0) {
        float lineX = UI_X_START_PX + (level - 1) * UI_INDENT_PX + 8.0f;
        drawTreeLinesPX(lineX, yPos - 10.0f, UI_LINE_HEIGHT_PX, UI_INDENT_PX - 6.0f);
    }

    // Label color
    bool isCategory = (valueStr == nullptr && !isToggle && !isSlider);
    if (isCategory) setColor(1.0f,1.0f,0.0f,1.0f);
    else           setColor(0.9f,0.9f,0.9f,1.0f);

    // draw label text
    {
        std::vector<float> textTris;
        addTextTriangles(textTris, xPos + UI_LABEL_PAD_PX, yPos, label);
        drawTriangles(uiMVP(Mat4::identity()), textTris);
    }

    float valX = xPos + UI_VALUE_COL_PX;

    // Slider
    if (isSlider && valueRef) {
        // value text
        setColor(0.5f,0.8f,1.0f,1.0f);
        {
            std::vector<float> textTris;
            addTextTriangles(textTris, valX, yPos, valueStr);
            drawTriangles(uiMVP(Mat4::identity()), textTris);
        }

        float barX = valX + UI_BAR_OFFSET_PX;
        float barY = yPos + 4.0f;

        // track
        setColor(0.3f,0.3f,0.3f,1.0f);
        drawQuad(uiMVP(rectModelPX(barX, barY, UI_BAR_W_PX, UI_BAR_H_PX)));

        float norm = (*valueRef - minV) / (maxV - minV);
        norm = clampf(norm, 0.0f, 1.0f);

        // fill
        setColor(0.0f,0.7f,0.0f,1.0f);
        drawQuad(uiMVP(rectModelPX(barX, barY, UI_BAR_W_PX * norm, UI_BAR_H_PX)));
    }
    // Toggle (checkbox)
    else if (isToggle && toggleRef) {
        float box = 14.0f;
        float bx  = valX;
        float by  = yPos + 2.0f;

        // outline box with lines
        std::vector<float> lines;
        // rectangle outline as 4 segments
        auto seg = [&](float x0,float y0,float x1,float y1){
            lines.push_back(x0); lines.push_back(y0);
            lines.push_back(x1); lines.push_back(y1);
        };
        seg(bx,by, bx+box,by);
        seg(bx+box,by, bx+box,by+box);
        seg(bx+box,by+box, bx,by+box);
        seg(bx,by+box, bx,by);

        setColor(1,1,1,1);
        drawLines(uiMVP(Mat4::identity()), lines);

        if (*toggleRef) {
            lines.clear();
            seg(bx,by, bx+box,by+box);
            seg(bx,by+box, bx+box,by);
            drawLines(uiMVP(Mat4::identity()), lines);
        }
    }
    // Static value text
    else if (valueStr != nullptr) {
        setColor(0.5f,0.8f,1.0f,1.0f);
        std::vector<float> textTris;
        addTextTriangles(textTris, valX, yPos, valueStr);
        drawTriangles(uiMVP(Mat4::identity()), textTris);
    }
}

static void drawUI() {
    // UI draw: alpha blended, pixel projection already set in MVP calls
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Panel background
    setColor(0.0f, 0.05f, 0.1f, 0.75f);
    drawQuad(uiMVP(rectModelPX(UI_X_START_PX, UI_Y_START_PX, UI_PANEL_WIDTH_PX, UI_PANEL_HEIGHT_PX)));

    // Title bar
    setColor(0.0f, 0.2f, 0.4f, 1.0f);
    drawQuad(uiMVP(rectModelPX(UI_X_START_PX, UI_Y_START_PX, UI_PANEL_WIDTH_PX, UI_HEADER_H_PX)));

    // Title text
    setColor(1,1,1,1);
    {
        std::vector<float> textTris;
        addTextTriangles(textTris, UI_X_START_PX + 90.0f, UI_Y_START_PX + 14.0f, "Interface");
        drawTriangles(uiMVP(Mat4::identity()), textTris);
    }

    // Items
    char buffer[64];
    int row = 0;

    drawTreeRowPX(row++, 0, "- Frame Stats", nullptr, false, nullptr,0,0, false,nullptr);

    std::snprintf(buffer, sizeof(buffer), "%.2f", fps);
    drawTreeRowPX(row++, 1, "Frame Rate", buffer, false, nullptr,0,0, false,nullptr);

    std::snprintf(buffer, sizeof(buffer), "%.4f", frameTime / 1000.0f);
    drawTreeRowPX(row++, 1, "Frame Time", buffer, false, nullptr,0,0, false,nullptr);

    drawTreeRowPX(row++, 0, "- Display", nullptr, false, nullptr,0,0, false,nullptr);

    drawTreeRowPX(row++, 1, "Wireframe", nullptr, false, nullptr,0,0, true, &wireframe);

    drawTreeRowPX(row++, 1, "- Background", nullptr, false, nullptr,0,0, false,nullptr);

    std::snprintf(buffer, sizeof(buffer), "%d", (int)(bgColor[0] * 255));
    drawTreeRowPX(row++, 2, "Red", buffer, true, &bgColor[0], 0.0f, 1.0f, false,nullptr);

    std::snprintf(buffer, sizeof(buffer), "%d", (int)(bgColor[1] * 255));
    drawTreeRowPX(row++, 2, "Green", buffer, true, &bgColor[1], 0.0f, 1.0f, false,nullptr);

    std::snprintf(buffer, sizeof(buffer), "%d", (int)(bgColor[2] * 255));
    drawTreeRowPX(row++, 2, "Blue", buffer, true, &bgColor[2], 0.0f, 1.0f, false,nullptr);

    drawTreeRowPX(row++, 2, "Mode", "RGB", false, nullptr,0,0, false,nullptr);

    drawTreeRowPX(row++, 0, "- Controls", nullptr, false, nullptr,0,0, false,nullptr);

    std::snprintf(buffer, sizeof(buffer), "%.1f", groundAngle);
    drawTreeRowPX(row++, 1, "Ground Slope", buffer, true, &groundAngle, GROUND_MIN, GROUND_MAX, false,nullptr);

    std::snprintf(buffer, sizeof(buffer), "%.3f", truckX);
    drawTreeRowPX(row++, 1, "Position", buffer, false, nullptr,0,0, false,nullptr);

    glDisable(GL_BLEND);
}

// ---------------- Rendering (Modern) ----------------
static void display() {
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    useProgram();

    // Wireframe affects triangle drawing (scene). We'll set for scene only.
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

    // ---------- Scene (NDC space) ----------
    // Unified scene transform (pivot at (1.0, -0.5))
    Mat4 scene = Mat4::identity();
    scene = mul(scene, Mat4::translate( 1.0f, -0.5f));
    scene = mul(scene, Mat4::rotateZ(groundAngle));
    scene = mul(scene, Mat4::translate(-1.0f,  0.5f));

    // Ground
    setColor(0.2f,0.6f,0.2f,1.0f);
    drawQuad(sceneMVP(mul(scene, rectModelNDC(-1.0f, -0.8f, 2.0f, 0.3f))));

    // Truck (child of ground rotation)
    Mat4 truck = mul(scene, Mat4::translate(truckX, -0.5f + wheelRadius, 0.0f));

    // Body parts (same coordinates you had)
    setColor(0.3f,0.3f,0.8f,1.0f);
    drawQuad(sceneMVP(mul(truck, rectModelNDC(-0.5f, 0.0f, 1.0f, 0.15f))));

    setColor(0.4f,0.4f,0.9f,1.0f);
    drawQuad(sceneMVP(mul(truck, rectModelNDC(-0.5f, 0.15f, 0.4f, 0.2f))));

    setColor(0.5f,0.5f,1.0f,1.0f);
    drawQuad(sceneMVP(mul(truck, rectModelNDC(-0.1f, 0.15f, 0.4f, 0.25f))));

    setColor(0.7f,0.9f,1.0f,1.0f);
    drawQuad(sceneMVP(mul(truck, rectModelNDC(-0.05f, 0.25f, 0.3f, 0.15f))));

    // Wheels (solid fill unless global wireframe is on)
    if (!wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Back wheel
    {
        Mat4 w = mul(truck, Mat4::translate(-0.3f, 0.0f, 0.0f));
        w = mul(w, Mat4::rotateZ(wheelAngle));
        // outer
        setColor(0.1f,0.1f,0.1f,1.0f);
        drawCircle(sceneMVP(mul(w, Mat4::scale(wheelRadius, wheelRadius, 1.0f))));
        // inner
        setColor(0.8f,0.8f,0.8f,1.0f);
        drawCircle(sceneMVP(mul(w, Mat4::scale(wheelRadius*0.5f, wheelRadius*0.5f, 1.0f))));
    }
    // Front wheel
    {
        Mat4 w = mul(truck, Mat4::translate(0.3f, 0.0f, 0.0f));
        w = mul(w, Mat4::rotateZ(wheelAngle));
        setColor(0.1f,0.1f,0.1f,1.0f);
        drawCircle(sceneMVP(mul(w, Mat4::scale(wheelRadius, wheelRadius, 1.0f))));
        setColor(0.8f,0.8f,0.8f,1.0f);
        drawCircle(sceneMVP(mul(w, Mat4::scale(wheelRadius*0.5f, wheelRadius*0.5f, 1.0f))));
    }

    // Restore wireframe if needed
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

    // ---------- UI (pixel space) ----------
    // Ensure UI looks normal (filled)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawUI();

    glutSwapBuffers();
}

// ---------------- Update ----------------
static void idle() {
    int t = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (t - prevTime) / 1000.0f;
    prevTime = t;

    if (deltaTime > 0.0f) {
        fps = 0.9f * fps + 0.1f * (1.0f / deltaTime);
        frameTime = deltaTime * 1000.0f;
    }
    glutPostRedisplay();
}

// ---------------- Input ----------------
static void specialKeys(int key, int, int) {
    switch (key) {
        case GLUT_KEY_LEFT:
            truckX -= truckSpeed * deltaTime;
            wheelAngle += 180.0f * deltaTime * truckSpeed;
            break;
        case GLUT_KEY_RIGHT:
            truckX += truckSpeed * deltaTime;
            wheelAngle -= 180.0f * deltaTime * truckSpeed;
            break;
        case GLUT_KEY_UP:
            groundAngle += 15.0f * deltaTime;
            if (groundAngle > GROUND_MAX) groundAngle = GROUND_MAX;
            break;
        case GLUT_KEY_DOWN:
            groundAngle -= 15.0f * deltaTime;
            if (groundAngle < GROUND_MIN) groundAngle = GROUND_MIN;
            break;
    }
}

static void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'w': wireframe = !wireframe; break;
        case 27: std::exit(0); break;
    }
}

static void mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON) return;

    if (state == GLUT_DOWN) {
        uiState.isDragging = true;

        // x,y already in pixel coords with origin top-left (matches our UI proj)
        // Determine row (matches drawUI order)
        float relY = (float)y - (UI_Y_START_PX + UI_HEADER_OFFSET_PX);
        int row = (int)(relY / UI_LINE_HEIGHT_PX);

        // within panel?
        if (x > UI_X_START_PX && x < UI_X_START_PX + UI_PANEL_WIDTH_PX && row >= 0) {
            if (row == 4) wireframe = !wireframe; // Wireframe toggle row

            if (row == 6) uiState.activeItemID = 0; // Red slider
            if (row == 7) uiState.activeItemID = 1; // Green slider
            if (row == 8) uiState.activeItemID = 2; // Blue slider
            if (row == 11) uiState.activeItemID = 3; // Ground slope slider
        }
    } else if (state == GLUT_UP) {
        uiState.isDragging = false;
        uiState.activeItemID = -1;
    }
}

static void motion(int x, int y) {
    if (!uiState.isDragging || uiState.activeItemID == -1) return;

    // Decide slider indentation level by item
    int level = (uiState.activeItemID <= 2) ? 2 : 1;

    // This matches drawTreeRowPX layout:
    // xPos = UI_X_START + level*indent
    // valX = xPos + VALUE_COL
    // barX = valX + BAR_OFFSET
    float xPos = UI_X_START_PX + level * UI_INDENT_PX;
    float valX = xPos + UI_VALUE_COL_PX;
    float sliderStart = valX + UI_BAR_OFFSET_PX;
    float sliderWidth = UI_BAR_W_PX;

    float val = ((float)x - sliderStart) / sliderWidth;
    val = clampf(val, 0.0f, 1.0f);

    if (uiState.activeItemID == 0) bgColor[0] = val;
    if (uiState.activeItemID == 1) bgColor[1] = val;
    if (uiState.activeItemID == 2) bgColor[2] = val;

    if (uiState.activeItemID == 3) {
        groundAngle = GROUND_MIN + val * (GROUND_MAX - GROUND_MIN);
    }

    glutPostRedisplay();
}

// ---------------- GL init ----------------
static void createQuadMesh() {
    // unit quad: (0,0)-(1,1) as 2 triangles
    float v[] = {
        0,0,  1,0,  1,1,
        0,0,  1,1,  0,1
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindVertexArray(0);
}

static void createCircleMesh(int segments=32) {
    std::vector<float> v;
    v.reserve((segments+2)*2);
    v.push_back(0.0f); v.push_back(0.0f); // center

    for (int i=0; i<=segments; i++) {
        float a = 2.0f * 3.1415926535f * (float)i / (float)segments;
        v.push_back(std::cos(a));
        v.push_back(std::sin(a));
    }
    circleVertexCount = (int)(v.size()/2);

    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);

    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(float), v.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindVertexArray(0);
}

static void createDynamicBuffers() {
    // Lines
    glGenVertexArrays(1, &dynLineVAO);
    glGenBuffers(1, &dynLineVBO);
    glBindVertexArray(dynLineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, dynLineVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    // Triangles (text)
    glGenVertexArrays(1, &dynTriVAO);
    glGenBuffers(1, &dynTriVBO);
    glBindVertexArray(dynTriVAO);
    glBindBuffer(GL_ARRAY_BUFFER, dynTriVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindVertexArray(0);
}

static void initGL() {
    // Shader sources
    const char* vsSrc = R"GLSL(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        uniform mat4 uMVP;
        void main() {
            gl_Position = uMVP * vec4(aPos, 0.0, 1.0);
        }
    )GLSL";

    const char* fsSrc = R"GLSL(
        #version 330 core
        uniform vec4 uColor;
        out vec4 FragColor;
        void main() {
            FragColor = uColor;
        }
    )GLSL";

    GLuint vs = compileShader(GL_VERTEX_SHADER, vsSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
    gProgram = linkProgram(vs, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    useProgram();
    uMVP_loc = glGetUniformLocation(gProgram, "uMVP");
    uColor_loc = glGetUniformLocation(gProgram, "uColor");

    createQuadMesh();
    createCircleMesh(32);
    createDynamicBuffers();

    // UI projection: origin top-left (0,0), y increases down
    gUIProj = Mat4::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);

    // Core settings
    glDisable(GL_DEPTH_TEST); // 2D only
}

// ---------------- Main ----------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);

    // Request OpenGL 3.3 Core profile context (freeglut must support it)
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
#ifdef GLUT_FORWARD_COMPATIBLE
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
#endif

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("u1234567 12345678 (OpenGL 3.3 Core)");

    if (!gladLoadGLLoader((GLADloadproc)glutGetProcAddress)) {
        std::fprintf(stderr, "Failed to initialize GLAD.\n");
        return 1;
    }

    const char* ver = (const char*)glGetString(GL_VERSION);
    const char* ren = (const char*)glGetString(GL_RENDERER);
    const char* ven = (const char*)glGetString(GL_VENDOR);

    std::printf("GL_VERSION : %s\n", ver ? ver : "(null)");
    std::printf("GL_VENDOR  : %s\n", ven ? ven : "(null)");
    std::printf("GL_RENDERER: %s\n", ren ? ren : "(null)");

    int major=0, minor=0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::printf("GL %d.%d\n", major, minor);

    if (major < 3 || (major == 3 && minor < 3)) {
        std::fprintf(stderr, "ERROR: Need OpenGL 3.3 core, but got %d.%d\n", major, minor);
        std::fprintf(stderr, "Likely your freeglut build cannot create a 3.3 core context.\n");
        std::exit(1);
    }


    initGL();

    prevTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}
