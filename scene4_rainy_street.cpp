#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <cstdlib>

// Standalone Scene 4: Rainy Urban Street
// Controls: R = regenerate rain pattern, Esc = exit

// Rain-house window state and beacon blink state retained from the combined animation
bool night = false;
bool s3BlinkOn = false;
int s3BlinkTimer = 0;

// ===== Scene 4 (Rain) =====
#define RAIN_DROPS 180
float rainX[RAIN_DROPS];
float rainY[RAIN_DROPS];
bool  rainInit            = false;
float person1X            = 100.0f;
float person2X            = 850.0f;
float person3X            = 400.0f;
int   lightningTimer      = 0;
bool  lightningOn         = false;
float rainCar1X           = 100.0f;
float rainCar2X           = 800.0f;
float rainCar3X           = 500.0f;
float armSwingPhase       = 0.0f;
float puddleRipple        = 0.0f;
float streetLightFlicker  = 0.0f;
float scene4DogX          = 250.0f;

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

// House windows (shared between big/small)
static void drawHouseWindows(int startX, int startY, int rows, int cols) {
    const int windowWidth  = 25;
    const int windowHeight = 30;
    const int gapX = 10, gapY = 15;

    int rw, gw, bw, rw1, gw1, bw1;
    if (!night) { rw = 0;   gw = 102; bw = 204; rw1 = 135; gw1 = 206; bw1 = 250; }
    else        { rw = 255; gw = 201; bw = 14;  rw1 = 255; gw1 = 201; bw1 = 14;  }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int x = startX + col * (windowWidth + gapX);
            int y = startY - row * (windowHeight + gapY);
            glBegin(GL_QUADS);
                glColor3ub(rw, gw, bw);
                glVertex2i(x, y);
                glVertex2i(x + windowWidth, y);
                glColor3ub(rw1, gw1, bw1);
                glVertex2i(x + windowWidth, y - windowHeight);
                glVertex2i(x, y - windowHeight);
            glEnd();
        }
    }
}

void drawHouseBig(int offsetX, int offsetY, unsigned char r, unsigned char g, unsigned char b) {
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2i(400 + offsetX, 180 + offsetY);
        glVertex2i(516 + offsetX, 180 + offsetY);
        glVertex2i(516 + offsetX, 380 + offsetY);
        glVertex2i(400 + offsetX, 380 + offsetY);
    glEnd();
    drawHouseWindows(410 + offsetX, 360 + offsetY, 3, 3);
}

void drawHouseSmall(int offsetX, int offsetY, unsigned char r, unsigned char g, unsigned char b) {
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2i(400 + offsetX, 180 + offsetY);
        glVertex2i(480 + offsetX, 180 + offsetY);
        glVertex2i(480 + offsetX, 380 + offsetY);
        glVertex2i(400 + offsetX, 380 + offsetY);
    glEnd();
    drawHouseWindows(410 + offsetX, 360 + offsetY, 2, 2);
}

// ====================== SCENE 4 : RAIN ======================
void initRain() {
    for (int i = 0; i < RAIN_DROPS; i++) {
        rainX[i] = (float)(rand() % 1000);
        rainY[i] = (float)(rand() % 600);
    }
    rainInit = true;
}

void drawCloudDark(float x, float y) {
    glColor3ub(70, 75, 85);
    drawCircle(x,      y,     25);
    drawCircle(x + 30, y + 5, 32);
    drawCircle(x + 65, y,     25);
    drawCircle(x + 30, y - 8, 28);
}

