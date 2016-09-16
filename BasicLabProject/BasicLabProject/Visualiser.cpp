#include "Visualizer.h"
 
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;
 
Textures * textureLoader;
std::vector<Shape> *shapesToBeDrawn;
ShapeTree *shapeTree;

bool bUsePredefinedCamera = true;
bool bFullsreen = false;
int nWindowID;

// camera attributes
float viewerPosition[3] = { -36.88, -28.0, -400.0 };
float viewerDirection[3] = { 50.0, 0.0, 50.0 };
float viewerUp[3] = { 0.0, 1.0, 0.0 };

// rotation values for the navigation
float navigationRotation[3] = { 53.0, 120.0, 0.0 };

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

	(*textureLoader).activateTexture("grass");
	drawPlain(Vector3D(-100,0,-100),Vector3D(400,-1.0f,400));


	 for (int i = 0; i < shapes.size(); i++){
		Shape& currentShape = shapes[i];
		(*textureLoader).activateTexture(currentShape.getName());
		switch (currentShape.getType()){
		case CUBE: 
			drawBlock(currentShape.getScopePosition(), currentShape.getSize());
			break;
		case CYLINDER:
			drawCylinder(currentShape.getScopePosition(), currentShape.getSize());
			break;
		case PLAIN:
			drawPlain(currentShape.getScopePosition(), currentShape.getSize());
			break;
		default:
			throw "Undrawable type";
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
	float areaHeight = 2.0 / 8;
	float blockHeight = areaHeight - 2 * padding;

	std::deque<Node *> shapeQueue;
	shapeQueue.push_back(root);
	
	int depth = 0;
	float blockX = 0, blockY = 0;
	while (!shapeQueue.empty()) {

		int queueSize = shapeQueue.size();
		float areaWidth =  2.0 / queueSize;
		float blockWidth = std::fmin(0.4,areaWidth -  padding);

		for (int j = 0; j < queueSize; j++) {
			Node *current = shapeQueue[j]; 
			glColor3f(1.0f, 1.0f, 0.0f); // Yellow

			//drawing shape
		    blockX = -1 + areaWidth / 2.0 + areaWidth*j;
			blockY = 1 - (areaHeight / 2.0 + areaHeight*(depth));
			draw2DBlock(blockX,blockY, blockWidth, blockHeight); 

			//drawing shapename
			glColor3f(1.0f, 0.0f, 0.0f); // Red
			renderBitmapString(blockX, blockY, 0.0, GLUT_BITMAP_HELVETICA_10, current->getShape().getName().c_str());
			
			//Adding its children to the list
			auto children = (*current).getChildren();
			for (unsigned k = 0; k < children.size(); k++) 
				shapeQueue.push_back(children[k]);
			}

		int childrenNumber = shapeQueue.size() - queueSize;
		float childAreaWidth = 2.0 / childrenNumber;
		int prevChildren=0 ,currentChildren;
		for (int k = 0; k < queueSize; k++) { //drawing lines from block to its children
			Node *current = shapeQueue.front();
			blockX = -1 + areaWidth / 2.0 + areaWidth*k;		
			
			currentChildren = current->getChildren().size();
			
			for (int n = 0; n < currentChildren; n++) {
				float childX = -1 + childAreaWidth / 2.0 + childAreaWidth*(n+ prevChildren);
				float childY = 1 - (areaHeight / 2.0 + areaHeight*(depth + 1));
				drawLine(blockX, blockY-blockHeight/2.0, childX, childY + blockHeight / 2.0);
			}
			prevChildren += currentChildren;
			shapeQueue.pop_front();
		}
		depth++;
	}
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
	 glEnable (GL_LINE_SMOOTH);
 
	if (whatToDraw ==0) // draw 2d tree
		glutDisplayFunc(displayFunc);
	else { //draw 3d
		textureLoader = new Textures();
		glutDisplayFunc(displayFunc2);
		glutReshapeFunc(reshapeFunc);
		glutKeyboardFunc(keyboardFunc);
		glutMouseFunc(mouseCallbackFunc);
		glutMotionFunc(mouseMotionFunc);
		glutIdleFunc(idleFunc);
		glEnable(GL_TEXTURE_2D);
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

	float timesX = size.getX() / 10;
	float timesY = size.getY() / 10;
	float timesZ = size.getZ() / 10;

	//top
	glColor3f(0.0f, 0.0f, 1.0f);
	glNormal3f(0.0F, 1.0F, 0.0F);
	glTexCoord2f(0, 0);
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ());
	glTexCoord2f(timesX, 0);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ());
	glTexCoord2f(timesX, timesZ);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	glTexCoord2f(0, timesZ);
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());

	//bottom
	glColor3f(0.0f, 1.0f, 1.0f);
	glNormal3f(0.0F, -1.0F, 0.0F);
	glTexCoord2f(0, 0);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ());
	glTexCoord2f(timesX, 0);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ());
	glTexCoord2f(timesX, timesZ);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glTexCoord2f(0, timesZ);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());

	//front
	glColor3f(1.0f, 0.0f, 0.0f);
	glNormal3f(0.0F, 0.0F, -1.0F);
	glTexCoord2f(0, 0);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ());
	glTexCoord2f(timesX, 0);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ());
	glTexCoord2f(timesX, timesY);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ());
	glTexCoord2f(0, timesY);
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ());
	
	//back
	glColor3f(0.0f, 1.0f, 0.0f);
	glNormal3f(0.0F, 0.0F, 1.0F);
	glTexCoord2f(0, 0);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glTexCoord2f(timesX, 0);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glTexCoord2f(timesX, timesY);
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	glTexCoord2f(0, timesY);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	// left
	glColor3f(0.1f, 0.1f, 0.1f);
	glNormal3f(-1.0F, 0.0F, 0.0F);
	glTexCoord2f(0, 0);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ());
	glTexCoord2f(timesZ,0);
	glVertex3f(basePoint.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glTexCoord2f(timesZ, timesY);
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	glTexCoord2f(0, timesY);
	glVertex3f(basePoint.getX(), basePoint.getY() + size.getY(), basePoint.getZ());

	//right
	glColor3f(1.0f, 1.0f, 0.0f);
	glNormal3f(1.0F, 0.0F, 0.0F);
	glTexCoord2f(0, 0);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ());
	glTexCoord2f(timesZ, 0);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY(), basePoint.getZ() + size.getZ());
	glTexCoord2f(timesZ, timesY);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ() + size.getZ());
	glTexCoord2f(0, timesY);
	glVertex3f(basePoint.getX() + size.getX(), basePoint.getY() + size.getY(), basePoint.getZ());
	glEnd();
}

