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
Point shearedVerts[MAX_VERTS];
float shx;
Point refPt;

void shearX(Point verts[], Point shearedVerts[], int nVerts, float shx, Point refPt)
{
    for (int i = 0; i < nVerts; i++) {
        float x = verts[i].x + shx * (verts[i].y - refPt.y);
        float y = verts[i].y;
        shearedVerts[i].x = x;
        shearedVerts[i].y = y;
    }
}

void drawPolygon(int nVerts, Point *verts, float color[3])
{
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < nVerts; i++) {
        glVertex2f(verts[i].x, verts[i].y);
    }
    glEnd();
}

void drawAxes()
{
    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_LINES);
    glVertex2f(-WIN_WIDTH / 2, 0);
    glVertex2f(WIN_WIDTH / 2, 0);
    glVertex2f(0, -WIN_HEIGHT / 2);
    glVertex2f(0, WIN_HEIGHT / 2);
    glEnd();
}

void displayFunc()
{
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    float red[3] = {1.0, 0.0, 0.0};
    float blue[3] = {0.0, 0.0, 1.0};

    drawPolygon(nVerts, verts, red);
    shearX(verts, shearedVerts, nVerts, shx, refPt);
    drawPolygon(nVerts, shearedVerts, blue);

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
        printf("Error: Enter between 3 and %d vertices.\n", MAX_VERTS);
        return 1;
    }

    for (int i = 0; i < nVerts; i++) {
        printf("Enter vertex %d (x y): ", i + 1);
        scanf("%f %f", &verts[i].x, &verts[i].y);
    }

    printf("Enter shear reference point (x y): ");
    scanf("%f %f", &refPt.x, &refPt.y);

    printf("Enter x-direction shear factor (shx): ");
    scanf("%f", &shx);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("2D X-direction Shear");

    init();
    glutDisplayFunc(displayFunc);
    glutMainLoop();

    return 0;
}