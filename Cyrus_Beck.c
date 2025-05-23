#include <GL/freeglut.h>
#include <stdio.h>

#define MAX_VERTICES 20

typedef struct {
    float x, y;
} Point;

Point polygon[MAX_VERTICES];
int vertexCount;

Point P0, P1; // Line endpoints

float left, right, top, bottom;

void drawAxes() {
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINES);
    glVertex2f(left, 0); glVertex2f(right, 0);
    glVertex2f(0, bottom); glVertex2f(0, top);
    glEnd();
}

void drawPolygon() {
    glColor3f(0, 0, 1); // Blue
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < vertexCount; i++) {
        glVertex2f(polygon[i].x, polygon[i].y);
    }
    glEnd();
}

void drawLine(Point a, Point b, float r, float g, float bcolor) {
    glColor3f(r, g, bcolor);
    glBegin(GL_LINES);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glEnd();
}

Point subtract(Point a, Point b) {
    return (Point){ a.x - b.x, a.y - b.y };
}

float dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

Point normal(Point a, Point b) {
    Point edge = subtract(b, a);
    return (Point){ -edge.y, edge.x }; // Perpendicular normal
}

void cyrusBeckClip(Point P0, Point P1) {
    Point D = subtract(P1, P0); // Direction vector

    float tE = 0.0, tL = 1.0;

    for (int i = 0; i < vertexCount; i++) {
        Point Pi = polygon[i];
        Point Pi1 = polygon[(i + 1) % vertexCount];

        Point n = normal(Pi, Pi1); // Outward normal
        Point W = subtract(P0, Pi);

        float num = -dot(n, W);
        float denom = dot(n, D);

        if (denom == 0) {
            if (num < 0) return; // Line is outside and parallel
            else continue;       // Line is inside and parallel
        }

        float t = num / denom;
        if (denom > 0) {
            if (t > 1.0) return;
            if (t > tE) tE = t;
        } else {
            if (t < 0.0) return;
            if (t < tL) tL = t;
        }
    }

    if (tE > tL) return;

    Point clippedStart = { P0.x + tE * D.x, P0.y + tE * D.y };
    Point clippedEnd = { P0.x + tL * D.x, P0.y + tL * D.y };

    drawLine(clippedStart, clippedEnd, 0.0, 1.0, 0.0); // Green: clipped line
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();
    drawPolygon();

    drawLine(P0, P1, 1.0, 0.0, 0.0); // Red: original line

    cyrusBeckClip(P0, P1);

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(left, right, bottom, top);
}

int main(int argc, char **argv) {
    printf("Enter ortho left, right, bottom, top: ");
    scanf("%f %f %f %f", &left, &right, &bottom, &top);

    printf("Enter number of vertices of convex polygon: ");
    scanf("%d", &vertexCount);
    if (vertexCount < 3 || vertexCount > MAX_VERTICES) {
        printf("Invalid vertex count.\n");
        return 1;
    }

    printf("Enter polygon vertices (x y):\n");
    for (int i = 0; i < vertexCount; i++) {
        scanf("%f %f", &polygon[i].x, &polygon[i].y);
    }

    printf("Enter line endpoints (x1 y1 x2 y2): ");
    scanf("%f %f %f %f", &P0.x, &P0.y, &P1.x, &P1.y);

    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Cyrus-Beck Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}