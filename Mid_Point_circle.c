#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define POINT_SIZE 10

#define ORTHO_LEFT -(int)(WIN_WIDTH / POINT_SIZE) / 2
#define ORTHO_RIGHT (int)(WIN_WIDTH / POINT_SIZE) / 2
#define ORTHO_BOTTOM -(int)(WIN_HEIGHT / POINT_SIZE) / 2
#define ORTHO_TOP (int)(WIN_HEIGHT / POINT_SIZE) / 2

int centerX, centerY, radius;

// Classic midpoint circle algorithm (pixel-based)
void drawMidpointCircle(int xc, int yc, int r)
{
    int p = 1 - r;
    int x = 0;
    int y = r;

    glColor3f(0.416, 0.173, 0.439);
    glBegin(GL_POINTS);
    while (x <= y) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);

        x++;
        if (p < 0) {
            p = p + 2 * x + 1;
        } else {
            y--;
            p = p + 2 * (x - y) + 1;
        }
    }
    glEnd();
}

void drawAxes()
{
    glColor3f(0.941, 0.541, 0.365);
    glBegin(GL_LINES);
    glVertex2f(ORTHO_LEFT, 0);
    glVertex2f(ORTHO_RIGHT, 0);
    glVertex2f(0, ORTHO_BOTTOM);
    glVertex2f(0, ORTHO_TOP);
    glEnd();
}

void displayFunc()
{
    glClearColor(0.976, 0.929, 0.412, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();
    drawMidpointCircle(centerX, centerY, radius);

    glFlush();
}

int main(int argc, char **argv)
{
    printf("Enter circle center (x y): ");
    scanf("%d %d", &centerX, &centerY);
    printf("Enter radius: ");
    scanf("%d", &radius);

    if (centerX < ORTHO_LEFT || centerX > ORTHO_RIGHT || centerY < ORTHO_BOTTOM || centerY > ORTHO_TOP) {
        printf("Error: Center is out of view bounds (%d, %d) to (%d, %d)\n",
               ORTHO_LEFT, ORTHO_BOTTOM, ORTHO_RIGHT, ORTHO_TOP);
        return 1;
    }

    if (radius < 1 || radius > (ORTHO_RIGHT - ORTHO_LEFT) / 2) {
        printf("Error: Radius too small or too large for view.\n");
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("Midpoint Circle with User Input");

    gluOrtho2D(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP);
    glPointSize(POINT_SIZE);

    glutDisplayFunc(displayFunc);
    glutMainLoop();

    return 0;
}