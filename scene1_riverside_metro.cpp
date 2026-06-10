#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <cstdlib>

// Standalone Scene 1: Riverside Metro and Pond
// Controls: S = stop/resume metro, N = day/night, arrow keys = move kite, Esc = exit

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



void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    scene1();
    glutSwapBuffers();
}

void update(int /*value*/)
{
    boatSpeed = night ? 0 : 2;
    boatPosition += boatSpeed;
    if (boatPosition > 1000) boatPosition = -600;
    boatPosition2 -= boatSpeed;
    if (boatPosition2 < -800) boatPosition2 = 700;

    metroPosition += metroSpeed;
    if (metroPosition > 1000) metroPosition = -1400;

    angle += 0.1f;
    if (angle > sunMaxAngle) angle = sunMinAngle;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void handleKeypress(unsigned char key, int /*x*/, int /*y*/)
{
    switch (key) {
        case 27: std::exit(0); break;
        case 's':
            metroStop = !metroStop;
            metroSpeed = metroStop ? 0 : 6;
            break;
        case 'n':
            night = !night;
            if (night) {
                boatPosition  = 0;
                boatPosition2 = 0;
            }
            break;
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
    glutCreateWindow("Urban City Life - Scene 1: Riverside Metro");

    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress);
    glutReshapeFunc(resize);
    glutTimerFunc(0, update, 0);
    glutDisplayFunc(display);

    myInit();
    glutMainLoop();
    return 0;
}
