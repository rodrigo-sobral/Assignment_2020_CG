#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <fstream>
#include <errno.h>
#include <GL\glew.h>
#include <GL\freeGlut.h>
#include "../Libraries/RgbImage.h"

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")

//	PATHS
char filenameV[] = "C://Users//sobra//OneDrive//Documentos//GitHub//Project_CG//Meta 3//Vshader.txt";
char filenameF[] = "C://Users//sobra//OneDrive//Documentos//GitHub//Project_CG//Meta 3//Fshader.txt";
char texture_path[] = "C://Users//sobra//OneDrive//Documentos//GitHub//Project_CG//Meta 3//steel.bmp";

//	CUBE VARS
int wScreen = 600, hScreen = 500, uniTime, uniOp, uniDir, uniCor, uniScale;
float cor[] = { 0, 0, 1, 0 }, Direcao[] = { 1, 0, 0 };
float opcao = -45, dim = 3, angle = 0, scale = 0.1, increment = 0.03;
bool stop = false, transparent = false;
//	TEXTURE VARS
RgbImage imag;
GLuint texture;
int textLoc;

//	SHADER VARS
char* VertexShaderSource;
char* FragmentShaderSource;
GLuint  VertexShader, FragmentShader;
GLuint  ShaderProgram;


//	STANDART FUNCTIONS
char* readShaderFile(char* FileName) {
	char* DATA = NULL;
	int   flength = 0;
	FILE* filepoint;
	errno_t err;

	err = fopen_s(&filepoint, FileName, "r");
	if (!err) {
		fseek(filepoint, 0, SEEK_END);
		flength = ftell(filepoint);
		rewind(filepoint);

		DATA = (char*)malloc(sizeof(char) * (flength + 1));
		flength = fread(DATA, sizeof(char), flength, filepoint);

		DATA[flength] = '\0';
		fclose(filepoint);
		return DATA;
	}
	else printf(" --------  Error while reading  %s ", FileName);
}
void BuiltShader(void) {
	GLEW_ARB_vertex_shader;
	GLEW_ARB_fragment_shader;

	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	VertexShaderSource = readShaderFile(filenameV);
	FragmentShaderSource = readShaderFile(filenameF);

	const char* VS = VertexShaderSource;
	const char* FS = FragmentShaderSource;
	glShaderSource(VertexShader, 1, &VS, NULL);
	glShaderSource(FragmentShader, 1, &FS, NULL);
	free(VertexShaderSource);
	free(FragmentShaderSource);

	glCompileShaderARB(VertexShader);
	glCompileShaderARB(FragmentShader);

	ShaderProgram = glCreateProgramObjectARB();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	glUseProgramObjectARB(ShaderProgram);
}
void DeInitShader(void) {
	glDetachShader(ShaderProgram, VertexShader);
	glDetachShader(ShaderProgram, FragmentShader);
	glDeleteShader(ShaderProgram);
}
void initOneTexture(void) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	imag.LoadBmpFile(texture_path);
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
GLvoid resize(GLsizei width, GLsizei height) {
	wScreen = width;
	hScreen = height;
	glViewport(0, 0, wScreen, hScreen);
	glutPostRedisplay();
}

void InitShader(void) {
	BuiltShader();
	textLoc = glGetUniformLocation(ShaderProgram, "textLoc");
	uniDir = glGetUniformLocation(ShaderProgram, "Direcao");
	uniCor = glGetUniformLocation(ShaderProgram, "cor");
	uniOp = glGetUniformLocation(ShaderProgram, "opcao");
	uniScale = glGetUniformLocation(ShaderProgram, "scale");

	glUniform1i(textLoc, 0);
	glUniform3fv(uniDir, 1, Direcao);
	glUniform4fv(uniCor, 1, cor);
	glUniform1f(uniOp, opcao);
	glUniform1f(uniScale, increment);
}
void initialize(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	initOneTexture();
}

