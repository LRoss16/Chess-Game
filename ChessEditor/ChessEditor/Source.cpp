#include <string>
#include <Windows.h>
#include <stdlib.h>
#include "glut.h"
#include <iostream>
#include "GameBoard.h"
using namespace std;

int gb_width, gb_height;
int old_x = 0, old_y = 0;
int drag = 0, context = 0;
float c;
GameBoard myGame;

void coordToBlock(int x, int y, int& nx, int& ny) {
	x = x;
	y = gb_height - y;
	if (c < x && x < 9 * c && c < y && y < 9 * c) {
		nx = x / c; ny = y / c;
	}
	else if (c < x && x < 3 * c && 9 * c + 5 < y && y < 10 * c - 5) {
		nx = 0; ny = 1;	//create;
	}
	else if (4 * c < x && x < 6 * c && 9 * c + 5 < y && y < 10 * c - 5) {
		nx = 1; ny = 0;	//delete;
	}
	else if (7 * c < x && x < 9 * c && 9 * c + 5 < y && y < 10 * c - 5) {
		nx = -1; ny = 0;	//quit;
	}
	else {
		nx = ny = 0;	//blank area
	}
	return;
}

enum MENU_TYPE
{
	MENU_ROTATE,
	MENU_BACK,
	MENU_SHOW_HISTORY,
	MENU_EXPORT
};

// Assign a default value
MENU_TYPE show = MENU_BACK;
void menu(int);

GLubyte space[] =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };//for spacebar,draw nothing
GLubyte dot[] =
{ 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };//draw dot
GLubyte letters[][13] = {
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},//draw A
{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},//draw B
{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},//draw C
{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},//draw D
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},//draw E
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},//draw F
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},//draw G
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},//draw H
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},//draw I
{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},//draw J
{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},//draw K
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},//draw L
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},//draw M
{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},//draw N
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},//draw O
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},//draw P
{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},//draw Q
{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},//draw R
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},//draw S
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},//draw T
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},//draw U
{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},//draw V
{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},//draw W
{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},//draw X
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},//draw Y
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff} //draw Z
};
GLuint fontOffset;
void makeRasterFont(void)
{
	GLuint i, j;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	fontOffset = glGenLists(128);
	for (i = 0, j = 'A'; i < 26; i++, j++) {
		glNewList(fontOffset + j, GL_COMPILE);
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, letters[i]);
		glEndList();
	}
	glNewList(fontOffset + ' ', GL_COMPILE);
	glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, space);
	glEndList();

	glNewList(fontOffset + '.', GL_COMPILE);
	glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, dot);
	glEndList();
}
void printString(char *s)
{
	glPushAttrib(GL_LIST_BIT);
	glListBase(fontOffset);
	glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *)s);
	glPopAttrib();
}

