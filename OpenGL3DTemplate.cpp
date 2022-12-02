#include <glut.h>

double width = 1280;
double height = 720;

// 0 = front view
// 1 = side view
// 2 = top view
int camera_mode = 0;

// player

double player_position_x = 0;
double player_position_y = 2.5;
double player_position_z = 0;
double player_speed = 1;

// ball

double ball_position_x = 0;
double ball_position_y = 0.25;
double ball_position_z = -1;
double ball_size = 0.25;

// draw a z-axis, with cone at end
void axis(double length)
{
	glPushMatrix();

	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, length); // along the z-axis
	glEnd();

	glTranslated(0, 0, length - 0.2);
	glutWireCone(0.04, 0.2, 12, 9);

	glPopMatrix();
}

void drawAxes(double length)
{
	glPushMatrix();
	glColor3d(0, 0, 1); // draw black lines
	axis(length);			// z-axis
	glColor3d(0, 1, 0); // draw black lines
	glRotated(90, 0, 1.0, 0);
	axis(length); // y-axis
	glRotated(-90.0, 1, 0, 0);
	glColor3d(1, 0, 0); // draw black lines
	axis(length); // x-axis
	glPopMatrix();

	glFlush();
}

void drawField(double width, double depth) {
	glPushMatrix();
	glScaled(width, 0, depth);
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.8f, 0.2f);
	glVertex3f(1, 0, -1);
	glVertex3f(-1, 0, -1);
	glVertex3f(-1.0f, 0, 1.0f);
	glVertex3f(1.0f, 0, 1.0f);
	glEnd();
	glPopMatrix();
}

void drawFootball(double x, double y, double z) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.5f, 0.5f, 0.5f);
	glutSolidSphere(ball_size, 25, 25);
	glPopMatrix();
}

void drawPlayer(double x, double y, double z) {
	glPushMatrix();
	glTranslatef(x, y, z);
	// head
	glColor3f(0.5f, 0.5f, 0.9f);
	glutSolidSphere(0.25, 25, 25);
	// torso
	glPushMatrix();
	glScalef(1, 1, 0.5);
	glColor3f(0.5f, 0.2f, 0.9f);
	glTranslatef(0, -0.75, 0);
	glutSolidCube(1);
	// left arm
	glPushMatrix();
	glColor3f(0.5f, 0.3f, 0.9f);
	glTranslatef(-0.75, 0, 0);
	glScalef(-0.5, 1, 1);
	glutSolidCube(1);
	glPopMatrix();
	// right arm
	glPushMatrix();
	glColor3f(0.5f, 0.3f, 0.9f);
	glTranslatef(0.75, 0, 0);
	glScalef(-0.5, 1, 1);
	glutSolidCube(1);
	glPopMatrix();
	// left leg
	glPushMatrix();
	glColor3f(0.5f, 0.3f, 0.9f);
	glTranslatef(-0.25, -1, 0);
	glScalef(-0.5, 1, 1);
	glutSolidCube(1);
	glPopMatrix();
	// right leg
	glPushMatrix();
	glColor3f(0.5f, 0.3f, 0.9f);
	glTranslatef(0.25, -1, 0);
	glScalef(-0.5, 1, 1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void drawNet(double width, double height) {
	glPushMatrix();
	glTranslated(0, height, 15);
	glColor3f(0.5f, 0.5f, 0.9f);
	//left side
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(width, 0, 0); // cylinder at (0,0,1)  
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.2, 0.2, height, 8, 8);
	glPopMatrix();
	//right side
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(-width, 0, 0); // cylinder at (0,0,1)  
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.2, 0.2, height, 8, 8);
	glPopMatrix();
	//top side
	glPushMatrix();
	glTranslated(-width, 0, 0); // cylinder at (0,0,1)  
	glRotated(90, 0, 1, 0);
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.2, 0.2, 2*width, 8, 8);
	glPopMatrix();
	glPopMatrix();
}

void keyCB(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		camera_mode++;
		if (camera_mode > 2)
			camera_mode = 0;
		break;
	case 'a':
		player_position_x += player_speed;
		/*if (player_position_x - player_size <= 0)
			player_position_x = player_size;*/
		break;
	case 'd':
		player_position_x -= player_speed;
		/*if (player_position_x + player_size >= window_x_size)
			player_position_x = window_x_size - player_size;*/
		break;
	case 'w':
		player_position_z += player_speed;
		/*if (player_position_z + player_size >= window_y_size)
			player_position_z = window_y_size - player_size;*/
		break;
	case 's':
		player_position_z -= player_speed;
		/*if (player_position_z - player_size <= 0)
			player_position_z = player_size;*/
		break;
	default:
		player_position_y++;
	}
	glutPostRedisplay();
}

void setCameraPerspective() {
	glLoadIdentity();
	gluPerspective(120.0f, width / height, 0.1f, 300.0f);
	switch (camera_mode)
	{
		// front view
	case 0:
		gluLookAt(
			0.0f, 3.0f, -5.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		);
		break;
		// side view
	case 1:
		gluLookAt(
			5.0f, 3.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		);
		break;
		// top view
	case 2:
		gluLookAt(
			0.0f, 5.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
		break;
	}
}

void displayCB(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setCameraPerspective();
	drawAxes(3);
	drawField(10, 20);
	//drawFootball(ball_position_x, ball_position_y, ball_position_z);
	//drawPlayer(player_position_x, player_position_y, player_position_z);
	drawNet(4,3);

	glFlush();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("OpenGL - 3D Template");
	glutDisplayFunc(displayCB);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutKeyboardFunc(keyCB);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}
