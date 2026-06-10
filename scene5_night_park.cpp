#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <cstdlib>

// Standalone Scene 5: Night Park
// Controls: F = fountain on/off, L = lamps on/off, R = regenerate fireflies, Esc = exit

// Star blink state retained from the combined animation
bool s3BlinkOn = false;
int s3BlinkTimer = 0;

// ===== Scene 5 (Night Park) =====
#define FIREFLY_N 22
float fireflyX[FIREFLY_N];
float fireflyY[FIREFLY_N];
float fireflyPhase[FIREFLY_N];
bool  fireflyInit    = false;
float swingAngle     = 0.0f;
float swingDir       = 1.0f;
float fountainPhase  = 0.0f;
float parkCloud      = 0.0f;
float parkBoatX      = -100.0f;
bool  fountainOn     = true;     // 'f' to toggle
bool  lampsOn        = true;     // 'l' to toggle
float owlBlinkPhase  = 0.0f;
float dogWalkerX     = 100.0f;
float walker1X       = 500.0f;
float walker2X       = 780.0f;
float walkPhase      = 0.0f;

// ----------------------------------------------------
void resize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1000.0, 0.0, 600.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ====================== SHARED ======================
void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float a  = 2.0f * 3.1415926f * i / 100;
        float dx = radius * cosf(a);
        float dy = radius * sinf(a);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

// ====================== SCENE 5 : NIGHT PARK ======================
void initFireflies() {
    for (int i = 0; i < FIREFLY_N; i++) {
        fireflyX[i]     = (float)(rand() % 1000);
        fireflyY[i]     = 120.0f + (float)(rand() % 280);
        fireflyPhase[i] = (float)((rand() % 628) / 100.0f);
    }
    fireflyInit = true;
}

void drawParkSky() {
    glBegin(GL_QUADS);
        glColor3ub(8, 8, 35);    glVertex2d(0, 600);    glVertex2d(1000, 600);
        glColor3ub(40, 30, 80);  glVertex2d(1000, 220); glVertex2d(0, 220);
    glEnd();

    glColor3ub(255, 255, 255);
    for (int i = 0; i < 80; i++) {
        int sx = (i * 137) % 1000;
        int sy = 240 + (i * 53) % 350;
        drawCircle((float)sx, (float)sy, 1.4f);
    }

    glColor3ub(255, 255, 200);
    for (int i = 0; i < 12; i++) {
        int sx = (i * 211) % 1000;
        int sy = 300 + (i * 67) % 280;
        float r = 1.6f + (s3BlinkOn ? 1.0f : 0.0f) * ((i % 2) ? 1.0f : 0.3f);
        drawCircle((float)sx, (float)sy, r);
    }

    // Moon
    glColor3ub(60, 55, 110);   drawCircle(840, 500, 55);
    glColor3ub(245, 240, 215); drawCircle(840, 500, 38);
    glColor3ub(225, 220, 195); drawCircle(830, 506, 32);

    // Cloud
    glColor3ub(70, 60, 110);
    drawCircle(parkCloud + 200, 540, 22);
    drawCircle(parkCloud + 230, 545, 28);
    drawCircle(parkCloud + 260, 540, 20);

    // Distant city silhouette
    glColor3ub(15, 12, 35);
    glBegin(GL_QUADS); glVertex2d(0,   220); glVertex2d(0,   280); glVertex2d(120,  280); glVertex2d(120,  220); glEnd();
    glBegin(GL_QUADS); glVertex2d(120, 220); glVertex2d(120, 310); glVertex2d(220,  310); glVertex2d(220,  220); glEnd();
    glBegin(GL_QUADS); glVertex2d(220, 220); glVertex2d(220, 270); glVertex2d(320,  270); glVertex2d(320,  220); glEnd();
    glBegin(GL_QUADS); glVertex2d(550, 220); glVertex2d(550, 290); glVertex2d(640,  290); glVertex2d(640,  220); glEnd();
    glBegin(GL_QUADS); glVertex2d(640, 220); glVertex2d(640, 320); glVertex2d(720,  320); glVertex2d(720,  220); glEnd();
    glBegin(GL_QUADS); glVertex2d(900, 220); glVertex2d(900, 280); glVertex2d(1000, 280); glVertex2d(1000, 220); glEnd();

    // City windows
    glColor3ub(255, 220, 110);
    const int wx[] = { 30, 60, 90, 140, 170, 200, 240, 270, 580, 610, 660, 690, 920, 960 };
    const int wy[] = { 240, 260, 240, 250, 270, 240, 250, 240, 250, 270, 260, 280, 240, 260 };
    for (int i = 0; i < 14; i++) {
        glBegin(GL_QUADS);
            glVertex2i(wx[i],     wy[i]);     glVertex2i(wx[i] + 6, wy[i]);
            glVertex2i(wx[i] + 6, wy[i] + 8); glVertex2i(wx[i],     wy[i] + 8);
        glEnd();
    }
}

