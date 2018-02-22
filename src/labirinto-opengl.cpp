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

#define CIRCLE_RADIUS 10
#define ORTHO_WIDTH 1920
#define ORTHO_HEIGTH 1080
#define WINDOW_PROPORTION 0.5
#define WINDOW_WIDTH (ORTHO_WIDTH*WINDOW_PROPORTION)
#define WINDOW_HEIGTH (ORTHO_HEIGTH*WINDOW_PROPORTION)
#define MAZE_STEP CIRCLE_RADIUS*2

using Random = effolkronium::random_static;
typedef struct
{
	int xA;
	int xB;
	int yA;
	int yB;
}vertex;

int x,y;
int xc = 0, yc = 0,raio = CIRCLE_RADIUS;
double corCircR,corCircG,corCircB;
double corFundR,corFundG,corFundB;
double corLabiR,corLabiG,corLabiB;
//  SISTEMA = {Xmin,Xmax,Ymin,Ymax]
int SRU[4] = {-(ORTHO_WIDTH/2),(ORTHO_WIDTH/2),-(ORTHO_HEIGTH/2),(ORTHO_HEIGTH/2)};
int SRD[4] = {0,0,0,0};

std::vector< vertex* > maze;
bool isOnMap(int x,int y)
{
	return (-(ORTHO_WIDTH/2)*0.9 <= x && x <= (ORTHO_WIDTH/2)*0.9) && (-(ORTHO_HEIGTH/2)*0.9 <= y && y <= (ORTHO_HEIGTH/2)*0.9);
}
void generateRandomMaze(int initialCenterX,int initialCenterY)
{
	int currentLimitXp=initialCenterX+1.5*raio,currentLimitYp=initialCenterY;
	int currentLimitXl=initialCenterX-1.5*raio,currentLimitYl=initialCenterY;
	int currentStep=0,stepCounter=0;
	std::vector< int > possibleSteps;

	while(stepCounter > 100 )
	{

		while(possibleSteps.size() > 0)
			possibleSteps.pop_back();

		if(isOnMap(currentLimitYp+raio,currentLimitYl+raio))
			possibleSteps.push_back(1);//labirinto vai pra cima
		if(isOnMap(currentLimitYl+((currentStep)*1.5*raio),currentLimitXl) &&
			 isOnMap(currentLimitYl+1.5*raio,currentLimitXl+1.5*raio) )
			possibleSteps.push_back(2);//labirinto vai  para a direita
		if(isOnMap(currentLimitYp+1.5*raio,currentLimitXp) &&
			 isOnMap(currentLimitYp+1.5*raio,currentLimitXp-1.5*raio) )
			possibleSteps.push_back(3);//labirinto vai para a esquerda


		if(!possibleSteps.size())
			break;

		int maxLimit = possibleSteps.size()-1;

		auto val = Random::get(0,maxLimit);
		int mov = possibleSteps[val];

		switch(mov)
		{
			case 1:
			{	currentStep = 1;
				vertex *a = new vertex;
				a->xA = currentLimitXp;
				a->yA = currentLimitYp;
				a->xB = currentLimitXp;
				a->yB = currentLimitYp+raio;
				currentLimitYp = currentLimitYp+raio;
				maze.push_back(a);
				vertex *b = new vertex;
				b->xA = currentLimitXl;
				b->yA = currentLimitYl;
				b->xB = currentLimitXl;
				b->yB = currentLimitYl+raio;
				currentLimitYl=currentLimitYl+raio;
				maze.push_back(b);
			}
			break;
			case 2:
			{
				vertex *a = new vertex;
				a->xA = currentLimitXp;
				a->yA = currentLimitYp;
				a->xB = currentLimitXp;
				a->yB = currentLimitYp+raio*1.5;
				maze.push_back(a);
				vertex *b = new vertex;
				b->xA = currentLimitXp;
				b->yA = currentLimitYp+raio*1.5;
				b->xB = currentLimitXp+raio*1.5;
				b->yB = currentLimitYp+raio*1.5;
				maze.push_back(b);
			}
		}
	}
}
//======================================================================//

//======================================================================//
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
	//y^2 + x^2 = r^2
	int xSRU = getXSRU(SRU, SRD,x);
	int ySRU = getYSRU(SRU, SRD,y);
	int d = (sqrt((xSRU-xc)*(xSRU-xc)+(ySRU-yc)*(ySRU-yc)));
	if (d <= raio) {
		corCircR = Random::get(0.0,1.0);
		corCircG = Random::get(0.0,1.0);
		corCircB = Random::get(0.0,1.0);
	}
	printf("m(%d, %d) c(%d, %d) -- d %d -- SRU(%d, %d)\n", x, y, xc, yc, d, xSRU, ySRU);

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
	SRD[X_MAX] = w;
	SRD[Y_MAX] = h;
	printf("m(%d, %d)", w, h);
}

//======================================================================//

// Inicializa parâmetros de rendering
void Inicializa (void)
{
	corCircR = corCircG = corCircB = 1;
	corFundR = corFundG = corFundB = fabs(1-corCircR);

	SRD[X_MIN] = 0;
	SRD[X_MAX] = WINDOW_WIDTH;
	SRD[Y_MIN] = 0;
	SRD[Y_MAX] = WINDOW_HEIGTH;

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
