#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <string>
#include <vector>

// -------------------------------------------------------------
// Window Dimensions & Constants
// -------------------------------------------------------------
const int WINDOW_WIDTH  = 1200;
const int WINDOW_HEIGHT = 600;
const float PI = 3.14159265358979323846f;

// -------------------------------------------------------------
// Day / Night Mode & State Variables
// -------------------------------------------------------------
float nightFactor     = 0.0f;     // 0.0f = Pure Day, 1.0f = Pure Night
float targetNight     = 0.0f;     // Target night factor for smooth lerping
bool  autoCycle       = false;    // Automatic Day -> Sunset -> Night -> Sunrise cycle
float cycleTimer      = 0.0f;     // Timer for continuous cycle
float beaconBlink     = 0.0f;     // Aviation beacon blink timer
float starTwinkle     = 0.0f;     // Stars twinkling phase

// Fixed Star Field Data
struct Star {
    float x, y, size, speed, phase;
};
const int NUM_STARS = 60;
Star stars[NUM_STARS];

// -------------------------------------------------------------
// Animation & Position Variables
// -------------------------------------------------------------
float busBaseX        = 100.0f;   // Lower-lane EDU Buses (Left -> Right)
float yellowBusX      = 1150.0f;  // Yellow School Bus (Right -> Left)
float planeX          = -160.0f;  // Aeroplane in Sky (Left -> Right)
float heliX           = 1250.0f;  // Helicopter in Sky (Right -> Left)
float heliRotor       = 0.0f;     // Rotor spin angle
float shipX           = 150.0f;   // Cruise Ship in River (Left -> Right)
float waterWave       = 0.0f;     // River ripples
float smokeAnim       = 0.0f;     // Ship funnel smoke
float kiteAnim        = 0.0f;     // Kite sway & tail flutter
float footballAnim    = 0.0f;     // Football passing animation between boys
float dogWalkerX      = -120.0f;  // Night-time Person Walking Dog (Left -> Right)
float walkAnim        = 0.0f;     // Walking leg swing cycle
float cloudOffset     = 0.0f;

// -------------------------------------------------------------
// Function Prototypes
// -------------------------------------------------------------
void drawRect(float x1, float y1, float x2, float y2, float r, float g, float b, float a = 1.0f);
void drawRectBorder(float x1, float y1, float x2, float y2, float r, float g, float b, float br, float bg, float bb, float lineWidth = 1.0f);
void drawCircle(float cx, float cy, float radius, float r, float g, float b, float a = 1.0f, int segments = 40);
void drawEllipse(float cx, float cy, float rx, float ry, float r, float g, float b, float a = 1.0f, int segments = 40);
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float a = 1.0f);
void drawText(float x, float y, const std::string& text, float r, float g, float b, void* font = GLUT_BITMAP_HELVETICA_12);

void initStars();
void drawSky();
void drawStars();
void drawSunAndMoon();
void drawRainbow();
void drawCloud(float cx, float cy, float scale = 1.0f);
void drawStreetLamps();

void drawExactPictureAeroplane(float x, float y);
void drawHelicopter(float x, float y);
void drawAntenna(float x, float y, float height);

void drawLeftCampusBuildings();
void drawMidApartmentBuilding();
void drawFoodCourtBuilding();
void drawAccurateEDUCampusBuilding();

void drawLargeTree(float cx, float cy);
void drawPineTree(float cx, float cy, float scale = 1.0f);
void drawGoldPineTree(float cx, float cy, float scale = 1.0f);
void drawFloweringBush(float cx, float cy, float scale = 1.0f);
void drawRoundBerryBush(float cx, float cy);

void drawBoyFlyingKite();
void drawBoysPlayingFootball(float baseX, float baseY);
void drawNightImage2DogWalker(float x, float y);
void drawBus(float bx, float by, float r, float g, float b);
void drawSchoolBus(float x, float y);
void drawRealisticShip(float x, float y);
void drawRoadAndRiver();

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// -------------------------------------------------------------
// Basic Drawing Helper Functions
// -------------------------------------------------------------
void drawRect(float x1, float y1, float x2, float y2, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void drawRectBorder(float x1, float y1, float x2, float y2, float r, float g, float b, float br, float bg, float bb, float lineWidth) {
    drawRect(x1, y1, x2, y2, r, g, b);
    glColor3f(br, bg, bb);
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void drawCircle(float cx, float cy, float radius, float r, float g, float b, float a, int segments) {
    glColor4f(r, g, b, a);
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * PI * float(i) / float(segments);
        glVertex2f(cx + radius * cosf(theta), cy + radius * sinf(theta));
    }
    glEnd();
}

void drawEllipse(float cx, float cy, float rx, float ry, float r, float g, float b, float a, int segments) {
    glColor4f(r, g, b, a);
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * PI * float(i) / float(segments);
        glVertex2f(cx + rx * cosf(theta), cy + ry * sinf(theta));
    }
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x3, y3);
    glEnd();
}

void drawText(float x, float y, const std::string& text, float r, float g, float b, void* font) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

// -------------------------------------------------------------
// Star Field Initialization
// -------------------------------------------------------------
void initStars() {
    for (int i = 0; i < NUM_STARS; i++) {
        stars[i].x = float((i * 73 + 19) % 1180 + 10);
        stars[i].y = float(360 + ((i * 47 + 31) % 225));
        stars[i].size = 1.0f + ((i % 3) * 0.7f);
        stars[i].speed = 1.5f + (i % 4) * 0.8f;
        stars[i].phase = float(i) * 0.45f;
    }
}

// -------------------------------------------------------------
// Sky, Celestial, Stars, Rainbow & Atmosphere
// -------------------------------------------------------------
void drawSky() {
    float topR = lerp(0.26f, 0.02f, nightFactor);
    float topG = lerp(0.70f, 0.04f, nightFactor);
    float topB = lerp(0.94f, 0.14f, nightFactor);

    float botR = lerp(0.86f, 0.08f, nightFactor);
    float botG = lerp(0.96f, 0.12f, nightFactor);
    float botB = lerp(0.82f, 0.28f, nightFactor);

    glBegin(GL_QUADS);
        glColor3f(topR, topG, topB);
        glVertex2f(0.0f, 600.0f);
        glVertex2f(1200.0f, 600.0f);
        glColor3f(botR, botG, botB);
        glVertex2f(1200.0f, 100.0f);
        glVertex2f(0.0f, 100.0f);
    glEnd();
}

void drawStars() {
    if (nightFactor < 0.05f) return;

    for (int i = 0; i < NUM_STARS; i++) {
        float alpha = (0.5f + 0.5f * sinf(starTwinkle * stars[i].speed + stars[i].phase)) * nightFactor;
        drawCircle(stars[i].x, stars[i].y, stars[i].size, 1.0f, 1.0f, 0.95f, alpha, 12);
        if (stars[i].size > 1.8f && alpha > 0.6f) {
            glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.6f);
            glLineWidth(1.0f);
            glBegin(GL_LINES);
                glVertex2f(stars[i].x - 3.5f, stars[i].y);
                glVertex2f(stars[i].x + 3.5f, stars[i].y);
                glVertex2f(stars[i].x, stars[i].y - 3.5f);
                glVertex2f(stars[i].x, stars[i].y + 3.5f);
            glEnd();
        }
    }
}

void drawSunAndMoon() {
    if (nightFactor < 0.95f) {
        float sunAlpha = 1.0f - nightFactor;
        float sunY = 520.0f - nightFactor * 80.0f;
        drawCircle(680.0f, sunY, 44.0f, 1.0f, 0.85f, 0.30f, 0.25f * sunAlpha);
        drawCircle(680.0f, sunY, 38.0f, 1.0f, 0.78f, 0.25f, 0.40f * sunAlpha);
        drawCircle(680.0f, sunY, 32.0f, 0.98f, 0.69f, 0.22f, sunAlpha);
    }

    if (nightFactor > 0.05f) {
        float moonX = 680.0f;
        float moonY = 440.0f + nightFactor * 80.0f;
        float moonAlpha = nightFactor;

        drawCircle(moonX, moonY, 65.0f, 0.75f, 0.85f, 1.0f, 0.08f * moonAlpha);
        drawCircle(moonX, moonY, 50.0f, 0.85f, 0.92f, 1.0f, 0.18f * moonAlpha);
        drawCircle(moonX, moonY, 38.0f, 0.95f, 0.97f, 1.0f, 0.30f * moonAlpha);
        drawCircle(moonX, moonY, 28.0f, 0.98f, 0.98f, 0.92f, moonAlpha);

        drawCircle(moonX - 7.0f, moonY + 6.0f, 5.0f, 0.86f, 0.88f, 0.84f, 0.55f * moonAlpha);
        drawCircle(moonX + 8.0f, moonY - 5.0f, 6.5f, 0.86f, 0.88f, 0.84f, 0.55f * moonAlpha);
        drawCircle(moonX - 4.0f, moonY - 10.0f, 4.0f, 0.86f, 0.88f, 0.84f, 0.45f * moonAlpha);
        drawCircle(moonX + 11.0f, moonY + 8.0f, 3.5f, 0.88f, 0.90f, 0.85f, 0.45f * moonAlpha);
    }
}

void drawRainbow() {
    if (nightFactor > 0.85f) return;

    float alphaMult = (1.0f - nightFactor);
    float cx = 420.0f;
    float cy = 160.0f;
    float startAngle = 18.0f * PI / 180.0f;
    float endAngle   = 162.0f * PI / 180.0f;
    int segments = 80;

    struct RainbowBand {
        float r, g, b, a;
    } bands[7] = {
        {0.98f, 0.20f, 0.20f, 0.38f},
        {1.00f, 0.55f, 0.15f, 0.38f},
        {1.00f, 0.92f, 0.20f, 0.38f},
        {0.25f, 0.85f, 0.35f, 0.38f},
        {0.20f, 0.75f, 0.95f, 0.38f},
        {0.20f, 0.35f, 0.90f, 0.38f},
        {0.65f, 0.25f, 0.85f, 0.38f}
    };

    float baseRadius = 380.0f;
    float bandWidth = 7.0f;

    for (int b = 0; b < 7; b++) {
        float innerR = baseRadius - (b * bandWidth);
        float outerR = innerR + bandWidth;

        glColor4f(bands[b].r, bands[b].g, bands[b].b, bands[b].a * alphaMult);
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= segments; i++) {
            float angle = startAngle + (endAngle - startAngle) * ((float)i / (float)segments);
            glVertex2f(cx + innerR * cosf(angle), cy + innerR * sinf(angle));
            glVertex2f(cx + outerR * cosf(angle), cy + outerR * sinf(angle));
        }
        glEnd();
    }
}

void drawCloud(float cx, float cy, float scale) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glScalef(scale, scale, 1.0f);

    float baseR = lerp(0.88f, 0.16f, nightFactor);
    float baseG = lerp(0.92f, 0.20f, nightFactor);
    float baseB = lerp(0.96f, 0.32f, nightFactor);

    float topR = lerp(1.0f, 0.26f, nightFactor);
    float topG = lerp(1.0f, 0.32f, nightFactor);
    float topB = lerp(1.0f, 0.48f, nightFactor);

    drawCircle(-35.0f, 0.0f, 18.0f, baseR, baseG, baseB);
    drawCircle(-15.0f, 16.0f, 22.0f, baseR, baseG, baseB);
    drawCircle(15.0f, 18.0f, 24.0f, baseR, baseG, baseB);
    drawCircle(35.0f, 2.0f, 18.0f, baseR, baseG, baseB);
    drawRect(-35.0f, -12.0f, 35.0f, 8.0f, baseR, baseG, baseB);

    drawCircle(-35.0f, 2.0f, 16.0f, topR, topG, topB);
    drawCircle(-15.0f, 18.0f, 20.0f, topR, topG, topB);
    drawCircle(15.0f, 20.0f, 22.0f, topR, topG, topB);
    drawCircle(35.0f, 4.0f, 16.0f, topR, topG, topB);
    drawRect(-35.0f, -8.0f, 35.0f, 8.0f, topR, topG, topB);

    glPopMatrix();
}

void drawAntenna(float x, float y, float height) {
    glColor3f(0.35f, 0.40f, 0.45f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x, y + height);
        glVertex2f(x - 5.0f, y + height * 0.7f);
        glVertex2f(x + 5.0f, y + height * 0.7f);
        glVertex2f(x - 3.0f, y + height * 0.85f);
        glVertex2f(x + 3.0f, y + height * 0.85f);
    glEnd();
    drawCircle(x, y + height, 2.5f, 0.35f, 0.40f, 0.45f);

    if (nightFactor > 0.2f) {
        float blink = (sinf(beaconBlink * 4.0f) > 0.0f) ? 1.0f : 0.15f;
        drawCircle(x, y + height + 2.0f, 7.0f, 1.0f, 0.1f, 0.1f, 0.35f * blink * nightFactor);
        drawCircle(x, y + height + 2.0f, 3.0f, 1.0f, 0.2f, 0.2f, blink * nightFactor);
    }
}