void drawParkGround() {
    glBegin(GL_QUADS);
        glColor3ub(20, 60, 30); glVertex2d(0, 220);  glVertex2d(1000, 220);
        glColor3ub(10, 35, 20); glVertex2d(1000, 0); glVertex2d(0, 0);
    glEnd();

    glColor3ub(80, 70, 55);
    glBegin(GL_QUADS);
        glVertex2d(0, 80);     glVertex2d(1000, 80);
        glVertex2d(1000, 130); glVertex2d(0, 130);
    glEnd();

    glColor3ub(60, 52, 40);
    for (int i = 0; i < 1000; i += 40) {
        glBegin(GL_QUADS);
            glVertex2d(i, 80);       glVertex2d(i + 4, 80);
            glVertex2d(i + 4, 130);  glVertex2d(i, 130);
        glEnd();
    }
}

void drawParkLake() {
    glColor3ub(20, 40, 90);
    glBegin(GL_QUADS);
        glVertex2d(0, 130);   glVertex2d(380, 130);
        glVertex2d(380, 215); glVertex2d(0, 215);
    glEnd();

    glColor3ub(30, 60, 130);
    glBegin(GL_QUADS);
        glVertex2d(0, 175);   glVertex2d(380, 175);
        glVertex2d(380, 215); glVertex2d(0, 215);
    glEnd();

    glColor3ub(220, 220, 200);
    for (int i = 0; i < 5; i++) {
        glBegin(GL_QUADS);
            glVertex2d(170, 200 - i * 14); glVertex2d(230, 200 - i * 14);
            glVertex2d(230, 203 - i * 14); glVertex2d(170, 203 - i * 14);
        glEnd();
    }

    // Boat
    glColor3ub(80, 50, 30);
    glBegin(GL_QUADS);
        glVertex2f(parkBoatX, 175);      glVertex2f(parkBoatX + 60, 175);
        glVertex2f(parkBoatX + 50, 168); glVertex2f(parkBoatX + 10, 168);
    glEnd();
    glColor3ub(255, 200, 90);
    drawCircle(parkBoatX + 30, 188, 5);
    glColor3ub(80, 60, 30);
    glBegin(GL_QUADS);
        glVertex2f(parkBoatX + 29, 188); glVertex2f(parkBoatX + 31, 188);
        glVertex2f(parkBoatX + 31, 178); glVertex2f(parkBoatX + 29, 178);
    glEnd();
}

void drawParkTree(float x, float y) {
    // Trunk
    glColor3ub(60, 35, 20);
    glBegin(GL_QUADS);
        glVertex2f(x - 7, y);      glVertex2f(x + 7, y);
        glVertex2f(x + 7, y + 62); glVertex2f(x - 7, y + 62);
    glEnd();

    glColor3ub(80, 50, 28);
    glBegin(GL_QUADS);
        glVertex2f(x - 7, y);      glVertex2f(x - 3, y);
        glVertex2f(x - 3, y + 62); glVertex2f(x - 7, y + 62);
    glEnd();

    glColor3ub(35, 20, 10);
    glBegin(GL_LINES);
        glVertex2f(x - 4, y + 10); glVertex2f(x + 4, y + 20);
        glVertex2f(x - 4, y + 35); glVertex2f(x + 4, y + 45);
    glEnd();

    // Foliage layers
    glColor3ub(15, 50, 25);
    drawCircle(x - 22, y + 58, 24);
    drawCircle(x + 22, y + 58, 24);
    drawCircle(x,      y + 55, 30);

    glColor3ub(25, 65, 35);
    drawCircle(x - 20, y + 78, 22);
    drawCircle(x + 20, y + 78, 22);
    drawCircle(x,      y + 78, 28);

    glColor3ub(35, 80, 45);
    drawCircle(x - 12, y + 95,  18);
    drawCircle(x + 12, y + 95,  18);
    drawCircle(x,      y + 100, 22);
}

