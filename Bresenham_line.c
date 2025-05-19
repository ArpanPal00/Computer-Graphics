#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define POINT_SIZE 5

#define ORTHO_LEFT -(int)(WIN_WIDTH / POINT_SIZE) / 2
#define ORTHO_RIGHT (int)(WIN_WIDTH / POINT_SIZE) / 2
#define ORTHO_BOTTOM -(int)(WIN_HEIGHT / POINT_SIZE) / 2
#define ORTHO_TOP (int)(WIN_HEIGHT / POINT_SIZE) / 2

int startX, startY, endX, endY;

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

void drawSmoothLine(int x0, int y0, int x1, int y1)
{
    glColor3f(0.2, 0.1, 0.7);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    glVertex2i(x0, y0);
    glVertex2i(x1, y1);
    glEnd();

    glDisable(GL_LINE_SMOOTH);
}

void displayFunc()
{
    glClearColor(0.976, 0.929, 0.412, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    drawSmoothLine(startX, startY, endX, endY);
    glFlush();
}

int main(int argc, char **argv)
{
    printf("Enter start point (x y): ");
    scanf("%d %d", &startX, &startY);
    printf("Enter end point (x y): ");
    scanf("%d %d", &endX, &endY);

    if (startX < ORTHO_LEFT || startX > ORTHO_RIGHT || endX < ORTHO_LEFT || endX > ORTHO_RIGHT ||
        startY < ORTHO_BOTTOM || startY > ORTHO_TOP || endY < ORTHO_BOTTOM || endY > ORTHO_TOP) {
        printf("Error: Coordinates must be within the range (%d, %d) to (%d, %d)\n",
               ORTHO_LEFT, ORTHO_BOTTOM, ORTHO_RIGHT, ORTHO_TOP);
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("Smooth Line with User Input");

    gluOrtho2D(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP);
    glPointSize(POINT_SIZE);

    glutDisplayFunc(displayFunc);
    glutMainLoop();

    return 0;
}