#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <GL/Glut.h>

#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define ORANGE   1.0, 0.5, 0.1, 1.0
#define CYAN     0.0, 1.0, 1.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.3, 0.3, 0.3, 1.0
#define PI		 3.14159

GLint msec = 10;
char texto[30];


GLint wScreen = 800, hScreen = 600;
GLfloat	yC = 100.0;

//------------------------------------------------------------ Observador 
GLfloat  rVisao = 8, aVisao = PI, incVisao = 0.05;
GLfloat  obsP[] = { rVisao * cos(aVisao), 0.0, rVisao * sin(aVisao) };
GLfloat  obsT[] = { obsP[0] - rVisao * cos(aVisao), obsP[1], obsP[2] - rVisao * sin(aVisao) };

float anguloZ = 95;
int   tipoProjeccao = 0;

float height = 5, width = 1.25, lenght = 2.5;
float upper_door_angle = 0, lower_door_tanslat = 0;
float upper_door_y = 0, lower_door_y = 0;
bool auto_upper = false, auto_lower = false;
bool opening_upper = false, opening_lower = false;

static GLfloat vertices[] = {	-lenght,  0.0,  lenght,			//0
								lenght,  0.0,  lenght,			//1
								lenght,  height,  lenght,		//2
								-lenght,  height,  lenght,		//3
								lenght,  0.0,  lenght,			//4
								lenght, 0.0,  width,			//5
								lenght, height,  width,			//6
								lenght,  height,  lenght,		//7
								lenght,  height,  lenght,		//8
								lenght, height,  width,			//9
								-lenght,  height,  width,		//10
								-lenght,  height,  lenght		//11
};

static GLfloat normais[] = { 0.0, 0.0, 1.0,	// front
							 0.0, 0.0, 1.0,
							 0.0, 0.0, 1.0,
							 0.0, 0.0, 1.0,
							 1.0, 0.0, 0.0,	// right
							 1.0, 0.0, 0.0,
							 1.0, 0.0, 0.0,
							 1.0, 0.0, 0.0,
							 0.0, 1.0, 0.0,	// up
							 0.0, 1.0, 0.0,
							 0.0, 1.0, 0.0,
							 0.0, 1.0, 0.0,
};

static GLfloat cores[] = {	1, 1, 1,
							1, 0.5, 1,
							1, 0.5, 0.5,
							0.5, 0.5, 1,
							1, 0.5, 1,
							0.5, 1, 0.5,
							0.5, 0.5, 0.5,
							1, 0.5, 1,
							0.5, 1, 0.5,
							0.5, 0.5, 1,
							0.5, 0.5, 0.5,
							1, 0.5, 0.5 };

static GLuint front[] = { 0, 1, 2, 3 };
static GLuint right[] = { 4, 5, 6, 7 };
static GLuint up[] =	{ 8, 9, 10, 11 };


void initialize(void) {
	glClearColor(BLACK);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	

	glEnable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	glCullFace(GL_BACK);		//………………………………………………………………………………Mostrar so as da frente

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………Vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cores);
	glEnableClientState(GL_COLOR_ARRAY);

	srand(1);
}

void desenhaTexto(char* string, GLfloat x, GLfloat y) {
	glPushMatrix();
	glTranslatef(x, 0, y);
	glRasterPos2f(0, 0);
	while (*string) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
	glPopMatrix();
}


void drawEixos() {
	glColor4f(ORANGE);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();
	glColor4f(CYAN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
	glEnd();
}

void drawObservador_Local() {
	glColor4f(GREEN);
	glPushMatrix();
	glTranslatef(obsP[0], obsP[1], obsP[2]);
	glutSolidCube(1);
	glPopMatrix();
}
void drawObservador_Target() {
	glColor4f(YELLOW);
	glPushMatrix();
	glTranslatef(obsT[0], obsT[1], obsT[2]);
	glutSolidCube(0.8);
	glPopMatrix();
}

void draw_Informacao() {
	if (tipoProjeccao) sprintf_s(texto, 30, "Projecao = Ortogonal");
	else sprintf_s(texto, 30, "Projecao = Perspectiva");
	desenhaTexto(texto, -15, -15);
}

//	====================================================================

void buildBlock(GLuint front[], GLuint right[], GLuint up[]) {
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, front);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, right);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, up);
	glRotatef(180, 0, 1, 0);
	glRotatef(-90, 0, 0, 1);
	glTranslatef(-lenght, -lenght, -lenght-width);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, front);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, right);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, up);
}