void drawUmbrellaPerson(float x,
                        unsigned char ur, unsigned char ug, unsigned char ub,
                        unsigned char shirtR, unsigned char shirtG, unsigned char shirtB,
                        float armPhase)
{
    glPushMatrix();
    glTranslatef(x, 144.0f, 0);
    glScalef(0.72f, 0.72f, 1.0f);
    glTranslatef(-x, -144.0f, 0);
    glTranslatef(0, 18.0f, 0);

    // Canopy
    glColor3ub(ur, ug, ub);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, 330);
        for (int i = 0; i <= 24; i++) {
            float a = 3.1415926f * i / 24.0f;
            glVertex2f(x + 70 * cosf(a), 330 + 32 * sinf(a));
        }
    glEnd();

    glColor3ub((unsigned char)(ur * 0.6f),
               (unsigned char)(ug * 0.6f),
               (unsigned char)(ub * 0.6f));
    glLineWidth(2);
    glBegin(GL_LINES);
        for (int i = 0; i <= 6; i++) {
            float a = 3.1415926f * i / 6.0f;
            glVertex2f(x, 330);
            glVertex2f(x + 70 * cosf(a), 330 + 32 * sinf(a));
        }
    glEnd();

    glColor3ub(20, 20, 20);
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 24; i++) {
            float a = 3.1415926f * i / 24.0f;
            glVertex2f(x + 70 * cosf(a), 330 + 32 * sinf(a));
        }
    glEnd();
    glLineWidth(1);

    glColor3ub(40, 40, 40);
    glBegin(GL_QUADS);
        glVertex2f(x - 2, 362); glVertex2f(x + 2, 362);
        glVertex2f(x + 2, 372); glVertex2f(x - 2, 372);
    glEnd();

    // Pole
    glBegin(GL_QUADS);
        glVertex2f(x - 2, 330); glVertex2f(x + 2, 330);
        glVertex2f(x + 2, 245); glVertex2f(x - 2, 245);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x - 2,  245); glVertex2f(x - 10, 245);
        glVertex2f(x - 10, 240); glVertex2f(x - 2,  240);
    glEnd();

    // Head
    glColor3ub(220, 180, 150);
    drawCircle(x, 270, 14);
    glColor3ub(40, 30, 25);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, 270);
        for (int i = 0; i <= 18; i++) {
            float a = 3.1415926f * i / 18.0f;
            glVertex2f(x + 14 * cosf(a), 270 + 14 * sinf(a));
        }
    glEnd();
    glColor3ub(20, 20, 20);
    drawCircle(x - 4, 271, 1.4f);
    drawCircle(x + 4, 271, 1.4f);

    glColor3ub(220, 180, 150);
    glBegin(GL_QUADS);
        glVertex2f(x - 3, 257); glVertex2f(x + 3, 257);
        glVertex2f(x + 3, 250); glVertex2f(x - 3, 250);
    glEnd();

    // Torso
    glColor3ub(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
        glVertex2f(x - 18, 252); glVertex2f(x + 18, 252);
        glVertex2f(x + 16, 200); glVertex2f(x - 16, 200);
    glEnd();
    glColor3ub((unsigned char)(shirtR * 0.7f),
               (unsigned char)(shirtG * 0.7f),
               (unsigned char)(shirtB * 0.7f));
    glBegin(GL_TRIANGLES);
        glVertex2f(x - 7, 252); glVertex2f(x + 7, 252); glVertex2f(x, 238);
    glEnd();

    // Arms
    float swing = sinf(armPhase) * 4.0f;
    glColor3ub(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
        glVertex2f(x + 16, 250); glVertex2f(x + 22, 250);
        glVertex2f(x + 18, 225); glVertex2f(x + 12, 225);
    glEnd();
    glColor3ub(220, 180, 150);
    glBegin(GL_QUADS);
        glVertex2f(x + 12, 225); glVertex2f(x + 18, 225);
        glVertex2f(x + 4,  248); glVertex2f(x - 2,  248);
    glEnd();
    drawCircle(x + 1, 246, 4);

    glColor3ub(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
        glVertex2f(x - 16, 250);          glVertex2f(x - 22, 250);
        glVertex2f(x - 22 + swing, 215);  glVertex2f(x - 16 + swing, 215);
    glEnd();
    glColor3ub(220, 180, 150);
    glBegin(GL_QUADS);
        glVertex2f(x - 22 + swing, 215);        glVertex2f(x - 16 + swing, 215);
        glVertex2f(x - 16 + swing * 1.4f, 192); glVertex2f(x - 22 + swing * 1.4f, 192);
    glEnd();
    drawCircle(x - 19 + swing * 1.4f, 188, 4);

    // Legs
    float legSwing = sinf(armPhase) * 3.0f;
    glColor3ub(30, 30, 60);
    glBegin(GL_QUADS);
        glVertex2f(x - 14, 200); glVertex2f(x - 2, 200);
        glVertex2f(x - 2  - legSwing, 150);
        glVertex2f(x - 14 - legSwing, 150);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x + 2, 200);  glVertex2f(x + 14, 200);
        glVertex2f(x + 14 + legSwing, 150);
        glVertex2f(x + 2  + legSwing, 150);
    glEnd();

    // Shoes
    glColor3ub(20, 20, 20);
    glBegin(GL_QUADS);
        glVertex2f(x - 16 - legSwing, 150);
        glVertex2f(x - 1  - legSwing, 150);
        glVertex2f(x - 1  - legSwing, 144);
        glVertex2f(x - 16 - legSwing, 144);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x + 1  + legSwing, 150);
        glVertex2f(x + 16 + legSwing, 150);
        glVertex2f(x + 16 + legSwing, 144);
        glVertex2f(x + 1  + legSwing, 144);
    glEnd();

    glPopMatrix();
}

