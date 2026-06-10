#include <windows.h>      // remove if not on Windows
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <cstdlib>

int currentScene = 1;

// ===== Auto scene cycling =====
int sceneTimer = 0;
const int SCENE_DURATION = 750;  // ~12 seconds per scene (750 * 16ms)

// ===== Scene 1 =====
bool  night            = false;
float angle            = 0.1f;
float sunMinAngle      = 0.2f;
float sunMaxAngle      = 110.0f;
int   metroPosition    = -1400;
int   metroSpeed       = 6;
int   kitePositionX    = 0;
int   kitePositionY    = 0;
int   boatPosition     = -600;
int   boatPosition2    = 700;
int   boatSpeed        = 2;
bool  metroStop        = false;
int   kiteSpeed        = 8;

// ===== Scene 2 =====
bool    redLight         = false;
bool    nagorOff         = false;
GLfloat angleCarw        = 0.0f;
GLfloat angleAmbulancew  = 0.0f;
GLfloat angleTruckw      = 0.0f;
int     carPosition      = -400;
int     AmbulancePosition= -700;
int     truckPosition    = 600;
int     carSpeed         = 6;
int     AmbulanceSpeed   = 5;
int     truckSpeed       = 4;
GLfloat nagorAngle       = 0.0f;
int     ballPosition     = 0;
int     ballSpeed        = 0;
bool    ballForward      = true;
float   cloudPosition1   = 0;
float   cloudPosition2   = 0;
float   cloudPosition3   = 0;
bool    sirenToggle      = false;
int     sirenTimer       = 0;
int     planePosition    = 500;
int     planeSpeed       = 3;

// ===== Scene 3 (Sunset -> Night city) =====
float sunsetY        = 470.0f;
float sunsetSpeed    = 0.25f;
float sunsetBoat     = -200.0f;
float sunsetBirdX    = 0.0f;
float sunsetCloud1   = 100.0f;
float sunsetCloud2   = 500.0f;
float sunsetProgress = 0.0f;
float sunsetCar1X    = -200.0f;
float sunsetCar2X    = 1100.0f;
bool  s3BlinkOn      = false;
int   s3BlinkTimer   = 0;

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

static inline unsigned char lerpUB(unsigned char a, unsigned char b, float t) {
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    return (unsigned char)(a + (b - a) * t);
}

// ====================== SCENE 1 HELPERS ======================
void drawMetroRoad() {
    glColor3ub(97, 97, 97);
    glBegin(GL_QUADS);
        glVertex2d(0, 60);    glVertex2d(1000, 60);
        glVertex2d(1000, 90); glVertex2d(0, 90);
    glEnd();

    glColor3ub(128, 128, 128);
    for (int i = 0; i <= 1000; i += 200) {
        glBegin(GL_QUADS);
            glVertex2d(i + 30, 0);  glVertex2d(i + 90, 0);
            glVertex2d(i + 90, 60); glVertex2d(i + 30, 60);
        glEnd();
    }
    for (int i = 0; i <= 1000; i += 200) {
        glBegin(GL_QUADS);
            glVertex2d(i, 40);       glVertex2d(i + 120, 40);
            glVertex2d(i + 120, 60); glVertex2d(i, 60);
        glEnd();
    }
}

void drawGreenField() {
    int r, g, b;
    if (!night) { r = 55; g = 125; b = 34; }
    else        { r = 44; g = 99;  b = 27; }
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2d(0, 0);      glVertex2d(1000, 0);
        glVertex2d(1000, 370); glVertex2d(0, 370);
    glEnd();
}

void drawSky() {
    int r, g, b, rc, gc, bc;
    if (!night) { r = 115; g = 251; b = 253; rc = 221; gc = 232; bc = 40;  }
    else        { r = 26;  g = 27;  b = 59;  rc = 217; gc = 214; bc = 200; }

    glBegin(GL_QUADS);
        glColor3ub(r, g, b);
        glVertex2d(0, 370);    glVertex2d(1000, 370);
        glVertex2d(1000, 600); glVertex2d(0, 600);
    glEnd();

    glPushMatrix();
    glTranslatef(500, 0, 0.0f);
    glRotatef(-angle, 0, 0, 1);
    glTranslatef(-500, -100, 0.0f);
    glColor3ub(rc, gc, bc);
    drawCircle(20, 470, 20);
    glPopMatrix();
}

void drawPond() {
    int r1, g1, b1, r2, g2, b2;
    if (!night) { r1 = 50; g1 = 130; b1 = 246; r2 = 37; g2 = 96; b2 = 181; }
    else        { r1 = 44; g1 = 115; b1 = 227; r2 = 30; g2 = 78; b2 = 148; }

    glBegin(GL_QUADS);
        glColor3ub(r1, g1, b1);
        glVertex2d(0, 150);    glVertex2d(1000, 150);
        glColor3ub(r2, g2, b2);
        glVertex2d(1000, 320); glVertex2d(0, 320);
    glEnd();
}

void drawKite() {
    if (night) return;

    glPushMatrix();
    glTranslatef(216.0f, 416.0f, 0.0f);
    glScalef(0.55f, 0.55f, 1.0f);
    glTranslatef(-216.0f, -416.0f, 0.0f);

    glColor3ub(111, 73, 164);
    glBegin(GL_QUADS);
        glVertex2d(kitePositionX + 300, 535 + kitePositionY);
        glVertex2d(kitePositionX + 276, 504 + kitePositionY);
        glVertex2d(kitePositionX + 300, 468 + kitePositionY);
        glVertex2d(kitePositionX + 324, 504 + kitePositionY);
    glEnd();

    glColor3ub(235, 51, 36);
    glBegin(GL_TRIANGLES);
        glVertex2d(kitePositionX + 300, 468 + kitePositionY);
        glVertex2d(kitePositionX + 296, 460 + kitePositionY);
        glVertex2d(kitePositionX + 304, 460 + kitePositionY);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
        glVertex2i(kitePositionX + 276, 504 + kitePositionY);
        glVertex2i(kitePositionX + 324, 504 + kitePositionY);
        glVertex2i(kitePositionX + 300, 535 + kitePositionY);
        glVertex2i(kitePositionX + 300, 468 + kitePositionY);
        glVertex2i(kitePositionX + 300, 468 + kitePositionY);
        glVertex2i(216, 416);
    glEnd();

    glPopMatrix();
}

void drawMan() {
    if (night) return;

    glPushMatrix();
    glTranslatef(216.0f, 416.0f, 0.0f);
    glScalef(0.55f, 0.55f, 1.0f);
    glTranslatef(-216.0f, -416.0f, 0.0f);

    glColor3ub(153, 43, 14);
    glBegin(GL_QUADS);
        glVertex2d(180, 400); glVertex2d(150, 400);
        glVertex2d(150, 355); glVertex2d(180, 355);
    glEnd();

    glColor3ub(224, 190, 189);
    glBegin(GL_QUADS);
        glVertex2d(180, 355); glVertex2d(170, 355);
        glVertex2d(170, 328); glVertex2d(180, 328);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(160, 355); glVertex2d(150, 355);
        glVertex2d(150, 328); glVertex2d(160, 328);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(207, 417); glVertex2d(180, 400);
        glVertex2d(180, 390); glVertex2d(210, 407);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(150, 400); glVertex2d(143, 400);
        glVertex2d(143, 365); glVertex2d(150, 365);
    glEnd();

    glColor3ub(20, 20, 20);
    drawCircle(165, 414, 13);

    glColor3ub(156, 87, 27);
    glBegin(GL_QUADS);
        glVertex2d(213, 426); glVertex2d(205, 423);
        glVertex2d(211, 401); glVertex2d(219, 405);
    glEnd();

    glColor3ub(20, 20, 20);
    glBegin(GL_QUADS);
        glVertex2d(215, 419); glVertex2d(207, 415);
        glVertex2d(209, 409); glVertex2d(217, 413);
    glEnd();

    glPopMatrix();
}

