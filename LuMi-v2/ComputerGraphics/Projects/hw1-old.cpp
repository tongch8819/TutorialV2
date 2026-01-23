#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>

// ---------------- Window ----------------
const int WIDTH = 800;
const int HEIGHT = 800;

// ---------------- Timing ----------------
int prevTime = 0;
float deltaTime = 0.0f;
float fps = 60.0f;
float frameTime = 16.67f;

// ---------------- Scene state ----------------
float truckX = 0.0f;
float truckSpeed = 2.0f;

float wheelAngle = 0.0f;
float wheelRadius = 0.07f;

float groundAngle = 0.0f;
const float GROUND_MIN = -15.0f;
const float GROUND_MAX = 15.0f;

bool wireframe = false;
float bgColor[3] = {0.2f, 0.2f, 0.2f};

// ---------------- UI State ----------------
struct UISlider {
    float x, y, width, height;
    float* value;
    float minVal, maxVal;
    bool isDragging;
    std::string name;
};

struct UIButton {
    float x, y, width, height;
    bool* toggleValue;
    std::string name;
    std::string onText;
    std::string offText;
};

std::vector<UISlider> sliders;
std::vector<UIButton> buttons;

int activeSlider = -1;
bool mouseDown = false;

// ---------------- Utility ----------------
void drawRect(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

void drawRoundedRect(float x, float y, float w, float h, float radius)
{
    const int segments = 8;
    glBegin(GL_POLYGON);
    
    // Top left corner
    for (int i = 0; i <= segments; i++)
    {
        float angle = 3.14159f * 1.5f + (3.14159f / 2.0f) * i / segments;
        glVertex2f(x + radius + cosf(angle) * radius, y + h - radius + sinf(angle) * radius);
    }
    
    // Top right corner
    for (int i = 0; i <= segments; i++)
    {
        float angle = 0.0f + (3.14159f / 2.0f) * i / segments;
        glVertex2f(x + w - radius + cosf(angle) * radius, y + h - radius + sinf(angle) * radius);
    }
    
    // Bottom right corner
    for (int i = 0; i <= segments; i++)
    {
        float angle = 3.14159f / 2.0f + (3.14159f / 2.0f) * i / segments;
        glVertex2f(x + w - radius + cosf(angle) * radius, y + radius + sinf(angle) * radius);
    }
    
    // Bottom left corner
    for (int i = 0; i <= segments; i++)
    {
        float angle = 3.14159f + (3.14159f / 2.0f) * i / segments;
        glVertex2f(x + radius + cosf(angle) * radius, y + radius + sinf(angle) * radius);
    }
    
    glEnd();
}

void drawCircle(float cx, float cy, float r, int segments = 16)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++)
    {
        float a = 2.0f * 3.1415926f * i / segments;
        glVertex2f(cx + cosf(a) * r, cy + sinf(a) * r);
    }
    glEnd();
}

void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_12)
{
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, text[i]);
    }
}

void drawTextRight(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_12)
{
    // Calculate text width
    int width = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        width += glutBitmapWidth(font, text[i]);
    }
    float pixelWidth = width / 400.0f; // Approximate conversion
    glRasterPos2f(x - pixelWidth, y);
    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, text[i]);
    }
}