void drawRainStreet() {
    glColor3ub(35, 40, 50);
    glBegin(GL_QUADS);
        glVertex2d(0, 0);      glVertex2d(1000, 0);
        glVertex2d(1000, 145); glVertex2d(0, 145);
    glEnd();

    glColor3ub(80, 80, 90);
    glBegin(GL_QUADS);
        glVertex2d(0, 145);    glVertex2d(1000, 145);
        glVertex2d(1000, 160); glVertex2d(0, 160);
    glEnd();

    glColor3ub(120, 130, 150);
    drawCircle(200, 30, 25);
    drawCircle(550, 50, 30);
    drawCircle(820, 25, 22);

    glColor3ub(180, 180, 180);
    for (int i = 0; i < 1000; i += 90) {
        glBegin(GL_QUADS);
            glVertex2d(i, 68);      glVertex2d(i + 50, 68);
            glVertex2d(i + 50, 74); glVertex2d(i, 74);
        glEnd();
    }
}

void drawRainHouses() {
    drawHouseBig  (-340, 30, 90,  80,  95);
    drawHouseSmall(-200, 50, 110, 95,  90);
    drawHouseBig  ( 50,  40, 95,  90,  105);
    drawHouseSmall( 280, 55, 120, 100, 95);
    drawHouseBig  ( 420, 35, 85,  85,  100);
    drawHouseSmall( 580, 50, 105, 95,  90);
}

void drawRainDrops() {
    glColor3ub(180, 200, 230);
    glBegin(GL_LINES);
        for (int i = 0; i < RAIN_DROPS; i++) {
            glVertex2f(rainX[i], rainY[i]);
            glVertex2f(rainX[i] - 4, rainY[i] - 12);
        }
    glEnd();
}

