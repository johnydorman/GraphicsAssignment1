// This script based on https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html

#include <glut.h>  
#include <math.h>
#include <iostream>
#include <vector>
#include "RObject.h"

GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat qaSomething[] = { 0.3, 0.5, 1.0, 1.0 };
GLfloat qaGreen[] = { 0.0, 1.0, 0.0, 1.0 };		// values for green
GLfloat qaPurple[] = { 1.0, 0.3, 1.0, 1.0 };		// values for white
GLfloat qaBlue[] = { 0.0, 0.0, 1.0, 1.0 };		// values for blue
GLfloat qaRed[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat qaOrange[] = { 1.0, 0.3, 0.0, 1.0 };// values for red
GLfloat qaEmmission[] = { 0.1, 0.1, 0.1, 1.0 }; // values for emmisve surface

GLint giXRes = 800;
GLint giYRes = 800;

float eyeX;
float eyeY;
float eyeZ;
int oldX, oldY;
float radius = 10.0f;
float phi = 0;
float theta = 0;
bool rotate = true;
int shapeEntry = 1;

float wireX = 0;
float wireY = 0;

std::vector<RObject> newObjects = std::vector<RObject>();

void drawCube(float x, float y, float z, float size) {

	glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);	// Set the reflective colour for ambient light
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);	// set the reflective colour for diffuse light
	//glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);	// set the colour for specular light
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);		// define how shiney the surface is
	glMaterialfv(GL_FRONT, GL_EMISSION, qaEmmission);		// define self illuminating surface behaviour
	glBegin(GL_QUADS);

	// Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.0f, 1.0f, 0.0f);		// Green
	glVertex3f(size+x, size + y, -size + z);
	glVertex3f(-size +x, size + y, -size + z);
	glVertex3f(-size + x, size + y, size + z);
	glVertex3f(size + x, size + y, size + z);

	// Bottom face (y = -1.0f)
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaRed);
	glVertex3f(size +x, -size + y, size + z);
	glVertex3f(-size +x, -size + y, size + z);
	glVertex3f(-size +x, -size + y, -size + z);
	glVertex3f(size +x, -size + y, -size + z);

	// Front face  (z = 1.0f)
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaBlue);		// Red
	glVertex3f(size + x, size + y, size + z);
	glVertex3f(-size + x, size + y, size + z);
	glVertex3f(-size + x, -size + y, size + z);
	glVertex3f(size + x, -size + y, size + z);

	// Back face (z = -1.0f)
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaPurple);		// Yellow
	glVertex3f(size + x, -size + y, -size + z);
	glVertex3f(-size + x, -size + y, -size + z);
	glVertex3f(-size + x, size + y, -size + z);
	glVertex3f(size + x, size + y, -size + z);

	// Left face (x = -1.0f)
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaOrange);		// Blue
	glVertex3f(-size + x, size + y, size);
	glVertex3f(-size + x, size + y, -size + z);
	glVertex3f(-size + x, -size + y, -size + z);
	glVertex3f(-size + x, -size + y, size + z);

	// Right face (x = 1.0f)
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaSomething);	// Magenta
	glVertex3f(size + x, size + y, -size + z);
	glVertex3f(size + x, size + y, size + z);
	glVertex3f(size + x, -size + y, size + z);
	glVertex3f(size + x, -size + y, -size + z);
	glEnd();

}
void drawPyramid(float x, float y, float, float size) {
	glBegin(GL_TRIANGLES);
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaRed);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, size);

	glMaterialfv(GL_FRONT, GL_AMBIENT, qaPurple);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(-size, -size, size);
	glVertex3f(0.0f, -size, -size);

	glMaterialfv(GL_FRONT, GL_AMBIENT, qaBlue);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, -size, -size);
	glVertex3f(size, -size, size);


	glMaterialfv(GL_FRONT, GL_AMBIENT, qaOrange);
	glVertex3f(-size, -size, size);
	glVertex3f(0.0f, -size, -size);
	glVertex3f(size, -size, size);

	glEnd();
}

