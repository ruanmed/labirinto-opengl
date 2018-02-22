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
#include <conversorSR.hpp>
#include <vector>
#ifdef _WIN32
    #include <windows.h>
#endif

#define CIRCLE_RADIUS 15
#define CIRCLE_POINT_SIZE 2.0f
#define CIRCLE_CENTER_SPEED (1/4.0)
#define CIRCLE_CENTER_DISPLACEMENT CIRCLE_CENTER_SPEED*MAZE_STEP
#define ORTHO_WIDTH 1920
#define ORTHO_HEIGTH 1080
#define WINDOW_PROPORTION 0.5
#define WINDOW_WIDTH (ORTHO_WIDTH*WINDOW_PROPORTION)
#define WINDOW_HEIGTH (ORTHO_HEIGTH*WINDOW_PROPORTION)
#define MAZE_STEP (CIRCLE_RADIUS*6)
#define MAZE_LINE_SIZE 6.0f
#define MESH_WIDTH_PARTS ORTHO_WIDTH/MAZE_STEP
#define MESH_HEIGTH_PARTS ORTHO_HEIGTH/MAZE_STEP
#define MESH_WIDTH_PARTS_OPENNING_PROBABILITY 0.5
#define MESH_HEIGHT_PARTS_OPENNING_PROBABILITY 0.7


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
//  SISTEMA = {Xmin,Xmax,Ymin,Ymax]
int SRU[4] = {-(ORTHO_WIDTH/2),(ORTHO_WIDTH/2),-(ORTHO_HEIGTH/2),(ORTHO_HEIGTH/2)};
int SRD[4] = {0,0,0,0};

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
void generateRandomMaze()
{
	resetMazeMesh();


	for(int l=0;l<MESH_WIDTH_PARTS;l++)
			for(int c=0;c<MESH_HEIGTH_PARTS;c++)
			{
				if(auto val = Random::get<bool>(MESH_WIDTH_PARTS_OPENNING_PROBABILITY))
					maze[l][c].top = 1;
				if(auto val = Random::get<bool>(MESH_HEIGHT_PARTS_OPENNING_PROBABILITY))
					maze[l][c].side = 1;
			}
}
void desenhaLabirinto(void)
{
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(corLabiR, corLabiG, corLabiB);

	glLineWidth(MAZE_LINE_SIZE);
	glBegin(GL_LINES);

		for(int l=0;l<MESH_WIDTH_PARTS;l++)
				for(int c=0;c<MESH_HEIGTH_PARTS;c++)
				{
					if(!maze[l][c].top)
					{
						glVertex2f(-(ORTHO_WIDTH/2)+l*MAZE_STEP,-(ORTHO_HEIGTH/2)+c*MAZE_STEP);
						glVertex2f(-(ORTHO_WIDTH/2)+(l+1)*MAZE_STEP,-(ORTHO_HEIGTH/2)+c*MAZE_STEP);
					}
					if(!maze[l][c].side)
					{
						glVertex2f(-(ORTHO_WIDTH/2)+l*MAZE_STEP,-(ORTHO_HEIGTH/2)+c*MAZE_STEP);
						glVertex2f(-(ORTHO_WIDTH/2)+l*MAZE_STEP,-(ORTHO_HEIGTH/2)+(c+1)*MAZE_STEP);
						//printf("%d %d\n", c*MAZE_STEP, l*MAZE_STEP);
					}

				}

	glEnd();
	//glutSwapBuffers();
	//glutPostRedisplay();
}
void desenhaCirculo(void)//Infelizmente esta função de Call Back não pode ter parametros ou eu não sei como...
{
	double theta;
	//glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(corCircR, corCircG, corCircB);

	glPointSize(CIRCLE_POINT_SIZE); // aumenta o tamanho dos pontos
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
    //glutSwapBuffers();
}

