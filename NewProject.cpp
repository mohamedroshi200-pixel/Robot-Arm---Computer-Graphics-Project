#define _CRT_SECURE_NO_WARNINGS

// ============================================================
// Robot Arm - Computer Graphics Project
// Using OpenGL (FreeGLUT) with GLU
// ============================================================
// Controls:
//   W/S - Move arm UP/DOWN (Translation Y)
//   A/D - Move arm LEFT/RIGHT (Translation X)
//   Q/E - Rotate Base (Rotation)
//   Z/X - Scale arm Bigger/Smaller (Scaling)
//   R   - Reflect arm (Reflection)
//   T/Y - Shear arm (Shearing)
//   SPACE - Reset all transformations
//   ESC   - Exit
// ============================================================

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Window dimensions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Transformation variables
float transX = 0.0f, transY = 0.0f;    // Translation
float rotation = 0.0f;                  // Rotation angle
float scaleX = 1.0f, scaleY = 1.0f;    // Scaling
float reflectX = 1.0f;                  // Reflection (1 or -1)
float shearX = 0.0f, shearY = 0.0f;    // Shearing

// Animation
bool animating = false;
float animAngle = 0.0f;

// Colors
GLfloat baseColor[] = { 0.3f, 0.3f, 0.7f };
GLfloat armColor[] = { 0.7f, 0.3f, 0.3f };
GLfloat jointColor[] = { 0.3f, 0.7f, 0.3f };
GLfloat handColor[] = { 0.8f, 0.8f, 0.2f };

// ============================================================
// Draw Functions
// ============================================================

void drawRectangle(float x, float y, float w, float h, GLfloat color[]) {
    glColor3fv(color);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    // Border
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawCircle(float cx, float cy, float r, GLfloat color[]) {
    glColor3fv(color);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = (float)i * 3.14159f / 180.0f;
        glVertex2f(cx + r * cosf(theta), cy + r * sinf(theta));
    }
    glEnd();

    // Border
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = (float)i * 3.14159f / 180.0f;
        glVertex2f(cx + r * cosf(theta), cy + r * sinf(theta));
    }
    glEnd();
}

void drawRobotArm() {
    glPushMatrix();

    // Apply transformations in order: Translate -> Rotate -> Scale -> Reflect -> Shear
    glTranslatef(transX, transY, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX * reflectX, scaleY, 1.0f);

    // Apply shearing using custom matrix
    GLfloat shearMatrix[16] = {
        1.0f, shearY, 0.0f, 0.0f,
        shearX, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    glMultMatrixf(shearMatrix);

    // ===== BASE =====
    drawRectangle(-40.0f, -100.0f, 80.0f, 20.0f, baseColor);

    // ===== JOINT 1 (Shoulder) =====
    drawCircle(0.0f, 0.0f, 15.0f, jointColor);

    // ===== UPPER ARM =====
    glPushMatrix();
    glRotatef(animAngle, 0.0f, 0.0f, 1.0f);
    drawRectangle(-10.0f, 0.0f, 20.0f, 80.0f, armColor);

    // ===== JOINT 2 (Elbow) =====
    drawCircle(0.0f, 80.0f, 12.0f, jointColor);

    // ===== LOWER ARM =====
    glPushMatrix();
    glTranslatef(0.0f, 80.0f, 0.0f);
    glRotatef(animAngle * 0.7f, 0.0f, 0.0f, 1.0f);
    drawRectangle(-8.0f, 0.0f, 16.0f, 60.0f, armColor);

    // ===== JOINT 3 (Wrist) =====
    drawCircle(0.0f, 60.0f, 10.0f, jointColor);

    // ===== HAND / GRIPPER =====
    glPushMatrix();
    glTranslatef(0.0f, 60.0f, 0.0f);
    glRotatef(animAngle * 0.5f, 0.0f, 0.0f, 1.0f);

    // Palm
    drawRectangle(-15.0f, 0.0f, 30.0f, 20.0f, handColor);

    // Fingers
    glColor3f(0.9f, 0.5f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(-15.0f, 20.0f);
    glVertex2f(-10.0f, 35.0f);
    glVertex2f(-5.0f, 20.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(5.0f, 20.0f);
    glVertex2f(10.0f, 35.0f);
    glVertex2f(15.0f, 20.0f);
    glEnd();

    // Finger borders
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-15.0f, 20.0f);
    glVertex2f(-10.0f, 35.0f);
    glVertex2f(-5.0f, 20.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(5.0f, 20.0f);
    glVertex2f(10.0f, 35.0f);
    glVertex2f(15.0f, 20.0f);
    glEnd();

    glPopMatrix(); // Hand
    glPopMatrix(); // Lower arm
    glPopMatrix(); // Upper arm
    glPopMatrix(); // Main
}

// ============================================================
// Display Info
// ============================================================

void drawText(float x, float y, const char* text) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *text);
        text++;
    }
}