void OnMouseMove(int x, int y) {
	theta += (x - oldX)*0.01f;
	phi += (y - oldY)*0.01f;
	oldX = x;
	oldY = y;
	glutPostRedisplay();
}

void drawShape(int shape) {
	switch (shape) {
	case 1:
		drawPyramid(0, 0, 0, 0.5);
		for (auto & element : newObjects) {
			std::cout << "element" << element.m_x << "," << element.m_y << "\n";
			drawPyramid(element.m_x, element.m_y, element.m_z, element.m_size);
		}
		break;
	case 2:
		drawCube(0, 0, 0, 0.5);
		for (auto & element : newObjects) {
			drawCube(element.m_x, element.m_y, element.m_z, element.m_size);
		}
		break;
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Add perspective view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1.0, 12.0);


	// moving the camera and target the same relative distance, creates a pan
	eyeX = 1 + radius*cos(phi)*sin(theta);
	eyeY = 1 + radius*sin(phi)*sin(theta);
	eyeZ = 1 + radius*cos(theta);
	gluLookAt(eyeX, eyeY, eyeZ,0, 0, 0, 0, 1, 0);


	// Switch back to Model view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.3, 0.3, 0.3, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	drawShape(shapeEntry);

	glTranslatef(wireX, wireY, 0);
	glutWireCube(1);
	glutSwapBuffers();

	glutPostRedisplay();
}

void Timer(int iUnused)
{
	glutPostRedisplay();
	glutTimerFunc(1, Timer, 0);
}

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);  // Use the normals from each surface to calculate lighting (FALSE = faster but not as accurate)
	glEnable(GL_LIGHTING);		// Enable Lighting
	glEnable(GL_LIGHT0);		// Create a light called LIGHT0

	GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 }; // ambient settings
	GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 }; // diffuse settings
	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 }; // specular settings
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);   // Define ambient light behaviour
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);   // Define diffuse ambient light behaviour
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight); // Define specular light behaviour

	////////////////////////////////////////////////////////////////
	// Include to make lightbulb
	////////////////////////////////////////////////////////////////
	GLfloat qaLightPosition[] = { 0.5, 0.5, 0.0, 1.0 };		// vector to store location of light0
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);		// Set position of light0
}

// Re-size event. Called back when the window first appears and
// whenever the window is re-sized with its new width and height 
void reshape(GLsizei width, GLsizei height) {		// GLsizei must be non-negative integer

													// Compute aspect ratio of the new window
	if (height == 0) height = 1;					// To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // Enable the Projection matrix
	glLoadIdentity();             // Reset

								  // Enable perspective projection with fovy, aspect, zNear and zFar

}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'x':
		glutSetWindowTitle("Window Name Change");
		break;
	case 'n':
		glutSetWindowTitle("Minimize Change");
		glutIconifyWindow();
	case 'l':
		glutPushWindow();
		break;
	case 'e': {
		RObject newObject = RObject(wireX, wireY, 0, 0.5);
		std::cout << "new object on " << newObject.m_x << ", " << newObject.m_y;
		newObjects.push_back(newObject);
		break;
	}
	case 'a' :
		wireX-=0.1;
		break;
	case 'd': 
		wireX+=0.1;
		break;
	case 'w':
		wireY+=0.1;
		break;
	case 's':
		wireY-=0.1;
		break;
	default:
		break;
	}
}

void myMenu(GLint menuNumber) {
	switch (menuNumber) {
	case 1:
		shapeEntry = 1;
		break;
	case 2:
		shapeEntry = 2;
		break;
	}
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(giXRes, giYRes);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Rotating Clipped Cube");
	glutCreateMenu(myMenu);
	glutAddMenuEntry("Pyramid", 1);
	glutAddMenuEntry("Cube", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(OnMouseMove);
	glutReshapeFunc(reshape);
	initGL();
	glutMainLoop();
	Timer(0);
	return 0;
}