// -------------------------------------------------------------
// Campus Buildings
// -------------------------------------------------------------
void drawLeftCampusBuildings() {
    float baseY = 100.0f;

    float b1_mainR = lerp(1.000f, 0.450f, nightFactor);
    float b1_mainG = lerp(0.973f, 0.420f, nightFactor);
    float b1_mainB = lerp(0.906f, 0.410f, nightFactor);

    float b1_secR  = lerp(0.651f, 0.300f, nightFactor);
    float b1_secG  = lerp(0.482f, 0.210f, nightFactor);
    float b1_secB  = lerp(0.357f, 0.160f, nightFactor);

    float goldenWinR = lerp(0.980f, 1.000f, nightFactor);
    float goldenWinG = lerp(0.850f, 0.940f, nightFactor);
    float goldenWinB = lerp(0.350f, 0.450f, nightFactor);

    float copperAccR = lerp(0.722f, 0.380f, nightFactor);
    float copperAccG = lerp(0.451f, 0.220f, nightFactor);
    float copperAccB = lerp(0.200f, 0.100f, nightFactor);

    float x1 = 88.0f, w1 = 105.0f, h1 = 285.0f;
    drawRect(x1, baseY, x1 + w1, baseY + h1, b1_mainR, b1_mainG, b1_mainB);

    float floorH = 44.0f;
    for (int i = 0; i < 6; i++) {
        float fy = baseY + 10.0f + i * floorH;
        if (fy + 32.0f <= baseY + h1) {
            drawRect(x1 + 2.0f, fy, x1 + 27.0f, fy + 32.0f, copperAccR, copperAccG, copperAccB);
            if (nightFactor > 0.1f) {
                drawRect(x1 + 4.0f, fy + 4.0f, x1 + 24.0f, fy + 28.0f, 1.0f, 0.9f, 0.3f, 0.35f * nightFactor);
            }
            drawRectBorder(x1 + 6.0f, fy + 6.0f, x1 + 22.0f, fy + 26.0f, goldenWinR, goldenWinG, goldenWinB, 1.0f, 1.0f, 1.0f, 1.5f);
            drawRect(x1 + 45.0f, fy, x1 + 75.0f, fy + 32.0f, b1_secR, b1_secG, b1_secB);
            drawRect(x1 + 80.0f, fy, x1 + 98.0f, fy + 32.0f, b1_mainR * 0.94f, b1_mainG * 0.94f, b1_mainB * 0.94f);
        }
    }

    drawRect(x1 + 70.0f, baseY + h1, x1 + 115.0f, baseY + h1 + 28.0f, b1_secR, b1_secG, b1_secB);
    drawRect(x1 + 85.0f, baseY + h1 + 5.0f, x1 + 98.0f, baseY + h1 + 24.0f, copperAccR, copperAccG, copperAccB);
    drawAntenna(x1 + 25.0f, baseY + h1, 80.0f);

    float b2_mainR = lerp(0.835f, 0.380f, nightFactor);
    float b2_mainG = lerp(0.761f, 0.320f, nightFactor);
    float b2_mainB = lerp(0.682f, 0.280f, nightFactor);

    float b2_secR  = lerp(0.522f, 0.250f, nightFactor);
    float b2_secG  = lerp(0.310f, 0.140f, nightFactor);
    float b2_secB  = lerp(0.424f, 0.200f, nightFactor);

    float b2_winR  = lerp(0.990f, 1.000f, nightFactor);
    float b2_winG  = lerp(0.950f, 0.920f, nightFactor);
    float b2_winB  = lerp(0.550f, 0.300f, nightFactor);

    float x2 = 193.0f, w2 = 142.0f, h2 = 260.0f;
    drawRect(x2, baseY, x2 + w2, baseY + h2, b2_mainR, b2_mainG, b2_mainB);

    drawRect(x2, baseY + h2 - 24.0f, x2 + w2, baseY + h2, b2_secR, b2_secG, b2_secB);
    drawRect(x2, baseY + h2 - 24.0f, x2 + w2, baseY + h2 - 22.0f, copperAccR, copperAccG, copperAccB);

    if (nightFactor > 0.2f) {
        drawText(x2 + 20.0f, baseY + h2 - 16.0f, "EDU  NOVAS", 1.0f, 0.85f, 0.3f, GLUT_BITMAP_HELVETICA_12);
    }
    drawText(x2 + 20.0f, baseY + h2 - 16.0f, "EDU  NOVAS", 1.0f, 0.98f, 0.90f, GLUT_BITMAP_HELVETICA_12);

    drawRect(x2 + 7.0f, baseY, x2 + 20.0f, baseY + h2 - 24.0f, b2_secR, b2_secG, b2_secB);

    float flH2 = 36.0f;
    for (int i = 0; i < 6; i++) {
        float fy = baseY + 12.0f + i * flH2;
        if (fy + 24.0f <= baseY + h2 - 26.0f) {
            drawRect(x2 + 25.0f, fy, x2 + 135.0f, fy + 24.0f, b2_secR, b2_secG, b2_secB);
            drawRect(x2 + 28.0f, fy + 2.0f, x2 + 38.0f, fy + 22.0f, copperAccR, copperAccG, copperAccB);

            if (nightFactor > 0.1f) {
                drawRect(x2 + 46.0f, fy + 3.0f, x2 + 87.0f, fy + 21.0f, 1.0f, 0.95f, 0.4f, 0.30f * nightFactor);
                drawRect(x2 + 90.0f, fy + 3.0f, x2 + 132.0f, fy + 21.0f, 1.0f, 0.95f, 0.4f, 0.30f * nightFactor);
            }
            drawRectBorder(x2 + 48.0f, fy + 5.0f, x2 + 85.0f, fy + 19.0f, b2_winR, b2_winG, b2_winB, 1.0f, 1.0f, 1.0f, 1.5f);
            drawRectBorder(x2 + 92.0f, fy + 5.0f, x2 + 130.0f, fy + 19.0f, b2_winR, b2_winG, b2_winB, 1.0f, 1.0f, 1.0f, 1.5f);
        }
    }
}

void drawMidApartmentBuilding() {
    float baseY = 100.0f;
    float x = 345.0f, w = 130.0f, h = 205.0f;

    float pinkMainR = lerp(0.98f, 0.44f, nightFactor);
    float pinkMainG = lerp(0.86f, 0.35f, nightFactor);
    float pinkMainB = lerp(0.89f, 0.38f, nightFactor);

    float goldWinR  = lerp(0.98f, 1.00f, nightFactor);
    float goldWinG  = lerp(0.88f, 0.92f, nightFactor);
    float goldWinB  = lerp(0.52f, 0.35f, nightFactor);

    float terraAccR = lerp(0.82f, 0.36f, nightFactor);
    float terraAccG = lerp(0.48f, 0.20f, nightFactor);
    float terraAccB = lerp(0.38f, 0.16f, nightFactor);

    float trimR     = lerp(0.90f, 0.38f, nightFactor);
    float trimG     = lerp(0.75f, 0.28f, nightFactor);
    float trimB     = lerp(0.78f, 0.30f, nightFactor);

    drawRect(x, baseY, x + w, baseY + h, pinkMainR, pinkMainG, pinkMainB);
    drawRect(x, baseY + h - 6.0f, x + w, baseY + h, terraAccR, terraAccG, terraAccB);
    drawAntenna(x + 115.0f, baseY + h, 70.0f);

    float signW = 76.0f, signH = 18.0f;
    float signX = x + (w - signW) * 0.5f;
    float signY = baseY + h - 22.0f;

    drawRect(signX - 1.0f, signY - 1.0f, signX + signW + 1.0f, signY + signH + 1.0f, 0.98f, 0.88f, 0.52f);
    drawRect(signX, signY, signX + signW, signY + signH, terraAccR, terraAccG, terraAccB);

    if (nightFactor > 0.1f) {
        drawRect(signX - 4.0f, signY - 4.0f, signX + signW + 4.0f, signY + signH + 4.0f, 1.0f, 0.85f, 0.3f, 0.35f * nightFactor);
    }
    drawText(signX + 16.0f, signY + 4.5f, "E D I S", 0.35f, 0.15f, 0.10f, GLUT_BITMAP_HELVETICA_12);
    drawText(signX + 15.0f, signY + 5.5f, "E D I S", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_12);

    float fH = 35.0f;
    for (int i = 0; i < 5; i++) {
        float fy = baseY + 8.0f + i * fH;
        if (fy + 25.0f <= signY - 4.0f) {
            drawRect(x + 5.0f, fy, x + w - 5.0f, fy + 25.0f, trimR, trimG, trimB);
            drawRect(x + 8.0f, fy + 2.0f, x + 20.0f, fy + 23.0f, terraAccR, terraAccG, terraAccB);
            drawRect(x + 12.0f, fy + 7.0f, x + 16.0f, fy + 18.0f, 1.0f, 1.0f, 1.0f);

            if (nightFactor > 0.1f) {
                drawRect(x + 26.0f, fy + 3.0f, x + 70.0f, fy + 22.0f, 1.0f, 0.9f, 0.35f, 0.35f * nightFactor);
                drawRect(x + 74.0f, fy + 3.0f, x + 122.0f, fy + 22.0f, 1.0f, 0.9f, 0.35f, 0.35f * nightFactor);
            }
            drawRectBorder(x + 28.0f, fy + 5.0f, x + 68.0f, fy + 20.0f, goldWinR, goldWinG, goldWinB, 1.0f, 1.0f, 1.0f, 1.5f);
            drawRectBorder(x + 76.0f, fy + 5.0f, x + 120.0f, fy + 20.0f, goldWinR, goldWinG, goldWinB, 1.0f, 1.0f, 1.0f, 1.5f);

            glColor3f(1.0f, 1.0f, 1.0f);
            glLineWidth(1.0f);
            glBegin(GL_LINES);
                glVertex2f(x + 48.0f, fy + 5.0f);
                glVertex2f(x + 48.0f, fy + 20.0f);
                glVertex2f(x + 98.0f, fy + 5.0f);
                glVertex2f(x + 98.0f, fy + 20.0f);
            glEnd();
        }
    }
}