void drawLamppost(float x) {
    glColor3ub(40, 40, 50);
    glBegin(GL_QUADS);
        glVertex2f(x - 3, 75);  glVertex2f(x + 3, 75);
        glVertex2f(x + 3, 200); glVertex2f(x - 3, 200);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x, 200);      glVertex2f(x + 30, 200);
        glVertex2f(x + 30, 195); glVertex2f(x, 195);
    glEnd();

    glColor3ub(60, 60, 70);
    glBegin(GL_QUADS);
        glVertex2f(x + 22, 195); glVertex2f(x + 38, 195);
        glVertex2f(x + 38, 175); glVertex2f(x + 22, 175);
    glEnd();

    if (lampsOn) {
        glColor3ub(80, 70, 25);
        drawCircle(x + 30, 183, 22);
        glColor3ub(255, 220, 110);
        drawCircle(x + 30, 183, 12);
        glColor3ub(100, 90, 35);
        glBegin(GL_TRIANGLES);
            glVertex2f(x + 30, 175);
            glVertex2f(x + 5,  90);
            glVertex2f(x + 65, 90);
        glEnd();
    } else {
        glColor3ub(40, 40, 50);
        drawCircle(x + 30, 183, 12);
    }
}

void drawBenchPerson(float x, unsigned char shirtR, unsigned char shirtG, unsigned char shirtB) {
    glColor3ub(70, 45, 25);
    glBegin(GL_QUADS); glVertex2f(x - 35, 86);  glVertex2f(x + 35, 86);  glVertex2f(x + 35, 92);  glVertex2f(x - 35, 92);  glEnd();
    glBegin(GL_QUADS); glVertex2f(x - 32, 75);  glVertex2f(x - 28, 75);  glVertex2f(x - 28, 86);  glVertex2f(x - 32, 86);  glEnd();
    glBegin(GL_QUADS); glVertex2f(x + 28, 75);  glVertex2f(x + 32, 75);  glVertex2f(x + 32, 86);  glVertex2f(x + 28, 86);  glEnd();
    glBegin(GL_QUADS); glVertex2f(x - 35, 92);  glVertex2f(x - 31, 92);  glVertex2f(x - 31, 115); glVertex2f(x - 35, 115); glEnd();
    glBegin(GL_QUADS); glVertex2f(x + 31, 92);  glVertex2f(x + 35, 92);  glVertex2f(x + 35, 115); glVertex2f(x + 31, 115); glEnd();
    glBegin(GL_QUADS); glVertex2f(x - 35, 110); glVertex2f(x + 35, 110); glVertex2f(x + 35, 115); glVertex2f(x - 35, 115); glEnd();

    glColor3ub(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
        glVertex2f(x - 8, 92);  glVertex2f(x + 8, 92);
        glVertex2f(x + 8, 116); glVertex2f(x - 8, 116);
    glEnd();

    glColor3ub(220, 180, 150);
    drawCircle(x, 124, 7);
}

void drawSwingSet(float x) {
    glColor3ub(120, 60, 40);
    glLineWidth(4);
    glBegin(GL_LINES);
        glVertex2f(x - 60, 80);  glVertex2f(x - 30, 200);
        glVertex2f(x + 60, 80);  glVertex2f(x + 30, 200);
        glVertex2f(x - 30, 200); glVertex2f(x + 30, 200);
    glEnd();

    glPushMatrix();
    glTranslatef(x, 200, 0);
    glRotatef(swingAngle, 0, 0, 1);
    glTranslatef(-x, -200, 0);

    glColor3ub(180, 180, 180);
    glBegin(GL_LINES);
        glVertex2f(x - 8, 200); glVertex2f(x - 8, 130);
        glVertex2f(x + 8, 200); glVertex2f(x + 8, 130);
    glEnd();

    glColor3ub(80, 50, 25);
    glBegin(GL_QUADS);
        glVertex2f(x - 12, 130); glVertex2f(x + 12, 130);
        glVertex2f(x + 12, 126); glVertex2f(x - 12, 126);
    glEnd();

    glColor3ub(220, 80, 60);
    glBegin(GL_QUADS);
        glVertex2f(x - 6, 130); glVertex2f(x + 6, 130);
        glVertex2f(x + 6, 150); glVertex2f(x - 6, 150);
    glEnd();

    glColor3ub(225, 185, 150);
    drawCircle(x, 156, 5);

    glColor3ub(40, 50, 80);
    glBegin(GL_QUADS); glVertex2f(x - 5, 130);  glVertex2f(x - 1, 130);  glVertex2f(x + 5, 118);  glVertex2f(x + 1, 118);  glEnd();
    glBegin(GL_QUADS); glVertex2f(x + 1, 130);  glVertex2f(x + 5, 130);  glVertex2f(x + 11, 118); glVertex2f(x + 7, 118);  glEnd();

    glLineWidth(1);
    glPopMatrix();
}

void drawFountain(float cx, float cy) {
    glColor3ub(110, 110, 120);
    glBegin(GL_QUADS);
        glVertex2f(cx - 60, cy - 6); glVertex2f(cx + 60, cy - 6);
        glVertex2f(cx + 60, cy + 6); glVertex2f(cx - 60, cy + 6);
    glEnd();

    glColor3ub(50, 90, 160);
    glBegin(GL_QUADS);
        glVertex2f(cx - 55, cy + 6);  glVertex2f(cx + 55, cy + 6);
        glVertex2f(cx + 55, cy + 14); glVertex2f(cx - 55, cy + 14);
    glEnd();

    glColor3ub(120, 120, 130);
    glBegin(GL_QUADS);
        glVertex2f(cx - 6, cy + 14); glVertex2f(cx + 6, cy + 14);
        glVertex2f(cx + 6, cy + 30); glVertex2f(cx - 6, cy + 30);
    glEnd();

    if (fountainOn) {
        glColor3ub(170, 220, 255);
        glLineWidth(2);
        for (int i = 0; i < 9; i++) {
            float a     = -3.1415926f * 0.5f + (i - 4) * 0.18f;
            float speed = 22.0f + 4.0f * sinf(fountainPhase + i);
            float vx    = speed * sinf(a);
            float vy    = speed * cosf(a) * 1.8f;
            glBegin(GL_LINE_STRIP);
                for (int s = 0; s < 18; s++) {
                    float t  = s * 0.06f + (fountainPhase * 0.05f);
                    float px = cx + vx * t;
                    float py = cy + 30 + vy * t - 9.8f * t * t * 0.5f;
                    if (py < cy + 14) break;
                    glVertex2f(px, py);
                }
            glEnd();
        }
        glLineWidth(1);

        glColor3ub(200, 230, 255);
        for (int i = 0; i < 6; i++) {
            float dx = (float)((i * 17) % 90 - 45);
            drawCircle(cx + dx, cy + 12, 1.5f);
        }
    }
}

void drawDogWalker(float x, float phase) {
    float legSwing = sinf(phase) * 4.0f;
    float armSwing = sinf(phase) * 3.0f;

    // Torso
    glColor3ub(70, 50, 130);
    glBegin(GL_QUADS);
        glVertex2f(x - 6, 100); glVertex2f(x + 6, 100);
        glVertex2f(x + 6, 130); glVertex2f(x - 6, 130);
    glEnd();

    // Head
    glColor3ub(220, 180, 150);
    drawCircle(x, 138, 6);

    // Hair
    glColor3ub(50, 35, 25);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, 138);
        for (int i = 0; i <= 12; i++) {
            float a = 3.1415926f * i / 12.0f;
            glVertex2f(x + 6 * cosf(a), 138 + 6 * sinf(a));
        }
    glEnd();

    // Legs
    glColor3ub(35, 35, 60);
    glBegin(GL_QUADS);
        glVertex2f(x - 5, 100); glVertex2f(x - 1, 100);
        glVertex2f(x - 1 - legSwing, 80); glVertex2f(x - 5 - legSwing, 80);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x + 1, 100); glVertex2f(x + 5, 100);
        glVertex2f(x + 5 + legSwing, 80); glVertex2f(x + 1 + legSwing, 80);
    glEnd();

    // Free arm
    glColor3ub(70, 50, 130);
    glBegin(GL_QUADS);
        glVertex2f(x - 6, 128); glVertex2f(x - 9, 128);
        glVertex2f(x - 9 - armSwing, 108); glVertex2f(x - 6 - armSwing, 108);
    glEnd();

        // Leash arm (extended forward, holds leash)
    glColor3ub(70, 50, 130);
    glBegin(GL_QUADS);
        glVertex2f(x + 6,  125); glVertex2f(x + 9,  125);
        glVertex2f(x + 12, 115); glVertex2f(x + 9,  115);
    glEnd();

    // Leash
    glColor3ub(220, 220, 220);
    glBegin(GL_LINES);
        glVertex2f(x + 10, 115); glVertex2f(x + 34, 88);
    glEnd();

    // Dog body
    glColor3ub(150, 110, 70);
    glBegin(GL_QUADS);
        glVertex2f(x + 22, 80); glVertex2f(x + 44, 80);
        glVertex2f(x + 44, 92); glVertex2f(x + 22, 92);
    glEnd();

    // Dog head
    drawCircle(x + 46, 92, 5);

    // Ears
    glColor3ub(120, 85, 50);
    glBegin(GL_TRIANGLES);
        glVertex2f(x + 44, 96); glVertex2f(x + 46, 92); glVertex2f(x + 48, 96);
    glEnd();

    // Eye
    glColor3ub(0, 0, 0);
    drawCircle(x + 47, 93, 0.8f);

    // Dog legs (animated)
    float dogLeg = sinf(phase * 1.5f) * 2.0f;
    glColor3ub(150, 110, 70);
    glBegin(GL_QUADS);
        glVertex2f(x + 24, 80);          glVertex2f(x + 26, 80);
        glVertex2f(x + 26 + dogLeg, 72); glVertex2f(x + 24 + dogLeg, 72);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x + 40, 80);          glVertex2f(x + 42, 80);
        glVertex2f(x + 42 - dogLeg, 72); glVertex2f(x + 40 - dogLeg, 72);
    glEnd();

    // Tail (wagging)
    float wag = sinf(phase * 2.0f) * 2.0f;
    glColor3ub(140, 100, 65);
    glBegin(GL_QUADS);
        glVertex2f(x + 22, 90);       glVertex2f(x + 14, 96 + wag);
        glVertex2f(x + 16, 98 + wag); glVertex2f(x + 24, 92);
    glEnd();
}

