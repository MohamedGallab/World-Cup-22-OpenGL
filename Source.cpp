#include <iostream>
#include <Windows.h>
#include <glut.h>
using namespace std;

// general

double window_width = 1280;
double window_height = 720;
int delta_time = 25;
bool game_over = false;
bool game_won;

// camera

// 0 = front view
// 1 = side view
// 2 = top view
int camera_mode = 2;
double camera_speed = 1.1;
double eye_x = 0;
double eye_y = 3;
double eye_z = -5;
double up_x = 0;
double up_y = 3;
double up_z = -5;

// player

double player_position_x = 0;
double player_position_y = 2.5;
double player_position_z = -7;
double player_speed = 1;
double player_size = 1;
// 0 = +ve z
// 1 = +ve x
// 2 = -ve z
// 3 = -ve x
int player_facing_direction = 0;

// ball

double ball_position_x = 0;
double ball_position_y = 0.25;
double ball_position_z = 1;
double ball_size = 0.25;
double ball_speed = 0;
// 0 = +ve z
// 1 = +ve x
// 2 = -ve z
// 3 = -ve x
int ball_direction = 0;

// field

double field_width = 10;
double field_depth = 20;

// net
double net_width = 4;
double net_height = 3;
double net_depth = 3;

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
}

void drawTrophy() {
	glPushMatrix();
	glColor3f(0.7, 0.9, 0.1);
	glTranslated(0, 3, -field_depth - 3);
	glRotated(180, 1, 0, 0);
	glutSolidSphere(1, 25, 25);
	glTranslated(0, 0.55, 0);
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glutSolidCone(1, 2, 25, 25);
	glPopMatrix();
	glTranslated(0, 2, 0);
	glScaled(1, 0.25, 1);
	glColor3f(0.1, 0.7, 0.1);
	glutSolidSphere(0.75, 25, 25);

	glPopMatrix();
}

void drawField() {
	glPushMatrix();
	glScaled(field_width, 0, field_depth);
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.8f, 0.2f);
	glVertex3f(1, 0, -1);
	glVertex3f(-1, 0, -1);
	glVertex3f(-1.0f, 0, 1.0f);
	glVertex3f(1.0f, 0, 1.0f);
	glEnd();
	glPopMatrix();
}

