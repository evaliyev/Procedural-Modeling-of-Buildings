#include <GL\glew.h>
#include <GL\freeglut.h>

/**
* glVertex2f(float x, float y).
* The point (0.0, 0.0) represents the middle of the window (not the top left corner).
* The "2f" suffix means 2 values of float type (x and y).
*/
void displayMe(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.0);                    // bottom left
	glVertex2f(0.5, 0.0);                    // bottom right
	glVertex2f(0.5, 0.5);                    // top right
	glVertex2f(0.0, 0.5);                    // top left
	glEnd();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(300, 300);                    // window size
	glutInitWindowPosition(500, 500);                // distance from the top-left screen
	glutCreateWindow("BadproG - Hello world :D");    // message displayed on top bar window
	glutDisplayFunc(displayMe);
	glutMainLoop();
	return 0;
}