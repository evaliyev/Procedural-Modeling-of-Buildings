#include "Visualizer.h"
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;

std::vector<Shape> *shapesToBeDrawn;
ShapeTree *shapeTree;
bool bUsePredefinedCamera = true;
bool bFullsreen = false;
int nWindowID;

// camera attributes
float viewerPosition[3] = { -36.88, -28.0, -60.0 };
float viewerDirection[3] = { 50.0, 0.0, 50.0 };
float viewerUp[3] = { 0.0, 1.0, 0.0 };

// rotation values for the navigation
float navigationRotation[3] = { 50.0, 100.0, 0.0 };

// parameters for the framecounter
char pixelstring[30];
int cframe = 0;
int time = 0;
int timebase = 0;

// position of the mouse when pressed
int mousePressedX = 0, mousePressedY = 0;
float lastXOffset = 0.0, lastYOffset = 0.0, lastZOffset = 0.0;
// mouse button states
int leftMouseButtonActive = 0, middleMouseButtonActive = 0, rightMouseButtonActive = 0;
// modifier state
int shiftActive = 0, altActive = 0, ctrlActive = 0;

void displayFunc2(void) {
	// clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, 1.33, 1.0, 100000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL); //uncomment to enable colors

	GLfloat lightpos[4] = { 5.0, 15.0, 10.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	glTranslatef(viewerPosition[0], viewerPosition[1], viewerPosition[2]);
	// add navigation rotation

	glRotatef(navigationRotation[0], 1.0f, 0.0f, 0.0f);
	glRotatef(navigationRotation[1], 0.0f, 1.0f, 0.0f);
	
	std::vector<Shape> &shapes = (*shapesToBeDrawn);
	drawPlain(0.0, 0.0, 100.0, 100.0);
	for (int i = 0; i < shapes.size(); i++){
		Shape& currentShape = shapes[i];
		switch (currentShape.getType()){
		case CUBE: 
			drawBlock(currentShape.getScopePosition(), currentShape.getSize());
			break;
		case CYLINDER:
			drawCylinder(currentShape.getScopePosition(), currentShape.getSize());
			break;
		default:
			throw "Undrowable type";
		}
	}

	countFrames();

	glutSwapBuffers();
}

void displayFunc(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
	float padding = 0.1f;
	

	Node *root = (*shapeTree).getRoot();
	float areaHeight = 2.0 / 5;
	float blockHeight = areaHeight - 2 * padding;

	std::deque<Node *> shapeQueue;
	shapeQueue.push_back(root);

	
	int depth = 0;
	while (!shapeQueue.empty()) {
		
		float areaWidth =  2.0 / shapeQueue.size();
		float blockWidth = std::fmin(0.4,areaWidth - 2 * padding);
		int queueSize = shapeQueue.size();

		for (int j = 0; j < queueSize; j++) {
			Node *current = shapeQueue.front();
			glColor3f(1.0f, 1.0f, 0.0f); // Yellow
			draw2DBlock(-1+ areaWidth / 2.0 + areaWidth*j, 1-( areaHeight / 2.0 + areaHeight*(depth)), blockWidth, blockHeight);
			glColor3f(1.0f, 0.0f, 0.0f); // Red
			renderBitmapString(-1 + areaWidth / 2.0 + areaWidth*j, 1 - (areaHeight / 2.0 + areaHeight*(depth)), 0.0, GLUT_BITMAP_HELVETICA_10, current->getShape().getName().c_str());
			auto children = (*current).getChildren();
			for (unsigned k = 0; k < children.size(); k++) 
				shapeQueue.push_back(children[k]);
			shapeQueue.pop_front();
		}
		depth++;
	}

 

	

	//glColor3f(0.0f, 1.0f, 0.0f); // Green

	//glColor3f(0.2f, 0.2f, 0.2f); // Dark Gray
	 


	 
	//glColor3f(0.0f, 0.0f, 1.0f); // Blue

	//glColor3f(1.0f, 0.0f, 0.0f); // Red
 
	
 
	glFlush();  // Render now
}

void initGlut(int whatToDraw, int argc, char **argv) {

	// GLUT Window Initialization:
	glutInit(&argc, argv);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	nWindowID = glutCreateWindow("Procedural Modeling");
	// Register callbacks:

	if (whatToDraw ==0) // draw 2d tree
		glutDisplayFunc(displayFunc);
	else { //draw 3d
		glutDisplayFunc(displayFunc2);
		glutReshapeFunc(reshapeFunc);
		glutKeyboardFunc(keyboardFunc);
		glutMouseFunc(mouseCallbackFunc);
		glutMotionFunc(mouseMotionFunc);
		glutIdleFunc(idleFunc);
	}
}

void idleFunc(void) {
	glutPostRedisplay();
}

void reshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0, 1.33, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void mouseCallbackFunc(int button, int state, int x, int y) {

	// get the modifiers
	switch (glutGetModifiers()) {

	case GLUT_ACTIVE_SHIFT:
		shiftActive = 1;
		break;
	case GLUT_ACTIVE_ALT:
		altActive = 1;
		break;
	case GLUT_ACTIVE_CTRL:
		ctrlActive = 1;
		break;
	default:
		shiftActive = 0;
		altActive = 0;
		ctrlActive = 0;
		break;
	}

	// get the mouse buttons
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {
			leftMouseButtonActive += 1;
		}
		else
			leftMouseButtonActive -= 1;
	else if (button == GLUT_MIDDLE_BUTTON)
		if (state == GLUT_DOWN) {
			middleMouseButtonActive += 1;
			lastXOffset = 0.0;
			lastYOffset = 0.0;
		}
		else
			middleMouseButtonActive -= 1;
	else if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {
			rightMouseButtonActive += 1;
			lastZOffset = 0.0;
		}
		else
			rightMouseButtonActive -= 1;

	mousePressedX = x;
	mousePressedY = y;
}

void mouseMotionFunc(int x, int y) {

	float xOffset = 0.0, yOffset = 0.0, zOffset = 0.0;

	// rotatation
	if (leftMouseButtonActive) {

		navigationRotation[0] += ((mousePressedY - y) * 180.0f) / 200.0f;
		navigationRotation[1] += ((mousePressedX - x) * 180.0f) / 200.0f;

		mousePressedY = y;
		mousePressedX = x;

	}
	// panning
	else if (middleMouseButtonActive) {

		xOffset = (mousePressedX + x);
		if (!lastXOffset == 0.0) {
			viewerPosition[0] -= (xOffset - lastXOffset) / 8.0;
			viewerDirection[0] -= (xOffset - lastXOffset) / 8.0;
		}
		lastXOffset = xOffset;

		yOffset = (mousePressedY + y);
		if (!lastYOffset == 0.0) {
			viewerPosition[1] += (yOffset - lastYOffset) / 8.0;
			viewerDirection[1] += (yOffset - lastYOffset) / 8.0;
		}
		lastYOffset = yOffset;

	}
	// depth movement
	else if (rightMouseButtonActive) {
		zOffset = (mousePressedX + x);
		if (!lastZOffset == 0.0) {
			viewerPosition[2] -= (zOffset - lastZOffset) / 5.0;
			viewerDirection[2] -= (zOffset - lastZOffset) / 5.0;
		}
		lastZOffset = zOffset;
	}
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
#ifdef WIN32
		// exit on escape
	case '\033':
		exit(0);
		break;
#endif

		// switch to fullscreen
	case 'f':

		bFullsreen = !bFullsreen;
		if (bFullsreen)
			glutFullScreen();
		else {
			glutSetWindow(nWindowID);
			glutPositionWindow(100, 100);
			glutReshapeWindow(640, 480);
		}
		break;

	case 'k':
		printf("Camera position: %2.2f %2.2f %2.2f \n", viewerPosition[0], viewerPosition[1], viewerPosition[2]);
		printf("Camera rotation: %2.2f %2.2f %2.2f \n", navigationRotation[0], navigationRotation[1]);
		break;
	}
}