void drawBench() {
	glColor3f(0.6, 0.3, 0.2);
	glPushMatrix();
	glTranslatef(0, 1, 0);

	glPushMatrix();
	glScaled(4, 0.2, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 1, -1);
	glScaled(4, 2, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	// back right leg
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(-2, -0.5, -1);
	glScaled(0.2, 1, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	// back left leg
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(2, -0.5, -1);
	glScaled(0.2, 1, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	// front right leg
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(-2, -0.5, 0.8);
	glScaled(0.2, 1, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	// front left leg
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(2, -0.5, 0.8);
	glScaled(0.2, 1, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
}

void drawFlag(
	float topColorR, float topColorG, float topColorB,
	float midColorR, float midColorG, float midColorB,
	float botColorR, float botColorG, float botColorB
) {
	// pole
	glPushMatrix();
	glColor3f(0, 0, 0);
	glScaled(0.2, 13.5, 0.2);
	glutSolidCube(1);
	glPopMatrix();
	// colors
	glPushMatrix();
	glTranslated(1.5, 0, 0);
	glTranslated(0, 6, 0);
	glScaled(3, 0.3, 0.1);
	// bottom color
	glColor3f(botColorR, botColorG, botColorB);
	glutSolidCube(1);
	// middle color
	glTranslated(0, 1, 0);
	glColor3f(midColorR, midColorG, midColorB);
	glutSolidCube(1);
	// top color
	glTranslated(0, 1, 0);
	glColor3f(topColorR, topColorG, topColorB);
	glutSolidCube(1);
	glPopMatrix();
}

void drawFootball() {
	glPushMatrix();
	glTranslatef(ball_position_x, ball_position_y, ball_position_z);
	glColor3f(0.9f, 0.9f, 0.9f);
	glutSolidSphere(ball_size, 25, 25);
	glPopMatrix();
}

void drawPlayer() {
	glPushMatrix();
	glTranslatef(player_position_x, player_position_y, player_position_z);
	glRotatef(player_facing_direction * 90, 0, 1, 0);

	// head
	glColor3f(0.9f, 0.75f, 0.67f);
	glutSolidSphere(0.25, 25, 25);
	// left eye
	glPushMatrix();
	glTranslatef(-0.1, 0.1, 0.15);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();
	// right eye
	glPushMatrix();
	glTranslatef(0.1, 0.1, 0.15);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();
	// torso
	glPushMatrix();
	glScalef(1, 1, 0.5);
	glColor3f(0.8f, 0.2f, 0.2f);
	glTranslatef(0, -0.75, 0);
	glutSolidCube(1);
	// right sleeve
	glPushMatrix();
	glColor3f(0.8f, 0.2f, 0.2f);
	glTranslatef(-0.75, 0.25, 0);
	glScalef(-0.5, 0.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	// right arm
	glPushMatrix();
	glColor3f(0.9f, 0.75f, 0.67f);
	glTranslatef(-0.75, -0.25, 0);
	glScalef(-0.5, 0.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	// left sleeve
	glPushMatrix();
	glColor3f(0.8f, 0.2f, 0.2f);
	glTranslatef(0.75, 0.25, 0);
	glScalef(-0.5, 0.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	// left arm
	glPushMatrix();
	glColor3f(0.9f, 0.75f, 0.67f);
	glTranslatef(0.75, -0.25, 0);
	glScalef(-0.5, 0.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	// right short
	glPushMatrix();
	glColor3f(0.9f, 0.9f, 0.9f);
	glTranslatef(-0.25, -0.75, 0);
	glScalef(-0.5, 0.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	// right leg
	glPushMatrix();
	glColor3f(0.9f, 0.75f, 0.67f);
	glTranslatef(-0.25, -1.25, 0);
	glScalef(-0.5, 0.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	// left short
	glPushMatrix();
	glColor3f(0.9f, 0.9f, 0.9f);
	glTranslatef(0.25, -0.75, 0);
	glScalef(-0.5, 0.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	// left leg
	glPushMatrix();
	glColor3f(0.9f, 0.75f, 0.67f);
	glTranslatef(0.25, -1.25, 0);
	glScalef(-0.5, 0.5, 1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void drawNet() {
	glPushMatrix();
	glTranslated(0, net_height, field_depth - net_depth);
	glColor3f(0.7f, 0.7f, 0.7f);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();

	//righ side
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(net_width, 0, 0);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.2, 0.2, net_height, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glRotated(45, 1, 0, 0);
	glTranslated(net_width, 0, 0);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.2, 0.2, net_height + 1, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslated(net_width, -net_height, 0);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.2, 0.2, net_depth, 8, 8);
	glPopMatrix();

	//left side
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(-net_width, 0, 0);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.2, 0.2, net_height, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glRotated(45, 1, 0, 0);
	glTranslated(-net_width, 0, 0);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.2, 0.2, net_height + 1, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-net_width, -net_height, 0);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.2, 0.2, net_depth, 8, 8);
	glPopMatrix();

	//top middle side
	glPushMatrix();
	glTranslated(-net_width, 0, 0);
	glRotated(90, 0, 1, 0);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.2, 0.2, 2 * net_width, 8, 8);
	glPopMatrix();

	//bottom middle side
	glPushMatrix();
	glTranslated(-net_width, -net_height, net_depth - 0.25);
	glRotated(90, 0, 1, 0);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.2, 0.2, 2 * net_width, 8, 8);
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
		player_facing_direction = 1;
		if (player_position_x + player_size >= field_width)
			player_position_x = field_width - player_size;
		break;
	case 'd':
		player_position_x -= player_speed;
		player_facing_direction = 3;
		if (player_position_x - player_size <= -field_width)
			player_position_x = -field_width + player_size;
		break;
	case 'w':
		player_position_z += player_speed;
		player_facing_direction = 0;
		if (player_position_z + player_size >= field_depth)
			player_position_z = field_depth - player_size;
		break;
	case 's':
		player_position_z -= player_speed;
		player_facing_direction = 2;
		if (player_position_z - player_size <= -field_depth)
			player_position_z = -field_depth + player_size;
		break;
		// eye left
	case 'j':
		camera_mode = 3;
		eye_x += camera_speed;
		break;
		// eye right
	case 'l':
		camera_mode = 3;
		eye_x -= camera_speed;
		break;
		// eye forward
	case 'i':
		camera_mode = 3;
		eye_z += camera_speed;
		break;
		// eye backward
	case 'k':
		camera_mode = 3;
		eye_z -= camera_speed;
		break;
		// eye up
	case 'o':
		camera_mode = 3;
		eye_y += camera_speed;
		break;
		// eye down
	case 'u':
		camera_mode = 3;
		eye_y -= camera_speed;
		break;
	}
}

void setCameraPerspective() {
	glLoadIdentity();
	gluPerspective(120.0f, window_width / window_height, 0.1f, 300.0f);
	switch (camera_mode)
	{
		// front view
	case 0:
		eye_x = 0;
		eye_y = 5.1;
		eye_z = -8.1;
		up_x = 0;
		up_y = 1;
		up_z = 0;
		break;
		// side view
	case 1:
		eye_x = 5.1;
		eye_y = 5.1;
		eye_z = 0;
		up_x = 0;
		up_y = 1;
		up_z = 0;
		break;
		// top view
	case 2:
		eye_x = 0;
		eye_y = 15;
		eye_z = -5;
		up_x = 0;
		up_y = 1;
		up_z = 1;
		break;
		// free cam
	case 3:
		break;
	}
	gluLookAt(
		eye_x, eye_y, eye_z,
		player_position_x, player_position_y, player_position_z,
		up_x, up_y, up_z
	);
}

// detects collision using rectanges for hitboxes
// x1,y1 = center of object 1
// size1 = size of object 1
// x2,y2 = center of object 2
// size2 = size of object 2
bool isCollision(int x1, int y1, int size1, int x2, int y2, int size2) {
	pair<int, int> top_right1 = make_pair(x1 + size1, y1 + size1);
	pair<int, int> bottom_left1 = make_pair(x1 - size1, y1 - size1);

	pair<int, int> top_right2 = make_pair(x2 + size2, y2 + size2);
	pair<int, int> bottom_left2 = make_pair(x2 - size2, y2 - size2);

	// If one rectangle is on left side of other
	if (top_right1.first < bottom_left2.first || top_right2.first < bottom_left1.first)
		return false;

	// If one rectangle is above other
	if (top_right1.second < bottom_left2.second || top_right2.second < bottom_left1.second)
		return false;

	return true;
}

void displayCB(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setCameraPerspective();
	// drawAxes(3);
	drawField();
	drawFootball();
	drawPlayer();
	drawNet();
	drawTrophy();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	drawNet();
	glPopMatrix();

	// middle flag
	glPushMatrix();
	glTranslated(0, 0, field_depth);
	drawFlag(
		1, 0, 0,
		1, 1, 1,
		1, 0, 0
	);
	glPopMatrix();

	// left flag
	glPushMatrix();
	glTranslated(field_width, 0, field_depth);
	drawFlag(
		1, 0, 0,
		1, 1, 1,
		0, 1, 0
	);
	glPopMatrix();

	// right flag
	glPushMatrix();
	glTranslated(-field_width, 0, field_depth);
	drawFlag(
		1, 0, 0,
		1, 1, 1,
		0, 0, 1
	);
	glPopMatrix();

	// right bench
	glPushMatrix();
	glTranslated(-field_depth / 2 - 1, 0, 0);
	glRotated(90, 0, 1, 0);
	drawBench();
	glPopMatrix();

	// left bench
	glPushMatrix();
	glTranslated(field_depth / 2 + 1, 0, 0);
	glRotated(-90, 0, 1, 0);
	drawBench();
	glPopMatrix();

	glFlush();
}

void gameLoopCB(int value) {
	if (game_over)
	{
		if (game_won)
			PlaySound(TEXT("won.wav"), NULL, SND_ASYNC);
		else
			PlaySound(TEXT("lost.wav"), NULL, SND_ASYNC);
		return;
	}

	if (isCollision(player_position_x, player_position_z, player_size / 2, ball_position_x, ball_position_z, ball_size))
	{
		ball_speed = 1;
		ball_direction = player_facing_direction;
		PlaySound(TEXT("ball_kicked.wav"), NULL, SND_ASYNC);
	}

	switch (ball_direction)
	{
	case 0:
		ball_position_z += ball_speed;
		if (ball_position_z + ball_size >= field_depth)
		{
			ball_position_z = field_depth - ball_size;
			game_over = true;
			game_won = true;
		}
		break;
	case 1:
		ball_position_x += ball_speed;
		if (ball_position_x + ball_size >= field_width)
		{
			ball_position_x = field_width - ball_size;
			game_over = true;
			game_won = false;
		}
		break;
	case 2:
		ball_position_z -= ball_speed;
		if (ball_position_z - ball_size <= -field_depth)
		{
			ball_position_z = -field_depth + ball_size;
			game_over = true;
			game_won = false;
		}
		break;
	case 3:
		ball_position_x -= ball_speed;
		if (ball_position_x - ball_size <= -field_width)
		{
			ball_position_x = -field_width + ball_size;
			game_over = true;
			game_won = false;
		}
		break;
	}

	glutTimerFunc(delta_time, gameLoopCB, 0);
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("World Cup’22");

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutKeyboardFunc(keyCB);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	PlaySound(TEXT("background_music.wav"), NULL, SND_ASYNC);
	glutDisplayFunc(displayCB);
	glutTimerFunc(delta_time, gameLoopCB, 0);

	glutMainLoop();
}