void drawFoodCourtBuilding() {
    float baseY = 100.0f;

    float wallR   = lerp(0.204f, 0.120f, nightFactor);
    float wallG   = lerp(0.251f, 0.140f, nightFactor);
    float wallB   = lerp(0.282f, 0.170f, nightFactor);

    float darkR   = lerp(0.125f, 0.070f, nightFactor);
    float darkG   = lerp(0.157f, 0.080f, nightFactor);
    float darkB   = lerp(0.180f, 0.090f, nightFactor);

    float glassR  = lerp(1.000f, 1.000f, nightFactor);
    float glassG  = lerp(0.965f, 0.920f, nightFactor);
    float glassB  = lerp(0.867f, 0.450f, nightFactor);

    float woodR   = lerp(0.812f, 0.550f, nightFactor);
    float woodG   = lerp(0.627f, 0.380f, nightFactor);
    float woodB   = lerp(0.439f, 0.220f, nightFactor);

    float tealR   = 0.360f, tealG = 0.620f, tealB = 0.610f;

    float lx = 485.0f, lw = 130.0f, lh = 105.0f;
    drawRect(lx, baseY, lx + lw, baseY + lh, wallR, wallG, wallB);
    drawRect(lx, baseY, lx + 12.0f, baseY + lh, darkR, darkG, darkB);
    drawRect(lx, baseY + lh - 8.0f, lx + lw, baseY + lh, darkR, darkG, darkB);

    float winLeftX = lx + 14.0f;
    float winLeftY = baseY + 4.0f;
    float winLeftW = lw - 18.0f;
    float winLeftH = lh - 16.0f;

    if (nightFactor > 0.1f) {
        drawRect(winLeftX - 6.0f, baseY - 12.0f, winLeftX + winLeftW + 6.0f, baseY + 4.0f, 1.0f, 0.92f, 0.5f, 0.30f * nightFactor);
    }

    drawRect(winLeftX, winLeftY, winLeftX + winLeftW, winLeftY + winLeftH, glassR, glassG, glassB);

    glColor3f(darkR, darkG, darkB);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(winLeftX, winLeftY);
        glVertex2f(winLeftX + winLeftW, winLeftY);
        glVertex2f(winLeftX + winLeftW, winLeftY + winLeftH);
        glVertex2f(winLeftX, winLeftY + winLeftH);
    glEnd();

    float colW = winLeftW / 4.0f;
    float transY = winLeftY + winLeftH * 0.28f;

    glLineWidth(2.2f);
    glBegin(GL_LINES);
        for (int i = 1; i <= 3; i++) {
            float vx = winLeftX + i * colW;
            glVertex2f(vx, winLeftY);
            glVertex2f(vx, winLeftY + winLeftH);
        }
        glVertex2f(winLeftX, transY);
        glVertex2f(winLeftX + winLeftW, transY);
    glEnd();

    float midMullionX = winLeftX + 2.0f * colW;
    glColor3f(tealR, tealG, tealB);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(midMullionX + 1.5f, transY - 14.0f);
        glVertex2f(midMullionX + 1.5f, transY + 22.0f);
    glEnd();

    float railBaseY = baseY + lh;
    float railH = 34.0f;
    drawRect(lx - 2.0f, railBaseY, lx + lw, railBaseY + 4.0f, darkR, darkG, darkB);

    glColor3f(darkR, darkG, darkB);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
        glVertex2f(lx, railBaseY + railH - 6.0f);
        glVertex2f(lx + lw, railBaseY + railH - 6.0f);
        glVertex2f(lx, railBaseY + railH * 0.50f);
        glVertex2f(lx + lw, railBaseY + railH * 0.50f);
        glVertex2f(lx, railBaseY + 8.0f);
        glVertex2f(lx + lw, railBaseY + 8.0f);
    glEnd();

    int numPosts = 6;
    for (int p = 0; p < numPosts; p++) {
        float px = lx + 4.0f + p * ((lw - 10.0f) / (numPosts - 1));
        drawRect(px - 2.0f, railBaseY + 4.0f, px + 2.0f, railBaseY + railH, darkR, darkG, darkB);
        drawRect(px - 3.0f, railBaseY + railH - 2.0f, px + 3.0f, railBaseY + railH + 2.0f, darkR, darkG, darkB);
    }

    float rx = lx + lw, rw = 150.0f, rh = 250.0f;
    drawRect(rx, baseY, rx + rw, baseY + rh, wallR, wallG, wallB);
    drawRect(rx, baseY, rx + 6.0f, baseY + rh, darkR, darkG, darkB);
    drawRect(rx + rw - 6.0f, baseY, rx + rw, baseY + rh, darkR, darkG, darkB);

    float roofY = baseY + rh;
    drawRect(rx - 4.0f, roofY - 10.0f, rx + rw + 4.0f, roofY - 4.0f, darkR * 1.15f, darkG * 1.15f, darkB * 1.15f);
    drawRect(rx - 7.0f, roofY - 4.0f, rx + rw + 7.0f, roofY, darkR, darkG, darkB);
    drawRect(rx - 10.0f, roofY, rx + rw + 10.0f, roofY + 7.0f, darkR * 0.85f, darkG * 0.85f, darkB * 0.85f);

    float louverY1 = roofY - 36.0f, louverY2 = roofY - 54.0f;
    drawRect(rx - 18.0f, louverY1, rx + 4.0f, louverY1 + 4.0f, darkR, darkG, darkB);
    drawRect(rx - 18.0f, louverY2, rx + 4.0f, louverY2 + 4.0f, darkR, darkG, darkB);

    float uWinX = rx + 14.0f;
    float uWinY = baseY + 138.0f;
    float uWinW = rw - 28.0f;
    float uWinH = rh - 152.0f;

    if (nightFactor > 0.1f) {
        drawRect(uWinX - 4.0f, uWinY - 4.0f, uWinX + uWinW + 4.0f, uWinY + uWinH + 4.0f, 1.0f, 0.92f, 0.45f, 0.25f * nightFactor);
    }
    drawRect(uWinX, uWinY, uWinX + uWinW, uWinY + uWinH, glassR, glassG, glassB);

    glColor3f(darkR, darkG, darkB);
    glLineWidth(3.5f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(uWinX, uWinY);
        glVertex2f(uWinX + uWinW, uWinY);
        glVertex2f(uWinX + uWinW, uWinY + uWinH);
        glVertex2f(uWinX, uWinY + uWinH);
    glEnd();

    float uColW = uWinW / 4.0f;
    glLineWidth(2.4f);
    glBegin(GL_LINES);
        for (int i = 1; i <= 3; i++) {
            float uvx = uWinX + i * uColW;
            glVertex2f(uvx, uWinY);
            glVertex2f(uvx, uWinY + uWinH);
        }
        float uTransY = uWinY + uWinH * 0.82f;
        glVertex2f(uWinX, uTransY);
        glVertex2f(uWinX + uWinW, uTransY);
    glEnd();
    drawRect(uWinX - 2.0f, uWinY - 4.0f, uWinX + uWinW + 2.0f, uWinY, darkR, darkG, darkB);

    float sbX = rx + 16.0f, sbY = baseY + 110.0f, sbW = rw - 32.0f, sbH = 22.0f;
    drawRect(sbX - 1.5f, sbY - 1.5f, sbX + sbW + 1.5f, sbY + sbH + 1.5f, 0.5f, 0.35f, 0.2f);
    drawRect(sbX, sbY, sbX + sbW, sbY + sbH, woodR, woodG, woodB);

    if (nightFactor > 0.1f) {
        drawRect(sbX - 2.0f, sbY - 2.0f, sbX + sbW + 2.0f, sbY + sbH + 2.0f, 1.0f, 0.85f, 0.35f, 0.35f * nightFactor);
    }
    drawText(sbX + 18.0f, sbY + 7.0f, "F O O D   C O U R T", darkR, darkG, darkB, GLUT_BITMAP_HELVETICA_10);

    float dDoorX = rx + 38.0f, dDoorW = rw - 76.0f, dDoorH = 88.0f, dDoorY = baseY;

    glColor3f(darkR * 1.3f, darkG * 1.3f, darkB * 1.3f);
    glLineWidth(1.4f);
    glBegin(GL_LINES);
        for (int g = 1; g <= 7; g++) {
            float gy = baseY + 12.0f + g * 11.0f;
            if (gy < sbY - 4.0f) {
                glVertex2f(rx + 6.0f, gy); glVertex2f(dDoorX - 4.0f, gy);
                glVertex2f(dDoorX + dDoorW + 4.0f, gy); glVertex2f(rx + rw - 6.0f, gy);
            }
        }
    glEnd();

    drawRect(dDoorX - 3.0f, dDoorY, dDoorX + dDoorW + 3.0f, dDoorY + dDoorH + 3.0f, darkR, darkG, darkB);
    float singleDoorW = (dDoorW - 4.0f) * 0.5f;
    float leftDoorX = dDoorX + 1.0f;
    float rightDoorX = leftDoorX + singleDoorW + 2.0f;

    drawRect(leftDoorX, dDoorY + 2.0f, leftDoorX + singleDoorW, dDoorY + dDoorH, glassR, glassG, glassB);
    drawRect(rightDoorX, dDoorY + 2.0f, rightDoorX + singleDoorW, dDoorY + dDoorH, glassR, glassG, glassB);

    glColor3f(darkR, darkG, darkB);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(leftDoorX, dDoorY + 2.0f); glVertex2f(leftDoorX + singleDoorW, dDoorY + 2.0f);
        glVertex2f(leftDoorX + singleDoorW, dDoorY + dDoorH); glVertex2f(leftDoorX, dDoorY + dDoorH);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2f(rightDoorX, dDoorY + 2.0f); glVertex2f(rightDoorX + singleDoorW, dDoorY + 2.0f);
        glVertex2f(rightDoorX + singleDoorW, dDoorY + dDoorH); glVertex2f(rightDoorX, dDoorY + dDoorH);
    glEnd();

    glColor3f(tealR, tealG, tealB);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(leftDoorX + singleDoorW - 5.0f, dDoorY + 24.0f);
        glVertex2f(leftDoorX + singleDoorW - 5.0f, dDoorY + 54.0f);
        glVertex2f(rightDoorX + 5.0f, dDoorY + 24.0f);
        glVertex2f(rightDoorX + 5.0f, dDoorY + 54.0f);
    glEnd();
    drawRect(dDoorX - 6.0f, baseY - 2.0f, dDoorX + dDoorW + 6.0f, baseY + 2.0f, darkR * 1.5f, darkG * 1.5f, darkB * 1.5f);
}

void drawAccurateEDUCampusBuilding() {
    float baseY = 100.0f;

    float brickR = lerp(0.65f, 0.32f, nightFactor);
    float brickG = lerp(0.28f, 0.12f, nightFactor);
    float brickB = lerp(0.20f, 0.08f, nightFactor);

    float lwx = 810.0f, lww = 100.0f, lwh = 145.0f;
    drawRect(lwx, baseY, lwx + lww, baseY + lwh, brickR, brickG, brickB);
    drawRect(lwx, baseY + lwh, lwx + lww, baseY + lwh + 6.0f, 0.88f, 0.88f, 0.90f);

    float balY = baseY + 90.0f;
    drawRect(lwx + 6.0f, balY, lwx + lww - 6.0f, balY + 4.0f, 0.90f, 0.90f, 0.92f);
    glColor3f(0.85f, 0.88f, 0.92f);
    glLineWidth(1.6f);
    glBegin(GL_LINES);
        glVertex2f(lwx + 6.0f, balY + 16.0f); glVertex2f(lwx + lww - 6.0f, balY + 16.0f);
        for (int p = 0; p <= 5; p++) {
            float px = lwx + 6.0f + p * 17.0f;
            glVertex2f(px, balY); glVertex2f(px, balY + 16.0f);
        }
    glEnd();
    drawRectBorder(lwx + 40.0f, balY + 4.0f, lwx + lww - 10.0f, baseY + lwh - 2.0f, 0.35f, 0.55f, 0.65f, 0.88f, 0.88f, 0.90f, 1.5f);

    float gwx = lwx + 8.0f, gwy = baseY, gww = lww - 16.0f, gwh = 75.0f;
    float glassCurtainR = lerp(0.22f, 0.90f, nightFactor);
    float glassCurtainG = lerp(0.52f, 0.85f, nightFactor);
    float glassCurtainB = lerp(0.58f, 0.40f, nightFactor);

    if (nightFactor > 0.1f) {
        drawRect(gwx - 3.0f, gwy - 6.0f, gwx + gww + 3.0f, gwy + gwh + 3.0f, 1.0f, 0.9f, 0.4f, 0.28f * nightFactor);
    }
    drawRect(gwx, gwy, gwx + gww, gwy + gwh, glassCurtainR, glassCurtainG, glassCurtainB);

    glColor3f(0.92f, 0.94f, 0.96f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        for (int i = 0; i <= 4; i++) {
            float gx = gwx + i * (gww / 4.0f);
            glVertex2f(gx, gwy); glVertex2f(gx, gwy + gwh);
        }
        glVertex2f(gwx, gwy + gwh * 0.5f);
        glVertex2f(gwx + gww, gwy + gwh * 0.5f);
    glEnd();

    float rwx = 980.0f, rww = 215.0f, rwh = 135.0f;
    drawRect(rwx, baseY, rwx + rww, baseY + rwh, brickR, brickG, brickB);

    float rgwX = rwx + 60.0f, rgwY = baseY, rgwW = rww - 70.0f, rgwH = 75.0f;
    float rGlassR = lerp(0.15f, 0.95f, nightFactor);
    float rGlassG = lerp(0.32f, 0.88f, nightFactor);
    float rGlassB = lerp(0.42f, 0.40f, nightFactor);

    if (nightFactor > 0.1f) {
        drawRect(rgwX - 4.0f, rgwY - 8.0f, rgwX + rgwW + 4.0f, rgwY + rgwH + 4.0f, 1.0f, 0.9f, 0.4f, 0.30f * nightFactor);
    }
    drawRect(rgwX, rgwY, rgwX + rgwW, rgwY + rgwH, rGlassR, rGlassG, rGlassB);

    glColor3f(0.12f, 0.14f, 0.16f);
    glLineWidth(2.2f);
    glBegin(GL_LINES);
        for (int c = 0; c <= 6; c++) {
            float cx = rgwX + c * (rgwW / 6.0f);
            glVertex2f(cx, rgwY); glVertex2f(cx, rgwY + rgwH);
        }
        for (int r = 1; r <= 2; r++) {
            float cy = rgwY + r * (rgwH / 3.0f);
            glVertex2f(rgwX, cy); glVertex2f(rgwX + rgwW, cy);
        }
    glEnd();
    drawRect(rgwX, rgwY + rgwH, rgwX + rgwW, rgwY + rgwH + 6.0f, 0.12f, 0.14f, 0.16f);

    float eduSignX = rgwX + 12.0f;
    float eduSignY = rgwY + rgwH + 12.0f;
    float eduSignW = 120.0f, eduSignH = 34.0f;

    if (nightFactor > 0.1f) {
        drawRect(eduSignX - 5.0f, eduSignY - 5.0f, eduSignX + eduSignW + 5.0f, eduSignY + eduSignH + 5.0f, 0.2f, 0.6f, 1.0f, 0.40f * nightFactor);
    }

    drawRect(eduSignX - 2.0f, eduSignY - 2.0f, eduSignX + eduSignW + 2.0f, eduSignY + eduSignH + 2.0f, 0.98f, 0.85f, 0.20f);
    drawRect(eduSignX, eduSignY, eduSignX + eduSignW, eduSignY + eduSignH, 0.08f, 0.32f, 0.65f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(eduSignX + 2.0f, eduSignY + 2.0f);
        glVertex2f(eduSignX + eduSignW - 2.0f, eduSignY + 2.0f);
        glVertex2f(eduSignX + eduSignW - 2.0f, eduSignY + eduSignH - 2.0f);
        glVertex2f(eduSignX + 2.0f, eduSignY + eduSignH - 2.0f);
    glEnd();

    drawText(eduSignX + 14.0f, eduSignY + 11.0f, "E D U", 0.02f, 0.10f, 0.22f, GLUT_BITMAP_TIMES_ROMAN_24);
    drawText(eduSignX + 13.0f, eduSignY + 12.0f, "E D U", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_24);

    float crestX = eduSignX + eduSignW - 22.0f;
    float crestY = eduSignY + eduSignH * 0.5f;
    drawCircle(crestX, crestY, 11.0f, 0.98f, 0.85f, 0.20f);
    drawCircle(crestX, crestY, 8.5f, 0.08f, 0.32f, 0.65f);
    drawText(crestX - 4.5f, crestY - 4.5f, "U", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_10);

    drawAntenna(rwx + 110.0f, baseY + rwh, 45.0f);

    float apexX = 1000.0f, apexY = baseY + 265.0f;
    float baseLeftX = 855.0f, baseLeftY = baseY + 105.0f;
    float baseRightX = 1145.0f, baseRightY = baseY + 105.0f;

    float canopyA = lerp(0.55f, 0.35f, nightFactor);
    glColor4f(0.82f, 0.88f, 0.92f, canopyA);
    glBegin(GL_TRIANGLES);
        glVertex2f(apexX, apexY);
        glVertex2f(baseLeftX, baseLeftY);
        glVertex2f(baseRightX, baseRightY);
    glEnd();

    glColor4f(0.92f, 0.94f, 0.96f, 0.85f);
    glLineWidth(1.8f);
    glBegin(GL_LINES);
        for (int i = 1; i <= 9; i++) {
            float t = (float)i / 10.0f;
            float x1 = baseLeftX + t * (apexX - baseLeftX);
            float y1 = baseLeftY + t * (apexY - baseLeftY);
            float x2 = apexX + t * (baseRightX - apexX);
            float y2 = apexY - t * (apexY - baseRightY);
            glVertex2f(x1, y1); glVertex2f(x2, y2);
        }
        for (int i = 1; i <= 9; i++) {
            float t = (float)i / 10.0f;
            float x1 = apexX - t * (apexX - baseLeftX);
            float y1 = apexY - t * (apexY - baseLeftY);
            float x2 = baseRightX - t * (baseRightX - apexX);
            float y2 = baseRightY + t * (apexY - baseRightY);
            glVertex2f(x1, y1); glVertex2f(x2, y2);
        }
    glEnd();

    float frameThick = 15.0f;
    glColor3f(lerp(0.88f, 0.50f, nightFactor), lerp(0.90f, 0.52f, nightFactor), lerp(0.94f, 0.58f, nightFactor));
    glBegin(GL_POLYGON);
        glVertex2f(apexX, apexY + frameThick);
        glVertex2f(baseRightX + frameThick * 0.8f, baseRightY);
        glVertex2f(baseRightX, baseRightY);
        glVertex2f(apexX, apexY);
    glEnd();

    glColor3f(lerp(0.80f, 0.45f, nightFactor), lerp(0.83f, 0.48f, nightFactor), lerp(0.87f, 0.52f, nightFactor));
    glBegin(GL_POLYGON);
        glVertex2f(apexX, apexY + frameThick);
        glVertex2f(baseLeftX - frameThick * 0.8f, baseLeftY);
        glVertex2f(baseLeftX, baseLeftY);
        glVertex2f(apexX, apexY);
    glEnd();

    float colX = apexX, colW = 18.0f, colTopY = apexY - 2.0f, colBotY = baseY - 4.0f;
    drawRect(colX - colW * 0.5f, colBotY, colX + colW * 0.5f, colTopY, lerp(0.94f, 0.55f, nightFactor), lerp(0.95f, 0.56f, nightFactor), lerp(0.96f, 0.58f, nightFactor));
    for (int s = 0; s < 4; s++) {
        drawRect(colX - 45.0f - s * 6.0f, baseY - 8.0f + s * 4.0f, colX + 45.0f + s * 6.0f, baseY - 4.0f + s * 4.0f, 0.58f, 0.60f, 0.64f);
    }
    drawRect(colX - 38.0f, baseY + 8.0f, colX + 38.0f, baseY + 60.0f, 0.12f, 0.15f, 0.18f);

    for (int b = 0; b < 18; b++) {
        float hx = 800.0f + b * 22.0f, hy = baseY - 2.0f;
        float hScale = (b % 2 == 0) ? 1.15f : 0.95f;
        glPushMatrix();
        glTranslatef(hx, hy, 0.0f);
        glScalef(hScale, hScale, 1.0f);

        float grnR = lerp(0.14f, 0.06f, nightFactor);
        float grnG = lerp(0.58f, 0.28f, nightFactor);
        float grnB = lerp(0.22f, 0.12f, nightFactor);

        drawCircle(0.0f, 12.0f, 16.0f, grnR, grnG, grnB);
        drawCircle(-8.0f, 16.0f, 14.0f, grnR * 1.2f, grnG * 1.1f, grnB * 1.2f);
        drawCircle(8.0f, 16.0f, 14.0f, grnR * 1.2f, grnG * 1.1f, grnB * 1.2f);
        glPopMatrix();
    }
}

// -------------------------------------------------------------
// Trees, Foliage & Streetlamps
// -------------------------------------------------------------
void drawLargeTree(float cx, float cy) {
    float trunkR = lerp(0.45f, 0.20f, nightFactor);
    float trunkG = lerp(0.28f, 0.12f, nightFactor);
    float trunkB = lerp(0.16f, 0.08f, nightFactor);
    drawRect(cx - 8.0f, cy, cx + 8.0f, cy + 65.0f, trunkR, trunkG, trunkB);

    float darkGreenR = lerp(0.12f, 0.04f, nightFactor);
    float darkGreenG = lerp(0.52f, 0.22f, nightFactor);
    float darkGreenB = lerp(0.20f, 0.10f, nightFactor);

    float midGreenR  = lerp(0.18f, 0.06f, nightFactor);
    float midGreenG  = lerp(0.65f, 0.28f, nightFactor);
    float midGreenB  = lerp(0.26f, 0.12f, nightFactor);

    float lightGreenR= lerp(0.24f, 0.08f, nightFactor);
    float lightGreenG= lerp(0.75f, 0.32f, nightFactor);
    float lightGreenB= lerp(0.32f, 0.15f, nightFactor);

    drawCircle(cx - 30.0f, cy + 85.0f, 32.0f, darkGreenR, darkGreenG, darkGreenB);
    drawCircle(cx + 28.0f, cy + 90.0f, 34.0f, darkGreenR, darkGreenG, darkGreenB);
    drawCircle(cx - 22.0f, cy + 120.0f, 35.0f, midGreenR, midGreenG, midGreenB);
    drawCircle(cx + 22.0f, cy + 122.0f, 36.0f, midGreenR, midGreenG, midGreenB);
    drawCircle(cx, cy + 95.0f, 38.0f, midGreenR, midGreenG, midGreenB);
    drawCircle(cx, cy + 130.0f, 42.0f, lightGreenR, lightGreenG, lightGreenB);
}

void drawGoldPineTree(float cx, float cy, float scale) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glScalef(scale, scale, 1.0f);
    drawRect(-3.0f, 0.0f, 3.0f, 15.0f, 0.35f, 0.25f, 0.15f);
    drawTriangle(-20.0f, 10.0f, 20.0f, 10.0f, 0.0f, 50.0f, lerp(0.72f, 0.32f, nightFactor), lerp(0.68f, 0.30f, nightFactor), lerp(0.18f, 0.08f, nightFactor));
    drawTriangle(-16.0f, 30.0f, 16.0f, 30.0f, 0.0f, 70.0f, lerp(0.82f, 0.38f, nightFactor), lerp(0.76f, 0.34f, nightFactor), lerp(0.22f, 0.10f, nightFactor));
    drawTriangle(-12.0f, 48.0f, 12.0f, 48.0f, 0.0f, 88.0f, lerp(0.90f, 0.42f, nightFactor), lerp(0.84f, 0.38f, nightFactor), lerp(0.25f, 0.12f, nightFactor));
    glPopMatrix();
}

void drawFloweringBush(float cx, float cy, float scale) {
    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glScalef(scale, scale, 1.0f);
    drawCircle(-15.0f, 12.0f, 16.0f, lerp(0.20f, 0.08f, nightFactor), lerp(0.65f, 0.28f, nightFactor), lerp(0.28f, 0.12f, nightFactor));
    drawCircle(15.0f, 12.0f, 16.0f, lerp(0.20f, 0.08f, nightFactor), lerp(0.65f, 0.28f, nightFactor), lerp(0.28f, 0.12f, nightFactor));
    drawCircle(0.0f, 20.0f, 20.0f, lerp(0.26f, 0.10f, nightFactor), lerp(0.74f, 0.32f, nightFactor), lerp(0.32f, 0.14f, nightFactor));
    glPopMatrix();
}

void drawRoundBerryBush(float cx, float cy) {
    drawCircle(cx - 8.0f, cy + 12.0f, 14.0f, lerp(0.12f, 0.05f, nightFactor), lerp(0.65f, 0.25f, nightFactor), lerp(0.35f, 0.15f, nightFactor));
    drawCircle(cx + 8.0f, cy + 12.0f, 14.0f, lerp(0.12f, 0.05f, nightFactor), lerp(0.65f, 0.25f, nightFactor), lerp(0.35f, 0.15f, nightFactor));
    drawCircle(cx, cy + 18.0f, 16.0f, lerp(0.18f, 0.07f, nightFactor), lerp(0.75f, 0.30f, nightFactor), lerp(0.38f, 0.16f, nightFactor));
}

void drawStreetLamps() {
    float lampPositions[4] = { 40.0f, 340.0f, 775.0f, 1180.0f };
    float poleBaseY = 98.0f;
    float poleH = 62.0f;

    for (int i = 0; i < 4; i++) {
        float lx = lampPositions[i];

        glColor3f(0.22f, 0.25f, 0.28f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
            glVertex2f(lx, poleBaseY);
            glVertex2f(lx, poleBaseY + poleH);
            glVertex2f(lx, poleBaseY + poleH);
            glVertex2f(lx + 14.0f, poleBaseY + poleH + 6.0f);
        glEnd();

        drawEllipse(lx + 14.0f, poleBaseY + poleH + 6.0f, 5.5f, 3.0f, 0.18f, 0.20f, 0.22f);

        if (nightFactor > 0.08f) {
            drawCircle(lx + 14.0f, poleBaseY + poleH + 4.5f, 3.5f, 1.0f, 0.95f, 0.7f, nightFactor);

            glBegin(GL_TRIANGLES);
                glColor4f(1.0f, 0.95f, 0.65f, 0.45f * nightFactor);
                glVertex2f(lx + 14.0f, poleBaseY + poleH + 4.5f);
                glColor4f(1.0f, 0.90f, 0.50f, 0.0f);
                glVertex2f(lx - 55.0f, 48.0f);
                glVertex2f(lx + 85.0f, 48.0f);
            glEnd();

            drawEllipse(lx + 15.0f, 50.0f, 60.0f, 12.0f, 1.0f, 0.92f, 0.60f, 0.25f * nightFactor);
        }
    }
}

// -------------------------------------------------------------
// 100% ACCURATE VECTOR AEROPLANE MATCHING USER REFERENCE IMAGE
// Compact, Balanced, Realistic Scale (~0.52x)
// -------------------------------------------------------------
void drawExactPictureAeroplane(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(0.52f, 0.52f, 1.0f); // Perfectly proportioned, not oversized

    float redR   = 0.92f, redG   = 0.14f, redB   = 0.14f; // Bright Cherry Red
    float darkRedR = 0.78f, darkRedG = 0.10f, darkRedB = 0.10f;
    float whiteR = 0.94f, whiteG = 0.96f, whiteB = 0.98f; // Fuselage Silver-White
    float lineR  = 0.12f, lineG  = 0.12f, lineB  = 0.14f; // Crisp Dark Outline

    // 1. REAR VERTICAL STABILIZER (Swept Red Tail Fin)
    glColor3f(redR, redG, redB);
    glBegin(GL_POLYGON);
        glVertex2f(-60.0f, 18.0f);
        glVertex2f(-78.0f, 18.0f);
        glVertex2f(-88.0f, 78.0f);
        glVertex2f(-72.0f, 78.0f);
    glEnd();
    // Tail Fin Dark Outline
    glColor3f(lineR, lineG, lineB);
    glLineWidth(2.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-60.0f, 18.0f);
        glVertex2f(-78.0f, 18.0f);
        glVertex2f(-88.0f, 78.0f);
        glVertex2f(-72.0f, 78.0f);
    glEnd();

    // 2. REAR TAIL CONE (Silver-Grey Aft Body)
    glColor3f(0.85f, 0.88f, 0.90f);
    glBegin(GL_POLYGON);
        glVertex2f(-60.0f, 4.0f);
        glVertex2f(-90.0f, 12.0f);
        glVertex2f(-88.0f, 20.0f);
        glVertex2f(-60.0f, 22.0f);
    glEnd();
    drawCircle(-88.0f, 16.0f, 4.0f, 0.85f, 0.88f, 0.90f);
    glColor3f(lineR, lineG, lineB);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(-60.0f, 4.0f);
        glVertex2f(-90.0f, 12.0f);
        glVertex2f(-88.0f, 20.0f);
        glVertex2f(-60.0f, 22.0f);
    glEnd();

    // 3. HORIZONTAL STABILIZER (Red Rear Left Tailplane)
    glColor3f(redR, redG, redB);
    glBegin(GL_POLYGON);
        glVertex2f(-68.0f, 10.0f);
        glVertex2f(-96.0f, -8.0f);
        glVertex2f(-88.0f, -14.0f);
        glVertex2f(-54.0f, 6.0f);
    glEnd();
    drawCircle(-92.0f, -11.0f, 3.5f, redR, redG, redB);
    glColor3f(lineR, lineG, lineB);
    glLineWidth(2.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-68.0f, 10.0f);
        glVertex2f(-96.0f, -8.0f);
        glVertex2f(-88.0f, -14.0f);
        glVertex2f(-54.0f, 6.0f);
    glEnd();

    // 4. TOP CABIN FAIRING / ANTENNA FIN (Red Hump above Cockpit)
    glColor3f(redR, redG, redB);
    glBegin(GL_POLYGON);
        glVertex2f(34.0f, 26.0f);
        glVertex2f(54.0f, 26.0f);
        glVertex2f(48.0f, 42.0f);
        glVertex2f(36.0f, 40.0f);
    glEnd();
    drawCircle(42.0f, 40.0f, 5.0f, redR, redG, redB);
    glColor3f(lineR, lineG, lineB);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(34.0f, 26.0f);
        glVertex2f(36.0f, 40.0f);
        glVertex2f(48.0f, 42.0f);
        glVertex2f(54.0f, 26.0f);
    glEnd();

    // 5. MAIN FUSELAGE
    // (a) Lower Bright Cherry-Red Underbelly
    glColor3f(redR, redG, redB);
    glBegin(GL_POLYGON);
        glVertex2f(-60.0f, 4.0f);
        glVertex2f(50.0f, 0.0f);
        glVertex2f(105.0f, 3.0f);
        glVertex2f(108.0f, -4.0f);
        glVertex2f(80.0f, -16.0f);
        glVertex2f(-20.0f, -15.0f);
        glVertex2f(-60.0f, -2.0f);
    glEnd();
    drawEllipse(35.0f, -6.0f, 72.0f, 10.5f, redR, redG, redB);

    // Dark Red Belly Shadow Inset
    drawEllipse(35.0f, -8.0f, 50.0f, 5.5f, darkRedR, darkRedG, darkRedB);

    // (b) Upper Silver-White Fuselage Body
    glColor3f(whiteR, whiteG, whiteB);
    glBegin(GL_POLYGON);
        glVertex2f(-60.0f, 2.0f);
        glVertex2f(50.0f, 0.0f);
        glVertex2f(104.0f, 2.0f);
        glVertex2f(78.0f, 24.0f);
        glVertex2f(40.0f, 26.0f);
        glVertex2f(-60.0f, 26.0f);
    glEnd();
    drawEllipse(15.0f, 14.0f, 75.0f, 12.0f, whiteR, whiteG, whiteB);

    // Fuselage Black Outlines
    glColor3f(lineR, lineG, lineB);
    glLineWidth(2.4f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(-60.0f, 26.0f);
        glVertex2f(40.0f, 26.0f);
        glVertex2f(78.0f, 24.0f);
        glVertex2f(105.0f, 3.0f);
        glVertex2f(108.0f, -4.0f);
        glVertex2f(80.0f, -16.0f);
        glVertex2f(-20.0f, -15.0f);
        glVertex2f(-60.0f, -2.0f);
        glVertex2f(-60.0f, 26.0f);
    glEnd();

    // Red/White Mid Separation Line
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(-60.0f, 2.0f);
        glVertex2f(105.0f, 3.0f);
    glEnd();

    // 6. COCKPIT WINDSHIELD (Angled Teal/Cyan Panes)
    float glassR = lerp(0.14f, 0.95f, nightFactor);
    float glassG = lerp(0.44f, 0.88f, nightFactor);
    float glassB = lerp(0.42f, 0.38f, nightFactor);

    // Rear Windshield Pane
    glColor3f(glassR, glassG, glassB);
    glBegin(GL_POLYGON);
        glVertex2f(56.0f, 9.0f);
        glVertex2f(70.0f, 9.0f);
        glVertex2f(74.0f, 20.0f);
        glVertex2f(56.0f, 20.0f);
    glEnd();
    glColor3f(lineR, lineG, lineB);
    glLineWidth(1.8f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(56.0f, 9.0f);
        glVertex2f(70.0f, 9.0f);
        glVertex2f(74.0f, 20.0f);
        glVertex2f(56.0f, 20.0f);
    glEnd();

    // Forward Windshield Pane
    glColor3f(glassR, glassG, glassB);
    glBegin(GL_POLYGON);
        glVertex2f(72.0f, 9.0f);
        glVertex2f(88.0f, 8.0f);
        glVertex2f(76.0f, 19.5f);
        glVertex2f(74.0f, 19.5f);
    glEnd();
    glColor3f(lineR, lineG, lineB);
    glLineWidth(1.8f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(72.0f, 9.0f);
        glVertex2f(88.0f, 8.0f);
        glVertex2f(76.0f, 19.5f);
        glVertex2f(74.0f, 19.5f);
    glEnd();

    // 7. BOARDING DOOR OUTLINE
    glColor3f(0.55f, 0.58f, 0.62f);
    glLineWidth(1.8f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(40.0f, 3.0f);
        glVertex2f(40.0f, 24.0f);
        glVertex2f(52.0f, 23.0f);
        glVertex2f(52.0f, 3.0f);
    glEnd();

    // 8. PASSENGER WINDOW RECESSED HOUSING & ROW OF ROUNDED WINDOWS
    drawRectBorder(-50.0f, 7.0f, 34.0f, 21.0f, 0.88f, 0.90f, 0.92f, 0.65f, 0.68f, 0.72f, 1.4f);

    float pWinR = lerp(0.32f, 1.00f, nightFactor);
    float pWinG = lerp(0.35f, 0.92f, nightFactor);
    float pWinB = lerp(0.40f, 0.40f, nightFactor);

    for (int w = 0; w < 8; w++) {
        float wx = -44.0f + w * 9.5f;
        drawEllipse(wx, 14.0f, 3.2f, 4.8f, pWinR, pWinG, pWinB);
        glColor3f(lineR, lineG, lineB);
        glLineWidth(1.2f);
        glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 16; i++) {
                float th = 2.0f * PI * float(i) / 16.0f;
                glVertex2f(wx + 3.2f * cosf(th), 14.0f + 4.8f * sinf(th));
            }
        glEnd();
    }

    // 9. JET ENGINE (Under-Wing Nacelle Pod with Dark Intake Ring)
    glColor3f(redR, redG, redB);
    glBegin(GL_POLYGON);
        glVertex2f(0.0f, -4.0f);
        glVertex2f(26.0f, -4.0f);
        glVertex2f(26.0f, -18.0f);
        glVertex2f(4.0f, -18.0f);
    glEnd();
    drawEllipse(15.0f, -18.0f, 11.0f, 3.0f, redR, redG, redB);

    // Intake Oval & Dark Turbine Lip
    drawEllipse(26.0f, -11.0f, 4.0f, 8.5f, 0.75f, 0.78f, 0.82f);
    drawEllipse(26.0f, -11.0f, 2.5f, 6.8f, 0.18f, 0.20f, 0.22f);
    drawCircle(26.0f, -11.0f, 1.4f, 0.85f, 0.88f, 0.92f);

    glColor3f(lineR, lineG, lineB);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 20; i++) {
            float th = 2.0f * PI * float(i) / 20.0f;
            glVertex2f(26.0f + 4.0f * cosf(th), -11.0f + 8.5f * sinf(th));
        }
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2f(0.0f, -4.0f);
        glVertex2f(4.0f, -18.0f);
        glVertex2f(26.0f, -18.0f);
    glEnd();

    // 10. MAIN FORWARD NEAR WING (Swept Red Trim with Silver Center Insert)
    // Red Base Wing
    glColor3f(redR, redG, redB);
    glBegin(GL_POLYGON);
        glVertex2f(26.0f, -2.0f);
        glVertex2f(-2.0f, -2.0f);
        glVertex2f(-64.0f, -38.0f);
        glVertex2f(-50.0f, -44.0f);
    glEnd();
    drawCircle(-57.0f, -41.0f, 6.0f, redR, redG, redB);

    // Silver-White Inset Panel
    glColor3f(0.88f, 0.90f, 0.94f);
    glBegin(GL_POLYGON);
        glVertex2f(20.0f, -4.0f);
        glVertex2f(2.0f, -4.0f);
        glVertex2f(-52.0f, -36.0f);
        glVertex2f(-44.0f, -39.0f);
    glEnd();

    // Wing Outer Black Outlines
    glColor3f(lineR, lineG, lineB);
    glLineWidth(2.4f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(26.0f, -2.0f);
        glVertex2f(-2.0f, -2.0f);
        glVertex2f(-64.0f, -38.0f);
        glVertex2f(-50.0f, -44.0f);
    glEnd();
    glLineWidth(1.6f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(20.0f, -4.0f);
        glVertex2f(2.0f, -4.0f);
        glVertex2f(-52.0f, -36.0f);
        glVertex2f(-44.0f, -39.0f);
    glEnd();

    // 11. NIGHT BEACON STROBES
    if (nightFactor > 0.15f) {
        float strobe = (sinf(beaconBlink * 6.0f) > 0.3f) ? 1.0f : 0.15f;
        drawCircle(-57.0f, -41.0f, 4.0f, 1.0f, 0.1f, 0.1f, strobe * nightFactor); // Red Wingtip
        drawCircle(-88.0f, 78.0f, 4.5f, 1.0f, 1.0f, 1.0f, strobe * nightFactor);  // White Tail Strobe
    }

    glPopMatrix();
}

// -------------------------------------------------------------
// Helicopter
// -------------------------------------------------------------
void drawHelicopter(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glColor3f(0.25f, 0.28f, 0.30f);
    glLineWidth(2.8f);
    glBegin(GL_LINES);
        glVertex2f(-32.0f, -22.0f); glVertex2f(20.0f, -22.0f);
        glVertex2f(-32.0f, -22.0f); glVertex2f(-36.0f, -18.0f);
        glVertex2f(-14.0f, -22.0f); glVertex2f(-10.0f, -11.0f);
        glVertex2f(8.0f, -22.0f);   glVertex2f(4.0f, -11.0f);
    glEnd();

    drawEllipse(0.0f, 0.0f, 26.0f, 14.5f, 0.88f, 0.18f, 0.18f);
    drawRect(-12.0f, -6.0f, 10.0f, 8.0f, 0.80f, 0.14f, 0.14f);
    drawRect(-6.0f, -2.0f, 16.0f, 4.0f, 0.95f, 0.95f, 0.95f);
    drawText(2.0f, -1.0f, "H", 0.88f, 0.18f, 0.18f, GLUT_BITMAP_HELVETICA_10);

    float hGlassR = lerp(0.15f, 0.85f, nightFactor);
    float hGlassG = lerp(0.25f, 0.95f, nightFactor);
    float hGlassB = lerp(0.35f, 0.70f, nightFactor);
    glColor3f(hGlassR, hGlassG, hGlassB);
    glBegin(GL_POLYGON);
        glVertex2f(-4.0f, -8.0f);
        glVertex2f(-22.0f, -6.0f);
        glVertex2f(-26.0f, 3.0f);
        glVertex2f(-12.0f, 11.0f);
        glVertex2f(-4.0f, 8.0f);
    glEnd();
    drawCircle(-10.0f, 1.0f, 3.0f, 0.12f, 0.14f, 0.16f);

    drawRect(-6.0f, 11.0f, 8.0f, 16.0f, 0.30f, 0.32f, 0.35f);
    drawRect(-1.5f, 16.0f, 1.5f, 22.0f, 0.20f, 0.22f, 0.24f);
    drawEllipse(0.0f, 22.0f, 4.0f, 2.0f, 0.12f, 0.12f, 0.15f);

    float bladeLen = 48.0f;
    float brCos = cosf(heliRotor);
    glColor3f(0.18f, 0.18f, 0.20f);
    glLineWidth(3.5f);
    glBegin(GL_LINES);
        glVertex2f(-bladeLen * brCos, 22.0f);
        glVertex2f(bladeLen * brCos, 22.0f);
    glEnd();
    drawEllipse(0.0f, 22.0f, bladeLen, 3.0f, 0.9f, 0.9f, 0.95f, 0.25f);

    glColor3f(0.88f, 0.18f, 0.18f);
    glBegin(GL_POLYGON);
        glVertex2f(16.0f, 5.0f);
        glVertex2f(48.0f, 4.0f);
        glVertex2f(48.0f, -1.0f);
        glVertex2f(16.0f, -5.0f);
    glEnd();

    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_POLYGON);
        glVertex2f(44.0f, -3.0f);
        glVertex2f(52.0f, -3.0f);
        glVertex2f(55.0f, 16.0f);
        glVertex2f(47.0f, 16.0f);
    glEnd();

    float trCos = cosf(heliRotor * 3.5f);
    float trSin = sinf(heliRotor * 3.5f);
    glColor3f(0.15f, 0.15f, 0.15f);
    glLineWidth(2.2f);
    glBegin(GL_LINES);
        glVertex2f(50.0f - trCos * 10.0f, 12.0f - trSin * 10.0f);
        glVertex2f(50.0f + trCos * 10.0f, 12.0f + trSin * 10.0f);
    glEnd();
    drawCircle(50.0f, 12.0f, 2.2f, 0.1f, 0.1f, 0.1f);

    if (nightFactor > 0.15f) {
        glBegin(GL_TRIANGLES);
            glColor4f(1.0f, 1.0f, 0.85f, 0.45f * nightFactor);
            glVertex2f(-18.0f, -8.0f);
            glColor4f(1.0f, 1.0f, 0.80f, 0.0f);
            glVertex2f(-110.0f, -180.0f);
            glVertex2f(20.0f, -180.0f);
        glEnd();
        float strobe = (sinf(beaconBlink * 5.0f) > 0.0f) ? 1.0f : 0.2f;
        drawCircle(55.0f, 16.0f, 3.5f, 1.0f, 0.15f, 0.15f, strobe * nightFactor);
    }

    glPopMatrix();
}

// -------------------------------------------------------------
// Day-Time Kite Flying Boy
// -------------------------------------------------------------
void drawBoyFlyingKite() {
    if (nightFactor > 0.85f) return;

    float boyAlpha = (1.0f - nightFactor);

    float boyX = 468.0f, boyY = 98.0f;
    float handX = boyX - 6.0f, handY = boyY + 28.0f;

    float kiteSwayX = sinf(kiteAnim * 1.2f) * 14.0f;
    float kiteSwayY = cosf(kiteAnim * 0.9f) * 10.0f;
    float tiltAngle = sinf(kiteAnim * 1.2f) * 12.0f;

    float kx = 435.0f + kiteSwayX;
    float ky = 445.0f + kiteSwayY;

    glColor4f(0.85f, 0.88f, 0.92f, 0.80f * boyAlpha);
    glLineWidth(1.6f);
    glBegin(GL_LINE_STRIP);
        glVertex2f(handX, handY);
        glVertex2f((handX + kx) * 0.5f - 8.0f, (handY + ky) * 0.5f - 15.0f);
        glVertex2f(kx, ky - 14.0f);
    glEnd();

    glPushMatrix();
    glTranslatef(kx, ky, 0.0f);
    glRotatef(tiltAngle, 0.0f, 0.0f, 1.0f);

    drawTriangle(0.0f, 0.0f, -16.0f, 0.0f, 0.0f, 22.0f, 0.98f, 0.45f, 0.08f, boyAlpha);
    drawTriangle(0.0f, 0.0f, 16.0f, 0.0f, 0.0f, 22.0f, 1.00f, 0.65f, 0.12f, boyAlpha);
    drawTriangle(0.0f, 0.0f, -16.0f, 0.0f, 0.0f, -24.0f, 0.92f, 0.35f, 0.05f, boyAlpha);
    drawTriangle(0.0f, 0.0f, 16.0f, 0.0f, 0.0f, -24.0f, 0.98f, 0.50f, 0.10f, boyAlpha);

    glColor4f(0.30f, 0.18f, 0.10f, boyAlpha);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 22.0f);  glVertex2f(0.0f, -24.0f);
        glVertex2f(-16.0f, 0.0f); glVertex2f(16.0f, 0.0f);
    glEnd();

    float tailTipY = -24.0f;
    float prevX = 0.0f, prevY = tailTipY;
    glColor4f(0.92f, 0.18f, 0.18f, boyAlpha);
    glLineWidth(2.2f);
    for (int t = 1; t <= 5; t++) {
        float nextX = sinf(kiteAnim * 2.0f + t * 0.8f) * (t * 6.5f) - (t * 4.0f);
        float nextY = tailTipY - (t * 14.0f);
        glBegin(GL_LINES);
            glVertex2f(prevX, prevY); glVertex2f(nextX, nextY);
        glEnd();

        float bowColR = (t % 2 == 0) ? 0.98f : 0.20f;
        float bowColG = (t % 2 == 0) ? 0.85f : 0.70f;
        float bowColB = (t % 2 == 0) ? 0.15f : 0.95f;
        drawTriangle(nextX - 6.0f, nextY + 3.0f, nextX, nextY, nextX - 6.0f, nextY - 3.0f, bowColR, bowColG, bowColB, boyAlpha);
        drawTriangle(nextX + 6.0f, nextY + 3.0f, nextX, nextY, nextX + 6.0f, nextY - 3.0f, bowColR, bowColG, bowColB, boyAlpha);
        drawCircle(nextX, nextY, 1.8f, 0.95f, 0.2f, 0.2f, boyAlpha);
        prevX = nextX; prevY = nextY;
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(boyX, boyY, 0.0f);

    drawEllipse(-5.0f, 2.0f, 4.5f, 2.2f, 0.15f, 0.15f, 0.18f, boyAlpha);
    drawRect(-8.0f, 0.0f, -1.0f, 1.8f, 0.95f, 0.95f, 0.95f, boyAlpha);
    drawEllipse(4.0f, 2.0f, 4.5f, 2.2f, 0.15f, 0.15f, 0.18f, boyAlpha);
    drawRect(1.0f, 0.0f, 8.0f, 1.8f, 0.95f, 0.95f, 0.95f, boyAlpha);

    drawRect(-6.0f, 2.5f, -1.5f, 14.0f, 0.18f, 0.32f, 0.60f, boyAlpha);
    drawRect(1.0f, 2.5f, 5.5f, 14.0f, 0.18f, 0.32f, 0.60f, boyAlpha);

    drawRect(-6.5f, 14.0f, 6.5f, 27.0f, 0.88f, 0.24f, 0.24f, boyAlpha);
    drawRect(-6.5f, 18.0f, 6.5f, 21.0f, 0.98f, 0.85f, 0.20f, boyAlpha);
    drawRect(-6.5f, 13.5f, 6.5f, 15.0f, 0.70f, 0.18f, 0.18f, boyAlpha);

    glColor4f(0.95f, 0.76f, 0.62f, boyAlpha);
    glLineWidth(3.5f);
    glBegin(GL_LINES);
        glVertex2f(-5.0f, 25.0f); glVertex2f(-9.0f, 29.0f);
        glVertex2f(4.0f, 24.0f);  glVertex2f(-4.0f, 28.0f);
    glEnd();

    drawRectBorder(-9.0f, 27.0f, -4.0f, 31.0f, 0.65f, 0.40f, 0.22f, 0.2f, 0.2f, 0.2f, 1.0f);
    drawCircle(-6.5f, 29.0f, 2.0f, 0.95f, 0.95f, 0.95f, boyAlpha);

    drawRect(-2.0f, 26.5f, 2.0f, 29.0f, 0.90f, 0.70f, 0.58f, boyAlpha);
    drawCircle(0.0f, 33.0f, 6.2f, 0.95f, 0.76f, 0.62f, boyAlpha);

    glColor4f(0.32f, 0.18f, 0.10f, boyAlpha);
    glBegin(GL_POLYGON);
        glVertex2f(-6.5f, 33.0f);
        glVertex2f(6.0f, 33.0f);
        glVertex2f(5.0f, 39.5f);
        glVertex2f(-3.0f, 40.5f);
        glVertex2f(-7.5f, 36.0f);
    glEnd();
    drawTriangle(-3.0f, 38.0f, -9.0f, 40.0f, -2.0f, 41.0f, 0.15f, 0.35f, 0.75f, boyAlpha);
    drawCircle(-2.0f, 34.0f, 1.0f, 0.15f, 0.15f, 0.15f, boyAlpha);

    glPopMatrix();
}

// -------------------------------------------------------------
// Night-Time Pet Dog Walker (Left to Right)
// -------------------------------------------------------------
void drawNightImage2DogWalker(float x, float y) {
    if (nightFactor < 0.15f) return;

    float alpha = nightFactor;
    float legSwing = sinf(walkAnim * 4.0f) * 7.0f;
    float dogLegSwing = sinf(walkAnim * 6.0f) * 6.0f;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    // 1. THE YOUNG MAN
    glPushMatrix();
    float lShoeX = -12.0f - legSwing;
    float rShoeX = 14.0f + legSwing;
    float shoeColR = 0.62f, shoeColG = 0.32f, shoeColB = 0.16f;

    drawEllipse(lShoeX, 3.0f, 6.5f, 3.2f, shoeColR, shoeColG, shoeColB, alpha);
    drawEllipse(rShoeX, 3.0f, 6.5f, 3.2f, shoeColR, shoeColG, shoeColB, alpha);

    float pantsColR = 0.46f, pantsColG = 0.32f, pantsColB = 0.22f;
    glColor4f(pantsColR, pantsColG, pantsColB, alpha);
    glLineWidth(6.5f);
    glBegin(GL_LINES);
        glVertex2f(-2.0f, 22.0f); glVertex2f(lShoeX, 5.0f);
        glVertex2f(3.0f, 22.0f);  glVertex2f(rShoeX, 5.0f);
    glEnd();

    float shirtR = 0.68f, shirtG = 0.82f, shirtB = 0.90f;
    drawRect(-9.0f, 22.0f, 9.0f, 42.0f, shirtR, shirtG, shirtB, alpha);

    glColor4f(0.96f, 0.76f, 0.62f, alpha);
    glLineWidth(4.2f);
    glBegin(GL_LINES);
        glVertex2f(-8.0f, 40.0f); glVertex2f(-18.0f, 26.0f);
    glEnd();
    drawCircle(-8.0f, 40.0f, 4.0f, shirtR, shirtG, shirtB, alpha);

    float handX = 18.0f, handY = 28.0f;
    glBegin(GL_LINES);
        glVertex2f(8.0f, 40.0f); glVertex2f(handX, handY);
    glEnd();
    drawCircle(8.0f, 40.0f, 4.0f, shirtR, shirtG, shirtB, alpha);
    drawCircle(handX, handY, 2.5f, 0.96f, 0.76f, 0.62f, alpha);

    drawRect(-2.5f, 41.0f, 2.5f, 46.0f, 0.94f, 0.72f, 0.58f, alpha);
    drawCircle(0.0f, 52.0f, 9.5f, 0.96f, 0.76f, 0.62f, alpha);

    drawCircle(3.5f, 53.0f, 1.2f, 0.15f, 0.15f, 0.15f, alpha);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBegin(GL_POLYGON);
        glVertex2f(1.0f, 48.5f);
        glVertex2f(6.5f, 48.5f);
        glVertex2f(5.5f, 46.5f);
        glVertex2f(2.0f, 46.5f);
    glEnd();

    glColor4f(0.25f, 0.14f, 0.08f, alpha);
    glBegin(GL_POLYGON);
        glVertex2f(-8.5f, 52.0f);
        glVertex2f(-10.0f, 61.0f);
        glVertex2f(-4.0f, 63.0f);
        glVertex2f(4.0f, 63.0f);
        glVertex2f(9.5f, 59.0f);
        glVertex2f(8.0f, 53.0f);
        glVertex2f(4.0f, 57.0f);
        glVertex2f(-4.0f, 56.0f);
    glEnd();

    glPopMatrix();

    // 2. THE PET DOG
    float dogX = 62.0f, dogY = 0.0f;
    float dogR = 0.92f, dogG = 0.66f, dogB = 0.40f;

    glPushMatrix();
    glTranslatef(dogX, dogY, 0.0f);

    float d1 = -10.0f - dogLegSwing;
    float d2 = -4.0f + dogLegSwing;
    float d3 = 8.0f - dogLegSwing;
    float d4 = 14.0f + dogLegSwing;

    drawEllipse(d1, 3.0f, 3.5f, 2.0f, dogR * 0.9f, dogG * 0.9f, dogB * 0.9f, alpha);
    drawEllipse(d2, 3.0f, 3.5f, 2.0f, dogR, dogG, dogB, alpha);
    drawEllipse(d3, 3.0f, 3.5f, 2.0f, dogR * 0.9f, dogG * 0.9f, dogB * 0.9f, alpha);
    drawEllipse(d4, 3.0f, 3.5f, 2.0f, dogR, dogG, dogB, alpha);

    drawEllipse(2.0f, 15.0f, 18.0f, 9.0f, dogR, dogG, dogB, alpha);

    glColor4f(dogR, dogG, dogB, alpha);
    glBegin(GL_POLYGON);
        glVertex2f(-12.0f, 18.0f);
        glVertex2f(-18.0f, 26.0f);
        glVertex2f(-14.0f, 28.0f);
        glVertex2f(-8.0f, 20.0f);
    glEnd();

    float collarX = 14.0f, collarY = 19.0f;
    drawRect(collarX - 2.0f, collarY - 3.0f, collarX + 3.0f, collarY + 6.0f, 0.12f, 0.14f, 0.16f, alpha);

    drawEllipse(20.0f, 22.0f, 9.0f, 7.0f, dogR, dogG, dogB, alpha);
    drawTriangle(16.0f, 26.0f, 14.0f, 34.0f, 21.0f, 28.0f, dogR * 0.95f, dogG * 0.95f, dogB * 0.95f, alpha);

    drawEllipse(27.0f, 20.0f, 6.5f, 4.0f, dogR, dogG, dogB, alpha);
    drawCircle(32.5f, 21.0f, 1.8f, 0.12f, 0.14f, 0.16f, alpha);
    drawCircle(22.0f, 24.0f, 1.2f, 0.15f, 0.15f, 0.15f, alpha);

    glPopMatrix();

    // 3. TAUT LEASH
    glColor4f(0.20f, 0.15f, 0.12f, alpha);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(handX, handY);
        glVertex2f(dogX + collarX, dogY + collarY + 2.0f);
    glEnd();

    glPopMatrix();
}

// -------------------------------------------------------------
// Boys Playing Football
// -------------------------------------------------------------
void drawBoysPlayingFootball(float baseX, float baseY) {
    glPushMatrix();
    glTranslatef(baseX, baseY, 0.0f);

    float passProgress = (sinf(footballAnim) + 1.0f) * 0.5f;
    float ballStartX = 18.0f, ballEndX = 68.0f;
    float ballCurrX  = ballStartX + passProgress * (ballEndX - ballStartX);
    float ballCurrY  = 4.0f + sinf(passProgress * PI) * 13.0f;

    // BOY 1 (Striker #10)
    glPushMatrix();
    drawEllipse(-5.0f, 2.0f, 4.5f, 2.2f, 0.12f, 0.12f, 0.14f);
    drawRect(-8.0f, 0.0f, -2.0f, 1.5f, 0.90f, 0.90f, 0.92f);
    drawRect(-6.0f, 2.0f, -2.0f, 8.0f, 0.12f, 0.38f, 0.82f);
    drawRect(-6.0f, 8.0f, -2.0f, 14.0f, 0.95f, 0.76f, 0.62f);

    glColor3f(0.95f, 0.76f, 0.62f);
    glLineWidth(4.2f);
    glBegin(GL_LINES);
        glVertex2f(2.0f, 14.0f); glVertex2f(10.0f, 6.0f);
    glEnd();
    drawEllipse(12.0f, 5.0f, 5.0f, 2.4f, 0.90f, 0.15f, 0.15f);

    drawRect(-7.0f, 12.0f, 4.0f, 19.5f, 0.96f, 0.96f, 0.98f);
    drawRect(-7.0f, 12.0f, -5.5f, 19.5f, 0.12f, 0.38f, 0.82f);

    drawRect(-7.5f, 19.5f, 5.5f, 32.5f, 0.12f, 0.38f, 0.82f);
    drawRect(-1.5f, 30.5f, 1.5f, 32.5f, 0.96f, 0.96f, 0.98f);
    drawText(-3.5f, 23.0f, "10", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_10);

    glColor3f(0.95f, 0.76f, 0.62f);
    glLineWidth(3.5f);
    glBegin(GL_LINES);
        glVertex2f(-6.5f, 31.0f); glVertex2f(-14.0f, 25.0f);
        glVertex2f(4.5f, 31.0f);  glVertex2f(13.0f, 27.0f);
    glEnd();

    drawRect(-1.5f, 32.5f, 1.5f, 34.5f, 0.90f, 0.70f, 0.58f);
    drawCircle(-1.0f, 38.5f, 6.2f, 0.95f, 0.76f, 0.62f);
    glColor3f(0.28f, 0.16f, 0.08f);
    glBegin(GL_POLYGON);
        glVertex2f(-7.0f, 38.5f);
        glVertex2f(5.0f, 38.5f);
        glVertex2f(4.0f, 45.5f);
        glVertex2f(-4.0f, 46.5f);
        glVertex2f(-8.0f, 42.5f);
    glEnd();
    drawCircle(2.0f, 39.5f, 1.0f, 0.15f, 0.15f, 0.15f);
    glPopMatrix();

    // TELSTAR FOOTBALL
    glPushMatrix();
    glTranslatef(ballCurrX, ballCurrY, 0.0f);
    glRotatef(footballAnim * 140.0f, 0.0f, 0.0f, 1.0f);
    drawCircle(0.0f, 0.0f, 5.8f, 0.98f, 0.98f, 0.98f);
    glColor3f(0.12f, 0.14f, 0.16f);
    glLineWidth(1.4f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 20; i++) {
            float theta = 2.0f * PI * float(i) / 20.0f;
            glVertex2f(5.8f * cosf(theta), 5.8f * sinf(theta));
        }
    glEnd();
    drawCircle(0.0f, 0.0f, 2.2f, 0.14f, 0.14f, 0.16f, 1.0f, 5);
    drawCircle(-3.2f, 2.6f, 1.5f, 0.14f, 0.14f, 0.16f, 1.0f, 5);
    drawCircle(3.2f, 2.6f, 1.5f, 0.14f, 0.14f, 0.16f, 1.0f, 5);
    drawCircle(0.0f, -3.8f, 1.5f, 0.14f, 0.14f, 0.16f, 1.0f, 5);
    glPopMatrix();

    // BOY 2 (Defender #7)
    glPushMatrix();
    glTranslatef(82.0f, 0.0f, 0.0f);
    drawEllipse(-6.0f, 2.0f, 4.5f, 2.2f, 0.15f, 0.65f, 0.25f);
    drawEllipse(6.0f, 2.0f, 4.5f, 2.2f, 0.15f, 0.65f, 0.25f);
    drawRect(-8.0f, 2.0f, -4.0f, 8.0f, 0.88f, 0.18f, 0.18f);
    drawRect(4.0f, 2.0f, 8.0f, 8.0f, 0.88f, 0.18f, 0.18f);

    drawRect(-8.0f, 8.0f, -4.0f, 14.0f, 0.95f, 0.76f, 0.62f);
    drawRect(4.0f, 8.0f, 8.0f, 14.0f, 0.95f, 0.76f, 0.62f);

    drawRect(-9.5f, 13.0f, 9.5f, 19.0f, 0.10f, 0.16f, 0.28f);

    drawRect(-8.5f, 19.0f, 8.5f, 32.0f, 0.88f, 0.18f, 0.18f);
    drawRect(-2.0f, 30.0f, 2.0f, 32.0f, 0.96f, 0.96f, 0.98f);
    drawText(-3.5f, 22.5f, "7", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_10);

    glColor3f(0.95f, 0.76f, 0.62f);
    glLineWidth(3.5f);
    glBegin(GL_LINES);
        glVertex2f(-7.5f, 30.0f); glVertex2f(-16.0f, 24.5f);
        glVertex2f(7.5f, 30.0f);  glVertex2f(15.0f, 25.5f);
    glEnd();

    drawRect(-1.5f, 32.0f, 1.5f, 34.0f, 0.90f, 0.70f, 0.58f);
    drawCircle(0.0f, 37.5f, 6.2f, 0.95f, 0.76f, 0.62f);
    glColor3f(0.12f, 0.12f, 0.12f);
    glBegin(GL_POLYGON);
        glVertex2f(-6.5f, 37.5f);
        glVertex2f(6.5f, 37.5f);
        glVertex2f(5.5f, 44.5f);
        glVertex2f(-3.0f, 45.5f);
        glVertex2f(-7.5f, 41.5f);
    glEnd();
    drawCircle(-2.5f, 38.5f, 1.0f, 0.15f, 0.15f, 0.15f);
    glPopMatrix();

    glPopMatrix();
}

// -------------------------------------------------------------
// Lower Lane: Synchronized EDU Buses
// -------------------------------------------------------------
void drawBus(float bx, float by, float r, float g, float b) {
    float bw = 135.0f, bh = 38.0f;

    drawRect(bx, by + 5.0f, bx + bw, by + bh, r, g, b);
    drawRect(bx + 4.0f, by + bh - 12.0f, bx + bw - 4.0f, by + bh, 0.95f, 0.95f, 0.95f);

    float winR = lerp(0.85f, 1.00f, nightFactor);
    float winG = lerp(0.94f, 0.92f, nightFactor);
    float winB = lerp(0.98f, 0.45f, nightFactor);

    drawRect(bx + bw - 16.0f, by + 10.0f, bx + bw - 4.0f, by + bh - 4.0f, winR, winG, winB);
    for (int w = 0; w < 4; w++) {
        float wx = bx + 8.0f + w * 25.0f;
        drawRectBorder(wx, by + 14.0f, wx + 19.0f, by + bh - 4.0f, winR, winG, winB, 0.2f, 0.2f, 0.2f, 1.2f);
    }

    drawCircle(bx + 55.0f, by + 13.0f, 9.0f, 0.95f, 0.85f, 0.40f);
    drawText(bx + 47.0f, by + 10.0f, "EDU", 0.85f, 0.15f, 0.15f, GLUT_BITMAP_HELVETICA_10);

    drawCircle(bx + 26.0f, by + 5.0f, 9.5f, 0.15f, 0.15f, 0.15f);
    drawCircle(bx + 26.0f, by + 5.0f, 5.0f, 0.88f, 0.88f, 0.88f);
    drawCircle(bx + bw - 28.0f, by + 5.0f, 9.5f, 0.15f, 0.15f, 0.15f);
    drawCircle(bx + bw - 28.0f, by + 5.0f, 5.0f, 0.88f, 0.88f, 0.88f);

    drawCircle(bx + bw - 2.0f, by + 10.0f, 3.5f, 1.0f, 0.95f, 0.4f);
    drawCircle(bx + 2.0f, by + 10.0f, 3.0f, 0.9f, 0.2f, 0.2f);

    if (nightFactor > 0.1f) {
        drawCircle(bx - 2.0f, by + 10.0f, 6.0f, 1.0f, 0.1f, 0.1f, 0.4f * nightFactor);
        glBegin(GL_TRIANGLES);
            glColor4f(1.0f, 0.98f, 0.70f, 0.55f * nightFactor);
            glVertex2f(bx + bw, by + 10.0f);
            glColor4f(1.0f, 0.92f, 0.40f, 0.0f);
            glVertex2f(bx + bw + 110.0f, by + 28.0f);
            glVertex2f(bx + bw + 110.0f, by - 6.0f);
        glEnd();
    }
}

// -------------------------------------------------------------
// School Bus
// -------------------------------------------------------------
void drawSchoolBus(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    float busLen = 138.0f;
    float busH   = 44.0f;
    float busYColR = 0.98f, busYColG = 0.68f, busYColB = 0.10f;

    drawRect(36.0f, busH, 48.0f, busH + 2.5f, 0.88f, 0.55f, 0.08f);
    drawRect(95.0f, busH, 108.0f, busH + 2.5f, 0.88f, 0.55f, 0.08f);

    glColor3f(busYColR, busYColG, busYColB);
    glBegin(GL_POLYGON);
        glVertex2f(12.0f, 4.0f);
        glVertex2f(0.0f, 4.0f);
        glVertex2f(0.0f, 15.0f);
        glVertex2f(10.0f, 21.0f);
        glVertex2f(22.0f, 24.0f);
        glVertex2f(25.0f, busH - 4.0f);
        glVertex2f(32.0f, busH);
        glVertex2f(busLen - 6.0f, busH);
        glVertex2f(busLen, busH - 4.0f);
        glVertex2f(busLen, 4.0f);
    glEnd();

    drawRect(-2.0f, 3.0f, 12.0f, 8.5f, 0.12f, 0.14f, 0.16f);
    drawRect(busLen - 4.0f, 3.0f, busLen + 4.0f, 9.0f, 0.12f, 0.14f, 0.16f);

    float doorX = 26.0f, doorW = 14.0f, doorH = 34.0f;
    drawRect(doorX, 4.0f, doorX + doorW, 4.0f + doorH, 0.12f, 0.14f, 0.16f);

    float winColR = lerp(0.82f, 1.00f, nightFactor);
    float winColG = lerp(0.92f, 0.90f, nightFactor);
    float winColB = lerp(0.96f, 0.35f, nightFactor);

    drawRect(doorX + 1.5f, 14.0f, doorX + 5.5f, 32.0f, winColR, winColG, winColB);
    drawRect(doorX + 7.5f, 14.0f, doorX + 11.5f, 32.0f, winColR, winColG, winColB);
    drawRect(doorX + 1.5f, 6.0f, doorX + 5.5f, 12.0f, winColR, winColG, winColB);
    drawRect(doorX + 7.5f, 6.0f, doorX + 11.5f, 32.0f, winColR, winColG, winColB);
    drawRect(doorX + doorW * 0.5f - 0.5f, 4.0f, doorX + doorW * 0.5f + 0.5f, 4.0f + doorH, busYColR, busYColG, busYColB);

    drawRectBorder(doorX - 2.0f, 4.0f + doorH + 1.0f, doorX + doorW + 2.0f, busH - 2.0f, busYColR * 0.85f, busYColG * 0.85f, busYColB * 0.85f, 0.12f, 0.14f, 0.16f, 1.0f);

    drawRect(18.0f, 24.0f, 25.0f, 36.0f, winColR, winColG, winColB);
    glColor3f(0.12f, 0.14f, 0.16f);
    glLineWidth(1.8f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(18.0f, 24.0f); glVertex2f(25.0f, 24.0f);
        glVertex2f(25.0f, 36.0f); glVertex2f(18.0f, 36.0f);
    glEnd();

    glColor3f(0.15f, 0.15f, 0.15f);
    glLineWidth(1.8f);
    glBegin(GL_LINES);
        glVertex2f(14.0f, 20.0f); glVertex2f(10.0f, 30.0f);
        glVertex2f(10.0f, 30.0f); glVertex2f(14.0f, 30.0f);
    glEnd();
    drawRectBorder(9.0f, 24.0f, 12.0f, 32.0f, busYColR, busYColG, busYColB, 0.12f, 0.14f, 0.16f, 1.2f);

    float winStartX = doorX + doorW + 4.0f;
    float winW = 10.0f;
    float winGap = 2.5f;

    for (int w = 0; w < 7; w++) {
        float wx = winStartX + w * (winW + winGap);
        drawRect(wx - 0.8f, 22.0f, wx + winW + 0.8f, 36.0f, 0.12f, 0.14f, 0.16f);
        drawRect(wx, 23.0f, wx + winW, 35.0f, winColR, winColG, winColB);
        glColor3f(0.12f, 0.14f, 0.16f);
        glLineWidth(1.2f);
        glBegin(GL_LINES);
            glVertex2f(wx, 29.0f); glVertex2f(wx + winW, 29.0f);
        glEnd();
    }

    drawRect(winStartX - 6.0f, 18.5f, busLen - 2.0f, 20.5f, 0.12f, 0.14f, 0.16f);
    drawRect(winStartX - 6.0f, 15.0f, busLen - 2.0f, 16.5f, 0.12f, 0.14f, 0.16f);
    drawRect(winStartX - 6.0f, 12.5f, busLen - 2.0f, 13.8f, 0.12f, 0.14f, 0.16f);
    drawRect(winStartX - 6.0f, 10.0f, busLen - 2.0f, 11.2f, 0.12f, 0.14f, 0.16f);

    drawRectBorder(54.0f, 4.5f, 66.0f, 9.0f, busYColR, busYColG, busYColB, 0.12f, 0.14f, 0.16f, 1.0f);
    drawRectBorder(72.0f, 4.5f, 84.0f, 9.0f, busYColR, busYColG, busYColB, 0.12f, 0.14f, 0.16f, 1.0f);

    float wheelRadius = 11.0f;
    float wheelY = 4.0f;
    float frontWheelX = 16.0f;
    float rearWheelX  = 108.0f;

    drawCircle(frontWheelX, wheelY, wheelRadius + 1.5f, 0.12f, 0.14f, 0.16f);
    drawCircle(rearWheelX, wheelY, wheelRadius + 1.5f, 0.12f, 0.14f, 0.16f);

    drawCircle(frontWheelX, wheelY, wheelRadius, 0.15f, 0.16f, 0.18f);
    drawCircle(frontWheelX, wheelY, 6.5f, 0.78f, 0.82f, 0.86f);
    drawCircle(frontWheelX, wheelY, 2.5f, 0.15f, 0.16f, 0.18f);
    for (int l = 0; l < 8; l++) {
        float la = float(l) * PI / 4.0f;
        drawCircle(frontWheelX + 4.5f * cosf(la), wheelY + 4.5f * sinf(la), 0.8f, 0.15f, 0.16f, 0.18f);
    }

    drawCircle(rearWheelX, wheelY, wheelRadius, 0.15f, 0.16f, 0.18f);
    drawCircle(rearWheelX, wheelY, 6.5f, 0.78f, 0.82f, 0.86f);
    drawCircle(rearWheelX, wheelY, 2.5f, 0.15f, 0.16f, 0.18f);
    for (int l = 0; l < 8; l++) {
        float la = float(l) * PI / 4.0f;
        drawCircle(rearWheelX + 4.5f * cosf(la), wheelY + 4.5f * sinf(la), 0.8f, 0.15f, 0.16f, 0.18f);
    }

    drawCircle(2.0f, 10.0f, 2.8f, 1.0f, 0.95f, 0.65f);
    drawCircle(busLen - 2.0f, 12.0f, 2.8f, 0.95f, 0.20f, 0.20f);

    if (nightFactor > 0.1f) {
        drawCircle(busLen + 4.0f, 12.0f, 7.0f, 1.0f, 0.1f, 0.1f, 0.45f * nightFactor);
        glBegin(GL_TRIANGLES);
            glColor4f(1.0f, 0.98f, 0.70f, 0.60f * nightFactor);
            glVertex2f(0.0f, 10.0f);
            glColor4f(1.0f, 0.92f, 0.40f, 0.0f);
            glVertex2f(-130.0f, 28.0f);
            glVertex2f(-130.0f, -8.0f);
        glEnd();
    }

    glPopMatrix();
}

// -------------------------------------------------------------
// Ultra-Realistic Cruise Ship in River
// -------------------------------------------------------------
void drawRealisticShip(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glColor4f(1.0f, 1.0f, 1.0f, 0.45f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(-55.0f, 2.0f); glVertex2f(-100.0f, 2.0f);
        glVertex2f(75.0f, 2.0f);  glVertex2f(90.0f, 0.0f);
    glEnd();

    glColor3f(0.75f, 0.15f, 0.15f);
    glBegin(GL_POLYGON);
        glVertex2f(-48.0f, 0.0f); glVertex2f(65.0f, 0.0f);
        glVertex2f(78.0f, 2.5f);  glVertex2f(60.0f, -5.0f);
        glVertex2f(-42.0f, -5.0f);
    glEnd();

    glColor3f(0.10f, 0.16f, 0.24f);
    glBegin(GL_POLYGON);
        glVertex2f(-48.0f, 2.5f); glVertex2f(65.0f, 2.5f);
        glVertex2f(80.0f, 13.0f); glVertex2f(-48.0f, 13.0f);
    glEnd();

    float portR = lerp(1.0f, 1.0f, nightFactor);
    float portG = lerp(0.95f, 0.90f, nightFactor);
    float portB = lerp(0.60f, 0.20f, nightFactor);

    for (int p = 0; p < 9; p++) {
        drawCircle(-38.0f + p * 11.0f, 7.5f, 1.6f, portR, portG, portB);
    }

    drawRect(-40.0f, 13.0f, 58.0f, 19.0f, 0.95f, 0.96f, 0.98f);
    float cabinR = lerp(0.20f, 1.0f, nightFactor);
    float cabinG = lerp(0.35f, 0.9f, nightFactor);
    float cabinB = lerp(0.50f, 0.3f, nightFactor);

    for (int w = 0; w < 10; w++) {
        drawRect(-36.0f + w * 8.0f, 14.5f, -32.0f + w * 8.0f, 17.5f, cabinR, cabinG, cabinB);
    }

    drawRect(-32.0f, 19.0f, 48.0f, 24.5f, 0.92f, 0.94f, 0.96f);
    for (int w = 0; w < 8; w++) {
        drawRect(-28.0f + w * 8.0f, 20.5f, -24.0f + w * 8.0f, 23.0f, cabinR, cabinG, cabinB);
    }

    glColor3f(0.88f, 0.90f, 0.92f);
    glBegin(GL_POLYGON);
        glVertex2f(-10.0f, 24.5f); glVertex2f(40.0f, 24.5f);
        glVertex2f(34.0f, 29.5f);  glVertex2f(-10.0f, 29.5f);
    glEnd();

    glColor3f(0.35f, 0.38f, 0.42f);
    glLineWidth(1.6f);
    glBegin(GL_LINES);
        glVertex2f(8.0f, 29.5f); glVertex2f(8.0f, 38.0f);
    glEnd();
    drawCircle(8.0f, 38.0f, 1.5f, 1.0f, 0.2f, 0.2f);

    glColor3f(0.85f, 0.15f, 0.15f);
    glBegin(GL_POLYGON);
        glVertex2f(-20.0f, 24.5f); glVertex2f(-6.0f, 24.5f);
        glVertex2f(-4.0f, 34.0f);  glVertex2f(-22.0f, 34.0f);
    glEnd();
    drawRect(-22.0f, 32.0f, -4.0f, 34.0f, 0.15f, 0.15f, 0.15f);

    float smX = -13.0f, smY = 36.0f;
    float smA = lerp(0.90f, 0.35f, nightFactor);
    drawCircle(smX - 5.0f, smY + 3.0f + sinf(smokeAnim) * 1.5f, 3.0f, smA, smA, smA);
    drawCircle(smX - 14.0f, smY + 6.0f + cosf(smokeAnim) * 2.0f, 4.5f, smA * 0.9f, smA * 0.9f, smA * 0.9f);

    if (nightFactor > 0.15f) {
        for (int r = 0; r < 5; r++) {
            float rx = -40.0f + r * 22.0f + sinf(waterWave + r) * 6.0f;
            drawEllipse(rx, -8.0f - r * 3.0f, 12.0f, 1.5f, 1.0f, 0.85f, 0.35f, 0.25f * nightFactor);
        }
    }

    glPopMatrix();
}

// -------------------------------------------------------------
// Road, Sidewalk & Animated River
// -------------------------------------------------------------
void drawRoadAndRiver() {
    drawRect(0.0f, 98.0f, 1200.0f, 108.0f, lerp(0.38f, 0.15f, nightFactor), lerp(0.68f, 0.28f, nightFactor), lerp(0.25f, 0.10f, nightFactor));
    drawRect(0.0f, 52.0f, 1200.0f, 98.0f, lerp(0.20f, 0.10f, nightFactor), lerp(0.22f, 0.11f, nightFactor), lerp(0.24f, 0.12f, nightFactor));

    glColor3f(0.95f, 0.95f, 0.95f);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 75.0f); glVertex2f(1200.0f, 75.0f);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 96.0f); glVertex2f(1200.0f, 96.0f);
        glVertex2f(0.0f, 54.0f); glVertex2f(1200.0f, 54.0f);
    glEnd();

    drawRect(0.0f, 46.0f, 1200.0f, 52.0f, lerp(0.48f, 0.24f, nightFactor), lerp(0.52f, 0.26f, nightFactor), lerp(0.55f, 0.28f, nightFactor));
    drawRect(0.0f, 45.0f, 1200.0f, 46.5f, 0.28f, 0.32f, 0.34f);

    float waterTopR = lerp(0.18f, 0.04f, nightFactor);
    float waterTopG = lerp(0.58f, 0.16f, nightFactor);
    float waterTopB = lerp(0.78f, 0.32f, nightFactor);

    float waterBotR = lerp(0.06f, 0.01f, nightFactor);
    float waterBotG = lerp(0.25f, 0.06f, nightFactor);
    float waterBotB = lerp(0.48f, 0.18f, nightFactor);

    glBegin(GL_QUADS);
        glColor3f(waterTopR, waterTopG, waterTopB);
        glVertex2f(0.0f, 45.0f);
        glVertex2f(1200.0f, 45.0f);
        glColor3f(waterBotR, waterBotG, waterBotB);
        glVertex2f(1200.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
    glEnd();

    glColor4f(0.90f, 0.98f, 1.0f, lerp(0.60f, 0.30f, nightFactor));
    glLineWidth(1.8f);
    for (int row = 0; row < 3; row++) {
        float wy = 7.0f + row * 12.0f;
        glBegin(GL_LINES);
        for (int i = 0; i < 18; i++) {
            float wx = i * 70.0f + sinf(waterWave + row * 1.5f + i) * 12.0f;
            glVertex2f(wx, wy);
            glVertex2f(wx + 30.0f, wy);
        }
        glEnd();
    }

    if (nightFactor > 0.15f) {
        for (int m = 0; m < 8; m++) {
            float mx = 680.0f + sinf(waterWave * 1.5f + m) * (15.0f + m * 8.0f);
            float my = 40.0f - m * 4.5f;
            drawEllipse(mx, my, 18.0f + m * 4.0f, 1.5f, 0.95f, 0.98f, 1.0f, 0.35f * nightFactor);
        }
    }
}

// -------------------------------------------------------------
// Main Display Function
// -------------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Sky, Sun/Moon, Stars & Rainbow
    drawSky();
    drawStars();
    drawSunAndMoon();
    drawRainbow();

    // 2. Exact Picture-Matched Aeroplane & Helicopter
    drawExactPictureAeroplane(planeX, 530.0f);
    drawHelicopter(heliX, 465.0f);

    // 3. Clouds
    drawCloud(250.0f + cloudOffset * 0.5f, 520.0f, 1.1f);
    drawCloud(570.0f + cloudOffset * 0.6f, 565.0f, 1.2f);
    drawCloud(720.0f + cloudOffset * 0.7f, 500.0f, 1.0f);
    drawCloud(1030.0f + cloudOffset * 0.5f, 525.0f, 0.95f);

    // 4. Campus Architecture & Buildings
    drawLeftCampusBuildings();
    drawMidApartmentBuilding();
    drawFoodCourtBuilding();
    drawAccurateEDUCampusBuilding();

    // 5. Foliage & Landscaping
    drawLargeTree(55.0f, 98.0f);
    drawFloweringBush(482.0f, 98.0f, 0.85f);
    drawGoldPineTree(780.0f, 98.0f, 0.95f);
    drawRoundBerryBush(795.0f, 98.0f);

    // 6. Characters:
    // Day Mode: Boy Flying Kite
    drawBoyFlyingKite();
    // Both Modes: Boys Playing Football
    drawBoysPlayingFootball(880.0f, 98.0f);
    // Night Mode: Person Walking Pet Dog (Left to Right)
    drawNightImage2DogWalker(dogWalkerX, 98.0f);

    // 7. Road, Sidewalk, Streetlamps & River
    drawRoadAndRiver();
    drawStreetLamps();

    // 8. Cruise Ship in River
    drawRealisticShip(shipX, 12.0f);

    // 9. Vehicles: Upper Lane Yellow School Bus & Lower Lane EDU Buses
    drawSchoolBus(yellowBusX, 72.0f);

    float bus1X = fmodf(busBaseX + 600.0f, 1500.0f) - 150.0f;
    float bus2X = fmodf(busBaseX, 1500.0f) - 150.0f;
    drawBus(bus1X, 56.0f, 0.85f, 0.22f, 0.22f);
    drawBus(bus2X, 56.0f, 0.20f, 0.38f, 0.68f);

    glutSwapBuffers();
}

// -------------------------------------------------------------
// Animation Timer (~60 FPS)
// -------------------------------------------------------------
void timer(int value) {
    if (autoCycle) {
        cycleTimer += 0.008f;
        targetNight = (sinf(cycleTimer) + 1.0f) * 0.5f;
    }
    nightFactor += (targetNight - nightFactor) * 0.06f;

    beaconBlink += 0.08f;
    starTwinkle += 0.05f;

    busBaseX += 1.8f;
    if (busBaseX > 3000.0f) busBaseX -= 1500.0f;

    // School Bus Movement (Right -> Left)
    yellowBusX -= 2.2f;
    if (yellowBusX < -160.0f) yellowBusX = 1280.0f;

    // Aeroplane Movement (Left -> Right)
    planeX += 2.6f;
    if (planeX > 1320.0f) planeX = -160.0f;

    // Helicopter Movement
    heliX -= 1.6f;
    if (heliX < -150.0f) heliX = 1350.0f;

    heliRotor += 0.45f;
    if (heliRotor > 2.0f * PI) heliRotor -= 2.0f * PI;

    // Cruise Ship Movement
    shipX += 1.1f;
    if (shipX > 1320.0f) shipX = -160.0f;

    // Football Animation
    footballAnim += 0.07f;
    if (footballAnim > 2.0f * PI) footballAnim -= 2.0f * PI;

    // Night Pet Dog Walker Movement (Left -> Right)
    if (nightFactor > 0.1f) {
        dogWalkerX += 1.5f;
        walkAnim += 0.08f;
        if (dogWalkerX > 1280.0f) dogWalkerX = -150.0f;
    }

    waterWave += 0.08f;
    if (waterWave > 2.0f * PI) waterWave -= 2.0f * PI;

    smokeAnim += 0.06f;
    if (smokeAnim > 2.0f * PI) smokeAnim -= 2.0f * PI;

    kiteAnim += 0.05f;
    if (kiteAnim > 2.0f * PI) kiteAnim -= 2.0f * PI;

    cloudOffset += 0.2f;
    if (cloudOffset > 1200.0f) cloudOffset = -600.0f;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// -------------------------------------------------------------
// Interactive Keyboard Handler
// -------------------------------------------------------------
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ' ': // Toggle Day / Night
            autoCycle = false;
            targetNight = (targetNight < 0.5f) ? 1.0f : 0.0f;
            break;
        case 'n': case 'N': // Switch to Night
            autoCycle = false;
            targetNight = 1.0f;
            break;
        case 'd': case 'D': // Switch to Day
            autoCycle = false;
            targetNight = 0.0f;
            break;
        case 't': case 'T': // Toggle Auto Cycle
            autoCycle = !autoCycle;
            break;
        case 27: // ESC key to exit
            exit(0);
            break;
    }
}

// -------------------------------------------------------------
// Interactive Mouse Click Handler
// -------------------------------------------------------------
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        autoCycle = false;
        targetNight = (targetNight < 0.5f) ? 1.0f : 0.0f;
    }
}

// -------------------------------------------------------------
// Window Setup & OpenGL Initialization
// -------------------------------------------------------------
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1200.0, 0.0, 600.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init() {
    glClearColor(0.26f, 0.70f, 0.94f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1200.0, 0.0, 600.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initStars();
}

// -------------------------------------------------------------
// Main Entry Point
// -------------------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Interactive 2D City Architecture (Day & Night Mode)");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