// ----------------------------------------------------
void drawParkWalker(float x, float phase,
                    unsigned char shirtR, unsigned char shirtG, unsigned char shirtB)
{
    float legSwing = sinf(phase) * 4.0f;
    float armSwing = sinf(phase) * 3.5f;

    // Legs
    glColor3ub(35, 30, 55);
    glBegin(GL_QUADS);
        glVertex2f(x - 5, 100);          glVertex2f(x - 1, 100);
        glVertex2f(x - 1 - legSwing, 80); glVertex2f(x - 5 - legSwing, 80);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x + 1, 100);          glVertex2f(x + 5, 100);
        glVertex2f(x + 5 + legSwing, 80); glVertex2f(x + 1 + legSwing, 80);
    glEnd();

    // Shoes
    glColor3ub(15, 15, 15);
    glBegin(GL_QUADS);
        glVertex2f(x - 6 - legSwing, 80); glVertex2f(x     - legSwing, 80);
        glVertex2f(x     - legSwing, 77); glVertex2f(x - 6 - legSwing, 77);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x     + legSwing, 80); glVertex2f(x + 6 + legSwing, 80);
        glVertex2f(x + 6 + legSwing, 77); glVertex2f(x     + legSwing, 77);
    glEnd();

    // Torso
    glColor3ub(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
        glVertex2f(x - 7, 100); glVertex2f(x + 7, 100);
        glVertex2f(x + 7, 128); glVertex2f(x - 7, 128);
    glEnd();

    // Arms
    glBegin(GL_QUADS);
        glVertex2f(x -  7, 126);            glVertex2f(x - 10, 126);
        glVertex2f(x - 10 + armSwing, 105); glVertex2f(x -  7 + armSwing, 105);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x +  7, 126);            glVertex2f(x + 10, 126);
        glVertex2f(x + 10 - armSwing, 105); glVertex2f(x +  7 - armSwing, 105);
    glEnd();

    // Hands
    glColor3ub(220, 180, 150);
    drawCircle(x - 8 + armSwing, 103, 2);
    drawCircle(x + 8 - armSwing, 103, 2);

    // Neck
    glBegin(GL_QUADS);
        glVertex2f(x - 2, 128); glVertex2f(x + 2, 128);
        glVertex2f(x + 2, 132); glVertex2f(x - 2, 132);
    glEnd();

    // Head
    drawCircle(x, 138, 6);

    // Hair
    glColor3ub(40, 25, 15);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, 138);
        for (int i = 0; i <= 12; i++) {
            float a = 3.1415926f * i / 12.0f;
            glVertex2f(x + 6 * cosf(a), 138 + 6 * sinf(a));
        }
    glEnd();
}

