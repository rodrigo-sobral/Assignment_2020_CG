#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <fstream>
#include <errno.h>
#include <GL\glew.h>
#include <GL\freeGlut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")

GLint	wScreen = 600, hScreen = 500, loc;
GLfloat angle = 0.0;
float  variavel = 0.5, variavel1 = 0.4;
float  dim = 6;

char filenameV[] = "C://Users//renat//OneDrive//Ambiente de Trabalho//parte2//Vshader_explosao.txt";
char filenameF[] = "C://Users//renat//OneDrive//Ambiente de Trabalho//parte2//Fshader_explosao.txt";

//---------------------------------------------------------- SHADERS variaveis
char* VertexShaderSource;
char* FragmentShaderSource;
GLuint  VertexShader, FragmentShader;
GLuint  ShaderProgram;

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


void InitShader(void) {
	BuiltShader();

	loc = glGetUniformLocation(ShaderProgram, "variavel");
	glUniform1f(loc, variavel);

}


void DeInitShader(void) {
	glDetachShader(ShaderProgram, VertexShader);
	glDetachShader(ShaderProgram, FragmentShader);
	glDeleteShader(ShaderProgram);
}

void Inicializa(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

}

void Desenha(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, wScreen, hScreen);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 15, 35, 0, 0, 0, 0, 1, 0);



	glUniform1f(loc, variavel);

	//-------------------------------------------- Cubo
	angle += 0.3;
	variavel1 = variavel1 + 0.01;
	variavel = 0.9 * variavel1 + variavel * 0.4;
	if (variavel > 10)
		variavel = 1;
	if (variavel1 > 10)
		variavel1 = 1;

	glRotatef(angle, 0, 1, 0);
	glRotatef(angle, 0, 0, 1);
	glColor3f(1, 0, 1);
	glBegin(GL_QUADS);		/* X = -dim */
	glNormal3f(-1, 0, 0);
	glVertex3f(-dim, -dim, dim);
	glVertex3f(-dim, dim, dim);
	glVertex3f(-dim, dim, -dim);
	glVertex3f(-dim, -dim, -dim);
	glEnd();
	glBegin(GL_QUADS);		/* X = dim */
	glNormal3f(1, 0, 0);
	glVertex3f(dim, -dim, dim);
	glVertex3f(dim, -dim, -dim);
	glVertex3f(dim, dim, -dim);
	glVertex3f(dim, dim, dim);
	glEnd();
	glBegin(GL_QUADS);		/* Y = -dim */
	glNormal3f(0, -1, 0);
	glVertex3f(-dim, -dim, -dim);
	glVertex3f(dim, -dim, -dim);
	glVertex3f(dim, -dim, dim);
	glVertex3f(-dim, -dim, dim);
	glEnd();
	glBegin(GL_QUADS);		/* Y = dim */
	glNormal3f(0, 1, 0);
	glVertex3f(-dim, dim, -dim);
	glVertex3f(-dim, dim, dim);
	glVertex3f(dim, dim, dim);
	glVertex3f(dim, dim, -dim);
	glEnd();
	glBegin(GL_QUADS);		/* Z = -dim */
	glNormal3f(0, 0, -1);
	glVertex3f(-dim, dim, -dim);
	glVertex3f(dim, dim, -dim);
	glVertex3f(dim, -dim, -dim);
	glVertex3f(-dim, -dim, -dim);
	glEnd();
	glBegin(GL_QUADS);		/* Z = dim */
	glNormal3f(0, 0, 1);
	glVertex3f(-dim, -dim, dim);
	glVertex3f(dim, -dim, dim);
	glVertex3f(dim, dim, dim);
	glVertex3f(-dim, dim, dim);
	glEnd();

	glutSwapBuffers();
}

void Teclado(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		dim = dim + 0.1;
		glutPostRedisplay();
		break;
	case 'a':
		dim = dim - 0.1;
		glutPostRedisplay();
		break;
		/*
	case 'r':
		cor[0] = 1.0; cor[1] = 0.0; cor[2] = 0.0;
		glutPostRedisplay();
		break;
	case 'g':
		cor[0] = 0.0; cor[1] = 1.0; cor[2] = 0.0;
		glutPostRedisplay();
		break;
	case 'b':
		cor[0] = 0.0; cor[1] = 0.0; cor[2] = 1.0;
		glutPostRedisplay();
		break;
		*/
	case 27:
		exit(0);
		break;
	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 500);
	glutInitWindowPosition(500, 40);
	glutCreateWindow(" Shaders | Animation 2 ");

	Inicializa();
	GLenum err = glewInit();
	InitShader();
	DeInitShader();

	if (glewIsSupported("GL_ARB_vertex_program")) printf(" Core extensions of OpenGL 1.1 to 1.5 are available!\n ");

	glutDisplayFunc(Desenha);
	glutKeyboardFunc(Teclado);
	glutIdleFunc(Desenha);

	glutMainLoop();
	return 1;
}