void drawCylinder(Vector3D & basePoint, Vector3D & size){
	glPushMatrix();
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluQuadricDrawStyle(quadratic, GLU_FILL);
	gluQuadricTexture(quadratic, GL_TRUE);
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

void drawPlain(Vector3D& position, Vector3D& size) {
	float timesX = size.getX() / 10;
	float timesY = size.getY() / 10;
	float timesZ = size.getZ() / 10;

	glBegin(GL_QUADS);	
	

	if (size.getX() < 0) {
		glNormal3f(size.getX()+2, 0.0F, 0.0F);
		glTexCoord2f(0, 0);
		glVertex3f(position.getX(), position.getY(), position.getZ());
		glTexCoord2f(timesZ, 0);
		glVertex3f(position.getX(), position.getY(), position.getZ()+size.getZ());
		glTexCoord2f(timesZ, timesY);
		glVertex3f(position.getX(), position.getY() + size.getY(), position.getZ() + size.getZ());
		glTexCoord2f(0, timesY);
		glVertex3f(position.getX(), position.getY() + size.getY(), position.getZ());
		glEnd();
	}
	else if (size.getY() < 0) {
		glNormal3f(0.0F, size.getY() + 2, 0.0F);
		glTexCoord2f(0, 0);
		glVertex3f(position.getX(), position.getY(), position.getZ());
		glTexCoord2f(0, timesZ);
		glVertex3f(position.getX(), position.getY(), position.getZ() + size.getZ());
		glTexCoord2f(timesX, timesZ);
		glVertex3f(position.getX() + size.getX(), position.getY(), position.getZ() + size.getZ());
		glTexCoord2f(timesX, 0);
		glVertex3f(position.getX() + size.getX(), position.getY(), position.getZ());
		glEnd();
	}
	else if (size.getZ() < 0) {
		glNormal3f(0.0F, 0.0F, size.getZ() + 2);
		glTexCoord2f(0, 0);
		glVertex3f(position.getX(), position.getY(), position.getZ());
		glTexCoord2f(timesX, 0);
		glVertex3f(position.getX() + size.getX(), position.getY(), position.getZ());
		glTexCoord2f(timesX, timesY);
		glVertex3f(position.getX() + size.getX(), position.getY() + size.getY(), position.getZ());
		glTexCoord2f(0, timesY);
		glVertex3f(position.getX(), position.getY() + size.getY(), position.getZ());
		glEnd();
	}
	else throw "Not a plain!";
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