void initUI()
{
    // Clear existing UI elements
    sliders.clear();
    buttons.clear();
    
    // Ground Slope Slider
    UISlider groundSlider;
    groundSlider.x = 0.65f;
    groundSlider.y = 0.16f;
    groundSlider.width = 0.15f;
    groundSlider.height = 0.02f;
    groundSlider.value = &groundAngle;
    groundSlider.minVal = GROUND_MIN;
    groundSlider.maxVal = GROUND_MAX;
    groundSlider.isDragging = false;
    groundSlider.name = "Ground Slope";
    sliders.push_back(groundSlider);
    
    // Red Slider
    UISlider redSlider;
    redSlider.x = 0.65f;
    redSlider.y = 0.47f;
    redSlider.width = 0.15f;
    redSlider.height = 0.02f;
    redSlider.value = &bgColor[0];
    redSlider.minVal = 0.0f;
    redSlider.maxVal = 1.0f;
    redSlider.isDragging = false;
    redSlider.name = "Red";
    sliders.push_back(redSlider);
    
    // Green Slider
    UISlider greenSlider;
    greenSlider.x = 0.65f;
    greenSlider.y = 0.41f;
    greenSlider.width = 0.15f;
    greenSlider.height = 0.02f;
    greenSlider.value = &bgColor[1];
    greenSlider.minVal = 0.0f;
    greenSlider.maxVal = 1.0f;
    greenSlider.isDragging = false;
    greenSlider.name = "Green";
    sliders.push_back(greenSlider);
    
    // Blue Slider
    UISlider blueSlider;
    blueSlider.x = 0.65f;
    blueSlider.y = 0.35f;
    blueSlider.width = 0.15f;
    blueSlider.height = 0.02f;
    blueSlider.value = &bgColor[2];
    blueSlider.minVal = 0.0f;
    blueSlider.maxVal = 1.0f;
    blueSlider.isDragging = false;
    blueSlider.name = "Blue";
    sliders.push_back(blueSlider);
    
    // Wireframe Button
    UIButton wireframeButton;
    wireframeButton.x = 0.82f;
    wireframeButton.y = 0.62f;
    wireframeButton.width = 0.08f;
    wireframeButton.height = 0.03f;
    wireframeButton.toggleValue = &wireframe;
    wireframeButton.name = "Wireframe";
    wireframeButton.onText = "ON";
    wireframeButton.offText = "OFF";
    buttons.push_back(wireframeButton);
}

