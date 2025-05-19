#include <GL/freeglut.h>
#include <stdio.h>

// Window size
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define ORTHO_LEFT -(int)(WIN_WIDTH/2)
#define ORTHO_RIGHT (int)(WIN_WIDTH/2)
#define ORTHO_BOTTOM -(int)(WIN_HEIGHT/2)
#define ORTHO_TOP (int)(WIN_HEIGHT/2)

// Clipping window boundaries
float x_min = -50, y_min = -50, x_max = 50, y_max = 50;

typedef struct {
    float x, y;
} Point;

// Global user point
Point userPoint;

int computeRegionCode(Point p) {
    int code = 0;

    if (p.x < x_min) code |= 1;  // Left
    if (p.x > x_max) code |= 2;  // Right
    if (p.y < y_min) code |= 4;  // Bottom
    if (p.y > y_max) code |= 8;  // Top

    return code;
}

int determineRegion(int code) {
    switch (code) {
        case 0b0010: return 1;
        case 0b1010: return 2;
        case 0b0110: return 3;
        case 0b0001: return 4;
        case 0b0000: return 5;
        case 0b0101: return 6;
        case 0b0100: return 7;
        case 0b1001: return 8;
        case 0b1000: return 9;
        default: return -1; // Diagonal or invalid region
    }
}

void drawAxes(){
        glColor3f(1.0,1.0,0.0);
        glBegin(GL_LINES);
        glVertex2f(ORTHO_LEFT,0);
        glVertex2f(ORTHO_RIGHT,0);
        glVertex2f(0,ORTHO_BOTTOM);
        glVertex2f(0,ORTHO_TOP);
        glEnd();
}

void drawClippingWindow() {
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_min, y_min);
    glVertex2f(x_max, y_min);
    glVertex2f(x_max, y_max);
    glVertex2f(x_min, y_max);
    glEnd();
}

void drawUserPoint(Point p) {

    int code = computeRegionCode(p);
    int region = determineRegion(code);

    if(code == 0){
            glColor3f(0.0,1.0,0.0);
    }else{
            glColor3f(1.0,0.0,0.0);
    }

    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2f(p.x,p.y);
    glEnd();

    printf("\nPoint (%.1f, %.1f) -> Region Code: %04d -> Region: %d\n",
           p.x, p.y, code, region);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    drawClippingWindow();
    drawUserPoint(userPoint);
    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // white background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP);
}

int main(int argc, char **argv) {
    printf("Enter coordinates of the point (x y): ");
    scanf("%f %f", &userPoint.x, &userPoint.y);

    glutInit(&argc, argv);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("Endpoint Code Generation (9 Regions)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}