void valuesReseter() {
	//	RESET VALUES AND TURN OFF AUTO OPEN/CLOSE
	//	UPPER DOOR
	if (upper_door_angle < 0) upper_door_angle = 0;
	else if (upper_door_angle > 90) { upper_door_angle = 90; auto_upper = false; }
	if (upper_door_y > width / 2) upper_door_y = width / 2;
	else if (upper_door_y < 0) { upper_door_y = 0; auto_upper = false; }

	//	LOWER DOOR
	if (lower_door_tanslat > 0) lower_door_tanslat = 0;
	else if (lower_door_tanslat < -2.0 * lenght) { lower_door_tanslat = -2.0 * lenght; auto_lower = false; }
	if (lower_door_y > width) lower_door_y = width;
	else if (lower_door_y < 0) { lower_door_y = 0; auto_lower = false; }
}

void autoOpeningUpdater() {
	//	UPDATE UPPER DOOR ANGLE
	if (auto_upper) {
		if (upper_door_angle == 0) {
			if (opening_upper && upper_door_y != width/2) upper_door_y += 0.01;
			else if (!opening_upper && upper_door_y != 0) upper_door_y -= 0.01;
		}
		if (opening_upper && upper_door_y==width/2) upper_door_angle += 0.15;
		else if (!opening_upper) upper_door_angle -= 0.15;
	}

	//	UPDATE LOWER DOOR TRANSLACTION
	if (auto_lower) {
		if (lower_door_tanslat == 0) {
			if (opening_lower && lower_door_y != width) lower_door_y += 0.01;
			else if (!opening_lower && lower_door_y != 0) lower_door_y -= 0.01;
		}
		if (opening_lower && lower_door_y == width) lower_door_tanslat -= 0.01;
		else if (!opening_lower) lower_door_tanslat += 0.01;
	}
	valuesReseter();
}

//	====================================================================

void drawScene() {

	autoOpeningUpdater();

	//	LEFT AND RIGHT WALLS
	glPushMatrix();
		glScalef(1.5, 2.5, 1.75);
		glTranslatef(-4.15, -2, -width - 0.5);
		buildBlock(front, right, up);
		glTranslatef(0, -lenght*3.31, 0);
		buildBlock(front, right, up);
	glPopMatrix();

	//	UPPER WALL
	glPushMatrix();
		glScaled(1, 0.5, 1.75);
		glTranslatef(0, height*2, -width - 0.5);
		buildBlock(front, right, up);
	glPopMatrix();

	//	UPPER DOOR
	glPushMatrix();
		glTranslatef(-lenght, 0, -width+upper_door_y);
		glRotatef(upper_door_angle, 0, 0, 1);
		glTranslatef(lenght, 0, upper_door_y);
		buildBlock(front, right, up);
	//	UPPER BUTTON
		if (auto_upper) { glTranslatef(0, 0, 0.85); glScalef(0.1, 0.1, 0.15); glTranslatef(0.5, height * 2, 0); }
		else { glScalef(0.1, 0.1, 0.3); glTranslatef(0.5, height * 2, width + 0.3); }
		buildBlock(front, right, up);
	glPopMatrix();

	//	LOWER DOOR
	glPushMatrix();
		glTranslatef(lower_door_tanslat, -5, -width+lower_door_y);
		buildBlock(front, right, up);
	//	LOWER BUTTON
		if (auto_lower) { glTranslatef(0, 0, 0.85); glScalef(0.1, 0.1, 0.15); glTranslatef(0.5, height * 2, 0); }
		else { glScalef(0.1, 0.1, 0.3); glTranslatef(0.5, height * 2, width + 0.3); }
		buildBlock(front, right, up);
	glPopMatrix();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//================================================================= Viewport 1 MAPA
	// projecao ortogonal
	glViewport(0, 0, 0.25 * wScreen, 0.25 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 30, 0, 0, 0, 0, 0, 0, -1);
	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawObservador_Local();
	drawObservador_Target();
	drawScene();
	draw_Informacao();
	
	//================================================================= Viewport 2
	glViewport(0.25 * wScreen, 0.25 * hScreen, 0.75 * wScreen, 0.75 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (tipoProjeccao == 0) gluPerspective(anguloZ, (float)wScreen / hScreen, 0.1, 9999);
	if (tipoProjeccao == 1) glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);

	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawScene();

	glutSwapBuffers();
}

void Timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

void keyboard(unsigned char key, int x, int y) {
	if (!auto_upper) {
		//	AUTO OPEN/CLOSE UPPER DOOR
		if (key == 'e' || key == 'E') {
			if (upper_door_angle == 90) { opening_upper = false; auto_upper = true; } 
			else if (upper_door_angle == 0) { opening_upper = true; auto_upper = true; }
		} 

		//	MANUAL OPEN/CLOSE UPPER DOOR
		else if (key == 'q' || key == 'Q') {
			valuesReseter();
			if (upper_door_angle == 0) {
				if (upper_door_y != width / 2) upper_door_y += 0.05;
				else upper_door_angle += 3;
			} else if (upper_door_angle != 90) upper_door_angle += 3;
		}
		else if (key == 'w' || key == 'W') {
			valuesReseter();
			if (upper_door_angle == 0) {
				if (upper_door_y != 0) upper_door_y -= 0.05;
			} else upper_door_angle -= 3;
		}
	}

	if (!auto_lower) {
		//	AUTO OPEN/CLOSE LOWER DOOR
		if (key == 'd' || key == 'D') {
			if (lower_door_tanslat == -2 * lenght) { opening_lower = false; auto_lower = true; } 
			else if (lower_door_tanslat == 0) { opening_lower = true; auto_lower = true; }
		}

		//	MANUAL OPEN/CLOSE LOWER DOOR
		else if (key == 'a' || key == 'A') {
			valuesReseter();
			if (lower_door_tanslat == 0) {
				if (lower_door_y != width) lower_door_y += 0.1;
				else lower_door_tanslat -= 0.25;
			} else if (lower_door_tanslat != -2 * lenght) lower_door_tanslat -= 0.25;
		}
		else if (key == 's' || key == 'S') {
			valuesReseter();
			if (lower_door_tanslat == 0) {
				if (lower_door_y != 0) lower_door_y -= 0.1;
			} else lower_door_tanslat += 0.25;
		}
	}

	//	ZOOM IN
	if (key == 'z' || key == 'Z') {
		rVisao--;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);			
	} 
	//	ZOOM OUT
	else if (key == 'x' || key == 'X') {
		rVisao++;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);		
	} 
	//	PERSPECTIVE
	else if (key == 'p' || key == 'P') tipoProjeccao = (tipoProjeccao + 1) % 2;
	//	QUIT
	else if (key == 27) exit(0);
	glutPostRedisplay();
}

void teclasNotAscii(int key, int x, int y) {
	//------------------------------- olhar para a origem
	obsT[0] = 0; obsT[1] = 0; obsT[2] = 0;

	if (key == GLUT_KEY_UP)    obsP[1] += 0.5;
	if (key == GLUT_KEY_DOWN)  obsP[1] -= 0.5;
	if (key == GLUT_KEY_LEFT)  aVisao += 0.1;
	if (key == GLUT_KEY_RIGHT) aVisao -= 0.1;

	if (obsP[1] > yC)   obsP[1] = yC;
	if (obsP[1] < -yC)  obsP[1] = -yC;
	obsP[0] = rVisao * cos(aVisao);
	obsP[2] = rVisao * sin(aVisao);

	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 200);
	glutCreateWindow("Futuristic Door");

	initialize();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}
