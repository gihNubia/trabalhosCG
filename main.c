#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

ObjetoJogo nave;

GLuint carregaTextura(char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
        arquivo,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    return idTextura;
}

// Callback: Cena deve ser desenhada
void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 1, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, nave.idTextura);
    glBegin(GL_TRIANGLE_FAN);
        //glTexCoord2f(nave.posicao.x-5, nave.posicao.y-5);
        glVertex3f(nave.posicao.x+nave.dimensoes.x, nave.posicao.y, 0); //baixo direito

        //glTexCoord2f(nave.posicao.x-5, nave.posicao.y+5);
        glVertex3f(nave.posicao.x+nave.dimensoes.x, nave.posicao.y+nave.dimensoes.y, 0); //cima direito

        //glTexCoord2f(nave.posicao.x+5, nave.posicao.y+5);
        glVertex3f(nave.posicao.x, nave.posicao.y+nave.dimensoes.y, 0); //cima esquerdo

        //glTexCoord2f(nave.posicao.x+5, nave.posicao.y-5);
        glVertex3f(nave.posicao.x, nave.posicao.y, 0); //baixo esquerdo
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

// Callback: tela foi redimensionada
void redimensionada(int width, int height) {
   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 100, 0, 100, -1, 1);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Callback: usuario pressionou uma tecla
void teclaPressionada(unsigned char key, int x, int y) {
    switch(key) {
        case 27:
            exit(0);
            break;

        default:
            break;
    }
}

// Callback: usuario pressionou uma seta
void setaPressionada(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_RIGHT:  //seta direita
            nave.velocidade.x=0.5;
            nave.velocidade.y=0;
            break;
        case GLUT_KEY_LEFT: //seta esquerda
            nave.velocidade.x=-0.5;
            nave.velocidade.y=0;
            break;

        default:
            break;
    }
}

// Callback: usuario tirou o dedo de uma seta
void setaNaoPressionada(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_RIGHT:  //seta direita ou esquerda
        case GLUT_KEY_LEFT:
            nave.velocidade.x=0;
            nave.velocidade.y=0;
            break;

        default:
            break;
    }
}

// Callback: atualiza valores dos objetos
void atualizaCena(int valor){
    //tratar para quando as coordenadas ultrapassarem o gortho
    nave.posicao.x += nave.velocidade.x;
    nave.posicao.y += nave.velocidade.y;

    glutPostRedisplay();

    glutTimerFunc(33, atualizaCena, 0);
}


void inicializa() {
    glClearColor(1, 1, 1, 1); // branco

    nave = new_ObjetoJogo(
        new_Vetor(50, 5),   // posicao
        new_Vetor(0, 0),    // velocidade inicial
        new_Vetor(6, 6),    // tamanho
        carregaTextura("pixel-art-spacecraft.png")  // textura
    );
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);

   glutInitContextVersion(1, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);

   glutCreateWindow("Movimentacao Horizontal");

   glutDisplayFunc(desenhaMinhaCena);
   glutReshapeFunc(redimensionada);
   
   glutKeyboardFunc(teclaPressionada);
   glutSpecialFunc(setaPressionada);
   glutSpecialUpFunc(setaNaoPressionada);


   glutTimerFunc(33, atualizaCena, 0);

   inicializa();

   glutMainLoop();
   return 0;
}