// ---------------- UI Rendering ----------------
void drawUI()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // Main UI panel background
    glColor3f(0.1f, 0.1f, 0.15f);
    drawRoundedRect(0.6f, -0.95f, 0.35f, 1.9f, 0.02f);
    
    // Section headers with proper spacing
    float currentY = 0.85f;
    
    // ===== Frame Stats Section =====
    glColor3f(0.2f, 0.2f, 0.3f);
    drawRect(0.62f, currentY - 0.05f, 0.31f, 0.05f);
    glColor3f(0.8f, 0.8f, 1.0f);
    drawText(0.65f, currentY - 0.025f, "Frame Stats");
    
    // Frame Rate
    glColor3f(0.9f, 0.9f, 0.9f);
    drawText(0.65f, currentY - 0.10f, "Frame Rate:");
    char buffer[50];
    sprintf(buffer, "%.2f", fps);
    glColor3f(0.7f, 0.8f, 1.0f);
    drawTextRight(0.90f, currentY - 0.10f, buffer);
    
    // Frame Time
    glColor3f(0.9f, 0.9f, 0.9f);
    drawText(0.65f, currentY - 0.15f, "Frame Time:");
    sprintf(buffer, "%.4f ms", frameTime);
    glColor3f(0.7f, 0.8f, 1.0f);
    drawTextRight(0.90f, currentY - 0.15f, buffer);
    
    currentY -= 0.25f;
    
    // ===== Display Section =====
    glColor3f(0.2f, 0.2f, 0.3f);
    drawRect(0.62f, currentY - 0.05f, 0.31f, 0.05f);
    glColor3f(0.8f, 0.8f, 1.0f);
    drawText(0.65f, currentY - 0.025f, "Display");
    
    // Wireframe label
    glColor3f(0.9f, 0.9f, 0.9f);
    drawText(0.65f, currentY - 0.10f, "Wireframe:");
    
    // Wireframe button
    for (const auto& button : buttons)
    {
        if (button.name == "Wireframe")
        {
            // Button background
            glColor3f(*(button.toggleValue) ? 0.3f : 0.1f, 
                     *(button.toggleValue) ? 0.6f : 0.1f, 
                     *(button.toggleValue) ? 0.3f : 0.1f);
            drawRoundedRect(button.x, button.y, button.width, button.height, 0.01f);
            
            // Button text centered
            glColor3f(1.0f, 1.0f, 1.0f);
            const char* buttonText = *(button.toggleValue) ? button.onText.c_str() : button.offText.c_str();
            float textWidth = strlen(buttonText) * 0.006f; // Approximate width
            drawText(button.x + (button.width - textWidth) / 2.0f, button.y + 0.01f, buttonText);
            
            // Show button value aligned right
            glColor3f(0.7f, 0.8f, 1.0f);
            drawTextRight(0.90f, currentY - 0.10f, buttonText);
        }
    }
    
    currentY -= 0.20f;
    
    // ===== Background Section =====
    glColor3f(0.2f, 0.2f, 0.3f);
    drawRect(0.62f, currentY - 0.05f, 0.31f, 0.05f);
    glColor3f(0.8f, 0.8f, 1.0f);
    drawText(0.65f, currentY - 0.025f, "Background");
    
    float bgStartY = currentY - 0.10f;
    
    // Red
    glColor3f(0.9f, 0.9f, 0.9f);
    drawText(0.65f, bgStartY, "Red:");
    sprintf(buffer, "%d", (int)(bgColor[0] * 255));
    glColor3f(0.7f, 0.8f, 1.0f);
    drawTextRight(0.90f, bgStartY, buffer);
    
    // Green
    glColor3f(0.9f, 0.9f, 0.9f);
    drawText(0.65f, bgStartY - 0.06f, "Green:");
    sprintf(buffer, "%d", (int)(bgColor[1] * 255));
    glColor3f(0.7f, 0.8f, 1.0f);
    drawTextRight(0.90f, bgStartY - 0.06f, buffer);
    
    // Blue
    glColor3f(0.9f, 0.9f, 0.9f);
    drawText(0.65f, bgStartY - 0.12f, "Blue:");
    sprintf(buffer, "%d", (int)(bgColor[2] * 255));
    glColor3f(0.7f, 0.8f, 1.0f);
    drawTextRight(0.90f, bgStartY - 0.12f, buffer);
    
    // Mode
    glColor3f(0.9f, 0.9f, 0.9f);
    drawText(0.65f, bgStartY - 0.18f, "Mode:");
    glColor3f(0.7f, 0.8f, 1.0f);
    drawTextRight(0.90f, bgStartY - 0.18f, "RGB");
    
    // Draw RGB sliders
    for (const auto& slider : sliders)
    {
        if (slider.name == "Red" || slider.name == "Green" || slider.name == "Blue")
        {
            // Slider track
            glColor3f(0.3f, 0.3f, 0.4f);
            drawRect(slider.x, slider.y, slider.width, slider.height);
            
            // Slider handle position
            float normalizedValue = (*slider.value - slider.minVal) / (slider.maxVal - slider.minVal);
            normalizedValue = std::max(0.0f, std::min(1.0f, normalizedValue));
            float handleX = slider.x + normalizedValue * slider.width;
            float handleY = slider.y + slider.height / 2.0f;
            
            // Slider handle
            glColor3f(0.5f, 0.6f, 1.0f);
            if (slider.isDragging)
                glColor3f(0.7f, 0.8f, 1.0f);
            drawCircle(handleX, handleY, 0.012f);
            
            // Color preview bar
            float previewX = 0.82f;
            float previewWidth = 0.08f;
            
            // Background of color preview
            glColor3f(0.2f, 0.2f, 0.2f);
            drawRect(previewX, slider.y - 0.008f, previewWidth, slider.height + 0.016f);
            
            // Color bar
            float colorValue = *slider.value;
            if (slider.name == "Red")
                glColor3f(colorValue, 0, 0);
            else if (slider.name == "Green")
                glColor3f(0, colorValue, 0);
            else if (slider.name == "Blue")
                glColor3f(0, 0, colorValue);
            
            drawRect(previewX + 0.002f, slider.y - 0.006f, 
                    colorValue * (previewWidth - 0.004f), 
                    slider.height + 0.012f);
        }
    }
    
    currentY = bgStartY - 0.24f;
    
    // ===== Controls Section =====
    glColor3f(0.2f, 0.2f, 0.3f);
    drawRect(0.62f, currentY - 0.05f, 0.31f, 0.05f);
    glColor3f(0.8f, 0.8f, 1.0f);
    drawText(0.65f, currentY - 0.025f, "Controls");
    
    // Ground Slope
    glColor3f(0.9f, 0.9f, 0.9f);
    drawText(0.65f, currentY - 0.10f, "Ground Slope:");
    sprintf(buffer, "%.1f", groundAngle);
    glColor3f(0.7f, 0.8f, 1.0f);
    drawTextRight(0.90f, currentY - 0.10f, buffer);
    
    // Position
    glColor3f(0.9f, 0.9f, 0.9f);
    drawText(0.65f, currentY - 0.15f, "Position:");
    sprintf(buffer, "%.3f", truckX);
    glColor3f(0.7f, 0.8f, 1.0f);
    drawTextRight(0.90f, currentY - 0.15f, buffer);
    
    // Draw Ground Slope slider
    for (const auto& slider : sliders)
    {
        if (slider.name == "Ground Slope")
        {
            // Slider track
            glColor3f(0.3f, 0.3f, 0.4f);
            drawRect(slider.x, slider.y, slider.width, slider.height);
            
            // Slider handle position
            float normalizedValue = (*slider.value - slider.minVal) / (slider.maxVal - slider.minVal);
            normalizedValue = std::max(0.0f, std::min(1.0f, normalizedValue));
            float handleX = slider.x + normalizedValue * slider.width;
            float handleY = slider.y + slider.height / 2.0f;
            
            // Slider handle
            glColor3f(0.5f, 0.6f, 1.0f);
            if (slider.isDragging)
                glColor3f(0.7f, 0.8f, 1.0f);
            drawCircle(handleX, handleY, 0.012f);
        }
    }
    
    // Current color preview at the bottom
    glColor3f(0.2f, 0.2f, 0.25f);
    drawRoundedRect(0.62f, -0.9f, 0.31f, 0.05f, 0.01f);
    
    glColor3f(0.8f, 0.8f, 1.0f);
    drawText(0.65f, -0.875f, "Current Color");
    
    glColor3f(bgColor[0], bgColor[1], bgColor[2]);
    drawRect(0.82f, -0.89f, 0.1f, 0.03f);
}

