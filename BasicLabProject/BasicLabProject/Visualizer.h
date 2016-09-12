#pragma once
#include <GL/glut.h>
#include "Textures.h"
#include "Vector3D.h"
#include "Shape.h"
#include "ShapeTree.h"



// GLUT callbacks and functions
void initGlut(int whatToDraw, int argc, char **argv);
void displayFunc(void);
void idleFunc(void);
void reshapeFunc(int width, int height);
void mouseCallbackFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);
void keyboardFunc(unsigned char key, int x, int y);

// other [OpenGL] functions
void countFrames(void);
void renderBitmapString(float x, float y, float z, void *font, const char *string);

//Drawing 3d objects
void drawBlock(Vector3D& basePoint, Vector3D& size);//draw cuboid
void drawCylinder(Vector3D& basePoint, Vector3D& size);
void drawPlain(Vector3D& position, Vector3D& size);

//Drawing 2D objects
void draw2DBlock(float centerX, float centerY, float width, float height);
void drawLine(float startX, float startY, float endX, float endY);


void drawShapes(std::vector<Shape> shapes, int argc, char **argv); // draws 3d model of building
void drawDerivationTree(ShapeTree tree, int argc, char **argv); // draw 2d tree
