#include <GL/freeglut.h>
#include <stdio.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define POINT_SIZE 8

#define ORTHO_LEFT  -50
#define ORTHO_RIGHT  50
#define ORTHO_BOTTOM -40
#define ORTHO_TOP    40

int px, py;
int xmin, xmax, ymin, ymax;

// Function to set a pixel at (x, y)
void setPixel(int x, int y) {
    glPointSize(POINT_SIZE);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Point visibility check and color assignment
void pointVisibility(int px, int py, int xmin, int xmax, int ymin, int ymax) {
    if (px >= xmin && px <= xmax && py >= ymin && py <= ymax) {
        glColor3f(0.0, 1.0, 0.0); // Green for visible
    } else {
        glColor3f(1.0, 0.0, 0.0); // Red for invisible
    }
    setPixel(px, py);
}

// Function to draw bounding rectangle
void drawRectangle(int xmin, int xmax, int ymin, int ymax) {
    glColor3f(0.0, 0.0, 1.0); // Blue rectangle
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();
}

// Drawing coordinate axes
void drawAxes() {
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2f(ORTHO_LEFT, 0);
    glVertex2f(ORTHO_RIGHT, 0);
    glVertex2f(0, ORTHO_BOTTOM);
    glVertex2f(0, ORTHO_TOP);
    glEnd();
}

void display() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();
    drawRectangle(xmin, xmax, ymin, ymax);
    pointVisibility(px, py, xmin, xmax, ymin, ymax);

    glFlush();
}

int main(int argc, char **argv) {
    printf("Enter point coordinates (x y): ");
    scanf("%d %d", &px, &py);

    printf("Enter rectangle boundaries (xmin xmax ymin ymax): ");
    scanf("%d %d %d %d", &xmin, &xmax, &ymin, &ymax);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("Point Visibility Check");

    gluOrtho2D(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}