// ---------------- Rendering ----------------
void display()
{
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // ----- Ground -----
    glPushMatrix();
        glTranslatef(1.0f, -0.8f, 0.0f);
        glRotatef(groundAngle, 0, 0, 1);
        glTranslatef(-1.0f, 0.8f, 0.0f);
        
        glColor3f(0.2f, 0.6f, 0.2f);
        drawRect(-1.0f, -0.8f, 2.0f, 0.3f);
    glPopMatrix();

    // ----- Truck -----
    glPushMatrix();
        float groundRad = groundAngle * 3.1415926f / 180.0f;
        float truckYOnSlope = truckX * tan(groundRad);
        
        glTranslatef(truckX, truckYOnSlope - 0.5f, 0.0f);
        glRotatef(groundAngle, 0, 0, 1);

        // Truck components
        glColor3f(0.3f, 0.3f, 0.8f);
        drawRect(-0.5f, 0.0f, 1.0f, 0.15f);
        
        glColor3f(0.4f, 0.4f, 0.9f);
        drawRect(-0.5f, 0.15f, 0.4f, 0.2f);
        
        glColor3f(0.5f, 0.5f, 1.0f);
        drawRect(-0.1f, 0.15f, 0.4f, 0.25f);
        
        glColor3f(0.7f, 0.9f, 1.0f);
        drawRect(-0.05f, 0.25f, 0.3f, 0.15f);
        
        // Wheels with rotation
        glPushMatrix();
            glTranslatef(-0.3f, 0.0f, 0.0f);
            glRotatef(wheelAngle, 0, 0, 1);
            glColor3f(0.1f, 0.1f, 0.1f);
            drawCircle(0, 0, wheelRadius);
            glColor3f(0.8f, 0.8f, 0.8f);
            drawCircle(0, 0, wheelRadius * 0.5);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.3f, 0.0f, 0.0f);
            glRotatef(wheelAngle, 0, 0, 1);
            glColor3f(0.1f, 0.1f, 0.1f);
            drawCircle(0, 0, wheelRadius);
            glColor3f(0.8f, 0.8f, 0.8f);
            drawCircle(0, 0, wheelRadius * 0.5);
        glPopMatrix();
    glPopMatrix();

    // Draw UI
    drawUI();

    glutSwapBuffers();
}

// ---------------- Update ----------------
void idle()
{
    int t = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (t - prevTime) / 1000.0f;
    prevTime = t;

    if (deltaTime > 0.0f)
    {
        fps = 0.9f * fps + 0.1f * (1.0f / deltaTime);
        frameTime = deltaTime * 1000.0f;
    }

    glutPostRedisplay();
}