void drawInfo() {
    char buffer[256];

    // Title
    glColor3f(1.0f, 1.0f, 0.0f);
    drawText(-380.0f, 270.0f, "=== ROBOT ARM - Computer Graphics Project ===");

    // Current transformations
    glColor3f(0.0f, 1.0f, 1.0f);
    sprintf_s(buffer, sizeof(buffer), "Translation: X=%.1f, Y=%.1f", transX, transY);
    drawText(-380.0f, 240.0f, buffer);

    sprintf_s(buffer, sizeof(buffer), "Rotation: %.1f degrees", rotation);
    drawText(-380.0f, 220.0f, buffer);

    sprintf_s(buffer, sizeof(buffer), "Scaling: X=%.2f, Y=%.2f", scaleX, scaleY);
    drawText(-380.0f, 200.0f, buffer);

    sprintf_s(buffer, sizeof(buffer), "Reflection: X=%s", (reflectX < 0) ? "MIRRORED" : "NORMAL");
    drawText(-380.0f, 180.0f, buffer);

    sprintf_s(buffer, sizeof(buffer), "Shearing: X=%.2f, Y=%.2f", shearX, shearY);
    drawText(-380.0f, 160.0f, buffer);

    // Controls
    glColor3f(0.8f, 0.8f, 0.8f);
    drawText(150.0f, 270.0f, "--- CONTROLS ---");
    drawText(150.0f, 250.0f, "W/S - Move Up/Down (Translation Y)");
    drawText(150.0f, 230.0f, "A/D - Move Left/Right (Translation X)");
    drawText(150.0f, 210.0f, "Q/E - Rotate Base (Rotation)");
    drawText(150.0f, 190.0f, "Z/X - Scale Bigger/Smaller (Scaling)");
    drawText(150.0f, 170.0f, "R - Reflect Arm (Reflection)");
    drawText(150.0f, 150.0f, "T/Y - Shear X +/- (Shearing)");
    drawText(150.0f, 130.0f, "G - Toggle Animation");
    drawText(150.0f, 110.0f, "SPACE - Reset All");
    drawText(150.0f, 90.0f, "ESC - Exit");
}

// ============================================================
// Display & Callbacks
// ============================================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw grid background
    glColor3f(0.15f, 0.15f, 0.15f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = -400; i <= 400; i += 50) {
        glVertex2f((float)i, -300.0f);
        glVertex2f((float)i, 300.0f);
    }
    for (int i = -300; i <= 300; i += 50) {
        glVertex2f(-400.0f, (float)i);
        glVertex2f(400.0f, (float)i);
    }
    glEnd();

    // Draw axes
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-400.0f, 0.0f);
    glVertex2f(400.0f, 0.0f);
    glVertex2f(0.0f, -300.0f);
    glVertex2f(0.0f, 300.0f);
    glEnd();

    // Draw robot arm
    drawRobotArm();

    // Draw info panel
    drawInfo();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Translation
    case 'w': case 'W': transY += 10.0f; break;
    case 's': case 'S': transY -= 10.0f; break;
    case 'a': case 'A': transX -= 10.0f; break;
    case 'd': case 'D': transX += 10.0f; break;

        // Rotation
    case 'q': case 'Q': rotation += 15.0f; break;
    case 'e': case 'E': rotation -= 15.0f; break;

        // Scaling
    case 'z': case 'Z':
        scaleX += 0.1f; scaleY += 0.1f;
        break;
    case 'x': case 'X':
        if (scaleX > 0.2f) { scaleX -= 0.1f; scaleY -= 0.1f; }
        break;

        // Reflection
    case 'r': case 'R':
        reflectX = -reflectX;
        break;

        // Shearing
    case 't': case 'T': shearX += 0.1f; break;
    case 'y': case 'Y': shearX -= 0.1f; break;
    case 'u': case 'U': shearY += 0.1f; break;
    case 'i': case 'I': shearY -= 0.1f; break;

        // Animation
    case 'g': case 'G':
        animating = !animating;
        break;

        // Reset
    case ' ':
        transX = transY = 0.0f;
        rotation = 0.0f;
        scaleX = scaleY = 1.0f;
        reflectX = 1.0f;
        shearX = shearY = 0.0f;
        animating = false;
        animAngle = 0.0f;
        break;

        // Exit
    case 27: // ESC
        exit(0);
    }
    glutPostRedisplay();
}

void timer(int value) {
    if (animating) {
        animAngle += 2.0f;
        if (animAngle > 360.0f) animAngle -= 360.0f;
        glutPostRedisplay();
    }
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400.0, 400.0, -300.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
// Main
// ============================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Robot Arm - Computer Graphics Project");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    printf("========================================\n");
    printf("  Robot Arm - Computer Graphics Project\n");
    printf("========================================\n");
    printf("Controls:\n");
    printf("  W/S - Move Up/Down (Translation Y)\n");
    printf("  A/D - Move Left/Right (Translation X)\n");
    printf("  Q/E - Rotate Base (Rotation)\n");
    printf("  Z/X - Scale Bigger/Smaller (Scaling)\n");
    printf("  R   - Reflect Arm (Reflection)\n");
    printf("  T/Y - Shear X +/- (Shearing)\n");
    printf("  G   - Toggle Animation\n");
    printf("  SPACE - Reset All\n");
    printf("  ESC - Exit\n");
    printf("========================================\n");

    glutMainLoop();
    return 0;
}