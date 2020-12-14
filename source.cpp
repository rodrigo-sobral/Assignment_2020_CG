#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <GL/Glut.h>
#include "RgbImage.h"

#define GREEN    0.0, 1.0, 0.0, 1.0
#define ORANGE   1.0, 0.5, 0.1, 1.0
#define CYAN     0.0, 1.0, 1.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define PI		 3.14159
#define TEXT_SIZE 50
#define PONTUAL 1
#define DIRECIONAL 0

#define DOOR_TEXTURE_PATH "C:\\Users\\sobra\\OneDrive\\UC\\3 Ano\\1 Semestre\\CG\\door_texture.bmp"
#define WALL_TEXTURE_PATH "C:\\Users\\sobra\\OneDrive\\UC\\3 Ano\\1 Semestre\\CG\\wall_texture.bmp"
#define GROUND_TEXTURE_PATH "C:\\Users\\sobra\\OneDrive\\UC\\3 Ano\\1 Semestre\\CG\\ground.bmp"
#define SKYBOX_TEXTURE_PATH "C:\\Users\\sobra\\OneDrive\\UC\\3 Ano\\1 Semestre\\CG\\city.bmp"

void initMaterials(int activated_material);

char materials[18][30] = {
	"Esmerald",  "Jade",  "obsidian",    "Pearl",        "Ruby",
	"Turquoise", "Brass", "Bronze",      "Chrome",       "Copper",
	"Gold",      "Silver","blackPlastic","cyankPlastic", "greenPlastic",
	"redPlastic", "whitePlastic","yellowPlastic" };
int activated_material = 15;
bool transparent = false;

//	RANDOM VARS
GLint msec = 10;
char text[TEXT_SIZE];
short lim_heigh_text = 45;
short showed_info = 1;

//	SCREEN VARS
GLint wScreen = 800, hScreen = 600;

//	OBSERVER VARS
GLfloat  rVisao = 8, aVisao = PI/2, incVisao = 0.05;
GLfloat  maxrVisao = 24;
GLfloat  obsP[] = { rVisao * (GLfloat)(cos(aVisao)), 0.0, rVisao * (GLfloat)(sin(aVisao)) };
GLfloat  obsT[] = { 0, 0, 0 };
GLfloat anguloZ = 95;

//	OBJECT VARS
float height = 5, width = 1.25, lenght = 2.5;
float upper_door_angle = 0, lower_door_tanslat = 0;
float upper_door_y = 0, lower_door_y = 0;
bool auto_upper = false, auto_lower = false;
bool opening_upper = false, opening_lower = false;