void countFrames(void) {

	time = glutGet(GLUT_ELAPSED_TIME);
	cframe++;
	if (time - timebase > 50) {
		sprintf_s(pixelstring, "fps: %4.2f", cframe*1000.0 / (time - timebase));
		timebase = time;
		cframe = 0;
		// Draw status text and uni-logo:
	}
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 200, 0, 200);
	glMatrixMode(GL_MODELVIEW);

	// render the string
	renderBitmapString(5, 5, 0.0, GLUT_BITMAP_HELVETICA_10, pixelstring);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, float z, void *font, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void drawBlock(Vector3D& basePoint, Vector3D& size) {
	glBegin(GL_QUADS);
	//top
	glColor3f(0.0f, 0.0f, 1.0f);
	glNormal3f(0.0F, 1.0F, 0.0F);
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());

	//bottom
	glColor3f(0.0f, 1.0f, 1.0f);
	glNormal3f(0.0F, -1.0F, 0.0F);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());

	//front
	glColor3f(1.0f, 0.0f, 0.0f);
	glNormal3f(0.0F, 0.0F, -1.0F);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ());
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ());
	
	//back
	glColor3f(0.0f, 1.0f, 0.0f);
	glNormal3f(0.0F, 0.0F, 1.0F);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	
	// left
	glColor3f(0.1f, 0.1f, 0.1f);
	glNormal3f(-1.0F, 0.0F, 0.0F);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ());
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ());

	//right
	glColor3f(1.0f, 1.0f, 0.0f);
	glNormal3f(1.0F, 0.0F, 0.0F);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ());

	glEnd();
}