void display() {
	int x, y; c = min(gb_width, gb_height) / 10.0;
	glClear(GL_COLOR_BUFFER_BIT);
	for (x = 1; x <= 8; x++) {
		for (y = 1; y <= 8; y++) {
			if ((x + y) % 2 == 0)glColor3f(182 / 255.0, 130 / 255.0, 98 / 255.0);
			else glColor3f(241 / 255.0, 218 / 255.0, 179 / 255.0);

			glBegin(GL_QUADS);
			glVertex2f(c + c * x, c + c * y);
			glVertex2f(c*x, c + c * y);
			glVertex2f(c*x, c*y);
			glVertex2f(c + c * x, c*y);
			glEnd();

			bool isBlack = false;
			char pieceName = myGame.getPiece(x, y, isBlack);
			if (pieceName != 'E') {

				if (!isBlack)glColor3f(1.0, 1.0, 1.0);
				else glColor3f(0.0, 0.0, 0.0);

				float delta_theta = 0.1, r;
				double PI = acos(-1);

				glBegin(GL_POLYGON);
				for (float angle = 0; angle < 2 * PI + delta_theta; angle += delta_theta) {
					r = c / 3;
					glVertex2f(c*x + c / 2 + r * cos(angle), c*y + c / 2 + r * sin(angle));
				}
				glEnd();

				if (isBlack)glColor3f(1.0, 1.0, 1.0);
				else glColor3f(0.0, 0.0, 0.0);
				glRasterPos2i(c*x + c / 2 - 3, c*y + c / 2 - 3);
				char arr[2];
				arr[0] = toupper(pieceName); arr[1] = '\0';
				printString(arr);
			}

		}
	}


	glColor4f(0.2, 0.3, 0.6, 0.3);
	glBegin(GL_QUADS);
	x = 2; y = 9;
	glVertex2f(c + c * x, c + c * y - 5);
	glVertex2f(c, c + c * y - 5);
	glVertex2f(c, c*y + 5);
	glVertex2f(c + c * x, c*y + 5);
	glEnd();

	glBegin(GL_QUADS);
	x = 5; y = 9;
	glVertex2f(c + c * x, c + c * y - 5);
	glVertex2f(c * 4, c + c * y - 5);
	glVertex2f(c * 4, c*y + 5);
	glVertex2f(c + c * x, c*y + 5);
	glEnd();

	glBegin(GL_QUADS);
	x = 8; y = 9;
	glVertex2f(c + c * x, c + c * y - 5);
	glVertex2f(c * 7, c + c * y - 5);
	glVertex2f(c * 7, c*y + 5);
	glVertex2f(c + c * x, c*y + 5);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i(c + c / 2 + 3, c*y + c / 2);
	char arr[10];
	strcpy_s(arr, "CREATE");
	printString(arr);


	glRasterPos2i(c * 4 + c / 2 + 3, c*y + c / 2);
	strcpy_s(arr, "DELETE");
	printString(arr);

	glRasterPos2i(c * 7 + c / 2 + 3, c*y + c / 2);
	strcpy_s(arr, "QUIT");
	printString(arr);

	glFlush();
}

void reshape(int w, int h) {
	gb_width = w; gb_height = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void mouse_func(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			coordToBlock(x, y, x, y);
			if (x && y) {
				old_x = x;
				old_y = y;
				drag = 1;
			}
			else if (x == 0 && y == 1) {
				myGame.init();	//create
				glutPostRedisplay();
			}
			else if (x == 1 && y == 0) {
				myGame.removePiece(old_x, old_y);	//delete
				glutPostRedisplay();
			}
			else if (x == -1) {
				//exit
				exit(0);
			}
		}
		else {
			coordToBlock(x, y, x, y);
			if (x && y && drag) {
				//release the piece here
				myGame.movePiece(old_x, old_y, x, y);
				glutPostRedisplay();

				old_x = x;
				old_y = y;
			}
			drag = 0;
		}
		return;
	}
}

void motion_func(int x, int y) {
	if (drag) {
		int dx = old_x - x;
		int dy = old_y - y;
		/* do something with dx and dy */
	}
}

int main(int argc, char** argv) {
	gb_width = GetSystemMetrics(SM_CXSCREEN);
	gb_height = GetSystemMetrics(SM_CYSCREEN) - 40;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowSize(gb_width, gb_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Lewis Ross | 1501830");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);
	makeRasterFont();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutCreateMenu(menu);

	// Add menu items
	glutAddMenuEntry("Undo", MENU_BACK);
	glutAddMenuEntry("Switch Players", MENU_ROTATE);
	glutAddMenuEntry("Show History", MENU_SHOW_HISTORY);
	glutAddMenuEntry("Export", MENU_EXPORT);

	// Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}

void menu(int item)
{
	switch (item)
	{
	case MENU_ROTATE:
		myGame.rotatePlayers();
		break;
	case MENU_BACK:
		myGame.undoMove();
		break;
	case MENU_SHOW_HISTORY:
		myGame.printHistory(cout);
		break;
	case MENU_EXPORT:
		myGame.exportData("");
		break;
	}

	glutPostRedisplay();

	return;
}