void drawRainCar(float x, unsigned char r, unsigned char g, unsigned char b, bool flipped) {
    glPushMatrix();
    glTranslatef(x, 0, 0);
    if (flipped) glScalef(-1.0f, 1.0f, 1.0f);

    glTranslatef(0, 68, 0);
    glScalef(1.30f, 1.30f, 1.0f);
    glTranslatef(0, -68, 0);

    // Shadow
    glColor3ub(20, 20, 25);
    glBegin(GL_QUADS);
        glVertex2f(-110, 78); glVertex2f(110, 78);
        glVertex2f(105, 73);  glVertex2f(-105, 73);
    glEnd();

    // Lower body
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(-100, 85);  glVertex2f(100, 85);
        glVertex2f(100, 115);  glVertex2f(-100, 115);
    glEnd();

    // Hood
    glBegin(GL_POLYGON);
        glVertex2f(100, 115); glVertex2f(60, 115);
        glVertex2f(70, 138);  glVertex2f(100, 138);
    glEnd();

    // Cabin
    glBegin(GL_POLYGON);
        glVertex2f(-60, 115); glVertex2f(60, 115);
        glVertex2f(45, 158);  glVertex2f(-45, 158);
    glEnd();

    // Trunk
    glBegin(GL_POLYGON);
        glVertex2f(-100, 115); glVertex2f(-60, 115);
        glVertex2f(-70, 138);  glVertex2f(-100, 138);
    glEnd();

    // Window
    glColor3ub(150, 200, 230);
    glBegin(GL_POLYGON);
        glVertex2f(-55, 118); glVertex2f(55, 118);
        glVertex2f(42, 154);  glVertex2f(-42, 154);
    glEnd();

    glColor3ub((unsigned char)(r * 0.7f),
               (unsigned char)(g * 0.7f),
               (unsigned char)(b * 0.7f));
    glBegin(GL_QUADS);
        glVertex2f(-3, 118); glVertex2f(3, 118);
        glVertex2f(3, 156);  glVertex2f(-3, 156);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(-2, 85);  glVertex2f(2, 85);
        glVertex2f(2, 115);  glVertex2f(-2, 115);
    glEnd();

    glColor3ub((unsigned char)(r * 0.55f),
               (unsigned char)(g * 0.55f),
               (unsigned char)(b * 0.55f));
    glBegin(GL_QUADS);
        glVertex2f(-95, 92); glVertex2f(95, 92);
        glVertex2f(95, 96);  glVertex2f(-95, 96);
    glEnd();

    // Headlight + beam
    glColor3ub(255, 240, 160);
    drawCircle(96, 102, 6);
    glColor3ub(255, 235, 120);
    glBegin(GL_TRIANGLES);
        glVertex2f(96, 102); glVertex2f(210, 130); glVertex2f(210, 75);
    glEnd();

    glColor3ub(220, 60, 50);
    drawCircle(-96, 102, 5);

    glColor3ub(40, 40, 45);
    glBegin(GL_QUADS);
        glVertex2f(95, 85);   glVertex2f(102, 85);
        glVertex2f(102, 92);  glVertex2f(95, 92);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(-102, 85); glVertex2f(-95, 85);
        glVertex2f(-95, 92);  glVertex2f(-102, 92);
    glEnd();

    // Wheels
    glColor3ub(25, 25, 30); drawCircle(-60, 82, 18); drawCircle(60, 82, 18);
    glColor3ub(20, 20, 20); drawCircle(-60, 82, 14); drawCircle(60, 82, 14);
    glColor3ub(160, 160, 170); drawCircle(-60, 82, 6); drawCircle(60, 82, 6);

    glColor3ub(90, 90, 100);
    glLineWidth(2);
    glBegin(GL_LINES);
        for (int s = 0; s < 4; s++) {
            float a = (3.1415926f / 4.0f) * s;
            glVertex2f(-60 + 6 * cosf(a), 82 + 6 * sinf(a));
            glVertex2f(-60 - 6 * cosf(a), 82 - 6 * sinf(a));
            glVertex2f( 60 + 6 * cosf(a), 82 + 6 * sinf(a));
            glVertex2f( 60 - 6 * cosf(a), 82 - 6 * sinf(a));
        }
    glEnd();
    glLineWidth(1);

    // Mirror
    glColor3ub(40, 40, 45);
    glBegin(GL_QUADS);
        glVertex2f(50, 122); glVertex2f(58, 122);
        glVertex2f(58, 130); glVertex2f(50, 130);
    glEnd();

    // Splashes
    glColor3ub(180, 200, 230);
    drawCircle(-72, 70, 3);
    drawCircle(-50, 68, 2);
    drawCircle(48,  68, 2);
    drawCircle(72,  70, 3);

    glPopMatrix();
}