// ---------------- Input ----------------
void specialKeys(int key, int, int)
{
    switch (key)
    {
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

// Convert screen coordinates to OpenGL coordinates
void screenToGL(int screenX, int screenY, float& glX, float& glY)
{
    glX = (screenX / (float)WIDTH) * 2.0f - 1.0f;
    glY = 1.0f - (screenY / (float)HEIGHT) * 2.0f;
}

// Check if point is inside a rectangle
bool pointInRect(float px, float py, float rx, float ry, float rw, float rh)
{
    return px >= rx && px <= rx + rw && py >= ry && py <= ry + rh;
}

void mouse(int button, int state, int x, int y)
{
    float glX, glY;
    screenToGL(x, y, glX, glY);
    
    if (button == GLUT_LEFT_BUTTON)
    {
        mouseDown = (state == GLUT_DOWN);
        
        if (state == GLUT_DOWN)
        {
            // Check buttons
            for (size_t i = 0; i < buttons.size(); i++)
            {
                if (pointInRect(glX, glY, buttons[i].x, buttons[i].y, 
                               buttons[i].width, buttons[i].height))
                {
                    if (buttons[i].toggleValue)
                        *(buttons[i].toggleValue) = !*(buttons[i].toggleValue);
                    return;
                }
            }
            
            // Check sliders
            for (size_t i = 0; i < sliders.size(); i++)
            {
                // Check if click is near slider handle
                float normalizedValue = (*sliders[i].value - sliders[i].minVal) / 
                                       (sliders[i].maxVal - sliders[i].minVal);
                normalizedValue = std::max(0.0f, std::min(1.0f, normalizedValue));
                float handleX = sliders[i].x + normalizedValue * sliders[i].width;
                float handleY = sliders[i].y + sliders[i].height / 2.0f;
                
                float handleRadius = 0.015f;
                
                if (fabs(glX - handleX) < handleRadius && 
                    fabs(glY - handleY) < handleRadius)
                {
                    activeSlider = i;
                    sliders[i].isDragging = true;
                    return;
                }
                
                // Also allow clicking anywhere on slider track
                if (pointInRect(glX, glY, sliders[i].x, sliders[i].y - 0.015f, 
                               sliders[i].width, sliders[i].height + 0.03f))
                {
                    activeSlider = i;
                    sliders[i].isDragging = true;
                    // Update value immediately
                    float normalized = (glX - sliders[i].x) / sliders[i].width;
                    normalized = std::max(0.0f, std::min(1.0f, normalized));
                    *sliders[i].value = sliders[i].minVal + normalized * (sliders[i].maxVal - sliders[i].minVal);
                    return;
                }
            }
        }
        else // GLUT_UP
        {
            // Stop dragging
            if (activeSlider >= 0 && activeSlider < (int)sliders.size())
            {
                sliders[activeSlider].isDragging = false;
            }
            activeSlider = -1;
        }
    }
}

void motion(int x, int y)
{
    float glX, glY;
    screenToGL(x, y, glX, glY);
    
    if (mouseDown && activeSlider >= 0 && activeSlider < (int)sliders.size())
    {
        UISlider& slider = sliders[activeSlider];
        
        // Calculate normalized position
        float normalized = (glX - slider.x) / slider.width;
        normalized = std::max(0.0f, std::min(1.0f, normalized));
        
        // Update value
        *slider.value = slider.minVal + normalized * (slider.maxVal - slider.minVal);
        
        // Clamp ground angle
        if (slider.name == "Ground Slope")
        {
            *slider.value = std::max(GROUND_MIN, std::min(GROUND_MAX, *slider.value));
        }
    }
}

void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case 'w':
        case 'W':
            wireframe = !wireframe;
            break;
        case 'r':
        case 'R':
            bgColor[0] = 1.0f;
            bgColor[1] = 0.0f;
            bgColor[2] = 0.0f;
            break;
        case 'g':
        case 'G':
            bgColor[0] = 0.0f;
            bgColor[1] = 1.0f;
            bgColor[2] = 0.0f;
            break;
        case 'b':
        case 'B':
            bgColor[0] = 0.0f;
            bgColor[1] = 0.0f;
            bgColor[2] = 1.0f;
            break;
        case 'd':
        case 'D':
            bgColor[0] = bgColor[1] = bgColor[2] = 0.2f;
            break;
        case 27: // ESC
            exit(0);
    }
}

// ---------------- Init ----------------
void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    
    initUI();
}

// ---------------- Main ----------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    
    // Replace with your actual UOW username and student ID
    glutCreateWindow("u1234567 12345678");
    
    init();
    
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