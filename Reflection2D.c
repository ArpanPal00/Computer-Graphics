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
Point reflectedVerts[MAX_VERTS];
Point pivot;
int reflectionType; // 1: X-axis, 2: Y-axis, 3: Origin

void reflect(Point verts[], Point reflectedVerts[], Point pivot, int nVerts, int type)
{
    for (int i = 0; i < nVerts; i++) {
        float x = verts[i].x - pivot.x;
        float y = verts[i].y - pivot.y;

        switch (type) {
            case 1: // Reflect across X-axis
                y = -y;
                break;
            case 2: // Reflect across Y-axis
                x = -x;
                break;
            case 3: // Reflect across origin
                x = -x;
                y = -y;
                break;
            default:
                break;
        }

        reflectedVerts[i].x = x + pivot.x;
        reflectedVerts[i].y = y + pivot.y;
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
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINES);
    glVertex2f(-WIN_WIDTH / 2, 0);
    glVertex2f(WIN_WIDTH / 2, 0);
    glVertex2f(0, -WIN_HEIGHT / 2);
    glVertex2f(0, WIN_HEIGHT / 2);
    glEnd();
}

void displayFunc()
{
    glClearColor(0.9, 0.9, 0.9, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    float red[3] = {1, 0, 0};
    float blue[3] = {0, 0, 1};

    drawPolygon(nVerts, verts, red);
    reflect(verts, reflectedVerts, pivot, nVerts, reflectionType);
    drawPolygon(nVerts, reflectedVerts, blue);

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

    printf("Enter pivot point (x y): ");
    scanf("%f %f", &pivot.x, &pivot.y);

    printf("\nReflection options:\n");
    printf("1. Reflect across X-axis\n");
    printf("2. Reflect across Y-axis\n");
    printf("3. Reflect across Origin\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &reflectionType);

    if (reflectionType < 1 || reflectionType > 3) {
        printf("Invalid choice!\n");
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("2D Reflection with Pivot Point");

    init();
    glutDisplayFunc(displayFunc);
    glutMainLoop();

    return 0;
}