// Helper to draw the hull/sail of either boat at a given base offset
static void drawBoatBody(int basePos, int dx, int dy) {
    glColor3ub(199, 99, 30);
    glBegin(GL_QUADS);
        glVertex2d(basePos + 240 + dx, 290 + dy);
        glVertex2d(basePos + 100 + dx, 290 + dy);
        glVertex2d(basePos + 100 + dx, 250 + dy);
        glVertex2d(basePos + 240 + dx, 250 + dy);
    glEnd();

    glColor3ub(0, 10, 99);
    glBegin(GL_QUADS);
        glVertex2d(basePos + 230 + dx, 280 + dy);
        glVertex2d(basePos + 180 + dx, 280 + dy);
        glVertex2d(basePos + 180 + dx, 260 + dy);
        glVertex2d(basePos + 230 + dx, 260 + dy);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(basePos + 160 + dx, 280 + dy);
        glVertex2d(basePos + 110 + dx, 280 + dy);
        glVertex2d(basePos + 110 + dx, 260 + dy);
        glVertex2d(basePos + 160 + dx, 260 + dy);
    glEnd();

    glColor3ub(28, 64, 17);
    glBegin(GL_TRIANGLES);
        glVertex2d(basePos + 100 + dx, 290 + dy);
        glVertex2d(basePos + 60  + dx, 270 + dy);
        glVertex2d(basePos + 100 + dx, 250 + dy);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2d(basePos + 240 + dx, 290 + dy);
        glVertex2d(basePos + 240 + dx, 250 + dy);
        glVertex2d(basePos + 280 + dx, 270 + dy);
    glEnd();

    if (!night) {
        glColor3ub(127, 130, 187);
        glBegin(GL_QUADS);
            glVertex2d(basePos + 140 + dx, 300 + dy);
            glVertex2d(basePos + 115 + dx, 300 + dy);
            glVertex2d(basePos + 115 + dx, 265 + dy);
            glVertex2d(basePos + 140 + dx, 265 + dy);
        glEnd();

        glColor3ub(50, 50, 50);
        drawCircle(basePos + 127.5 + dx, 312 + dy, 12);

        glColor3ub(224, 190, 189);
        glBegin(GL_QUADS);
            glVertex2d(basePos + 140 + dx, 300 + dy);
            glVertex2d(basePos + 140 + dx, 290 + dy);
            glVertex2d(basePos + 165 + dx, 290 + dy);
            glVertex2d(basePos + 170 + dx, 300 + dy);
        glEnd();
        glBegin(GL_QUADS);
            glVertex2d(basePos + 115 + dx, 300 + dy);
            glVertex2d(basePos + 115 + dx, 290 + dy);
            glVertex2d(basePos + 140 + dx, 260 + dy);
            glVertex2d(basePos + 145 + dx, 270 + dy);
        glEnd();
    }

    glColor3ub(50, 50, 50);
    glBegin(GL_QUADS);
        glVertex2d(basePos + 168 + dx, 311 + dy);
        glVertex2d(basePos + 119 + dx, 230 + dy);
        glVertex2d(basePos + 124 + dx, 228 + dy);
        glVertex2d(basePos + 174 + dx, 308 + dy);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2d(basePos + 166 + dx, 323 + dy);
        glVertex2d(basePos + 168 + dx, 307 + dy);
        glVertex2d(basePos + 184 + dx, 311 + dy);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2d(basePos + 124 + dx, 233 + dy);
        glVertex2d(basePos + 109 + dx, 225 + dy);
        glVertex2d(basePos + 125 + dx, 218 + dy);
    glEnd();
}

void drawBoat() {
    int dx = night ? 70 : 0;
    int dy = night ? 40 : 0;
    drawBoatBody(boatPosition, dx, dy);
}

void drawBoat2() {
    int dx = night ? 300 : 400;
    int dy = night ? 40  : -60;

    glPushMatrix();
    float pivotX = boatPosition2 + dx + 170;
    glTranslatef(pivotX, 0, 0);
    glScalef(-1.0f, 1.0f, 1.0f);
    glTranslatef(-pivotX, 0, 0);

    drawBoatBody(boatPosition2, dx, dy);

    glPopMatrix();
}