void drawCube() {
	glBegin(GL_QUADS);		/* X = -dim */
		glNormal3f(-1, 0, 0);
		glTexCoord2d(0, 0);
		glVertex3f(-dim, -dim, dim);
		glTexCoord2d(0, 1);
		glVertex3f(-dim, dim, dim);
		glTexCoord2d(1, 0);
		glVertex3f(-dim, dim, -dim);
		glTexCoord2d(1, 1);
		glVertex3f(-dim, -dim, -dim);
	glEnd();
	glBegin(GL_QUADS);		/* X = dim */
		glNormal3f(1, 0, 0);
		glTexCoord2d(0, 0);
		glVertex3f(dim, -dim, dim);
		glTexCoord2d(0, 1);
		glVertex3f(dim, -dim, -dim);
		glTexCoord2d(1, 0);
		glVertex3f(dim, dim, -dim);
		glTexCoord2d(1, 1);
		glVertex3f(dim, dim, dim);
	glEnd();
	glBegin(GL_QUADS);		/* Y = -dim */
		glNormal3f(0, -1, 0);
		glTexCoord2d(0, 0);
		glVertex3f(-dim, -dim, -dim);
		glTexCoord2d(0, 1);
		glVertex3f(dim, -dim, -dim);
		glTexCoord2d(1, 0);
		glVertex3f(dim, -dim, dim);
		glTexCoord2d(1, 1);
		glVertex3f(-dim, -dim, dim);
	glEnd();
	glBegin(GL_QUADS);		/* Y = dim */
		glNormal3f(0, 1, 0);
		glTexCoord2d(0, 0);
		glVertex3f(-dim, dim, -dim);
		glTexCoord2d(0, 1);
		glVertex3f(-dim, dim, dim);
		glTexCoord2d(1, 0);
		glVertex3f(dim, dim, dim);
		glTexCoord2d(1, 1);
		glVertex3f(dim, dim, -dim);
	glEnd();
	glBegin(GL_QUADS);		/* Z = -dim */
		glNormal3f(0, 0, -1);
		glTexCoord2d(0, 0);
		glVertex3f(-dim, dim, -dim);
		glTexCoord2d(0, 1);
		glVertex3f(dim, dim, -dim);
		glTexCoord2d(1, 0);
		glVertex3f(dim, -dim, -dim);
		glTexCoord2d(1, 1);
		glVertex3f(-dim, -dim, -dim);
	glEnd();
	glBegin(GL_QUADS);		/* Z = dim */
		glNormal3f(0, 0, 1);
		glTexCoord2d(0, 0);
		glVertex3f(-dim, -dim, dim);
		glTexCoord2d(0, 1);
		glVertex3f(dim, -dim, dim);
		glTexCoord2d(1, 0);
		glVertex3f(dim, dim, dim);
		glTexCoord2d(1, 1);
		glVertex3f(-dim, dim, dim);
	glEnd();
}
void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, wScreen, hScreen);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 15, 35, 0, 0, 0, 0, 1, 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textLoc, 0);

	glUniform1f(uniOp, opcao);
	glUniform3fv(uniDir, 1, Direcao);
	glUniform4fv(uniCor, 1, cor);
	glUniform1f(uniScale, scale);

	Direcao[0] = cos(3.14 * opcao / 180.0);
	Direcao[1] = cos(3.14 * opcao / 180.0);
	Direcao[2] = sin(3.14 * opcao / 180.0);
	
	if (transparent) { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
	if (!stop) {
		angle = angle + 0.7;
		scale += increment;
		if (scale > 1.5 || scale < 0.1) increment = -increment;
	}
	if (cor[3] == 1) {
		opcao += 1;
		if (opcao == 5000) opcao = 0;
	} else {
		opcao += 5;
		if (opcao == 5000) opcao = 0;
	}
	glRotatef(angle, 0, 1, 0);
	glRotatef(45, 0, 0, 1);
	glRotatef(45, 1, 0, 0);
	drawCube();	
	//glutSolidTeapot(5);
	if (transparent) glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)	 {
	if (key == 'r' || key == 'R') {
		if (cor[0] == 1) cor[0] = 0;
		else cor[0] += 0.2;
	}
	else if (key == 'g' || key == 'G') {
		if (cor[1] == 1) cor[1] = 0;
		else cor[1] += 0.2;
	}
	else if (key == 'b' || key == 'B') {
		if (cor[2] == 1) cor[2] = 0;
		else cor[2] += 0.2;
	}
	else if (key == '1') cor[3] = 0;
	else if (key == '2') cor[3] = 1;
	else if (key == 's' || key == 'S') stop = !stop;
	else if (key == 'k' || key == 'K') transparent = !transparent;
	else if (key == 27) exit(0);
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 500);
	glutInitWindowPosition(500, 40);
	glutCreateWindow("WINDOWS 2000");

	initialize();
	GLenum err = glewInit();
	InitShader();
	DeInitShader();

	//if (glewIsSupported("GL_ARB_vertex_program")) printf(" Core extensions of OpenGL 1.1 to 1.5 are available!\n ");
	
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(draw);

	glutMainLoop();
	return 1;
}
