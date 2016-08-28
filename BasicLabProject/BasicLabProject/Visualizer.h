#pragma once
#include <GL\glut.h>
#include "Visualizer.h"
#include "Vector3D.h"
#include "Shape.h"
#ifdef WIN32
#include <windows.h>
#endif

// GLUT callbacks and functions
void initGlut(int argc, char **argv);
void displayFunc(void);
void idleFunc(void);
void reshapeFunc(int width, int height);
void mouseCallbackFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);
void keyboardFunc(unsigned char key, int x, int y);

// other [OpenGL] functions
void countFrames(void);
void renderBitmapString(float x, float y, float z, void *font, char *string);
GLuint loadTexture(const char * filename);

void drawShapes(std::vector<Shape> shapes, int argc, char **argv);
void drawBlock(Vector3D& basePoint, Vector3D& size);//draw cuboid
void drawCylinder(Vector3D& basePoint, Vector3D& size);
void drawPlain(float x, float z, float sizeX, float sizeZ);