void drawMetro() {
    int rb, gb, bb;
    if (!night) { rb = 222; gb = 219; bb = 205; }
    else        { rb = 184; gb = 181; bb = 170; }

    glColor3ub(rb, gb, bb);
    glBegin(GL_QUADS);
        glVertex2d(metroPosition + 650, 180);
        glVertex2d(metroPosition + 15,  180);
        glVertex2d(metroPosition + 15,  90);
        glVertex2d(metroPosition + 650, 90);
    glEnd();

    int r, g, b;
    if (!night) { r = 171; g = 169; b = 158; }
    else        { r = 255; g = 201; b = 14;  }

    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2d(metroPosition + 650, 115);
        glVertex2d(metroPosition + 630, 115);
        glVertex2d(metroPosition + 630, 95);
        glVertex2d(metroPosition + 650, 95);
    glEnd();

    if (night) {
        glBegin(GL_LINES);
            glVertex2i(metroPosition + 652, 115); glVertex2i(metroPosition + 690, 120);
            glVertex2i(metroPosition + 652, 105); glVertex2i(metroPosition + 690, 105);
            glVertex2i(metroPosition + 652, 95);  glVertex2i(metroPosition + 690, 90);
        glEnd();
    }

    glColor3ub(1, 71, 45);
    glBegin(GL_QUADS);
        glVertex2d(metroPosition + 650, 180); glVertex2d(metroPosition + 600, 180);
        glVertex2d(metroPosition + 600, 125); glVertex2d(metroPosition + 650, 125);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(metroPosition + 650, 180); glVertex2d(metroPosition + 15, 180);
        glVertex2d(metroPosition + 15,  172); glVertex2d(metroPosition + 650, 172);
    glEnd();

    int rw, gw, bw;
    if (!night) { rw = 33;  gw = 33;  bw = 33; }
    else        { rw = 255; gw = 201; bw = 14; }

    glColor3ub(rw, gw, bw);
    for (int i = 0; i <= 500; i += 150) {
        glBegin(GL_QUADS);
            glVertex2d(metroPosition + 575 - i, 160);
            glVertex2d(metroPosition + 525 - i, 160);
            glVertex2d(metroPosition + 525 - i, 125);
            glVertex2d(metroPosition + 575 - i, 125);
        glEnd();
    }
    glBegin(GL_QUADS);
        glVertex2d(metroPosition + 55, 160); glVertex2d(metroPosition + 25, 160);
        glVertex2d(metroPosition + 25, 125); glVertex2d(metroPosition + 55, 125);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(metroPosition + 650, 172); glVertex2d(metroPosition + 615, 172);
        glVertex2d(metroPosition + 615, 130); glVertex2d(metroPosition + 650, 130);
    glEnd();

    for (int i = 0; i <= 300; i += 150) {
        glColor3ub(128, 128, 128);
        glBegin(GL_QUADS);
            glVertex2d(metroPosition + 500 - i, 172);
            glVertex2d(metroPosition + 450 - i, 172);
            glVertex2d(metroPosition + 450 - i, 90);
            glVertex2d(metroPosition + 500 - i, 90);
        glEnd();

        glColor3ub(173, 38, 27);
        glBegin(GL_QUADS);
            glVertex2d(metroPosition + 479 - i, 172);
            glVertex2d(metroPosition + 471 - i, 172);
            glVertex2d(metroPosition + 471 - i, 90);
            glVertex2d(metroPosition + 479 - i, 90);
        glEnd();

        glColor3ub(33, 33, 33);
        glBegin(GL_QUADS);
            glVertex2d(metroPosition + 495 - i, 160);
            glVertex2d(metroPosition + 484 - i, 160);
            glVertex2d(metroPosition + 484 - i, 125);
            glVertex2d(metroPosition + 495 - i, 125);
        glEnd();
        glBegin(GL_QUADS);
            glVertex2d(metroPosition + 466 - i, 160);
            glVertex2d(metroPosition + 455 - i, 160);
            glVertex2d(metroPosition + 455 - i, 125);
            glVertex2d(metroPosition + 466 - i, 125);
        glEnd();
    }
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

void drawTreeType1(int offsetX, int offsetY) {
    glBegin(GL_QUADS);
        glColor3ub(80, 49, 30);
        glVertex2i(457 + offsetX, 200 + offsetY);
        glVertex2i(457 + offsetX, 225 + offsetY);
        glColor3ub(100, 49, 30);
        glVertex2i(442 + offsetX, 225 + offsetY);
        glVertex2i(442 + offsetX, 200 + offsetY);
    glEnd();

    const unsigned char greens[4][3] = {
        { 30,  75,  40},
        { 30,  95,  40},
        { 30, 115,  40},
        { 30, 135,  40},
    };
    const int yBase[4] = {225, 240, 255, 270};
    const int yTip[4]  = {260, 270, 280, 290};
    const int half[4]  = {30, 30, 25, 20};

    for (int i = 0; i < 4; i++) {
        glColor3ub(greens[i][0], greens[i][1], greens[i][2]);
        glBegin(GL_TRIANGLES);
            glVertex2i(450 + half[i] + offsetX, yBase[i] + offsetY);
            glVertex2i(450 + offsetX,           yTip[i]  + offsetY);
            glVertex2i(450 - half[i] + offsetX, yBase[i] + offsetY);
        glEnd();
    }
}

// ====================== SCENE 2 HELPERS ======================
void drawRoad() {
    glColor3ub(50, 50, 50);
    glBegin(GL_QUADS);
        glVertex2d(0, 0);      glVertex2d(1000, 0);
        glVertex2d(1000, 200); glVertex2d(0, 200);
    glEnd();

    glColor3ub(255, 255, 255);
    for (int i = 0; i <= 1000; i += 70) {
        glBegin(GL_QUADS);
            glVertex2d(i, 90);       glVertex2d(i + 50, 90);
            glVertex2d(i + 50, 110); glVertex2d(i, 110);
        glEnd();
    }
}

void drawRoadField() {
    int r, g, b;
    if (!night) { r = 27; g = 140; b = 26; }
    else        { r = 19; g = 99;  b = 18; }
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2d(0, 200);    glVertex2d(0, 210);
        glVertex2d(1000, 210); glVertex2d(1000, 200);
    glEnd();
}

void drawSky2() {
    int r, g, b, rc, gc, bc, rcl, gcl, bcl;
    if (!night) {
        r = 160; g = 217; b = 239;
        rc = 255; gc = 147; bc = 68;
        rcl = 255; gcl = 255; bcl = 255;
    } else {
        r = 26; g = 27; b = 59;
        rc = 217; gc = 214; bc = 200;
        rcl = 204; gcl = 204; bcl = 204;
    }

    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2d(0, 380);    glVertex2d(0, 600);
        glVertex2d(1000, 600); glVertex2d(1000, 380);
    glEnd();

    glColor3ub(rc, gc, bc);
    drawCircle(80, 530, 20);

    glColor3ub(rcl, gcl, bcl);
    drawCircle(cloudPosition1 + 180, 560, 20);
    drawCircle(cloudPosition1 + 200, 560, 25);
    drawCircle(cloudPosition1 + 230, 560, 15);
    drawCircle(cloudPosition2 + 500, 500, 20);
    drawCircle(cloudPosition2 + 535, 500, 30);
    drawCircle(cloudPosition2 + 570, 500, 18);
    drawCircle(cloudPosition3 + 800, 560, 20);
    drawCircle(cloudPosition3 + 830, 560, 30);
    drawCircle(cloudPosition3 + 865, 560, 18);
}

void drawPlayField() {
    int rd, gd, bd, rg, gg, bg;
    if (!night) { rd = 225; gd = 191; bd = 146; rg = 80; gg = 134; bg = 33; }
    else        { rd = 161; gd = 136; bd = 104; rg = 58; gg = 97;  bg = 24; }

    glColor3ub(rd, gd, bd);
    glBegin(GL_QUADS);
        glVertex2d(0, 210);    glVertex2d(0, 380);
        glVertex2d(1000, 380); glVertex2d(1000, 210);
    glEnd();

    glColor3ub(rg, gg, bg);
    glBegin(GL_QUADS);
        glVertex2d(20, 350);  glVertex2d(20, 230);
        glVertex2d(600, 230); glVertex2d(600, 350);
    glEnd();

    if (night) return;

    // Player 1 (right side)
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
        glVertex2d(550, 280); glVertex2d(575, 280);
        glVertex2d(575, 310); glVertex2d(550, 310);
    glEnd();
    glColor3ub(0, 0, 255);
    glBegin(GL_QUADS);
        glVertex2d(550, 280); glVertex2d(540, 250);
        glVertex2d(550, 250); glVertex2d(565, 280);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(565, 250); glVertex2d(575, 250);
        glVertex2d(575, 280); glVertex2d(565, 280);
    glEnd();
    glColor3ub(219, 164, 155);
    glBegin(GL_QUADS);
        glVertex2d(535, 295); glVertex2d(535, 285);
        glVertex2d(550, 300); glVertex2d(550, 310);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(560, 285); glVertex2d(575, 300);
        glVertex2d(575, 310); glVertex2d(560, 295);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(562.5, 320, 9);

    // Player 2 (left side)
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
        glVertex2d(50, 280); glVertex2d(75, 280);
        glVertex2d(75, 310); glVertex2d(50, 310);
    glEnd();
    glColor3ub(0, 0, 255);
    glBegin(GL_QUADS);
        glVertex2d(50, 280); glVertex2d(50, 250);
        glVertex2d(60, 250); glVertex2d(60, 280);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(70, 250); glVertex2d(80, 250);
        glVertex2d(75, 280); glVertex2d(60, 280);
    glEnd();
    glColor3ub(219, 164, 155);
    glBegin(GL_QUADS);
        glVertex2d(75, 300); glVertex2d(95, 290);
        glVertex2d(95, 300); glVertex2d(75, 310);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(50, 310); glVertex2d(50, 300);
        glVertex2d(75, 290); glVertex2d(75, 300);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(62.5, 320, 9);

    // Ball
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(ballPosition + 90, 260, 12);
}

void drawCabin(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(nagorAngle, 0, 0, 1);

    glColor3ub(143, 145, 132);
    glBegin(GL_TRIANGLES);
        glVertex2d(0, 20); glVertex2d(-20, -10); glVertex2d(20, -10);
    glEnd();

    glColor3ub(194, 10, 5);
    glBegin(GL_QUADS);
        glVertex2d(-20, -10); glVertex2d(-20, -40);
        glVertex2d(20, -40);  glVertex2d(20, -10);
    glEnd();

    glPopMatrix();
}

void drawNagor() {
    glColor3ub(150, 71, 53);
    glBegin(GL_QUADS);
        glVertex2d(800, 320); glVertex2d(810, 320);
        glVertex2d(810, 420); glVertex2d(800, 420);
    glEnd();

    glPushMatrix();
    const float cx = 805.0f, cy = 440.0f;
    glTranslatef(cx, cy, 0);
    glRotatef(-nagorAngle, 0, 0, 1);
    glTranslatef(-cx, -cy, 0);

    glColor3ub(150, 71, 53);
    glBegin(GL_QUADS);
        glVertex2d(725, 485); glVertex2d(725, 480);
        glVertex2d(885, 396); glVertex2d(885, 401);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(725, 396); glVertex2d(725, 401);
        glVertex2d(885, 480); glVertex2d(885, 485);
    glEnd();

    drawCabin(730, 400);
    drawCabin(880, 400);
    drawCabin(730, 480);
    drawCabin(880, 480);
    glPopMatrix();

    glColor3ub(150, 71, 53);
    glBegin(GL_QUADS);
        glVertex2d(780, 460); glVertex2d(780, 420);
        glVertex2d(830, 420); glVertex2d(830, 460);
    glEnd();
}

void drawVarsity() {
    int rg, gg, bg;
    if (!night) { rg = 132; gg = 166; bg = 191; }
    else        { rg = 255; gg = 201; bg = 14;  }

    glColor3ub(220, 222, 191);
    glBegin(GL_QUADS);
        glVertex2d(200, 370); glVertex2d(200, 570);
        glVertex2d(320, 570); glVertex2d(320, 370);
    glEnd();

    glColor3ub(rg, gg, bg);
    glBegin(GL_QUADS); glVertex2d(207, 565); glVertex2d(207, 505); glVertex2d(313, 505); glVertex2d(313, 565); glEnd();
    glBegin(GL_QUADS); glVertex2d(207, 500); glVertex2d(207, 440); glVertex2d(313, 440); glVertex2d(313, 500); glEnd();
    glBegin(GL_QUADS); glVertex2d(207, 435); glVertex2d(207, 375); glVertex2d(313, 375); glVertex2d(313, 435); glEnd();

    glColor3ub(143, 145, 132);
    glBegin(GL_QUADS);
        glVertex2d(320, 570); glVertex2d(320, 370);
        glVertex2d(350, 370); glVertex2d(350, 500);
    glEnd();

    glColor3ub(96, 115, 128);
    glBegin(GL_POLYGON);
        glVertex2d(420, 480); glVertex2d(385, 500);
        glVertex2d(350, 500); glVertex2d(350, 370); glVertex2d(420, 370);
    glEnd();

    glColor3ub(rg, gg, bg);
    glBegin(GL_POLYGON);
        glVertex2d(358, 495); glVertex2d(358, 460); glVertex2d(385, 460);
        glVertex2d(412, 450); glVertex2d(412, 475); glVertex2d(385, 495);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2d(358, 455); glVertex2d(358, 420); glVertex2d(385, 420);
        glVertex2d(412, 410); glVertex2d(412, 445); glVertex2d(385, 455);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2d(358, 415); glVertex2d(358, 380); glVertex2d(385, 380);
        glVertex2d(412, 375); glVertex2d(412, 405); glVertex2d(385, 415);
    glEnd();

    glColor3ub(131, 133, 121);
    glBegin(GL_QUADS);
        glVertex2d(420, 480); glVertex2d(420, 370);
        glVertex2d(550, 370); glVertex2d(550, 480);
    glEnd();

    glColor3ub(rg, gg, bg);
    glBegin(GL_QUADS); glVertex2d(425, 475); glVertex2d(425, 450); glVertex2d(482, 450); glVertex2d(482, 475); glEnd();
    glBegin(GL_QUADS); glVertex2d(488, 475); glVertex2d(488, 450); glVertex2d(545, 450); glVertex2d(545, 475); glEnd();
    glBegin(GL_QUADS); glVertex2d(425, 445); glVertex2d(425, 410); glVertex2d(481, 410); glVertex2d(481, 445); glEnd();
    glBegin(GL_QUADS); glVertex2d(488, 445); glVertex2d(488, 410); glVertex2d(545, 410); glVertex2d(545, 445); glEnd();
    glBegin(GL_QUADS); glVertex2d(425, 405); glVertex2d(425, 375); glVertex2d(481, 375); glVertex2d(481, 405); glEnd();
    glBegin(GL_QUADS); glVertex2d(488, 405); glVertex2d(488, 375); glVertex2d(545, 375); glVertex2d(545, 405); glEnd();
}

void drawTrafficSignal() {
    glColor3ub(85, 87, 79);
    glBegin(GL_QUADS);
        glVertex2d(950, 350); glVertex2d(950, 200);
        glVertex2d(930, 200); glVertex2d(930, 350);
    glEnd();

    glColor3ub(55, 56, 51);
    glBegin(GL_QUADS);
        glVertex2d(960, 340); glVertex2d(960, 280);
        glVertex2d(920, 280); glVertex2d(920, 340);
    glEnd();

    if (!redLight) {
        glColor3f(0.0f, 1.0f, 0.0f);
        drawCircle(940, 295, 12);
    } else {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(940, 325, 12);
    }
}

void drawPlane() {
    int r, g, b;
    if (!night) { r = 132; g = 166; b = 191; }
    else        { r = 255; g = 201; b = 14;  }

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
        glVertex2d(planePosition + 950, 530); glVertex2d(planePosition + 960, 570);
        glVertex2d(planePosition + 950, 570); glVertex2d(planePosition + 932, 545);
        glVertex2d(planePosition + 870, 545); glVertex2d(planePosition + 850, 530);
    glEnd();

    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
        glVertex2d(planePosition + 946, 520); glVertex2d(planePosition + 950, 530);
        glVertex2d(planePosition + 850, 530); glVertex2d(planePosition + 880, 520);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
        glVertex2d(planePosition + 904.5, 540); glVertex2d(planePosition + 917.5, 560);
        glVertex2d(planePosition + 910.5, 560); glVertex2d(planePosition + 893.5, 540);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2d(planePosition + 856, 532); glVertex2d(planePosition + 882, 532);
        glVertex2d(planePosition + 888, 542); glVertex2d(planePosition + 872, 542);
    glEnd();
    drawCircle(planePosition + 898, 537, 3);
    drawCircle(planePosition + 908, 537, 3);
    drawCircle(planePosition + 918, 537, 3);
    drawCircle(planePosition + 928, 537, 3);
}

void drawCar() {
    glColor3ub(235, 51, 36);
    glBegin(GL_QUADS);
        glVertex2d(carPosition + 314, 185); glVertex2d(carPosition + 70, 185);
        glVertex2d(carPosition + 70,  150); glVertex2d(carPosition + 314, 150);
    glEnd();

    int r, g, b;
    if (!night) { r = 132; g = 166; b = 191; }
    else        { r = 255; g = 201; b = 14;  }
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2d(carPosition + 314, 175); glVertex2d(carPosition + 300, 175);
        glVertex2d(carPosition + 300, 160); glVertex2d(carPosition + 314, 160);
    glEnd();

    if (night) {
        glBegin(GL_LINES);
            glVertex2i(carPosition + 316, 175); glVertex2i(carPosition + 336, 178);
            glVertex2i(carPosition + 316, 168); glVertex2i(carPosition + 336, 168);
            glVertex2i(carPosition + 316, 160); glVertex2i(carPosition + 336, 157);
        glEnd();
    }

    glColor3ub(50, 130, 246);
    glBegin(GL_QUADS);
        glVertex2d(carPosition + 126, 215); glVertex2d(carPosition + 105, 185);
        glVertex2d(carPosition + 260, 185); glVertex2d(carPosition + 220, 215);
    glEnd();

    // Wheels
    const float wheelY = 149.0f;
    const float wx[2]  = { carPosition + 270.0f, carPosition + 110.0f };
    glColor3ub(0, 0, 0);
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(wx[i], wheelY, 0.0f);
        glRotatef(-angleCarw, 0, 0, 1);
        glTranslatef(-wx[i], -wheelY, 0.0f);
        float ox = wx[i] - (carPosition + 270.0f);
        glBegin(GL_POLYGON);
            glVertex2d(carPosition + 260 + ox, 165); glVertex2d(carPosition + 250 + ox, 147);
            glVertex2d(carPosition + 264 + ox, 133); glVertex2d(carPosition + 282 + ox, 141);
            glVertex2d(carPosition + 280 + ox, 160);
        glEnd();
        glPopMatrix();
    }
}