// ----------------------------------------------------
void drawFireflies()
{
    for (int i = 0; i < FIREFLY_N; i++) {
        float br = 0.6f + 0.4f * sinf(fireflyPhase[i]);
        unsigned char r = (unsigned char)(255 * br);
        unsigned char g = (unsigned char)(220 * br);
        unsigned char b = (unsigned char)(120 * br * 0.5f);

        // Halo
        glColor3ub((unsigned char)(r * 0.35f), (unsigned char)(g * 0.35f), 0);
        drawCircle(fireflyX[i], fireflyY[i], 5);
        // Core
        glColor3ub(r, g, b);
        drawCircle(fireflyX[i], fireflyY[i], 2.2f);
    }
}

// ----------------------------------------------------
void drawGazebo(float cx, float cy)
{
    // Posts
    glColor3ub(80, 55, 30);
    glBegin(GL_QUADS);
        glVertex2f(cx - 50, cy);      glVertex2f(cx - 45, cy);
        glVertex2f(cx - 45, cy + 65); glVertex2f(cx - 50, cy + 65);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(cx + 45, cy);      glVertex2f(cx + 50, cy);
        glVertex2f(cx + 50, cy + 65); glVertex2f(cx + 45, cy + 65);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(cx - 5, cy);      glVertex2f(cx + 5, cy);
        glVertex2f(cx + 5, cy + 65); glVertex2f(cx - 5, cy + 65);
    glEnd();

    // Base platform
    glColor3ub(95, 65, 35);
    glBegin(GL_QUADS);
        glVertex2f(cx - 55, cy - 5); glVertex2f(cx + 55, cy - 5);
        glVertex2f(cx + 55, cy);     glVertex2f(cx - 55, cy);
    glEnd();

    // Roof
    glColor3ub(140, 50, 40);
    glBegin(GL_TRIANGLES);
        glVertex2f(cx - 60, cy + 65); glVertex2f(cx + 60, cy + 65); glVertex2f(cx, cy + 100);
    glEnd();

    // Roof trim
    glColor3ub(180, 70, 55);
    glBegin(GL_QUADS);
        glVertex2f(cx - 60, cy + 62); glVertex2f(cx + 60, cy + 62);
        glVertex2f(cx + 60, cy + 67); glVertex2f(cx - 60, cy + 67);
    glEnd();

    // Hanging lantern
    glColor3ub(40, 40, 50);
    glBegin(GL_QUADS);
        glVertex2f(cx - 0.5f, cy + 65); glVertex2f(cx + 0.5f, cy + 65);
        glVertex2f(cx + 0.5f, cy + 50); glVertex2f(cx - 0.5f, cy + 50);
    glEnd();
    glColor3ub(255, 220, 130);
    drawCircle(cx, cy + 45, 5);
    glColor3ub(255, 240, 180);
    drawCircle(cx, cy + 45, 2.5f);
}

