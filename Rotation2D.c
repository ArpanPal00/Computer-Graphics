#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define MAX_VERTS 100

typedef struct Point {
    float x, y;
} Point;

int nVerts;
Point verts[MAX_VERTS];
Point rotVerts[MAX_VERTS];
Point pvtPt;
double theta; // in radians

const double PI = 3.14159265359;

void rotate(Point verts[], Point rotVerts[], Point pvtPt, int nVerts, double theta) {
    for (int i = 0; i < nVerts; i++) {
        rotVerts[i].x = pvtPt.x + (verts[i].x - pvtPt.x) * cos(theta) -
                        (verts[i].y - pvtPt.y) * sin(theta);
        rotVerts[i].y = pvtPt.y + (verts[i].x - pvtPt.x) * sin(theta) +
                        (verts[i].y - pvtPt.y) * cos(theta);
    }
}

void drawPolygon(int nVerts, Point *verts, float colors[]) {
    glColor3f(colors[0], colors[1], colors[2]);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < nVerts; i++) {
        glVertex2f(verts[i].x, verts[i].y);
    }
    glEnd();
}

void drawAxes() {
    glColor3f(0.941, 0.541, 0.365);
    glBegin(GL_LINES);
    glVertex2f(-(int)(WIN_WIDTH / 2), 0);
    glVertex2f((int)(WIN_WIDTH / 2), 0);
    glVertex2f(0, -(int)(WIN_HEIGHT / 2));
    glVertex2f(0, (int)(WIN_HEIGHT / 2));
    glEnd();
}

void displayFunc() {
    float color1[3] = { 1.0, 0.0, 0.0 };  // Red
    float color2[3] = { 0.0, 0.0, 1.0 };  // Blue

    glClearColor(0.976, 0.929, 0.412, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();
    drawPolygon(nVerts, verts, color1);
    rotate(verts, rotVerts, pvtPt, nVerts, theta);
    drawPolygon(nVerts, rotVerts, color2);

    glFlush();
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WIN_WIDTH / 2, WIN_WIDTH / 2, -WIN_HEIGHT / 2, WIN_HEIGHT / 2);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    printf("Enter number of vertices (max %d): ", MAX_VERTS);
    scanf("%d", &nVerts);
    if (nVerts < 3 || nVerts > MAX_VERTS) {
        printf("Error: Polygon must have at least 3 vertices and less than %d.\n", MAX_VERTS);
        return 1;
    }

    for (int i = 0; i < nVerts; i++) {
        printf("Enter vertex %d (x y): ", i + 1);
        scanf("%f %f", &verts[i].x, &verts[i].y);
    }

    printf("Enter pivot point (x y): ");
    scanf("%f %f", &pvtPt.x, &pvtPt.y);

    float angleDeg;
    printf("Enter rotation angle in degrees: ");
    scanf("%f", &angleDeg);
    theta = angleDeg * (PI / 180.0); // Convert to radians

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("2D Polygon Rotation with User Input");

    init();
    glutDisplayFunc(displayFunc);
    glutMainLoop();

    return 0;
}