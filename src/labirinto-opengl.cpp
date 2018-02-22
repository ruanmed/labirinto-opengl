//============================================================================
// Name        : labirinto-opengl.cpp
// Author      : Ricardo Valério Teixeira de Medeiros Silva e Ruan de Medeiros Bahia
// Version     :
// Copyright   : 
// Description : Jogo de labirinto feito com OpenGL
//  Universidade Federal do Vale do São Francisco
// 	Professor Jorge Luis Cavalcanti
// 	Trabalho de Computação Gráfica
// 		para compilar no linux: g++ labirinto-opengl.c -lglut -lGL -lGLU -lm
//============================================================================
/*
	O objetivo é passar um objeto (circulo, triângulo ou retângulo) sem tocar nas paredes
	do labirinto. Cada vez que ocorrer um toque na parede, o objeto volta para o início e o
	jogador perde uma “vida”. O jogo acaba quando ele atravessar sem tocar (vitória) ou
	bate 4 vezes em uma parede (derrota).
	Outras características:
	1 - O objeto deve estar preenchido com uma cor de escolha da dupla;
	2 - A largura do labirinto deve ser 1,5 vezes (aproximadamente) o tamanho do objeto;
	3 - Utilizar as setas do teclado para movimento do objeto no labirinto.
	4 – O botão esquerdo do mouse deve ser usado para mudar as cores da janela, do
	labirinto e do objeto.
	5 - Outros requisitos para o funcionamento do jogo (dimensões, cores, formato do
	labirinto etc..) podem ser definidos pela dupla.
 */

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <effolkronium/random.hpp>
#include <vector>
#ifdef _WIN32
    #include <windows.h>
#endif

#define CIRCLE_RADIUS 10
#define ORTHO_WIDTH 1920
#define ORTHO_HEIGTH 1080
#define WINDOW_PROPORTION 0.5
#define WINDOW_WIDTH (ORTHO_WIDTH*WINDOW_PROPORTION)
#define WINDOW_HEIGTH (ORTHO_HEIGTH*WINDOW_PROPORTION)
#define MAZE_STEP CIRCLE_RADIUS*2
#define MESH_WIDTH_PARTS 128
#define MESH_HEIGTH_PARTS 72
#define MESH_WIDTH_PARTS_OPENNING_PROBABILITY 0.2
#define MESH_HEIGHT_PARTS_OPENNING_PROBABILITY 0.5

using Random = effolkronium::random_static;
typedef struct
{
	char side;
	char top;
}mesh;

mesh maze[MESH_WIDTH_PARTS][MESH_HEIGTH_PARTS];
int x,y;
int xc = 0, yc = 0,raio = CIRCLE_RADIUS;
double corCircR,corCircG,corCircB;
double corFundR,corFundG,corFundB;
double corLabiR,corLabiG,corLabiB;


void resetMazeMesh()
{
	for(int l=0;l<MESH_WIDTH_PARTS;l++)
		for(int c=0;c<MESH_HEIGTH_PARTS;c++)
		{
			maze[l][c].side = 0;
			maze[l][c].top = 0;
		}
}
bool isOnMap(int x,int y)
{
	return (-(ORTHO_WIDTH/2)*0.9 <= x && x <= (ORTHO_WIDTH/2)*0.9) && (-(ORTHO_HEIGTH/2)*0.9 <= y && y <= (ORTHO_HEIGTH/2)*0.9);
}
void generateRandomMaze(int initialCenterX,int initialCenterY)
{
	resetMazeMesh();


	for(int l=0;l<MESH_WIDTH_PARTS;l++)
			for(int c=0;c<MESH_HEIGTH_PARTS;c++)
			{
				if(auto val = Random::get<bool>(MESH_WIDTH_PARTS_OPENNING_PROBABILITY))
					maze[l][c].side = 1;
				if(auto val = Random::get<bool>(MESH_HEIGHT_PARTS_OPENNING_PROBABILITY))
					maze[l][c].top = 1;
			}
}
void desenhaLabirinto(void)
{

}
void desenhaCirculo(void)//Infelizmente esta função de Call Back não pode ter parametros ou eu não sei como...
{
	double theta;

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(corCircR, corCircG, corCircB);

	glPointSize(2.0f); // aumenta o tamanho dos pontos
    	glBegin(GL_POINTS);

   while(raio>0) {

		for(theta = 0; theta <0.8;theta+=0.01)
		{

			x = (int)(raio*cos(theta));
			y = (int)(raio*sin(theta));

			glVertex2f(xc+(x),yc+(y));
			glVertex2f(xc+(-x),yc+(y));
			glVertex2f(xc+(-x),yc+(-y));
			glVertex2f(xc+(x),yc+(-y));
			glVertex2f(xc+(y),yc+(x));
			glVertex2f(xc+(-y),yc+(x));
			glVertex2f(xc+(-y),yc+(-x));
			glVertex2f(xc+(y),yc+(-x));

		}
		raio -= 0.1;
    }
    raio = CIRCLE_RADIUS;

    	glEnd();
	glutSwapBuffers();
}

//==== The Mouse Clicks Function =======================================//
void myMouseFunc(int button, int state, int x, int y){


}
//======================================================================//
void myMotionFunc(int x, int y){

	corCircR = Random::get(0.0,1.0);
	corCircG = Random::get(0.0,1.0);
	corCircB = Random::get(0.0,1.0);
	printf("m(%d, %d) c(%d, %d)\n", x, y, xc, yc);

}

//======================================================================//
void myKeyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
		case GLUT_KEY_LEFT:
			xc--;
			break;
		case GLUT_KEY_UP:
			yc++;
			break;
		case GLUT_KEY_RIGHT:
			xc++;
			break;
		case GLUT_KEY_DOWN:
			yc--;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}
//======================================================================//
void mySpecialFunc(int key, int x, int y){
	switch (key) {
		case GLUT_KEY_LEFT:
			xc -= MAZE_STEP;
			break;
		case GLUT_KEY_UP:
			yc += MAZE_STEP;
			break;
		case GLUT_KEY_RIGHT:
			xc += MAZE_STEP;
			break;
		case GLUT_KEY_DOWN:
			yc -= MAZE_STEP;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}
//======================================================================//
void myDisplayFunc(){
	glClearColor(corFundR, corFundG, corFundB, 0.0f);
	desenhaCirculo();
}
//======================================================================//
void myReshapeFunc(int w, int h){


}

//======================================================================//

// Inicializa parâmetros de rendering
void Inicializa (void)
{
	corCircR = corCircG = corCircB = 1;
	corFundR = corFundG = corFundB = fabs(1-corCircR);

	glClearColor(corFundR, corFundG, corFundB, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-(ORTHO_WIDTH/2),(ORTHO_WIDTH/2),-(ORTHO_HEIGTH/2),(ORTHO_HEIGTH/2));
	glMatrixMode(GL_MODELVIEW);
}

// Programa principal

int main(int argc, char** argv)
{
	char str[50];
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
 	glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGTH);
	glutInitWindowPosition(10,10);
	glutCreateWindow(str);

	glutDisplayFunc 	( myDisplayFunc	);
	//glutMouseFunc   	( myMouseFunc   );
	glutMotionFunc  	( myMotionFunc  );
	glutKeyboardFunc	( myKeyboardFunc);
	glutSpecialFunc		( mySpecialFunc	);
	//glutReshapeFunc ( myReshapeFunc  );

	Inicializa();
	glutMainLoop();

	return 0;
}