void drawWetStreetLamp(float x) {
    glColor3ub(35, 38, 45);
    glBegin(GL_QUADS);
        glVertex2f(x - 3, 160); glVertex2f(x + 3, 160);
        glVertex2f(x + 3, 360); glVertex2f(x - 3, 360);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x, 360);      glVertex2f(x + 35, 360);
        glVertex2f(x + 35, 355); glVertex2f(x, 355);
    glEnd();

    glColor3ub(50, 55, 65);
    glBegin(GL_QUADS);
        glVertex2f(x + 27, 355); glVertex2f(x + 43, 355);
        glVertex2f(x + 43, 340); glVertex2f(x + 27, 340);
    glEnd();

    float f = 0.85f + 0.15f * sinf(streetLightFlicker);
    glColor3ub((unsigned char)(255 * f), (unsigned char)(220 * f), 110);
    drawCircle(x + 35, 332, 8);

    glColor3ub((unsigned char)(120 * f), (unsigned char)(100 * f), 40);
    glBegin(GL_TRIANGLES);
        glVertex2f(x + 35, 332);
        glVertex2f(x + 5,  200);
        glVertex2f(x + 70, 200);
    glEnd();
}

void drawPuddle(float cx, float cy, float w) {
    glColor3ub(70, 85, 105);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 24; i++) {
            float a = 2.0f * 3.1415926f * i / 24.0f;
            glVertex2f(cx + w * cosf(a), cy + (w * 0.28f) * sinf(a));
        }
    glEnd();

    glColor3ub(140, 170, 200);
    glBegin(GL_QUADS);
        glVertex2f(cx - w * 0.5f, cy + 1); glVertex2f(cx + w * 0.5f, cy + 1);
        glVertex2f(cx + w * 0.5f, cy + 3); glVertex2f(cx - w * 0.5f, cy + 3);
    glEnd();

    glColor3ub(190, 210, 230);
    glLineWidth(1);
    float r1 = 4.0f + 6.0f * (0.5f + 0.5f * sinf(puddleRipple));
    float r2 = 4.0f + 6.0f * (0.5f + 0.5f * sinf(puddleRipple + 2.0f));
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 24; i++) {
            float a = 2.0f * 3.1415926f * i / 24.0f;
            glVertex2f(cx - w * 0.25f + r1 * cosf(a), cy + (r1 * 0.3f) * sinf(a));
        }
    glEnd();
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 24; i++) {
            float a = 2.0f * 3.1415926f * i / 24.0f;
            glVertex2f(cx + w * 0.3f + r2 * cosf(a), cy + (r2 * 0.3f) * sinf(a));
        }
    glEnd();
}

void drawScene4Dog(float x) {
    glColor3ub(120, 85, 55);
    glBegin(GL_QUADS);
        glVertex2f(x - 18, 150); glVertex2f(x + 18, 150);
        glVertex2f(x + 18, 168); glVertex2f(x - 18, 168);
    glEnd();

    glColor3ub(135, 95, 60);
    glBegin(GL_QUADS);
        glVertex2f(x + 14, 168); glVertex2f(x + 32, 168);
        glVertex2f(x + 32, 184); glVertex2f(x + 14, 184);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x + 30, 170); glVertex2f(x + 38, 170);
        glVertex2f(x + 38, 178); glVertex2f(x + 30, 178);
    glEnd();

    glColor3ub(90, 60, 35);
    glBegin(GL_TRIANGLES);
        glVertex2f(x + 16, 184); glVertex2f(x + 20, 192); glVertex2f(x + 22, 184);
    glEnd();

    glColor3ub(0, 0, 0);
    drawCircle(x + 26, 178, 1.5f);

    glColor3ub(95, 65, 40);
    glBegin(GL_QUADS);
        glVertex2f(x - 14, 150); glVertex2f(x - 10, 150);
        glVertex2f(x - 10, 142); glVertex2f(x - 14, 142);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x + 10, 150); glVertex2f(x + 14, 150);
        glVertex2f(x + 14, 142); glVertex2f(x + 10, 142);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(x - 22, 164); glVertex2f(x - 16, 164);
        glVertex2f(x - 14, 174); glVertex2f(x - 20, 174);
    glEnd();
}

