//============================================================================
// Name        : labirinto-opengl.cpp
// Author      : Ricardo Valério Teixeira de Medeiros Silva e Ruan de Medeiros Bahia
// Version     :
// Copyright   : 
// Description : Jogo de labirinto feito com OpenGL
//  Universidade Federal do Vale do São Francisco
// 	Professor Jorge Luis Cavalcanti
// 	Trabalho de Computação Gráfica
// 		para compilar: g++ AtividadeCG5.c -lglut -lGL -lGLU -lm
//============================================================================

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <effolkronium/random.hpp>
#include <vector>
#ifdef _WIN32
    #include <windows.h>
#endif

using Random = effolkronium::random_static;
typedef struct
{
	int a;
	int b;
}vertex;

int x,y;
int xc,yc,raio;
double r,g,b;

std::vector< vertex* > maze;
bool isOnMap(int x,int y)
{
	return (-900 <= x && x <= 900) && (-900 <= y && y <= 900);
}
void generateRandomMaze(int initialCenterX,int initialCenterY)
{
	int currentLimitXp=initialCenterX+0.75*raio,currentLimitYp=initialCenterY+0.75*raio;
	int currentLimitXl=initialCenterX-0.75*raio,currentLimitYl=initialCenterY-0.75*raio;
	int curretStep=-1,stepCounter=0;
	std::vector< int > possibleSteps;

	while(stepCounter > 100 )
	{

		while(possibleSteps.size() > 0)
			possibleSteps.pop_back();

		if(isOnMap(currentLimitYp+raio,currentLimitYl+raio))
			possibleSteps.push_back(1);//labirinto vai pra cima
		if(isOnMap(currentLimitYp-raio,currentLimitYl-raio))
			possibleSteps.push_back(2);//labirinto vai pra baixo
		if(isOnMap(currentLimitYp+raio/2,currentLimitYl+raio/2) &&
			 isOnMap(currentLimitXp+raio/2,currentLimitXl+raio/2) )
			possibleSteps.push_back(3);//labirinto vai pra cima e para a direita
		if(isOnMap(currentLimitYp+raio/2,currentLimitYl+raio/2) &&
			 isOnMap(currentLimitXp-raio/2,currentLimitXl-raio/2) )
			possibleSteps.push_back(4);//labirinto vai pra cima e para a esquerda
		if(isOnMap(currentLimitYp-raio/2,currentLimitYl-raio/2) &&
			 isOnMap(currentLimitXp+raio/2,currentLimitXl+raio/2) )
			possibleSteps.push_back(5);//labirinto vai pra baixo e para a direita
		if(isOnMap(currentLimitYp-raio/2,currentLimitYl-raio/2) &&
			 isOnMap(currentLimitXp-raio/2,currentLimitXl-raio/2) )
			possibleSteps.push_back(6);//labirinto vai pra baixo e para a esquerda


		if(!possibleSteps.size())
			break;

		int maxLimit = possibleSteps.size()-1;

		auto val = Random::get(0,maxLimit);
	}
}

void desenhaCirculo(void)//Infelizmente esta função de Call Back não pode ter parametros ou eu não sei como...
{

	double theta;

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(r, g, b); // vermelho

	glPointSize(2.0f); // aumenta o tamanho dos pontos
    	glBegin(GL_POINTS);

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

    	glEnd();
	glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa (void)
{
        glClearColor(fabs(1-r), fabs(1-g), fabs(1-b), 0.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-1000,1000,-1000,1000);
        glMatrixMode(GL_MODELVIEW);

}

// Programa principal

int main(int argc, char** argv)
{

	char str[50];
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
 	glutInitWindowSize(800,800);
	glutInitWindowPosition(800,800);
	glutCreateWindow(str);
	glutDisplayFunc(desenhaCirculo);
	Inicializa();
	glutMainLoop();
}
