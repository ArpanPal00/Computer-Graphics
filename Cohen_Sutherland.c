#include <GL/freeglut.h>
#include <stdio.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define LEFT   1   // 0001
#define RIGHT  2   // 0010
#define BOTTOM 4   // 0100
#define TOP    8   // 1000

float x_min = -100, y_min = -100, x_max = 100, y_max = 100;

typedef struct {
    float x, y;
} Point;

Point p1, p2;

int computeCode(Point p) {
    int code = 0;
    if (p.x < x_min) code |= LEFT;
    if (p.x > x_max) code |= RIGHT;
    if (p.y < y_min) code |= BOTTOM;
    if (p.y > y_max) code |= TOP;
    return code;
}

int cohenSutherlandClip(Point *p1, Point *p2) {
    int code1 = computeCode(*p1);
    int code2 = computeCode(*p2);
    int accept = 0;

    while (1) {
        if ((code1 | code2) == 0) {
            accept = 1;
            break;
        } else if (code1 & code2) {
            break;
        } else {
            int codeOut = code1 ? code1 : code2;
            float x, y;

            if (codeOut & TOP) {
                x = p1->x + (p2->x - p1->x) * (y_max - p1->y) / (p2->y - p1->y);
                y = y_max;
            } else if (codeOut & BOTTOM) {
                x = p1->x + (p2->x - p1->x) * (y_min - p1->y) / (p2->y - p1->y);
                y = y_min;
            } else if (codeOut & RIGHT) {
                y = p1->y + (p2->y - p1->y) * (x_max - p1->x) / (p2->x - p1->x);
                x = x_max;
            } else {
                y = p1->y + (p2->y - p1->y) * (x_min - p1->x) / (p2->x - p1->x);
                x = x_min;
            }

            if (codeOut == code1) {
                p1->x = x;
                p1->y = y;
                code1 = computeCode(*p1);
            } else {
                p2->x = x;
                p2->y = y;
                code2 = computeCode(*p2);
            }
        }
    }

    return accept;
}

void drawAxes() {
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2f(-WIN_WIDTH, 0); glVertex2f(WIN_WIDTH, 0); // X-axis
    glVertex2f(0, -WIN_HEIGHT); glVertex2f(0, WIN_HEIGHT); // Y-axis
    glEnd();
}

void drawClippingWindow() {
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_min, y_min);
    glVertex2f(x_max, y_min);
    glVertex2f(x_max, y_max);
    glVertex2f(x_min, y_max);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    drawClippingWindow();

    // Original Line (Red)
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();

    Point clippedP1 = p1, clippedP2 = p2;
    if (cohenSutherlandClip(&clippedP1, &clippedP2)) {
        // Clipped Line (Green)
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
        glVertex2f(clippedP1.x, clippedP1.y);
        glVertex2f(clippedP2.x, clippedP2.y);
        glEnd();
    }

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-WIN_WIDTH / 2, WIN_WIDTH / 2, -WIN_HEIGHT / 2, WIN_HEIGHT / 2);
}

int main(int argc, char **argv) {
    printf("Enter coordinates of line endpoints (x1 y1 x2 y2): ");
    scanf("%f %f %f %f", &p1.x, &p1.y, &p2.x, &p2.y);

    glutInit(&argc, argv);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Cohen-Sutherland Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}