void drawDistantSpire() {
    unsigned char base = lightningOn ? 140 : 55;
    glColor3ub(base, base, (unsigned char)(base + 15));
    glBegin(GL_QUADS);
        glVertex2f(850, 200); glVertex2f(880, 200);
        glVertex2f(880, 380); glVertex2f(850, 380);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(850, 380); glVertex2f(880, 380); glVertex2f(865, 430);
    glEnd();
    if (s3BlinkOn) {
        glColor3ub(255, 50, 50);
        drawCircle(865, 432, 3);
    }
}

void scene4() {
    if (!rainInit) initRain();
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
        glColor3ub(55, 60, 75);    glVertex2d(0, 600);    glVertex2d(1000, 600);
        glColor3ub(95, 100, 115);  glVertex2d(1000, 200); glVertex2d(0, 200);
    glEnd();

    if (lightningOn) {
        glColor3ub(255, 255, 200);
        glLineWidth(3);
        glBegin(GL_LINE_STRIP);
            glVertex2f(300, 580); glVertex2f(310, 520);
            glVertex2f(295, 470); glVertex2f(320, 410); glVertex2f(300, 360);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertex2f(700, 580); glVertex2f(690, 530); glVertex2f(710, 480);
            glVertex2f(695, 430); glVertex2f(715, 380);
        glEnd();
        glLineWidth(1);
    }

    drawDistantSpire();
    drawRainHouses();
    drawRainStreet();

    drawPuddle(150, 50, 55);
    drawPuddle(480, 35, 70);
    drawPuddle(820, 55, 60);

    drawWetStreetLamp(80);
    drawWetStreetLamp(560);
    drawWetStreetLamp(940);

    drawCloudDark(120, 530);
    drawCloudDark(420, 555);
    drawCloudDark(720, 540);
    drawCloudDark(900, 560);

    // People (back sidewalk)
    drawUmbrellaPerson(person1X, 220, 50,  50,  50,  90,  160, armSwingPhase);
    drawUmbrellaPerson(person2X, 40,  130, 80,  200, 180, 60,  armSwingPhase + 1.5f);
    drawUmbrellaPerson(person3X, 30,  60,  160, 180, 60,  60,  armSwingPhase + 3.0f);

    // Dog
    drawScene4Dog(scene4DogX);

    // Cars (in front of people)
    drawRainCar(rainCar1X, 200, 70,  60,  false);
    drawRainCar(rainCar2X, 60,  120, 200, true);
    drawRainCar(rainCar3X, 230, 180, 70,  false);

    drawRainDrops();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    scene4();
    glutSwapBuffers();
}

void update(int /*value*/)
{
    if (!rainInit) initRain();

    for (int i = 0; i < RAIN_DROPS; i++) {
        rainY[i] -= 14;
        rainX[i] -= 4;
        if (rainY[i] < 0) {
            rainY[i] = 600;
            rainX[i] = (float)(rand() % 1100);
        }
    }

    person1X += 1.2f; if (person1X > 1050) person1X = -50;
    person2X -= 1.0f; if (person2X < -50)  person2X = 1050;
    person3X += 0.8f; if (person3X > 1050) person3X = -50;

    armSwingPhase      += 0.18f;
    puddleRipple       += 0.08f;
    streetLightFlicker += 0.25f;

    scene4DogX += 1.6f;
    if (scene4DogX > 1100) scene4DogX = -80;

    if (++lightningTimer > 180) {
        lightningOn    = !lightningOn;
        lightningTimer = lightningOn ? 175 : 0;
    }

    rainCar1X += 3.2f; if (rainCar1X > 1200) rainCar1X = -200;
    rainCar2X -= 2.6f; if (rainCar2X < -200) rainCar2X = 1200;
    rainCar3X += 2.0f; if (rainCar3X > 1200) rainCar3X = -200;

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
        case 'r': rainInit = false; break;
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
    glutCreateWindow("Urban City Life - Scene 4: Rainy Street");

    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(resize);
    glutTimerFunc(0, update, 0);
    glutDisplayFunc(display);

    myInit();
    glutMainLoop();
    return 0;
}
