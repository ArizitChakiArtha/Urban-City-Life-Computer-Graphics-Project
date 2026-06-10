#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <cstdlib>

// Standalone Scene 2: Urban Road, Playground and Ferris Wheel
// Controls: S = traffic stop/resume, N = day/night, P = pause Ferris wheel, B = kick ball, Esc = exit

// Shared day/night state used by this standalone scene
bool night = false;

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



void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    scene2();
    glutSwapBuffers();
}

void update(int /*value*/)
{
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

    if (++sirenTimer > 15) {
        sirenToggle = !sirenToggle;
        sirenTimer = 0;
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

void handleKeypress(unsigned char key, int /*x*/, int /*y*/)
{
    switch (key) {
        case 27: std::exit(0); break;
        case 's':
            if (!redLight) {
                redLight       = true;
                carSpeed       = 0;
                AmbulanceSpeed = 0;
                truckSpeed     = 0;
            } else {
                redLight       = false;
                carSpeed       = 6;
                AmbulanceSpeed = 6;
                truckSpeed     = 6;
            }
            break;
        case 'n':
            night = !night;
            nagorOff = night;
            break;
        case 'p': nagorOff = !nagorOff; break;
        case 'b':
            ballSpeed   = ballForward ? 5 : -5;
            ballForward = !ballForward;
            break;
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
    glutCreateWindow("Urban City Life - Scene 2: Urban Road");

    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(resize);
    glutTimerFunc(0, update, 0);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    myInit();
    glutMainLoop();
    return 0;
}
