/*
    Integrantes:    Lara Gama Santos (20213001497)
                    Lucas Santos Rodrigues (20213001521)
                    Marcela Camarano Caram (20213006528)
                    Sulamita Ester Costa (20213001568)
                    Vinicius Ferreira Pinheiro (20213005208)

*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SOIL/SOIL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL.h>
#include "SDL/SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <math.h>
#include <vector>
#include <cmath>

GLfloat angle = 100, fAspect;
bool tamanhoReal = false; // HABILITAR PROPORCAO REAL DOS PLANETAS
bool iluminar = true; // HABILITAR ILUMINACAO
int modoCamera = 1; //MODO DA CAMERA
Mix_Music *som = NULL; //MUSICA

typedef struct{
    int id;

    double raio; //RAIO DO ASTRO

    float translacao; //TRANSLACAO DO ASTRO
    float rotacao; //ANGULO DE ROTACAO DO ASTRO
    float velRotacao; //VELOCIDADE DE ROTACAO DO ASTRO

    float orbita; //DISTANCIA ORBITAL DO ASTRO

    GLuint textura; //TEXTURA DO ASTRO

}astro;

void EspecificaParametrosVisualizacao(void);

//Cria os diferentes astros
astro sol;
astro mercurio;
astro venus;
astro terra;
astro marte;
astro jupiter;
astro saturno;
astro urano;
astro netuno;
astro lua;

//cria variáveis de luz
float ambiente[] = {0.0, 0.0, 0.0, 1.0};
float difusa[] = {1, 1, 1, 1.0};
float epecular[] = {1, 1, 1, 1.0};
float posicional[] = {0.0, 0.0, 3.0, 1};
float global[] = {0.2, 0.2, 0.2, 1.0};

//funcao para importar a textura de cada um dos astros
GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
            arquivo,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y
    );

    return idTextura;
}

//funcao para carregar a musica da cena
void carregarMusica(char  *music){
    if(!Mix_PlayingMusic()){
        Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,1024);
        som = Mix_LoadMUS(music);
        Mix_PlayMusic(som,-1);
    }
}


void rotacionar()
{
    sol.rotacao      = sol.rotacao      + 2.0f;
    mercurio.rotacao = mercurio.rotacao + 2.0f;
    venus.rotacao    = venus.rotacao    + 2.0f;
    terra.rotacao    = terra.rotacao    + 2.0f;
    marte.rotacao    = marte.rotacao    + 2.0f;
    jupiter.rotacao  = jupiter.rotacao  + 2.0f;
    saturno.rotacao  = saturno.rotacao  + 2.0f;
    urano.rotacao    = urano.rotacao    + 2.0f;
    netuno.rotacao   = netuno.rotacao   + 2.0f;
    lua.rotacao      = lua.rotacao      + 2.0f;
}

void transladar()
{
    mercurio.translacao = mercurio.translacao + 2.0f;
    venus.translacao    = venus.translacao    + 1.47f;
    terra.translacao    = terra.translacao    + 1.25f;
    marte.translacao    = marte.translacao    + 1.01f;
    jupiter.translacao  = jupiter.translacao  + 0.55f;
    saturno.translacao  = saturno.translacao  + 0.40f;
    urano.translacao    = urano.translacao    + 0.28f;
    netuno.translacao   = netuno.translacao   + 0.22f;
    lua.translacao      = lua.translacao      + 5.0f;

    rotacionar();
    glutPostRedisplay();
}

void estadoAtualizacao(int time){
    transladar();

    EspecificaParametrosVisualizacao();
    glutPostRedisplay();

    glutTimerFunc(time, estadoAtualizacao, time);
}

//define as caracteristicas de cada um dos astros
void defineAstros(){
    // tamanho real dos planetas
    if(tamanhoReal){
        //SOL
        sol.raio = 100;
        sol.rotacao = 0;
        sol.textura = carregaTextura("sol.jpg");
        sol.id = 0;

        //MERCURIO
        mercurio.raio = (sol.raio*0.0035);
        mercurio.translacao = 0;
        mercurio.rotacao = 0;
        mercurio.velRotacao = 97;
        mercurio.orbita = (80 + 6.6);
        mercurio.textura = carregaTextura("mercurio.jpg");
        mercurio.id = 1;

        //VENUS
        venus.raio = (sol.raio*0.0086);
        venus.translacao = 0;
        venus.rotacao = 0;
        venus.velRotacao = -267;
        venus.orbita = (80 + 12.49);
        venus.textura = carregaTextura("venus.jpg");
        venus.id = 2;

        //TERRA
        terra.raio = (sol.raio*0.0091);
        terra.translacao = 0;
        terra.rotacao = 0;
        terra.velRotacao = -133.5;
        terra.orbita = (80 + 17.2);
        terra.textura = carregaTextura("terra.jpg");
        terra.id = 3;

        //MARTE
        marte.raio = (sol.raio*0.0048);
        marte.translacao = 0;
        marte.rotacao = 0;
        marte.velRotacao = 115;
        marte.orbita = (80 + 26.3);
        marte.textura = carregaTextura("marte.jpg");
        marte.id = 4;

        //JUPITER
        jupiter.raio = (sol.raio*0.1);
        jupiter.translacao = 0;
        jupiter.rotacao = 0;
        jupiter.velRotacao = 93;
        jupiter.orbita = (80 + 89.85);
        jupiter.textura = carregaTextura("jupiter.jpg");
        jupiter.id = 5;

        //SATURNO
        saturno.raio = (sol.raio*0.083);
        saturno.translacao = 0;
        saturno.rotacao = 0;
        saturno.velRotacao = 117;
        saturno.orbita = (80 + 165.01);
        saturno.textura = carregaTextura("saturno.jpg");
        saturno.id = 6;

        //URANO
        urano.raio = (sol.raio*0.036);
        urano.translacao = 0;
        urano.rotacao = 0;
        urano.velRotacao = -188;
        urano.orbita = (80 + 331.44);
        urano.textura = carregaTextura("urano.jpg");
        urano.id = 7;

        //NETUNO
        netuno.raio = (sol.raio*0.035);
        netuno.translacao = 0;
        netuno.rotacao = 0;
        netuno.velRotacao = 120;
        netuno.orbita = (80 + 520);
        netuno.textura = carregaTextura("netuno.jpg");
        netuno.id = 8;

        //LUA
        lua.raio = (sol.raio * 0.0024);
        lua.translacao = 0;
        lua.rotacao = 0;
        lua.velRotacao = 90;
        lua.orbita = 0.088;
        lua.textura = carregaTextura("lua.jpg");
        lua.id = 9;
    }

    // tamanho visivel dos planetas
    else{
        //SOL
        sol.raio = 80;
        sol.rotacao = 0;
        sol.textura = carregaTextura("sol.jpg");
        sol.id = 0;

        //MERCURIO
        mercurio.raio = (10);
        mercurio.translacao = 0;
        mercurio.rotacao = 0;
        mercurio.velRotacao = 97;
        mercurio.orbita = 80;
        mercurio.textura = carregaTextura("mercurio.jpg");
        mercurio.id = 1;

        //VENUS
        venus.raio = (20);
        venus.translacao = 0;
        venus.rotacao = 0;
        venus.velRotacao = -267;
        venus.orbita = 154;
        venus.textura = carregaTextura("venus.jpg");
        venus.id = 2;

        //TERRA
        terra.raio = (23);
        terra.translacao = 0;
        terra.rotacao = 0;
        terra.velRotacao = -133.5;
        terra.orbita = 228;
        terra.textura = carregaTextura("terra.jpg");
        terra.id = 3;

        //MARTE
        marte.raio = (27);
        marte.translacao = 0;
        marte.rotacao = 0;
        marte.velRotacao = 115;
        marte.orbita = 302;
        marte.textura = carregaTextura("marte.jpg");
        marte.id = 4;

        //JUPITER
        jupiter.raio = (40);
        jupiter.translacao = 0;
        jupiter.rotacao = 0;
        jupiter.velRotacao = 93;
        jupiter.orbita = 376;
        jupiter.textura = carregaTextura("jupiter.jpg");
        jupiter.id = 5;

        //SATURNO
        saturno.raio = (33);
        saturno.translacao = 0;
        saturno.rotacao = 0;
        saturno.velRotacao = 117;
        saturno.orbita = 450;
        saturno.textura = carregaTextura("saturno.jpg");
        saturno.id = 6;

        //URANO
        urano.raio = (37);
        urano.translacao = 0;
        urano.rotacao = 0;
        urano.velRotacao = -188;
        urano.orbita = 524;
        urano.textura = carregaTextura("urano.jpg");
        urano.id = 7;

        //NETUNO
        netuno.raio = (30);
        netuno.translacao = 0;
        netuno.rotacao = 0;
        netuno.velRotacao = 120;
        netuno.orbita = 600;
        netuno.textura = carregaTextura("netuno.jpg");
        netuno.id = 8;

        //LUA
        lua.raio = (10);
        lua.translacao = 0;
        lua.rotacao = 0;
        lua.velRotacao = 90;
        lua.orbita = 20;
        lua.textura = carregaTextura("lua.jpg");
        lua.id = 9;
    }
}

// Gera a Lua
void gerarLua(GLUquadric *quad)
{
    glBindTexture(GL_TEXTURE_2D, lua.textura);
    glPushMatrix();
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluQuadricTexture(quad, GL_TRUE);
        glRotatef(lua.translacao, 0, 0, 1);
        glTranslatef(terra.raio+lua.orbita, 0, 0);
        glRotatef(lua.rotacao, 0, 1, 0);
        glRotatef(lua.velRotacao, 1, 0, 0);
        gluSphere(quad, lua.raio, 50, 50);
    glPopMatrix();
}

// Gera os aneis dos planetas Gasosos
void aneis(astro planeta, GLUquadric *quad)
{
    if(planeta.id == 6){
        glBindTexture(GL_TEXTURE_2D, carregaTextura("aneis.png"));
                if(tamanhoReal){
                    glPushMatrix();
                        glRotatef(0, 0, -1, 0);
                        glRotatef(90, 1, 0, 0);
                        gluDisk(quad, planeta.raio+1, planeta.raio+2, 600, 600);
                    glPopMatrix();

                    glPushMatrix();
                        glRotatef(0, 0, 1, 0);
                        glRotatef(90, -1, 0, 0);
                        gluDisk(quad, planeta.raio+1, planeta.raio+2, 600, 600);
                    glPopMatrix();
                }
                else{
                    glPushMatrix();
                        glRotatef(0, 0, -1, 0);
                        glRotatef(90, 1, 0, 0);
                        gluDisk(quad, planeta.raio+10, planeta.raio+20, 600, 600);
                    glPopMatrix();

                    glPushMatrix();
                        glRotatef(0, 0, 1, 0);
                        glRotatef(90, -1, 0, 0);
                        gluDisk(quad, planeta.raio+10, planeta.raio+20, 600, 600);
                    glPopMatrix();
                }
    }
        if(planeta.id == 5 || planeta.id == 7 || planeta.id == 8){
                if(tamanhoReal){
                    glPushMatrix();
                        glRotatef(0, 0, -1, 0);
                        glRotatef(90, 1, 0, 0);
                        gluDisk(quad, planeta.raio+1, planeta.raio+1.1, 600, 600);
                    glPopMatrix();

                    glPushMatrix();
                        glRotatef(0, 0, 1, 0);
                        glRotatef(90, -1, 0, 0);
                        gluDisk(quad, planeta.raio+1, planeta.raio+2, 600, 600);
                    glPopMatrix();
                }
                else{
                    glPushMatrix();
                        glRotatef(0, 0, -1, 0);
                        glRotatef(90, 1, 0, 0);
                        gluDisk(quad, planeta.raio+10, planeta.raio+11, 600, 600);
                    glPopMatrix();

                    glPushMatrix();
                        glRotatef(0, 0, 1, 0);
                        glRotatef(90, -1, 0, 0);
                        gluDisk(quad, planeta.raio+10, planeta.raio+11, 600, 600);
                    glPopMatrix();
                }
    }
}

// Funcao que desenha o Sol
void desenhaSol()
{
    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);
    GLUquadric *quad_sol;
    quad_sol = gluNewQuadric();

    //DESENHA SOL
    glBindTexture(GL_TEXTURE_2D, sol.textura);
    glPushMatrix();

        glRotatef(sol.rotacao, 0.0f, 1.0f, 0.0f);
        glRotatef(90, 1.0f, 0.0f, 0.0f);

        gluQuadricDrawStyle(quad_sol, GLU_FILL);
        gluQuadricNormals(quad_sol, GLU_SMOOTH);
        gluQuadricTexture(quad_sol, GL_TRUE);
        gluSphere(quad_sol,sol.raio,50,50);
        gluDeleteQuadric(quad_sol);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void iluminacao(){
	if(iluminar){
	    glEnable(GL_LIGHT0);
	    glDisable(GL_LIGHTING);

	    glPushMatrix();
	        glRotatef(0, 1.0, 0.0, 0.0);
	        glRotatef(0, 0.0, 1.0, 0.0);
	        glLightfv(GL_LIGHT0, GL_POSITION, posicional);
	        glTranslatef(posicional[0], posicional[1], posicional[2]);
	        glColor3f(1, 1, 1);
	        //SOL
            desenhaSol();
	    glPopMatrix();

	}else{
		glDisable(GL_LIGHT0);
		//SOL
        desenhaSol();
	}

	glEnable(GL_LIGHTING);

}

// Funcao que desenha 1 astro
void desenhaUmPlaneta(astro planeta)
{
    GLUquadric *quad;
    quad = gluNewQuadric();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, planeta.textura);
    glPushMatrix();
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluQuadricTexture(quad, GL_TRUE);
        glRotatef(planeta.translacao, 0, 1, 0);
        glTranslatef(planeta.orbita, 0, planeta.orbita);
        glRotatef(planeta.rotacao, 0, 1, 0);
        glRotatef(planeta.velRotacao, 1, 0, 0);
        gluSphere(quad, planeta.raio, 50, 50);

        if(planeta.id == 3)
            gerarLua(quad);

        if(planeta.id == 5 || planeta.id == 6 || planeta.id == 7 || planeta.id == 8)
        {
            aneis(planeta, quad);
        }

        gluDeleteQuadric(quad);
    glPopMatrix();
}

// Funcao callback chamada para fazer o desenho
void desenhaPlanetas()
{
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLUquadric *quad;
    quad = gluNewQuadric();

    EspecificaParametrosVisualizacao();

    //ILUMINACAO
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, epecular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, false);
    iluminacao();

    glColor3f(1.0f, 1.0f, 1.0f);

    // DESENHA OS PLANETAS
    desenhaUmPlaneta(mercurio);
    desenhaUmPlaneta(venus);
    desenhaUmPlaneta(terra);
    desenhaUmPlaneta(marte);
    desenhaUmPlaneta(jupiter);
    desenhaUmPlaneta(saturno);
    desenhaUmPlaneta(urano);
    desenhaUmPlaneta(netuno);

    // Executa os comandos OpenGL
    glutSwapBuffers();

    glDisable(GL_TEXTURE_2D);
}

// Inicializa parametros de rendering
void Inicializa (void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);

    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    carregarMusica("som.mp3");
}

// Funcao usada para especificar o volume de visualiza��o
void EspecificaParametrosVisualizacao(void)
{
    // Especifica sistema de coordenadas de proje��o
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de proje��o
    glLoadIdentity();

    // Especifica a proje��o perspectiva
    gluPerspective(angle,fAspect,0.5,2000);

    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();

    // MODOS CAMERAS
    if(modoCamera == 1)
        gluLookAt(0,10,1000, 0,0,0, 0,1,0);

    else if(modoCamera == 2)
        gluLookAt(0,1000,0, 0,0,0, 1,0,0);

}

// Funcao callback chamada quando o tamanho da janela � alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Para previnir uma divis�o por zero
    if ( h == 0 ) h = 1;
    // Especifica o tamanho da viewport
    glViewport(0, 0, w, h);

    // Calcula a corre��o de aspecto
    fAspect = (GLfloat)w/(GLfloat)h;

    EspecificaParametrosVisualizacao();
}

// Funcao callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-in
            if (angle >= 10) angle -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-out
            if (angle <= 500) angle += 5;
        }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

void gerenciaTeclado(unsigned char key, int x, int y)
{
    switch(key)
    {
        //Alterar camera
        case '1': modoCamera = 1; break;
        case '2': modoCamera = 2; break;

        //Alterar proporcao dos planetas
        case 'f': if(tamanhoReal){
                        tamanhoReal = false;
                        defineAstros();
                    }
                  else{
                        tamanhoReal = true;
                        defineAstros();
                  }
                  break;
        //Habilita a ilumincao
        case 'i': if(iluminar)
                        iluminar = false;
                  else
                        iluminar = true;
                  break;
    }
}

// Programa Principal
int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB  | GLUT_DEPTH);
    glutInitWindowSize(1400,700);
    glutCreateWindow("Sistema Solar");
    defineAstros(); // DEFINE AS CARACTERISTICAS DOS ASTROS
    glutDisplayFunc(desenhaPlanetas); //DESENHA OS PLANETAS
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse); //MOUSE, ZOOM-IN E OUT
    glutKeyboardFunc(gerenciaTeclado); //TECLADO
    glutTimerFunc(10, estadoAtualizacao, 10);
    transladar(); //translacao e rotacao
    Inicializa();
    glutMainLoop(); //LOOP
    Mix_CloseAudio();
	SDL_Quit();

}
