#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <cstdlib>

// Standalone Scene 3: Sunset-to-Night City
// Controls: R = restart sunset animation, Esc = exit

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


void resetSunset()
{
    sunsetY        = 470.0f;
    sunsetProgress = 0.0f;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    scene3();
    glutSwapBuffers();
}

void update(int /*value*/)
{
    sunsetY -= sunsetSpeed;
    if (sunsetY < 385.0f) sunsetY = 385.0f;
    sunsetProgress += 0.0015f;
    if (sunsetProgress > 1.15f) resetSunset();

    sunsetBoat   += 0.6f;  if (sunsetBoat   >  900) sunsetBoat   = -250;
    sunsetBirdX  += 1.2f;  if (sunsetBirdX  > 1000) sunsetBirdX  = -300;
    sunsetCloud1 += 0.4f;  if (sunsetCloud1 > 1100) sunsetCloud1 = -100;
    sunsetCloud2 += 0.3f;  if (sunsetCloud2 > 1100) sunsetCloud2 = -100;
    sunsetCar1X  += 2.0f;  if (sunsetCar1X  > 1100) sunsetCar1X  = -150;
    sunsetCar2X  -= 1.6f;  if (sunsetCar2X  < -150) sunsetCar2X  = 1100;

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
        case 'r': resetSunset(); break;
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
    glutCreateWindow("Urban City Life - Scene 3: Sunset City");

    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(resize);
    glutTimerFunc(0, update, 0);
    glutDisplayFunc(display);

    myInit();
    glutMainLoop();
    return 0;
}