void drawAmbulance() {
    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);
        glVertex2d(AmbulancePosition + 560, 137); glVertex2d(AmbulancePosition + 814, 137);
        glVertex2d(AmbulancePosition + 814, 175); glVertex2d(AmbulancePosition + 790, 185);
        glVertex2d(AmbulancePosition + 750, 215); glVertex2d(AmbulancePosition + 560, 215);
    glEnd();

    int r, g, b;
    if (!night) { r = 132; g = 166; b = 191; }
    else        { r = 255; g = 201; b = 14;  }
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2d(AmbulancePosition + 814, 170); glVertex2d(AmbulancePosition + 800, 170);
        glVertex2d(AmbulancePosition + 800, 155); glVertex2d(AmbulancePosition + 814, 155);
    glEnd();

    if (night) {
        glBegin(GL_LINES);
            glVertex2i(AmbulancePosition + 816, 170); glVertex2i(AmbulancePosition + 840, 175);
            glVertex2i(AmbulancePosition + 816, 162); glVertex2i(AmbulancePosition + 840, 162);
            glVertex2i(AmbulancePosition + 816, 155); glVertex2i(AmbulancePosition + 840, 150);
        glEnd();
    }

    glColor3ub(50, 130, 246);
    glBegin(GL_QUADS);
        glVertex2d(AmbulancePosition + 690, 185); glVertex2d(AmbulancePosition + 780, 185);
        glVertex2d(AmbulancePosition + 745, 210); glVertex2d(AmbulancePosition + 690, 210);
    glEnd();

    // Siren (toggling colors)
    if (sirenToggle) glColor3ub(255, 0, 0); else glColor3ub(50, 130, 246);
    glBegin(GL_QUADS);
        glVertex2d(AmbulancePosition + 730, 215); glVertex2d(AmbulancePosition + 740, 215);
        glVertex2d(AmbulancePosition + 740, 225); glVertex2d(AmbulancePosition + 730, 225);
    glEnd();
    if (sirenToggle) glColor3ub(50, 130, 246); else glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
        glVertex2d(AmbulancePosition + 730, 215); glVertex2d(AmbulancePosition + 730, 225);
        glVertex2d(AmbulancePosition + 720, 225); glVertex2d(AmbulancePosition + 720, 215);
    glEnd();

    // Red cross
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
        glVertex2d(AmbulancePosition + 599.5, 176.5); glVertex2d(AmbulancePosition + 620.5, 176.5);
        glVertex2d(AmbulancePosition + 620.5, 181.5); glVertex2d(AmbulancePosition + 599.5, 181.5);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(AmbulancePosition + 607.5, 168); glVertex2d(AmbulancePosition + 612.5, 168);
        glVertex2d(AmbulancePosition + 612.5, 189); glVertex2d(AmbulancePosition + 607.5, 189);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(AmbulancePosition + 560, 150); glVertex2d(AmbulancePosition + 814, 150);
        glVertex2d(AmbulancePosition + 814, 156); glVertex2d(AmbulancePosition + 560, 156);
    glEnd();

    // Wheels
    const float wheelY = 130.0f;
    const float wx[2]  = { AmbulancePosition + 591.5f, AmbulancePosition + 777.0f };
    glColor3ub(0, 0, 0);
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(wx[i], wheelY, 0.0f);
        glRotatef(-angleAmbulancew, 0, 0, 1);
        glTranslatef(-wx[i], -wheelY, 0.0f);
        float ox = wx[i] - (AmbulancePosition + 591.5f);
        glBegin(GL_POLYGON);
            glVertex2d(AmbulancePosition + 584.5 + ox, 146); glVertex2d(AmbulancePosition + 575.5 + ox, 128);
            glVertex2d(AmbulancePosition + 589.5 + ox, 114); glVertex2d(AmbulancePosition + 607.5 + ox, 122);
            glVertex2d(AmbulancePosition + 605.5 + ox, 141);
        glEnd();
        glPopMatrix();
    }
}