// ----------------------------------------------------
void drawOwlOnBranch(float x, float y)
{
    // Branch
    glColor3ub(60, 40, 20);
    glBegin(GL_QUADS);
        glVertex2f(x - 18, y);     glVertex2f(x + 18, y);
        glVertex2f(x + 18, y + 3); glVertex2f(x - 18, y + 3);
    glEnd();

    // Body and head
    glColor3ub(85, 65, 45);
    drawCircle(x, y + 14, 10);
    drawCircle(x, y + 25, 7);

    // Ear tufts
    glColor3ub(60, 45, 30);
    glBegin(GL_TRIANGLES);
        glVertex2f(x - 6, y + 28); glVertex2f(x - 4, y + 34); glVertex2f(x - 2, y + 28);
        glVertex2f(x + 6, y + 28); glVertex2f(x + 4, y + 34); glVertex2f(x + 2, y + 28);
    glEnd();

    // Eyes (blink)
    if (sinf(owlBlinkPhase) > 0.85f) {
        glColor3ub(60, 45, 30);
        glBegin(GL_LINES);
            glVertex2f(x - 5, y + 25); glVertex2f(x - 1, y + 25);
            glVertex2f(x + 1, y + 25); glVertex2f(x + 5, y + 25);
        glEnd();
    } else {
        glColor3ub(255, 230, 100);
        drawCircle(x - 3, y + 25, 2);
        drawCircle(x + 3, y + 25, 2);
        glColor3ub(0, 0, 0);
        drawCircle(x - 3, y + 25, 0.8f);
        drawCircle(x + 3, y + 25, 0.8f);
    }

    // Beak
    glColor3ub(180, 120, 40);
    glBegin(GL_TRIANGLES);
        glVertex2f(x - 1, y + 22); glVertex2f(x + 1, y + 22); glVertex2f(x, y + 19);
    glEnd();

    // Feet
    glBegin(GL_LINES);
        glVertex2f(x - 3, y + 4); glVertex2f(x - 3, y);
        glVertex2f(x + 3, y + 4); glVertex2f(x + 3, y);
    glEnd();
}