static GLfloat vertices[] = { -lenght,  0.0,  lenght,			//0
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
static GLuint up[] = { 8, 9, 10, 11 };

//	TEXTURE VARS
static GLfloat textures_pos[] = {	0, 0,
									1, 0,
									1, 1,
									0, 1,
									0, 0,
									1, 0,
									1, 1,
									0, 1,
									0, 0,
									1, 0,
									1, 1,
									0, 1 };
GLuint   textures[4];
RgbImage imag;

//	MALHA
bool activated_malha = false;
GLfloat malha = 128;

//	AMBIENT LIGHT VARS
GLfloat day_light = 1;
GLfloat light_globalCorAmb[4] = { day_light, day_light,day_light, 1.0 };

//	ARTICIAL LIGHT VARS
GLshort activated_light_id = 0;
GLfloat specular_coeficient = 0;
GLfloat light_components[2][3] = { {1, 1, 1}, {1, 1, 1} };
GLfloat ceiling_light[2] = {1, 1};
GLfloat localPos[2][4] = { { 0, 3.25, 50, PONTUAL }, { 0, 3.25, -50, DIRECIONAL } };
GLfloat localCorAmb[2][4] = { { 1, 1, 1, 1 }, { 1 , 1 , 1 , 1 } };
GLfloat localCorDif[2][4] = {{ light_components[0][0], light_components[0][1], light_components[0][2], 1 }, 
							{ light_components[1][0], light_components[1][1], light_components[1][2], 1 } };
GLfloat localCorEsp[2][4] = {{ light_components[0][0], light_components[0][1], light_components[0][2], 1 },
							{ light_components[1][0], light_components[1][1], light_components[1][2], 1 } };


//	INIT LIGHTS
void initAmbientLight() {
	// Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_globalCorAmb);
}
void initLight(const int light, int id) {
	// Teto
	glLightfv(light, GL_POSITION, localPos[id]);
	glLightfv(light, GL_AMBIENT, localCorAmb[id]);
	glLightfv(light, GL_DIFFUSE, localCorDif[id]);
	glLightfv(light, GL_SPECULAR, localCorEsp[id]);
}

//	INIT TEXTURES
void initOneTexture(const char* texture_to_init, int id) {
	glGenTextures(1, &textures[id]);
	glBindTexture(GL_TEXTURE_2D, textures[id]);
	imag.LoadBmpFile(texture_to_init);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}
void initTextures() {
	int id_texture = 0;
	//	DOOR'S TEXTURE
	initOneTexture(DOOR_TEXTURE_PATH, id_texture++);

	//	WALLS' TEXTURE
	initOneTexture(WALL_TEXTURE_PATH, id_texture++);

	//	GROUND' TEXTURE
	initOneTexture(GROUND_TEXTURE_PATH, id_texture++);

	//	SKYBOX' TEXTURE
	initOneTexture(SKYBOX_TEXTURE_PATH, id_texture++);
}

//	INIT OBJECTS
void initialize(void) {
	glClearColor(WHITE);		//………………………………………………………………………………Apagar
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	
	
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………Vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cores);
	glEnableClientState(GL_COLOR_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textures_pos);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	initAmbientLight();
	initLight(GL_LIGHT0, 0);
	initLight(GL_LIGHT1, 1);
	initTextures();
}

//	INSERTED FUNCTIONS
void buildBlock(GLuint front[], GLuint right[], GLuint up[]) {
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, front);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, right);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, up);
	glRotatef(180, 0, 1, 0);
	glRotatef(-90, 0, 0, 1);
	glTranslatef(-lenght, -lenght, -lenght - width);
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
			if (opening_upper && upper_door_y != width / 2) upper_door_y += 0.01;
			else if (!opening_upper && upper_door_y != 0) upper_door_y -= 0.01;
		}
		if (opening_upper && upper_door_y == width / 2) upper_door_angle += 0.15;
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

//	LIGHT FUNCTIONS
void updateLight() {
	for (short i = 0; i < 3; i++) {
		localCorAmb[activated_light_id][i] = light_components[activated_light_id][i] * ceiling_light[activated_light_id];
		localCorDif[activated_light_id][i] = light_components[activated_light_id][i] * ceiling_light[activated_light_id];
		localCorEsp[activated_light_id][i] = light_components[activated_light_id][i] * ceiling_light[activated_light_id];
	}

	int aux=0;
	if (activated_light_id == 0) aux = GL_LIGHT0;
	else if (activated_light_id == 1) aux = GL_LIGHT1;
	glLightfv(aux, GL_AMBIENT, localCorAmb[activated_light_id]);
	glLightfv(aux, GL_DIFFUSE, localCorDif[activated_light_id]);
	glLightfv(aux, GL_SPECULAR, localCorEsp[activated_light_id]);
}
void iluminate() {
	glLightfv(GL_LIGHT0, GL_POSITION, localPos[0]);
	glLightfv(GL_LIGHT1, GL_POSITION, localPos[1]);
	
	int aux=0;
	if (activated_light_id == 0) aux = GL_LIGHT0;
	else if (activated_light_id == 1) aux = GL_LIGHT1;

	glLightfv(aux, GL_AMBIENT, localCorAmb[activated_light_id]);
	glLightfv(aux, GL_DIFFUSE, localCorDif[activated_light_id]);
	glLightfv(aux, GL_SPECULAR, localCorEsp[activated_light_id]);
	if (ceiling_light[activated_light_id] != 0) glEnable(aux);
	else glDisable(aux);
}

