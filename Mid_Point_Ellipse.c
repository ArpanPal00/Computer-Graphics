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

int centerX, centerY, radiusX, radiusY;

void drawEllipse(int xc, int yc, int rx, int ry)
{
        int x = 0;
        int y = ry;

        int rxSq = rx * rx;
        int rySq = ry * ry;

        float p1 = rySq - (rxSq * ry) + (0.25f * rxSq);

        glColor3f(0.416, 0.173, 0.439);
        glBegin(GL_POINTS);

        // Region 1
        while (2 * rySq * x < 2 * rxSq * y) {
                glVertex2i(xc + x, yc + y);
                glVertex2i(xc - x, yc + y);
                glVertex2i(xc + x, yc - y);
                glVertex2i(xc - x, yc - y);

                x++;
                if (p1 < 0) {
                        p1 += 2 * rySq * x + rySq;
                } else {
                        y--;
                        p1 += 2 * rySq * x - 2 * rxSq * y + rySq;
                }
        }

        // Region 2
        float p2 = rySq * (x + 0.5f) * (x + 0.5f) + rxSq * (y - 1) * (y - 1) - rxSq * rySq;

        while (y >= 0) {
                glVertex2i(xc + x, yc + y);
                glVertex2i(xc - x, yc + y);
                glVertex2i(xc + x, yc - y);
                glVertex2i(xc - x, yc - y);

                y--;
                if (p2 > 0) {
                        p2 -= 2 * rxSq * y - rxSq;
                } else {
                        x++;
                        p2 += 2 * rySq * x - 2 * rxSq * y + rxSq;
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
        drawEllipse(centerX, centerY, radiusX, radiusY);
        glFlush();
}

int main(int argc, char **argv)
{
        printf("Enter ellipse center (x y): ");
        scanf("%d %d", &centerX, &centerY);
        printf("Enter radius along x-axis (rx): ");
        scanf("%d", &radiusX);
        printf("Enter radius along y-axis (ry): ");
        scanf("%d", &radiusY);

        if (radiusX <= 0 || radiusY <= 0) {
                printf("Error: Radii must be positive values.\n");
                return 1;
        }

        if (centerX + radiusX > ORTHO_RIGHT || centerX - radiusX < ORTHO_LEFT ||
            centerY + radiusY > ORTHO_TOP || centerY - radiusY < ORTHO_BOTTOM) {
                printf("Warning: The ellipse might be partially or fully out of view bounds.\n");
        }

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
        glutCreateWindow("Midpoint Ellipse with User Input");

        gluOrtho2D(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP);
        glPointSize(POINT_SIZE);

        glutDisplayFunc(displayFunc);
        glutMainLoop();

        return 0;
}