void drawTruck() {
    glColor3ub(255, 171, 51);
    glBegin(GL_QUADS);
        glVertex2d(truckPosition + 750, 100); glVertex2d(truckPosition + 680, 100);
        glVertex2d(truckPosition + 680, 30);  glVertex2d(truckPosition + 750, 30);
    glEnd();

    int r, g, b;
    if (!night) { r = 132; g = 166; b = 191; }
    else        { r = 255; g = 201; b = 14;  }
    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2d(truckPosition + 680, 58); glVertex2d(truckPosition + 695, 58);
        glVertex2d(truckPosition + 695, 43); glVertex2d(truckPosition + 680, 43);
    glEnd();

    if (night) {
        glBegin(GL_LINES);
            glVertex2i(truckPosition + 678, 58); glVertex2i(truckPosition + 650, 63);
            glVertex2i(truckPosition + 678, 50); glVertex2i(truckPosition + 650, 50);
            glVertex2i(truckPosition + 678, 43); glVertex2i(truckPosition + 650, 38);
        glEnd();
    }

    glColor3ub(50, 130, 246);
    glBegin(GL_QUADS);
        glVertex2d(truckPosition + 710, 96); glVertex2d(truckPosition + 682, 96);
        glVertex2d(truckPosition + 682, 70); glVertex2d(truckPosition + 710, 70);
    glEnd();

    glColor3ub(100, 100, 100);
    glBegin(GL_QUADS);
        glVertex2d(truckPosition + 950, 80); glVertex2d(truckPosition + 750, 80);
        glVertex2d(truckPosition + 750, 30); glVertex2d(truckPosition + 950, 30);
    glEnd();

    // Wheels
    const float wheelY = 30.0f;
    const float wx[2]  = { truckPosition + 715.0f, truckPosition + 900.0f };
    glColor3ub(0, 0, 0);
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(wx[i], wheelY, 0.0f);
        glRotatef(angleTruckw, 0, 0, 1);
        glTranslatef(-wx[i], -wheelY, 0.0f);
        float ox = wx[i] - (truckPosition + 715.0f);
        glBegin(GL_POLYGON);
            glVertex2d(truckPosition + 705 + ox, 45); glVertex2d(truckPosition + 695 + ox, 27);
            glVertex2d(truckPosition + 709 + ox, 13); glVertex2d(truckPosition + 727 + ox, 21);
            glVertex2d(truckPosition + 725 + ox, 40);
        glEnd();
        glPopMatrix();
    }
}