//	DRAWERS
void drawSentence(char* string, GLfloat x, GLfloat y) {
	glColor4f(WHITE);
	glPushMatrix();
	glTranslatef(x, 0, y);
	glRasterPos2f(0, 0);
	while (*string) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
	glPopMatrix();
}
void draw_Informacao() {
	if (showed_info) {
		sprintf_s(text, TEXT_SIZE, "Modo Informação [Space]");
		drawSentence(text, -15, -40);

		if (activated_malha) sprintf_s(text, TEXT_SIZE, "Malha: Ativada - Tamanho: %.0f", malha);
		else sprintf_s(text, TEXT_SIZE, "Malha: Desativada");
		drawSentence(text, -15, -35);

		sprintf_s(text, TEXT_SIZE, "Material Ativo: %s", materials[activated_material]);
		drawSentence(text, -15, -30);

		sprintf_s(text, TEXT_SIZE, "Coeficiente Especularidade: %.1f", specular_coeficient);
		drawSentence(text, -15, -25);

		if (transparent) sprintf_s(text, TEXT_SIZE, "Transparência: Ativada");
		else sprintf_s(text, TEXT_SIZE, "Transparência: Desativada");
		drawSentence(text, -15, -20);

		if (day_light == 0) sprintf_s(text, TEXT_SIZE, "Luz Ambiente: Noite");
		else if (day_light == 1) sprintf_s(text, TEXT_SIZE, "Luz Ambiente: Dia");
		else sprintf_s(text, TEXT_SIZE, "Luz Ambiente: %.1f", day_light);
		drawSentence(text, -15, -15);

		if (activated_light_id == 0) sprintf_s(text, TEXT_SIZE, "Luz Ativada: Frontal");
		else if (activated_light_id == 1) sprintf_s(text, TEXT_SIZE, "Luz Ativada: Traseira");
		drawSentence(text, -15, -10);

		if (ceiling_light[activated_light_id] == 0) sprintf_s(text, TEXT_SIZE, "Luz Teto: Desligada");
		else if (ceiling_light[activated_light_id] == 1) sprintf_s(text, TEXT_SIZE, "Luz Teto: Ligada");
		else sprintf_s(text, TEXT_SIZE, "Luz Teto: %.1f", ceiling_light[activated_light_id]);
		drawSentence(text, -15, -5);

		sprintf_s(text, TEXT_SIZE, "R= %.1f G= %.1f B= %.1f", light_components[activated_light_id][0], light_components[activated_light_id][1], light_components[activated_light_id][2]);
		drawSentence(text, -15, 0);
	} else {
		sprintf_s(text, TEXT_SIZE, "Modo Tutorial [Space]");
		drawSentence(text, -15, -lim_heigh_text);
		sprintf_s(text, TEXT_SIZE, "Manipular Porta Superior [Q/W/E]");
		drawSentence(text, -15, -40);
		sprintf_s(text, TEXT_SIZE, "Manipular Porta Inferior [A/S/D]");
		drawSentence(text, -15, -35);
		sprintf_s(text, TEXT_SIZE, "Zoom [Z/X]");
		drawSentence(text, -15, -30);
		sprintf_s(text, TEXT_SIZE, "Componentes Luz [R/G/B]");
		drawSentence(text, -15, -25);
		sprintf_s(text, TEXT_SIZE, "Manipular Intensidades [T/L]");
		drawSentence(text, -15, -20);
		sprintf_s(text, TEXT_SIZE, "Manipular Malha [J/O/P]");
		drawSentence(text, -15, -15);
		sprintf_s(text, TEXT_SIZE, "Transparência [K]");
		drawSentence(text, -15, -10);
		sprintf_s(text, TEXT_SIZE, "Materiais [M]");
		drawSentence(text, -15, -5);
		sprintf_s(text, TEXT_SIZE, "Coeficiente Especular [I]");
		drawSentence(text, -15,  0);
	}
}
void drawMalha(double yC) {
	GLfloat loop = (yC * 2) / malha;
	glPushMatrix();
		glNormal3f(0, 0, 1);
		glBegin(GL_QUADS);
		for (GLfloat i = 0; i < malha; i++) {
			for (GLfloat j = 0; j < malha; j++) {
				glTexCoord2d(0, 0);
				glVertex3d(-yC - 5 +j * loop, -5.15, -yC + i * loop);
				glTexCoord2d(1, 0);
				glVertex3d(-yC - 5 + j * loop + loop, -5.15, -yC + i * loop);
				glTexCoord2d(1, 1);
				glVertex3d(-yC + 5 + j * loop + loop, -5.15, -yC + i * loop + loop);
				glTexCoord2d(0, 1);
				glVertex3d(-yC + 5 + j * loop, -5.15, -yC + i * loop + loop);
			}
		}
		glEnd();
	glPopMatrix();
}
void drawCarpet() {
	double yC = 10;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	initMaterials(activated_material);
	glMaterialf(GL_FRONT, GL_SHININESS, specular_coeficient);
	if (transparent) { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
	if (activated_malha) drawMalha(yC);
	else {
		glPushMatrix();
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0);
			glVertex3d(-15, -5.15, -yC);
			glTexCoord2d(1, 0);
			glVertex3d(-15, -5.15, yC);
			glTexCoord2d(1, 1);
			glVertex3d(15, -5.15, yC);
			glTexCoord2d(0, 1);
			glVertex3d(15, -5.15, -yC);
			glEnd();
		glPopMatrix();
	}
	if (transparent) glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	initMaterials(3);
}
void drawSkyBox() {
	GLUquadricObj* box = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glPushMatrix();
		glTranslatef(2, 4, 2);
		glRotatef(-90, 1, 0, 0);
		glRotatef(25, 0, 0, 1);
		gluQuadricDrawStyle(box, GLU_FILL);
		gluQuadricNormals(box, GLU_SMOOTH);
		gluQuadricTexture(box, GL_TRUE);
		gluSphere(box, maxrVisao, 100, 100);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void drawScene() {	
	autoOpeningUpdater();
	drawSkyBox();

	//	LEFT AND RIGHT WALLS
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glPushMatrix();
		glScalef(1.5, 2.5, 1);
		glTranslatef(-4.15, -2, -width);
		buildBlock(front, right, up);
		glTranslatef(0, -lenght * 3.33, 0);
		buildBlock(front, right, up);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	//	UPPER WALL
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glPushMatrix();
		glScaled(1, 0.5, 1);
		glTranslatef(0, height * 2, -width);
		buildBlock(front, right, up);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//	UPPER DOOR
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glPushMatrix();
		glTranslatef(-lenght, 0, -width + upper_door_y);
		glRotatef(upper_door_angle, 0, 0, 1);
		glTranslatef(lenght, 0, upper_door_y);
		buildBlock(front, right, up);
		//	UPPER BUTTON
		glPushMatrix();
			initMaterials(activated_material);
			glMaterialf(GL_FRONT, GL_SHININESS, specular_coeficient);
			if (transparent) { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
			if (auto_upper) { glTranslatef(0, 0, 0.85); glScalef(0.1, 0.1, 0.15); glTranslatef(0.5, height * 2, 0); }
			else { glScalef(0.1, 0.1, 0.3); glTranslatef(0.5, height * 2, width + 0.3); }
			buildBlock(front, right, up);
			if (transparent) glDisable(GL_BLEND);
			initMaterials(3);
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//	LOWER DOOR
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glPushMatrix();
		glTranslatef(lower_door_tanslat, -5, -width + lower_door_y);
		buildBlock(front, right, up);
		//	LOWER BUTTON
		glPushMatrix();
			initMaterials(activated_material);
			glMaterialf(GL_FRONT, GL_SHININESS, specular_coeficient);
			if (transparent) { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
			if (auto_lower) { glTranslatef(0, 0, 0.85); glScalef(0.1, 0.1, 0.15); glTranslatef(0.5, height * 2, 0); }
			else { glScalef(0.1, 0.1, 0.3); glTranslatef(0.5, height * 2, width + 0.3); }
			buildBlock(front, right, up);
			if (transparent) glDisable(GL_BLEND);
			initMaterials(3);
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	drawCarpet();

}

//	KEYBOARD
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
			}
			else if (upper_door_angle != 90) upper_door_angle += 3;
		}
		else if (key == 'w' || key == 'W') {
			valuesReseter();
			if (upper_door_angle == 0) {
				if (upper_door_y != 0) upper_door_y -= 0.05;
			}
			else upper_door_angle -= 3;
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
			}
			else if (lower_door_tanslat != -2 * lenght) lower_door_tanslat -= 0.25;
		}
		else if (key == 's' || key == 'S') {
			valuesReseter();
			if (lower_door_tanslat == 0) {
				if (lower_door_y != 0) lower_door_y -= 0.1;
			}
			else lower_door_tanslat += 0.25;
		}
	}

	//	ZOOM IN
	if ((key == 'z' || key == 'Z') && rVisao >= 0) {
		rVisao--;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
	}
	//	ZOOM OUT
	else if ((key == 'x' || key == 'X') && rVisao <= maxrVisao - 5) {
		rVisao++;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
	}

	else if (key == 'l' || key == 'L') {
		day_light += 0.2;
		if (day_light > 1) day_light = 0;
		light_globalCorAmb[0] = day_light;
		light_globalCorAmb[1] = day_light;
		light_globalCorAmb[2] = day_light;
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_globalCorAmb);
	}
	else if (key == 't' || key == 'T') {
		ceiling_light[activated_light_id] += 0.2;
		if (ceiling_light[activated_light_id] > 1) ceiling_light[activated_light_id] = 0;
		updateLight();
	}
	else if (key == 'r' || key == 'R') {
		light_components[activated_light_id][0] += 0.2;
		if (light_components[activated_light_id][0] > 1) light_components[activated_light_id][0] = 0;
		updateLight();
	}
	else if (key == 'g' || key == 'G') {
		light_components[activated_light_id][1] += 0.2;
		if (light_components[activated_light_id][1] > 1) light_components[activated_light_id][1] = 0;
		updateLight();
	}
	else if (key == 'b' || key == 'B') {
		light_components[activated_light_id][2] += 0.2;
		if (light_components[activated_light_id][2] > 1) light_components[activated_light_id][2] = 0;
		updateLight();
	}
	else if (key == '1') activated_light_id = 0;
	else if (key == '2') activated_light_id = 1;
	else if (key == 'm' || key == 'M') activated_material = (activated_material + 1) % 18;
	else if (key == 'i' || key == 'I') { specular_coeficient += 0.2; if (specular_coeficient > 1) specular_coeficient = 0; }
	else if (key == 'k' || key == 'K') transparent = !transparent;
	else if (key == 'p' || key == 'P') { if (malha != 2048) malha *= 2; }
	else if (key == 'o' || key == 'O') { if (malha != 1) malha /= 2; }
	else if (key == 'j' || key == 'J') activated_malha = !activated_malha;

	//	SWITCH INFO
	else if (key == 32) showed_info = (showed_info + 1) % 2;
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

	if (obsP[1] > maxrVisao/2)   obsP[1] = maxrVisao/2;
	if (obsP[1] < -maxrVisao/2)  obsP[1] = -maxrVisao/2;
	obsP[0] = rVisao * cos(aVisao);
	obsP[2] = rVisao * sin(aVisao);

	glutPostRedisplay();
}

//	DEFAULT
void display(void) {
	glClearColor(BLACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	MAP
	glViewport(0, 0, 0.25 * wScreen, 0.25 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -20, lim_heigh_text, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 30, 0, 0, 0, 0, 0, 0, -1);
	iluminate();
	draw_Informacao();
	
	//	FIRST PERSON
	glEnable(GL_LIGHTING);
	glViewport(0, 0,wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(anguloZ, (float)wScreen / hScreen, 0.1, 9999);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);

	//	Objectos
	iluminate();
	drawScene();

	glutSwapBuffers();
}
GLvoid resize(GLsizei width, GLsizei height) {
	wScreen = width;
	hScreen = height;
	glViewport(0, 0, wScreen, hScreen);
	glutPostRedisplay();
}
void Timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

//	MAIN
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 200);
	glutCreateWindow("Futuristic Door");

	initialize();

	glutSpecialFunc(teclasNotAscii);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}