void drawCylinder(Vector3D & basePoint, Vector3D & size){
	glPushMatrix();

	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	glTranslatef(basePoint.getX()+ size.getX() / 2,basePoint.getY(),basePoint.getZ()+ size.getX() / 2);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadratic, size.getX()/2, size.getX()/2, size.getY(), 32, 2);
	
	glTranslatef(0.0f, 0.0f,  size.getY()); 
	gluDisk(quadratic, 0.0f, size.getX() / 2, 30, 1);//top 

	glTranslatef(0.0f, 0.0f, -size.getY());
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(quadratic, 0.0f, size.getX() / 2, 30, 1); //bottom
	
	glPopMatrix();
}

void drawPlain(float x, float z, float sizeX, float sizeZ) {
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(x, 0.0F, z);
	glVertex3f(x+sizeX, 0.0F, z);
	glVertex3f(x + sizeX, 0.0F, z+sizeZ);
	glVertex3f(x, 0.0F, z + sizeZ);
	glEnd();
}

void draw2DBlock(float centerX, float centerY, float width, float height) {
	// Define shapes enclosed within a pair of glBegin and glEnd
	glBegin(GL_QUADS);               
	glVertex2f(centerX - width/2, centerY - height/2);         // Define vertices in counter-clockwise (CCW) order
	glVertex2f(centerX + width / 2, centerY - height / 2);     //  so that the normal (front-face) is facing you
	glVertex2f(centerX + width / 2, centerY + height / 2);
	glVertex2f(centerX - width / 2, centerY + height / 2);
	glEnd();
}

void drawLine(float startX, float startY, float endX, float endY) {
	glLineWidth(2.5);
	//glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(startX, startY);
	glVertex2f(endX, endY);
	glEnd();
}

void drawShapes(std::vector<Shape> shapes, int argc, char **argv) {
	shapesToBeDrawn = &shapes;
	printf("keys:\n\tf\t- toggle fullscreen\n\tesc\t- exit\n\tk\t- camera position and rotation\n\n");
	printf("mouse:\n\tleft button\t- rotation\n\tmiddle button\t- panning\n\tright button\t- zoom in and out\n");

	initGlut(1, argc, argv);
	glutMainLoop();
}

void drawDerivationTree(ShapeTree tree, int argc, char **argv) {
	shapeTree = &tree;
	initGlut(0, argc, argv);
	glutMainLoop();
}

unsigned char * loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight, bool flipY) {
	//printf("Reading image %s\n", imagepath);
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;
	// Open the file
	FILE * file; errno_t err;

	if ((err = fopen_s(&file, imagepath, "r")) != 0) {
		std::cout << "Couldn't load image" <<err;
	}
	// Read the header, i.e. the 54 first bytes
	// If less than 54 byes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return NULL; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return NULL; }
	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = outWidth*outHeight * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way
	data = new unsigned char[imageSize]; // Create a buffer
	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);
	// Everything is in memory now, the file wan be closed
	fclose(file);
	if (flipY) {
		// swap y-axis
		unsigned char * tmpBuffer = new unsigned char[outWidth * 3];
		int size = outWidth * 3;
		for (int i = 0; i<outHeight / 2; i++) {
			// copy row i to tmp
			memcpy_s(tmpBuffer, size, data + outWidth * 3 * i, size);
			// copy row h-i-1 to i
			memcpy_s(data + outWidth * 3 * i, size, data + outWidth * 3 * (outHeight - i - 1), size);
			// copy tmp to row h-i-1
			memcpy_s(data + outWidth * 3 * (outHeight - i - 1), size, tmpBuffer, size);
		}
		delete[] tmpBuffer;
	}
	return data;
}