// ----------------------------------------------------
void drawPicnicTable(float cx)
{
    // Top
    glColor3ub(110, 70, 40);
    glBegin(GL_QUADS);
        glVertex2f(cx - 35, 100); glVertex2f(cx + 35, 100);
        glVertex2f(cx + 35, 106); glVertex2f(cx - 35, 106);
    glEnd();

    // Legs
    glColor3ub(80, 55, 30);
    glBegin(GL_QUADS);
        glVertex2f(cx - 30, 100); glVertex2f(cx - 26, 100);
        glVertex2f(cx - 26, 80);  glVertex2f(cx - 30, 80);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(cx + 26, 100); glVertex2f(cx + 30, 100);
        glVertex2f(cx + 30, 80);  glVertex2f(cx + 26, 80);
    glEnd();

    // Benches
    glColor3ub(95, 65, 35);
    glBegin(GL_QUADS);
        glVertex2f(cx - 40, 92); glVertex2f(cx - 18, 92);
        glVertex2f(cx - 18, 95); glVertex2f(cx - 40, 95);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(cx + 18, 92); glVertex2f(cx + 40, 92);
        glVertex2f(cx + 40, 95); glVertex2f(cx + 18, 95);
    glEnd();

    // Lantern on table
    glColor3ub(40, 40, 50);
    glBegin(GL_QUADS);
        glVertex2f(cx - 3, 106); glVertex2f(cx + 3, 106);
        glVertex2f(cx + 3, 118); glVertex2f(cx - 3, 118);
    glEnd();
    glColor3ub(255, 210, 120);
    drawCircle(cx, 122, 4.5f);
    glColor3ub(255, 240, 180);
    drawCircle(cx, 122, 2);
}

