#include <GL/freeglut.h>
#include <stdio.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define MAX_VERTS 100

typedef struct Point {
    float x, y;
} Point;

int nVerts;
Point verts[MAX_VERTS];
Point scaleVerts[MAX_VERTS];
Point fixedPt;
float sx, sy;

void scale(Point verts[], Point scaleVerts[], Point fixedPt, int nVerts, float sx, float sy)
{
    for (int i = 0; i < nVerts; i++) {
        scaleVerts[i].x = verts[i].x * sx + fixedPt.x * (1 - sx);
        scaleVerts[i].y = verts[i].y * sy + fixedPt.y * (1 - sy);
    }
}

void drawPolygon(int nVerts, Point *verts, float colors[])
{
    glColor3f(colors[0], colors[1], colors[2]);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < nVerts; i++) {
        glVertex2f(verts[i].x, verts[i].y);
    }
    glEnd();
}

void drawAxes()
{
    glColor3f(0.941, 0.541, 0.365);
    glBegin(GL_LINES);
    glVertex2f(-WIN_WIDTH / 2, 0);
    glVertex2f(WIN_WIDTH / 2, 0);
    glVertex2f(0, -WIN_HEIGHT / 2);
    glVertex2f(0, WIN_HEIGHT / 2);
    glEnd();
}

void displayFunc()
{
    float color1[3] = {1.0, 0.0, 0.0}; // Red
    float color2[3] = {0.0, 0.0, 1.0}; // Blue

    glClearColor(0.976, 0.929, 0.412, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();
    drawPolygon(nVerts, verts, color1);
    scale(verts, scaleVerts, fixedPt, nVerts, sx, sy);
    drawPolygon(nVerts, scaleVerts, color2);

    glFlush();
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WIN_WIDTH / 2, WIN_WIDTH / 2, -WIN_HEIGHT / 2, WIN_HEIGHT / 2);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
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

    printf("Enter fixed point for scaling (x y): ");
    scanf("%f %f", &fixedPt.x, &fixedPt.y);

    printf("Enter scaling factors (sx sy): ");
    scanf("%f %f", &sx, &sy);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("2D Polygon Scaling with User Input");

    init();
    glutDisplayFunc(displayFunc);
    glutMainLoop();

    return 0;
}