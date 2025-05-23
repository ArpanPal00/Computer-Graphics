#include <GL/freeglut.h>
#include <stdio.h>

#define MAX_DEPTH 10

float x_min, y_min, x_max, y_max;
float left, right, bottom, top;

typedef struct {
    float x, y;
} Point;

Point p1, p2;

int inside(Point p) {
    return (p.x >= x_min && p.x <= x_max &&
            p.y >= y_min && p.y <= y_max);
}

void drawLine(Point a, Point b, float r, float g, float bcolor) {
    glColor3f(r, g, bcolor);
    glBegin(GL_LINES);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
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

void drawAxes() {
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2f(left, 0); glVertex2f(right, 0); // X-axis
    glVertex2f(0, bottom); glVertex2f(0, top); // Y-axis
    glEnd();
}

void midpointSubdivision(Point a, Point b, int depth) {
    if (depth > MAX_DEPTH)
        return;

    if (inside(a) && inside(b)) {
        drawLine(a, b, 0.0, 1.0, 0.0);  // Green = visible part
        return;
    }

    if (!inside(a) && !inside(b)) {
        return;
    }

    Point mid = { (a.x + b.x) / 2.0f, (a.y + b.y) / 2.0f };

    midpointSubdivision(a, mid, depth + 1);
    midpointSubdivision(mid, b, depth + 1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();
    drawClippingWindow();

    drawLine(p1, p2, 1.0, 0.0, 0.0);  // Red = original

    midpointSubdivision(p1, p2, 0);

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    gluOrtho2D(left, right, bottom, top);
}

int main(int argc, char **argv) {
    printf("Enter ortho left, right, bottom, top: ");
    scanf("%f %f %f %f", &left, &right, &bottom, &top);

    printf("Enter clipping window x_min y_min x_max y_max: ");
    scanf("%f %f %f %f", &x_min, &y_min, &x_max, &y_max);

    printf("Enter line endpoints x1 y1 x2 y2: ");
    scanf("%f %f %f %f", &p1.x, &p1.y, &p2.x, &p2.y);

    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Midpoint Subdivision Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}