// ----------------------------------------------------
void drawShootingStar()
{
    // Animated shooting star using parkCloud as a clock
    float p  = parkCloud * 0.5f;
    float sx = fmodf(p, 1200.0f) - 100.0f;
    float sy = 500.0f - fmodf(p * 0.3f, 80.0f);

    glColor3ub(255, 255, 220);
    drawCircle(sx, sy, 2.5f);

    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
        glColor3ub(255, 255, 220); glVertex2f(sx,      sy);
        glColor3ub(200, 200, 180); glVertex2f(sx - 25, sy + 8);
        glColor3ub(120, 120, 100); glVertex2f(sx - 50, sy + 16);
    glEnd();
    glLineWidth(1);
}

// ----------------------------------------------------
void scene5()
{
    if (!fireflyInit) initFireflies();
    glClear(GL_COLOR_BUFFER_BIT);

    drawParkSky();
    drawShootingStar();
    drawParkLake();
    drawParkGround();

    drawParkTree(60,  130);
    drawParkTree(440, 130);
    drawParkTree(560, 135);
    drawParkTree(700, 130);
    drawParkTree(950, 132);

    drawGazebo(140, 90);
    drawFountain(500, 110);
    drawPicnicTable(820);

    drawLamppost(260);
    drawLamppost(380);
    drawLamppost(640);
    drawLamppost(880);

    drawBenchPerson(220, 200, 90,  90);
    drawBenchPerson(750, 90,  140, 200);
    drawSwingSet(620);

    drawDogWalker(dogWalkerX, walkPhase);
    drawParkWalker(walker1X, walkPhase + 1.2f, 200, 80,  80);
    drawParkWalker(walker2X, walkPhase + 2.4f, 70,  130, 200);

    drawOwlOnBranch(700, 200);
    drawFireflies();
}



void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    scene5();
    glutSwapBuffers();
}

void update(int /*value*/)
{
    if (!fireflyInit) initFireflies();

    parkCloud += 0.3f;
    if (parkCloud > 800) parkCloud = -400;
    parkBoatX += 0.4f;
    if (parkBoatX > 380) parkBoatX = -80;

    swingAngle += swingDir * 0.6f;
    if (swingAngle >  22.0f) swingDir = -1.0f;
    if (swingAngle < -22.0f) swingDir =  1.0f;

    if (fountainOn) fountainPhase += 0.15f;
    owlBlinkPhase += 0.04f;
    walkPhase     += 0.18f;

    dogWalkerX += 1.1f; if (dogWalkerX > 1080) dogWalkerX = -80;
    walker1X   -= 0.9f; if (walker1X   < -40)  walker1X   = 1040;
    walker2X   += 0.7f; if (walker2X   > 1040) walker2X   = -40;

    for (int i = 0; i < FIREFLY_N; i++) {
        fireflyPhase[i] += 0.08f + (i % 5) * 0.005f;
        fireflyX[i]     += sinf(fireflyPhase[i]) * 0.6f + 0.2f;
        fireflyY[i]     += cosf(fireflyPhase[i] * 0.7f) * 0.5f;
        if (fireflyX[i] > 1010) fireflyX[i] = -10;
        if (fireflyX[i] < -10)  fireflyX[i] = 1010;
        if (fireflyY[i] >  420) fireflyY[i] = 120;
        if (fireflyY[i] <  110) fireflyY[i] = 420;
    }

    if (++s3BlinkTimer > 22) {
        s3BlinkOn = !s3BlinkOn;
        s3BlinkTimer = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void handleKeypress(unsigned char key, int /*x*/, int /*y*/)
{
    switch (key) {
        case 27: std::exit(0); break;
        case 'f': fountainOn = !fountainOn; break;
        case 'l': lampsOn    = !lampsOn;    break;
        case 'r': fireflyInit = false; break;
    }
}


void myInit()
{
    glClearColor(1, 1, 1, 0);
    glColor3f(0, 0, 0);
    glPointSize(4);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1000, 0.0, 600);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 700);
    glutCreateWindow("Urban City Life - Scene 5: Night Park");

    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(resize);
    glutTimerFunc(0, update, 0);
    glutDisplayFunc(display);

    myInit();
    glutMainLoop();
    return 0;
}