// ====================== SCENE 3 : SUNSET -> NIGHT CITY ======================
void drawScene3Sky() {
    float t = sunsetProgress;
    unsigned char top_r = lerpUB(60,  5,  t);
    unsigned char top_g = lerpUB(25,  5,  t);
    unsigned char top_b = lerpUB(90,  35, t);
    unsigned char mid_r = lerpUB(230, 30, t);
    unsigned char mid_g = lerpUB(95,  25, t);
    unsigned char mid_b = lerpUB(60,  70, t);
    unsigned char low_r = lerpUB(255, 50, t);
    unsigned char low_g = lerpUB(200, 35, t);
    unsigned char low_b = lerpUB(90,  95, t);

    glBegin(GL_QUADS);
        glColor3ub(top_r, top_g, top_b); glVertex2d(0, 600);    glVertex2d(1000, 600);
        glColor3ub(mid_r, mid_g, mid_b); glVertex2d(1000, 480); glVertex2d(0, 480);
    glEnd();
    glBegin(GL_QUADS);
        glColor3ub(mid_r, mid_g, mid_b); glVertex2d(0, 480);    glVertex2d(1000, 480);
        glColor3ub(low_r, low_g, low_b); glVertex2d(1000, 380); glVertex2d(0, 380);
    glEnd();

    // Stars
    if (t > 0.45f) {
        float sa = (t - 0.45f) / 0.55f;
        unsigned char sv = (unsigned char)(220 * sa);
        glColor3ub(sv, sv, sv);
        for (int i = 0; i < 70; i++) {
            int sx = (i * 137) % 1000;
            int sy = 400 + (i * 53) % 200;
            drawCircle((float)sx, (float)sy, 1.4f);
        }
    }

    // Setting sun
    if (sunsetY > 385.0f) {
        unsigned char sr = lerpUB(255, 200, t);
        unsigned char sg = lerpUB(140, 70,  t);
        unsigned char sb = lerpUB(40,  30,  t);
        glColor3ub(sr, sg, sb); drawCircle(500, sunsetY, 55);
        glColor3ub(255, 180, 80); drawCircle(500, sunsetY, 40);
    }

    // Rising moon
    if (t > 0.35f) {
        float mt = (t - 0.35f) / 0.65f;
        float my = 430.0f + mt * 130.0f;
        glColor3ub((unsigned char)(70 * mt + 20),
                   (unsigned char)(65 * mt + 20),
                   (unsigned char)(45 * mt + 15));
        drawCircle(800, my, 50);
        glColor3ub((unsigned char)(120 * mt + 40),
                   (unsigned char)(115 * mt + 40),
                   (unsigned char)(95  * mt + 30));
        drawCircle(800, my, 42);
        glColor3ub(245, 240, 215); drawCircle(800, my, 32);
        glColor3ub(225, 220, 195); drawCircle(792, my + 5, 26);
    }

    // Clouds
    unsigned char cr = lerpUB(120, 35, t);
    unsigned char cg = lerpUB(60,  25, t);
    unsigned char cb = lerpUB(80,  55, t);
    glColor3ub(cr, cg, cb);
    drawCircle(sunsetCloud1,      555, 22);
    drawCircle(sunsetCloud1 + 30, 560, 28);
    drawCircle(sunsetCloud1 + 60, 555, 20);
    drawCircle(sunsetCloud2,      510, 18);
    drawCircle(sunsetCloud2 + 28, 515, 24);
    drawCircle(sunsetCloud2 + 55, 510, 17);

    // Birds
  // Birds
if (t < 0.7f) {
    glColor3ub(20, 20, 20);
    glLineWidth(2);
    for (int i = 0; i < 4; i++) {
        float bx = sunsetBirdX + i * 60;
        float f  = sinf(sunsetBirdX * 0.15f + i * 0.7f) * 6.0f; // wing flap
        glBegin(GL_LINE_STRIP);
        glVertex2f(bx, 540 + f); glVertex2f(bx + 8, 548 - f * 0.5f); glVertex2f(bx + 16, 540 + f);
        glEnd();
    }
    glLineWidth(1);
}
}

void drawScene3Water() {
    float t = sunsetProgress;
    unsigned char r1 = lerpUB(180, 30, t);
    unsigned char g1 = lerpUB(70,  35, t);
    unsigned char b1 = lerpUB(50,  80, t);
    unsigned char r2 = lerpUB(60,  10, t);
    unsigned char g2 = lerpUB(30,  10, t);
    unsigned char b2 = lerpUB(70,  35, t);

    glBegin(GL_QUADS);
        glColor3ub(r1, g1, b1); glVertex2d(0, 300);  glVertex2d(1000, 300);
        glColor3ub(r2, g2, b2); glVertex2d(1000, 0); glVertex2d(0, 0);
    glEnd();

    // Sun reflection
    if (sunsetY > 385.0f) {
        unsigned char rr = lerpUB(255, 200, t);
        unsigned char rg = lerpUB(180, 110, t);
        unsigned char rb = lerpUB(80,  50,  t);

        glColor3ub(rr, rg, rb);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(500, 295);
            for (int i = 0; i <= 24; i++) {
                float a = 3.1415926f + 3.1415926f * i / 24.0f;
                glVertex2f(500 + 110 * cosf(a), 295 + 70 * sinf(a));
            }
        glEnd();

        for (int i = 0; i < 9; i++) {
            float ph   = (float)i * 0.6f + sunsetCloud1 * 0.02f;
            float topW = 60.0f - i * 4.0f;
            float botW = 16.0f - i * 1.2f; if (botW < 2.0f) botW = 2.0f;
            float topY = 295.0f - i * 2.0f;
            float botY = topY - (28.0f + i * 5.0f);
            float wob  = sinf(ph) * 6.0f;
            unsigned char a1 = (unsigned char)(220 - i * 18);
            unsigned char a2 = (unsigned char)(110 - i * 10);
            glBegin(GL_QUADS);
                glColor3ub(rr, rg, a1);
                glVertex2f(500 - topW * 0.5f + wob, topY);
                glVertex2f(500 + topW * 0.5f + wob, topY);
                glColor3ub(rr, (unsigned char)(rg * 0.8f), a2);
                glVertex2f(500 + botW * 0.5f + wob * 0.5f, botY);
                glVertex2f(500 - botW * 0.5f + wob * 0.5f, botY);
            glEnd();
        }

        glColor3ub(255, 230, 160);
        for (int i = 0; i < 5; i++) {
            float y = 290.0f - i * 16.0f;
            float w = 70.0f  - i * 10.0f;
            glBegin(GL_QUADS);
                glVertex2f(500 - w, y);     glVertex2f(500 + w, y);
                glVertex2f(500 + w, y + 2); glVertex2f(500 - w, y + 2);
            glEnd();
        }
    }

    // Moon reflection
    if (t > 0.45f) {
        glColor3ub(230, 225, 205);
        for (int i = 0; i < 7; i++) {
            float y = 290.0f - i * 14.0f;
            float w = 36.0f  - i * 4.0f; if (w < 4.0f) w = 4.0f;
            glBegin(GL_QUADS);
                glVertex2f(800 - w, y);     glVertex2f(800 + w, y);
                glVertex2f(800 + w, y + 3); glVertex2f(800 - w, y + 3);
            glEnd();
        }
    }
}

