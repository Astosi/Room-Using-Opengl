#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sstream>

#ifdef _APPLE_
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif


#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

double clockRotation = 0;


double roomScaleX = 1;
double roomScaleY = 1;
double roomTranslationX = 0;
double roomTranslationY = 0;
double targetRoomScaleX = 1;
double targetRoomScaleY = 1;
double targetRoomTranslationX = 0;
double targetRoomTranslationY = 0;


int mouseCoorX = 0;
int mouseCoorY = 0;

bool light0 = false;
bool light1 = false;


class Vector3f
{
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f const& v)
	{
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v)
	{
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n)
	{
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n)
	{
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit()
	{
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v)
	{
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera
{
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f)
	{
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
		Vector3f view = (center - eye).unit();
		eye = eye + view * -0.7;
		center = center + view * -0.7;
	}

	void moveX(float d)
	{
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d)
	{
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d)
	{
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a)
	{
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a)
	{
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void lookUp(float a) {
		center.y += a;
	}

	void lookDown(float a) {
		center.y -= a;
	}

	void lookLeft(float a) {
		center.x -= a;
	}

	void lookRight(float a) {
		center.x += a;
	}

	void look()
	{
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z);
	}
};

Camera camera;

void setDefaultColor()
{
	glColor3f(0.8, 0.8, 0.8);
}

void setupLights()
{
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat light0Intensity[] = { 0.7f, 0.7f, 1.0f, 1.0f };
	GLfloat light0Position[] = { 1.0f, 1.0f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Intensity);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	
}

void setupCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void drawWall(double thickness)
{
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glColor3f(0.4 , 0.3 ,0.3);
	glutSolidCube(1);
	glPopMatrix();
	setDefaultColor();
}

void drawFloor(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glColor3f(1, 1, 1);
	glutSolidCube(1);
	glPopMatrix();
	setDefaultColor();
}

void drawTableLeg(double thick, double len)
{
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawTable(double topWid, double topThick, double legThick, double legLen)
{
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}

void drawChair()
{
	glPushMatrix();

	// CHAIR BACK
	glPushMatrix();
	glColor3f(0.5, 0.25, 0.25);
	glTranslated(-.25, 1.03, -0.1);
	glScaled(0.8, 1.1, 0.08);
	glutSolidCube(1);
	glPopMatrix();
	// CHAIR BACK

	// CHAIR BOTTOM
	glPushMatrix();
	glColor3f( 0.1 , 0.1 , 0.2);
	glTranslated(-.25, 0.5, 0.25);
	glScaled(0.8, 0.08, 0.8);
	glutSolidCube(1);
	glPopMatrix();
	// CHAIR BOTTOM

	// 4 LEGS
	glPushMatrix();
	glScaled(0.1, 1, 0.1);
	glColor3f(0.2,0.2,0.2);
	glPushMatrix();
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5, 0, 0);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslated(-5, 0, 5);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 5);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
	// 4 LEGS

	glPopMatrix();
}

void drawLamp()
{
	glPushMatrix();

	// BODY
	glPushMatrix();
	glColor3f(0.6 , 0.3 , 0.1 );
	glScaled(0.3, 22, 0.35);
	glutSolidCube(1);
	glPopMatrix();

	// TOP
	glPushMatrix();
	glColor3f(0.2 , 0.4 , 0.5 );
	glTranslated(0, 12, 0);
	glRotated(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 1, 2, 5, 20, 20);
	glPopMatrix();


	glPopMatrix();
}

void drawRoom()
{

	glPushMatrix();
	glTranslated(roomTranslationX, roomTranslationY, 0);

	// CHAIR
	glPushMatrix();
	glTranslated(0.25, 0.13, 0.7);
	glScaled(0.15, 0.15, 0.15);
	glRotated(45, 0, 1, 0);
	drawChair();
	glPopMatrix();
	// CHAIR

	// LAMP
	glPushMatrix();
	glTranslated(0.75, 0.35, 0.1);
	glScaled(0.05, 0.03, 0.05);
	drawLamp();
	glPopMatrix();
	// LAMP

	// CLOCK
	glPushMatrix();
	glColor3f(0 , 0 , 0);
	glTranslated(0.4, 0.55, 0);
	glScaled(0.15, 0.15, 0.15);
	
	// CLOCK ARM
	glPushMatrix();
	glRotated(clockRotation, 0, 0, 1);
	glTranslated(0.5, 0.1, 0);
	glScaled(1, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	// CLOCK ARM

	glutSolidTorus(0.1, 1, 25, 25);
	glPopMatrix();
	// CLOCK

	glPushMatrix();
	setDefaultColor();
	glScaled(0.8, 0.8, 0.8);
	glTranslated(0.4, 0, 0.4);

	setDefaultColor();

	glPushMatrix();
	glTranslated(0.4, 0.0, 0.4);
	glColor3f(1 , 1 , 0.7 );
	drawTable(0.6, 0.02, 0.02, 0.3);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	
	// ============ WALLS ============ //
	drawFloor(0.02);
	glPushMatrix();
	glRotated(90, 0, 0, 1.0);
	drawWall(0.02);
	glPopMatrix();
	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	drawWall(0.02);
	glPopMatrix();

}


void Display()
{
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glPushMatrix();
	glScaled(roomScaleX, roomScaleY, 1);
	drawRoom();
	glPopMatrix();

	glFlush();
}




void onKeyPress(unsigned char key, int x, int y)
{
	float d = 0.01 * 10;

	switch (key)
	{
	
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;
	case 'g':
		if (light0) {
			glDisable(GL_LIGHT0);
			light0 = false;
		}
		else {
			glEnable(GL_LIGHT0);
			light0 = true;
		}
		break;
	case 'f':
		if (light1) {
			glDisable(GL_LIGHT1);
			light1 = false;
		}
		else {
			glEnable(GL_LIGHT1);
			light1 = true;
		}
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void onSpecialKey(int key, int x, int y)
{
	float a = 3.0;

	switch (key)
	{
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void onMouseMove(int x, int y) {
	if(mouseCoorY < y){
		camera.lookDown(0.02f);
	}
	else if (mouseCoorY > y) {
		camera.lookUp(0.02f);
	}

	if (mouseCoorX < x) {
		camera.lookRight(0.02f);
	}
	else if (mouseCoorX > x) {
		camera.lookLeft(0.02f);
	}

	mouseCoorX = x;
	mouseCoorY = y;
}

void idleCallback()
{
	if (clockRotation < 0)
	{
		clockRotation = 360;
	}
	clockRotation -= 0.05;	

	glutPostRedisplay();
}



int main(int argc, char** argv)
{
	srand(time(NULL));
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("3d Room");
	glutDisplayFunc(Display);
	glutKeyboardFunc(onKeyPress);
	glutSpecialFunc(onSpecialKey);
	glutMotionFunc(onMouseMove);

	glutIdleFunc(idleCallback);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
	return 0;
}