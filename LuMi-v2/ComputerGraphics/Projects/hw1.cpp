#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <string>

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
float bgColor[3] = { 0.2f, 0.2f, 0.2f };

// ---------------- UI State ----------------
struct UIInteractionState {
    int activeItemID = -1; // ID of the element currently being dragged
    bool isDragging = false;
} uiState;

// Layout constants for the Tree View UI
const float UI_X_START = -0.98f;
const float UI_Y_START = 0.95f;
const float UI_LINE_HEIGHT = 0.08f; 
const float UI_INDENT = 0.05f;
const float UI_PANEL_WIDTH = 0.7f;
const float UI_PANEL_HEIGHT = 1.2f;

// ---------------- Utility ----------------
void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_12)
{
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, text[i]);
    }
}

void drawRect(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
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

// ---------------- UI Implementation ----------------

// Helper to draw the dotted lines for the tree structure
void drawTreeLines(float x, float y, float height, float width) {
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(1, 0xAAAA); // Dotted line pattern
    glEnable(GL_LINE_STIPPLE);
    
    glColor3f(0.5f, 0.5f, 0.5f); // Grey lines
    glBegin(GL_LINES);
    // Vertical line segment
    glVertex2f(x, y);
    glVertex2f(x, y - height);
    // Horizontal connector
    glVertex2f(x, y - height/2.0f);
    glVertex2f(x + width, y - height/2.0f);
    glEnd();
    
    glPopAttrib();
}

// Render a single row in the tree
void drawTreeRow(int row_index, int level, const char* label, const char* valueStr, bool isSlider, float* valueRef, float minV, float maxV, bool isToggle, bool* toggleRef) {
    
    float yPos = UI_Y_START - (row_index * UI_LINE_HEIGHT) - 0.1f; // Offset for header
    float xPos = UI_X_START + (level * UI_INDENT);
    
    // 1. Draw Tree Structure Lines (if not root)
    if (level > 0) {
        float lineX = UI_X_START + ((level-1) * UI_INDENT) + 0.02f;
        drawTreeLines(lineX, yPos + 0.02f, UI_LINE_HEIGHT, UI_INDENT - 0.02f);
    }

    // 2. Draw Label
    // Yellow for categories, White for leaves
    if (valueStr == NULL && !isToggle && !isSlider) glColor3f(1.0f, 1.0f, 0.0f); 
    else glColor3f(0.9f, 0.9f, 0.9f);
    
    drawText(xPos + 0.02f, yPos, label);

    // 3. Draw Interactable Elements
    float valX = xPos + 0.25f; // Fixed column for values
    
    // --- Slider Logic ---
    if (isSlider && valueRef) {
        // Draw the value text
        glColor3f(0.5f, 0.8f, 1.0f); // Cyan for values
        drawText(valX, yPos, valueStr);
        
        // Draw a small visual bar underneath or next to it to represent the slider
        float barW = 0.15f;
        float barH = 0.015f;
        float barX = valX + 0.12f;
        
        // Background track
        glColor3f(0.3f, 0.3f, 0.3f);
        drawRect(barX, yPos, barW, barH);
        
        // Fill based on value
        float norm = (*valueRef - minV) / (maxV - minV);
        if(norm < 0) norm = 0; if(norm > 1) norm = 1;
        
        glColor3f(0.0f, 0.7f, 0.0f); // Green fill
        drawRect(barX, yPos, barW * norm, barH);
    }
    // --- Toggle Logic ---
    else if (isToggle && toggleRef) {
        // Draw Checkbox
        float boxSz = 0.03f;
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(valX, yPos);
        glVertex2f(valX + boxSz, yPos);
        glVertex2f(valX + boxSz, yPos + boxSz);
        glVertex2f(valX, yPos + boxSz);
        glEnd();
        
        if (*toggleRef) {
            // Draw Check
            glBegin(GL_LINES);
            glVertex2f(valX, yPos);
            glVertex2f(valX + boxSz, yPos + boxSz);
            
            glVertex2f(valX, yPos + boxSz);
            glVertex2f(valX + boxSz, yPos);
            glEnd();
        }
    }
    // --- Static Text ---
    else if (valueStr != NULL) {
        glColor3f(0.5f, 0.8f, 1.0f);
        drawText(valX, yPos, valueStr);
    }
}

void drawUI()
{
    // Setup 2D overlay
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);

    // 1. Draw Main Panel Background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.05f, 0.1f, 0.75f); // Dark translucent blue/black
    drawRect(UI_X_START, UI_Y_START - UI_PANEL_HEIGHT, UI_PANEL_WIDTH, UI_PANEL_HEIGHT + 0.05f);
    
    // 2. Window Title Bar "Interface"
    glColor4f(0.0f, 0.2f, 0.4f, 1.0f);
    drawRect(UI_X_START, UI_Y_START, UI_PANEL_WIDTH, 0.05f);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(UI_X_START + 0.2f, UI_Y_START + 0.015f, "Interface");
    
    // 3. Draw Tree Items
    char buffer[32];
    int row = 0;
    
    // --- Frame Stats ---
    drawTreeRow(row++, 0, "- Frame Stats", NULL, false, NULL, 0, 0, false, NULL);
    
    sprintf(buffer, "%.2f", fps);
    drawTreeRow(row++, 1, "Frame Rate", buffer, false, NULL, 0, 0, false, NULL);
    
    sprintf(buffer, "%.4f", frameTime / 1000.0f);
    drawTreeRow(row++, 1, "Frame Time", buffer, false, NULL, 0, 0, false, NULL);
    
    // --- Display ---
    drawTreeRow(row++, 0, "- Display", NULL, false, NULL, 0, 0, false, NULL);
    
    // Wireframe
    drawTreeRow(row++, 1, "Wireframe", NULL, false, NULL, 0, 0, true, &wireframe);
    
    // Background Group
    drawTreeRow(row++, 1, "- Background", NULL, false, NULL, 0, 0, false, NULL);
    
    // Colors (0-255 display)
    sprintf(buffer, "%d", (int)(bgColor[0] * 255));
    drawTreeRow(row++, 2, "Red", buffer, true, &bgColor[0], 0.0f, 1.0f, false, NULL);
    
    sprintf(buffer, "%d", (int)(bgColor[1] * 255));
    drawTreeRow(row++, 2, "Green", buffer, true, &bgColor[1], 0.0f, 1.0f, false, NULL);
    
    sprintf(buffer, "%d", (int)(bgColor[2] * 255));
    drawTreeRow(row++, 2, "Blue", buffer, true, &bgColor[2], 0.0f, 1.0f, false, NULL);
    
    drawTreeRow(row++, 2, "Mode", "RGB", false, NULL, 0, 0, false, NULL);
    
    // --- Controls ---
    drawTreeRow(row++, 0, "- Controls", NULL, false, NULL, 0, 0, false, NULL);
    
    sprintf(buffer, "%.1f", groundAngle);
    drawTreeRow(row++, 1, "Ground Slope", buffer, true, &groundAngle, GROUND_MIN, GROUND_MAX, false, NULL);
    
    sprintf(buffer, "%.3f", truckX);
    drawTreeRow(row++, 1, "Position", buffer, false, NULL, 0, 0, false, NULL);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    
    // Restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// ---------------- Rendering ----------------
void display()
{
    // Update background color
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
    
    // Clear both Color and Depth buffers (Good practice)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // CRITICAL FIX: Disable Depth Testing for the 2D Scene.
    // This ensures the Ground and Truck are drawn on top of the background
    // based purely on the order of code (Painter's Algorithm).
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // ==========================================
    // Unified Scene Transformation
    // ==========================================
    glPushMatrix();
        // 1. Pivot Point: Top-Right of the ground surface (1.0, -0.5)
        glTranslatef(1.0f, -0.5f, 0.0f);   // Move pivot to origin
        glRotatef(groundAngle, 0, 0, 1);   // Rotate
        glTranslatef(-1.0f, 0.5f, 0.0f);   // Move pivot back

        // 2. Draw Ground
        glColor3f(0.2f, 0.6f, 0.2f);
        drawRect(-1.0f, -0.8f, 2.0f, 0.3f);

        // 3. Draw Truck (Child of Ground Rotation)
        glPushMatrix();
            // Position truck: X = distance along slope, Y = surface height + wheel radius
            // This ensures the wheels sit exactly on the line y = -0.5
            glTranslatef(truckX, -0.5f + wheelRadius, 0.0f);

            // Truck Body
            glColor3f(0.3f, 0.3f, 0.8f); // Base
            drawRect(-0.5f, 0.0f, 1.0f, 0.15f);
            
            glColor3f(0.4f, 0.4f, 0.9f); // Cabin
            drawRect(-0.5f, 0.15f, 0.4f, 0.2f);
            
            glColor3f(0.5f, 0.5f, 1.0f); // Cabin Top
            drawRect(-0.1f, 0.15f, 0.4f, 0.25f);
            
            glColor3f(0.7f, 0.9f, 1.0f); // Back
            drawRect(-0.05f, 0.25f, 0.3f, 0.15f);
            
            // Draw Wheels (Force solid fill unless global wireframe is on)
            if (!wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // Back Wheel
            glPushMatrix();
                glTranslatef(-0.3f, 0.0f, 0.0f);
                glRotatef(wheelAngle, 0, 0, 1);
                glColor3f(0.1f, 0.1f, 0.1f);
                drawCircle(0, 0, wheelRadius);
                glColor3f(0.8f, 0.8f, 0.8f);
                drawCircle(0, 0, wheelRadius * 0.5f);
            glPopMatrix();
            
            // Front Wheel
            glPushMatrix();
                glTranslatef(0.3f, 0.0f, 0.0f);
                glRotatef(wheelAngle, 0, 0, 1);
                glColor3f(0.1f, 0.1f, 0.1f);
                drawCircle(0, 0, wheelRadius);
                glColor3f(0.8f, 0.8f, 0.8f);
                drawCircle(0, 0, wheelRadius * 0.5f);
            glPopMatrix();

            // Restore Wireframe state if needed
            if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPopMatrix(); // End Truck

    glPopMatrix(); // End Scene Rotation

    // Draw UI Overlay last
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

void mouse(int button, int state, int x, int y)
{
    float glX, glY;
    screenToGL(x, y, glX, glY);
    
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            uiState.isDragging = true;
            
            // FIX: Adjusted the offset from +0.05f to -0.02f
            // This aligns the click detection with the visual draw position (-0.1f offset).
            // Formula: row = (Top_Of_Row_0 - glY) / Row_Height
            float relY = UI_Y_START - glY;
            int row = (int)((relY - 0.02f) / UI_LINE_HEIGHT); 
            
            // Check if click is within the UI panel width
            if (glX > UI_X_START && glX < UI_X_START + UI_PANEL_WIDTH && row >= 0)
            {
                // Map rows to functions (Corresponds to drawUI order)
                // Row 4: Wireframe Toggle
                if (row == 4) wireframe = !wireframe;
                
                // Row 6: Red Slider
                if (row == 6) uiState.activeItemID = 0; 
                // Row 7: Green Slider
                if (row == 7) uiState.activeItemID = 1; 
                // Row 8: Blue Slider
                if (row == 8) uiState.activeItemID = 2; 
                
                // Row 11: Ground Slope Slider
                if (row == 11) uiState.activeItemID = 3; 
            }
        }
        else if (state == GLUT_UP)
        {
            uiState.isDragging = false;
            uiState.activeItemID = -1;
        }
    }
}

void motion(int x, int y)
{
    float glX, glY;
    screenToGL(x, y, glX, glY);
    
    if (uiState.isDragging && uiState.activeItemID != -1)
    {
        // Define where the slider track is visually located on the screen
        // These numbers align with the layout in drawTreeRow/drawUI
        float sliderStart = UI_X_START + (2 * UI_INDENT) + 0.25f + 0.12f; 
        float sliderWidth = 0.15f;
        
        // Calculate value 0.0 to 1.0
        float val = (glX - sliderStart) / sliderWidth;
        if (val < 0) val = 0;
        if (val > 1) val = 1;
        
        if (uiState.activeItemID == 0) bgColor[0] = val;
        if (uiState.activeItemID == 1) bgColor[1] = val;
        if (uiState.activeItemID == 2) bgColor[2] = val;
        
        if (uiState.activeItemID == 3) {
            // Slope has a custom range (-15 to 15)
            groundAngle = GROUND_MIN + val * (GROUND_MAX - GROUND_MIN);
        }
        
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case 'w': wireframe = !wireframe; break;
        case 27: exit(0); break; // ESC
    }
}

// ---------------- Init ----------------
void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    
    // UI is immediate mode, no init required
}

// ---------------- Main ----------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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