void drawScene3City() {
    float t = sunsetProgress;
    unsigned char br = lerpUB(35, 18, t);
    unsigned char bg = lerpUB(25, 12, t);
    unsigned char bb = lerpUB(55, 35, t);

    const int heights[] = { 60, 90, 70, 110, 80, 130, 75, 95, 105, 70 };
    int x = 0;
    for (int i = 0; i < 10; i++) {
        glColor3ub(br, bg, bb);
        glBegin(GL_QUADS);
            glVertex2d(x, 380);       glVertex2d(x + 100, 380);
            glVertex2d(x + 100, 380 + heights[i]); glVertex2d(x, 380 + heights[i]);
        glEnd();

        int rows = heights[i] / 22;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < 4; col++) {
                int key = (i * 73 + row * 17 + col * 31) % 100;
                float onAt = 0.20f + (key % 60) / 100.0f;
                bool on = (t > onAt);
                if (on && (key % 7 == 0) && s3BlinkOn) on = false;
                if (!on) continue;

                if      (key % 3 == 0) glColor3ub(255, 220, 110);
                else if (key % 3 == 1) glColor3ub(255, 200, 90);
                else                   glColor3ub(180, 220, 255);

                int wx = x + 12 + col * 20;
                int wy = 380 + 8 + row * 22;
                glBegin(GL_QUADS);
                    glVertex2i(wx, wy);           glVertex2i(wx + 10, wy);
                    glVertex2i(wx + 10, wy + 12); glVertex2i(wx, wy + 12);
                glEnd();
            }
        }
        x += 100;
    }

    // Domed building
    glColor3ub(br, bg, bb);
    drawCircle(900, 410, 35);
    glBegin(GL_QUADS);
        glVertex2d(896, 380); glVertex2d(904, 380);
        glVertex2d(904, 410); glVertex2d(896, 410);
    glEnd();
    if (t > 0.5f) { glColor3ub(255, 220, 100); drawCircle(900, 440, 3); }
}

void drawScene3Road() {
    glColor3ub(40, 40, 50);
    glBegin(GL_QUADS);
        glVertex2d(0, 300);    glVertex2d(1000, 300);
        glVertex2d(1000, 380); glVertex2d(0, 380);
    glEnd();

    glColor3ub(80, 80, 90);
    glBegin(GL_QUADS);
        glVertex2d(0, 376);    glVertex2d(1000, 376);
        glVertex2d(1000, 380); glVertex2d(0, 380);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(0, 300);    glVertex2d(1000, 300);
        glVertex2d(1000, 304); glVertex2d(0, 304);
    glEnd();

    unsigned char ll = lerpUB(255, 220, sunsetProgress);
    glColor3ub(ll, ll, 200);
    for (int i = 0; i < 1000; i += 60) {
        glBegin(GL_QUADS);
            glVertex2d(i, 338);      glVertex2d(i + 35, 338);
            glVertex2d(i + 35, 342); glVertex2d(i, 342);
        glEnd();
    }
}

void drawCityCar(float x, unsigned char r, unsigned char g, unsigned char b, bool flipped) {
    glPushMatrix();
    glTranslatef(x, 0, 0);
    if (flipped) glScalef(-1.0f, 1.0f, 1.0f);

    glColor3ub(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(-50, 320); glVertex2f(50, 320);
        glVertex2f(50, 340);  glVertex2f(-50, 340);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(-30, 340); glVertex2f(25, 340);
        glVertex2f(15, 358);  glVertex2f(-22, 358);
    glEnd();

    glColor3ub(140, 200, 230);
    glBegin(GL_QUADS);
        glVertex2f(-26, 342); glVertex2f(20, 342);
        glVertex2f(12, 356);  glVertex2f(-19, 356);
    glEnd();

    if (sunsetProgress > 0.25f) {
        glColor3ub(255, 240, 140); drawCircle(46, 328, 4);
    }
    glColor3ub(220, 60, 50); drawCircle(-46, 328, 3);

    glColor3ub(20, 20, 20);
    drawCircle(-30, 318, 8);
    drawCircle(28,  318, 8);
    glColor3ub(120, 120, 120);
    drawCircle(-30, 318, 3);
    drawCircle(28,  318, 3);

    glPopMatrix();
}

void drawScene3SilhouetteBoat() {
    glColor3ub(15, 10, 25);
    glBegin(GL_QUADS);
        glVertex2d(sunsetBoat + 100, 230); glVertex2d(sunsetBoat + 240, 230);
        glVertex2d(sunsetBoat + 220, 215); glVertex2d(sunsetBoat + 120, 215);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2d(sunsetBoat + 168, 230); glVertex2d(sunsetBoat + 172, 230);
        glVertex2d(sunsetBoat + 172, 295); glVertex2d(sunsetBoat + 168, 295);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2d(sunsetBoat + 172, 295); glVertex2d(sunsetBoat + 172, 240); glVertex2d(sunsetBoat + 222, 240);
    glEnd();
}

void scene3() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawScene3Sky();
    drawScene3City();
    drawScene3Road();
 glPushMatrix(); glTranslatef(0, 34, 0); drawCityCar(sunsetCar2X, 60,  110, 200, true);  glPopMatrix();
glPushMatrix(); glTranslatef(0, 6,  0); drawCityCar(sunsetCar1X, 220, 60,  70,  false); glPopMatrix();
    drawScene3Water();
    drawScene3SilhouetteBoat();
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

// ====================== SCENE 1 / 2 ASSEMBLY ======================
void scene1()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawHouseSmall(190, 120, 146, 72,  46);
    drawHouseBig  (70,  155, 181, 201, 178);
    drawHouseBig  (280, 170, 45,  66,  23);
    drawHouseBig  (390, 150, 126, 106, 95);
    drawHouseSmall(-10, 110, 0,   94,  102);
    drawHouseSmall(525, 110, 117, 140, 165);

    drawTreeType1(-150, 155);
    drawTreeType1(-120, 160);
    drawTreeType1(-180, 170);
    drawTreeType1(-410, 160);

    drawGreenField();
    drawPond();
    drawMetroRoad();
    drawBoat();
    drawBoat2();
    drawMetro();

    drawTreeType1(150,  170);
    drawTreeType1(450,  170);
    drawTreeType1(-380, 170);

    drawKite();
    drawMan();
}