//==== The Mouse Clicks Function =======================================//
void myMouseFunc(int button, int state, int x, int y){
	//y^2 + x^2 = r^2
	int xSRU = getXSRU(SRU, SRD,x);
	int ySRU = getYSRU(SRU, SRD,y);
	int paramCirculo = (sqrt((xSRU-xc)*(xSRU-xc)+(ySRU-yc)*(ySRU-yc)));

	GLubyte *data = (GLubyte *) malloc( 3 * 1 * 1);
	if( data ) {
	    glReadPixels(x, glutGet( GLUT_WINDOW_HEIGHT ) - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	switch(button) {
		case GLUT_LEFT_BUTTON:
			if (paramCirculo <= raio) {	//	Identifica se a região do clique foi dentro do círculo
					corCircR = Random::get(0.0,1.0);
					corCircG = Random::get(0.0,1.0);
					corCircB = Random::get(0.0,1.0);
			}
			else if (ceil(data[0]/25.5f) == ceil(corLabiR*10) &&
					ceil(data[1]/25.5f) == ceil(corLabiG*10) &&
					ceil(data[2]/25.5f) == ceil(corLabiB*10)
					) {
				corLabiR = Random::get(0.0,1.0);
				corLabiG = Random::get(0.0,1.0);
				corLabiB = Random::get(0.0,1.0);
			}
			else if (ceil(data[0]/25.5f) == ceil(corFundR*10) &&
					ceil(data[1]/25.5f) == ceil(corFundG*10) &&
					ceil(data[2]/25.5f) == ceil(corFundB*10)
					) {
				corFundR = Random::get(0.0,1.0);
				corFundG = Random::get(0.0,1.0);
				corFundB = Random::get(0.0,1.0);
			}
			break;
		default:
			break;
	}
	printf("m(%d, %d) c(%d, %d) -- d %d -- SRU(%d, %d)\n", x, y, xc, yc, paramCirculo, xSRU, ySRU);
	printf("rgb( %d, %d, %d) - rgb.f( %f, %f, %f)\nRGB( %d, %d, %d) -  RGB.f( %f, %f, %f)\n",
			data[0], data[1], data[2], data[0]/255.0f, data[1]/255.0f, data[2]/255.0f,
			(int) corLabiR*255, (int)corLabiG*255, (int)corLabiB*255, corLabiR, corLabiG, corLabiB);
	glutPostRedisplay();
}
//======================================================================//
void myMotionFunc(int x, int y){

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
			xc -= CIRCLE_CENTER_DISPLACEMENT;
			break;
		case GLUT_KEY_UP:
			yc += CIRCLE_CENTER_DISPLACEMENT;
			break;
		case GLUT_KEY_RIGHT:
			xc += CIRCLE_CENTER_DISPLACEMENT;
			break;
		case GLUT_KEY_DOWN:
			yc -= CIRCLE_CENTER_DISPLACEMENT;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}
//======================================================================//
void myDisplayFunc(){
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(corFundR, corFundG, corFundB, 0.0f);
	desenhaLabirinto();
	desenhaCirculo();
	glutSwapBuffers();
	//glutPostRedisplay();
	//glutSwapBuffers();
}
//======================================================================//
void myReshapeFunc(int w, int h){
	SRD[X_MAX] = w;
	SRD[Y_MAX] = h;
	//printf("m(%d, %d)", w, h);
	glutReshapeWindow(w, h);
	glViewport(0,0, w, h);
	glutPostRedisplay();
}

//======================================================================//

// Inicializa parâmetros de rendering
void Inicializa (void)
{
	corCircR = corCircG = corCircB = 1;
	corFundR = corFundG = corFundB = fabs(1-corCircR);
	corLabiR = corLabiG = corLabiB = 1;

	SRD[X_MIN] = 0;
	SRD[X_MAX] = WINDOW_WIDTH;
	SRD[Y_MIN] = 0;
	SRD[Y_MAX] = WINDOW_HEIGTH;

	glClearColor(corFundR, corFundG, corFundB, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(-(ORTHO_WIDTH/2)-50,(ORTHO_WIDTH/2)+50,-(ORTHO_HEIGTH/2)-50,(ORTHO_HEIGTH/2)+50);
	gluOrtho2D(-(ORTHO_WIDTH/2),(ORTHO_WIDTH/2),-(ORTHO_HEIGTH/2),(ORTHO_HEIGTH/2));
	glMatrixMode(GL_MODELVIEW);
	generateRandomMaze();
}

// Programa principal

int main(int argc, char** argv)
{
	char str[50] = "Wastelands Maze by Ricardo e Ruan Medeiros";
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 	glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGTH);
	glutInitWindowPosition(10,10);
	glutCreateWindow(str);

	glutDisplayFunc 	( myDisplayFunc	);
	glutMouseFunc   	( myMouseFunc   );
	glutMotionFunc  	( myMotionFunc  );
	glutKeyboardFunc	( myKeyboardFunc);
	glutSpecialFunc		( mySpecialFunc	);
	glutReshapeFunc 	( myReshapeFunc );

	Inicializa();
	glutMainLoop();

	return 0;
}