void scene2()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawSky2();
    drawPlane();
    drawPlayField();
    drawNagor();
    drawVarsity();
    drawRoadField();
    drawRoad();
    drawTrafficSignal();
    drawCar();
    drawAmbulance();
    drawTruck();

    drawTreeType1(-420, 180);
    drawTreeType1(-360, 180);
    drawTreeType1(-300, 180);
    drawTreeType1(140,  180);
    drawTreeType1(200,  180);
    drawTreeType1(500,  180);
}

// ====================== DISPLAY / UPDATE ======================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (currentScene) {
        case 1: scene1(); break;
        case 2: scene2(); break;
        case 3: scene3(); break;
        case 4: scene4(); break;
        case 5: scene5(); break;
    }
    glutSwapBuffers();
}

void update(int /*value*/)
{
    // ---- Auto scene change ----
    sceneTimer++;
    if (sceneTimer >= SCENE_DURATION) {
        sceneTimer = 0;
        currentScene++;
        if (currentScene > 5) currentScene = 1;
        if (currentScene == 3) {
            sunsetProgress = 0.0f;
            sunsetY        = 470.0f;
        }
    }

    // ---- Scene 1 ----
    boatSpeed = night ? 0 : 2;
    boatPosition += boatSpeed;
    if (boatPosition > 1000) boatPosition = -600;
    boatPosition2 -= boatSpeed;
    if (boatPosition2 < -800) boatPosition2 = 700;

    metroPosition += metroSpeed;
    if (metroPosition > 1000) metroPosition = -1400;

    angle += 0.1f;
    if (angle > sunMaxAngle) angle = sunMinAngle;

    // ---- Scene 2 ----
    carPosition += carSpeed;
    if (carPosition > 1000) carPosition = -500;
    AmbulancePosition += AmbulanceSpeed;
    if (AmbulancePosition > 1000) AmbulancePosition = -900;
    truckPosition -= truckSpeed;
    if (truckPosition < -1000) truckPosition = 600;
    planePosition -= planeSpeed;
    if (planePosition < -1000) planePosition = 800;

    ballPosition += ballSpeed;
    if (ballPosition > 440 || ballPosition < 0) ballSpeed = 0;

    cloudPosition1 += 1.4f;
    cloudPosition2 += 1.7f;
    cloudPosition3 += 1.0f;
    if (cloudPosition1 > 900) cloudPosition1 = -300;
    if (cloudPosition2 > 600) cloudPosition2 = -900;
    if (cloudPosition3 > 300) cloudPosition3 = -1000;

    if (++sirenTimer > 15) { sirenToggle = !sirenToggle; sirenTimer = 0; }

    // ---- Scene 3 ----
    sunsetY -= sunsetSpeed;
    if (sunsetY < 385.0f) sunsetY = 385.0f;
    sunsetProgress += 0.0015f;
    if (sunsetProgress > 1.15f) {
        sunsetProgress = 0.0f;
        sunsetY        = 470.0f;
    }
    sunsetBoat   += 0.6f;  if (sunsetBoat   >  900) sunsetBoat   = -250;
    sunsetBirdX  += 1.2f;  if (sunsetBirdX  > 1000) sunsetBirdX  = -300;
    sunsetCloud1 += 0.4f;  if (sunsetCloud1 > 1100) sunsetCloud1 = -100;
    sunsetCloud2 += 0.3f;  if (sunsetCloud2 > 1100) sunsetCloud2 = -100;
    sunsetCar1X  += 2.0f;  if (sunsetCar1X  > 1100) sunsetCar1X  = -150;
    sunsetCar2X  -= 1.6f;  if (sunsetCar2X  < -150) sunsetCar2X  = 1100;

    if (++s3BlinkTimer > 22) { s3BlinkOn = !s3BlinkOn; s3BlinkTimer = 0; }

    // ---- Scene 4 ----
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
        lightningOn   = !lightningOn;
        lightningTimer = lightningOn ? 175 : 0;
    }
    rainCar1X += 3.2f; if (rainCar1X > 1200) rainCar1X = -200;
    rainCar2X -= 2.6f; if (rainCar2X < -200) rainCar2X = 1200;
    rainCar3X += 2.0f; if (rainCar3X > 1200) rainCar3X = -200;

    // ---- Scene 5 ----
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

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void idle()
{
    if (carSpeed != 0 || truckSpeed != 0 || AmbulanceSpeed != 0) {
        angleCarw       += 0.5f;
        angleAmbulancew += 0.35f;
        angleTruckw     += 0.25f;
    }
    if (!nagorOff) nagorAngle += 0.01f;
    glutPostRedisplay();
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

// ====================== INPUT ======================
void handleKeypress(unsigned char key, int /*x*/, int /*y*/)
{
    switch (key) {
        case '1': currentScene = 1; sceneTimer = 0; break;
        case '2': currentScene = 2; sceneTimer = 0; break;
        case '3':
            currentScene   = 3;
            sunsetProgress = 0.0f;
            sunsetY        = 470.0f;
            sceneTimer     = 0;
            break;
        case '4': currentScene = 4; sceneTimer = 0; break;
        case '5': currentScene = 5; sceneTimer = 0; break;

        case 's':
            if (!metroStop || !redLight) {
                metroSpeed     = 0; metroStop = true;
                redLight       = true;
                carSpeed       = 0;
                AmbulanceSpeed = 0;
                truckSpeed     = 0;
            } else {
                metroSpeed     = 6; metroStop = false;
                carSpeed       = 6;
                AmbulanceSpeed = 6;
                truckSpeed     = 6;
                redLight       = false;
            }
            break;

        case 'n':
            if (!night) {
                night         = true;
                boatPosition  = 0;
                boatPosition2 = 0;
                nagorOff      = true;
            } else {
                night    = false;
                nagorOff = false;
            }
            break;

        case 'p': nagorOff = !nagorOff; break;

        case 'b':
            ballSpeed   = ballForward ? 5 : -5;
            ballForward = !ballForward;
            break;

        // ===== Scene 5 toggles =====
        case 'f': fountainOn = !fountainOn; break;  // fountain jets
        case 'l': lampsOn    = !lampsOn;    break;  // park lamps
    }
}

void handleSpecialKeypress(int key, int /*x*/, int /*y*/)
{
    switch (key) {
        case GLUT_KEY_LEFT:  kitePositionX -= kiteSpeed; break;
        case GLUT_KEY_RIGHT: kitePositionX += kiteSpeed; break;
        case GLUT_KEY_UP:    kitePositionY += kiteSpeed; break;
        case GLUT_KEY_DOWN:  kitePositionY -= kiteSpeed; break;
    }
    if (kitePositionX < -276) kitePositionX = -276;
    if (kitePositionX >  676) kitePositionX =  676;
    if (kitePositionY < -120) kitePositionY = -120;
    if (kitePositionY >   65) kitePositionY =   65;
    glutPostRedisplay();
}

// ====================== MAIN ======================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 700);
    glutCreateWindow("Urban Life (Uttara City life)");

    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress);
    glutReshapeFunc(resize);
    glutTimerFunc(0, update, 0);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    myInit();
    glutMainLoop();
    return 0;
}
