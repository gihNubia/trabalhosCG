#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

// Aqui declaramos os objetos e listas de objeto
ObjetoJogo nave;
ListaObjetos tiros;
// TODO: Declarar lista de inimigos


GLuint carregaTextura(char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
        arquivo,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    return idTextura;
}

void removeTirosFora(){
    for (int i = 0; i < getSize(tiros); i++){
        if (getObjetoJogo(tiros, i)->posicao.y > 100){
            pop(&tiros, i);
            return removeTirosFora();
        }
    }
    return;
}

// Recebe um ObjetoJogo e printa ele na tela
void desenhaObjeto(ObjetoJogo obj) {
    glColor3f(0, 1, 0);
    glEnable(GL_TEXTURE_2D);
    // TODO: Fazer a textura funcionar
    glBindTexture(GL_TEXTURE_2D, obj.idTextura);
    glBegin(GL_TRIANGLE_FAN);
        //glTexCoord2f(obj.posicao.x-5, obj.posicao.y-5);
        glVertex3f(obj.posicao.x+obj.dimensoes.x, obj.posicao.y, 0); //baixo direito

        //glTexCoord2f(obj.posicao.x-5, obj.posicao.y+5);
        glVertex3f(obj.posicao.x+obj.dimensoes.x, obj.posicao.y+obj.dimensoes.y, 0); //cima direito

        //glTexCoord2f(obj.posicao.x+5, obj.posicao.y+5);
        glVertex3f(obj.posicao.x, obj.posicao.y+obj.dimensoes.y, 0); //cima esquerdo

        //glTexCoord2f(obj.posicao.x+5, obj.posicao.y-5);
        glVertex3f(obj.posicao.x, obj.posicao.y, 0); //baixo esquerdo
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Callback: Cena deve ser desenhada
void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);

    // desenha nave
    desenhaObjeto(nave);

    // desenha tiros
    for(int i = 0; i < getSize(tiros); i++)
        desenhaObjeto(*getObjetoJogo(tiros, i));
    
    // TODO: desenhar inimigos

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
            freeLista(&tiros);
            // TODO: liberar lista de inimigos
            exit(0);
            break;
        case 32:    // espaço
            // cria um tiro e o adiciona na lista de tiros
            ObjetoJogo novoTiro = new_ObjetoJogo(
                new_Vetor(nave.posicao.x + nave.dimensoes.x/2, nave.posicao.y + nave.dimensoes.y),  // tiro sai do meio da nave
                new_Vetor(0, 2),    // velocidade apenas na vertical
                new_Vetor(1, 3),    // (largura, altura) do tiro
                0   // TODO: Textura do tiro
            );
            append(&tiros, novoTiro);

            break;

        default:
            break;
    }
}

// Callback: usuario pressionou uma seta
void setaPressionada(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_RIGHT:  //seta direita
            // nave tem velocidade para direita ->
            nave.velocidade.x=0.5;
            nave.velocidade.y=0;
            break;
        case GLUT_KEY_LEFT: //seta esquerda
            // nave tem velocidade para esquerda <-
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
            // nave perde toda velocidade
            nave.velocidade.x=0;
            nave.velocidade.y=0;
            break;

        default:
            break;
    }
}

// Callback: atualiza valores dos objetos
void atualizaCena(int valor){

    // atualiza posicao da nave
    nave.posicao.x += nave.velocidade.x;
    nave.posicao.y += nave.velocidade.y;
    // TODO: Checar se posicao esta fora do gortho

    // atualiza posicao dos tiros
    for (int i = 0; i < getSize(tiros); i++){
        ObjetoJogo * tiro = getObjetoJogo(tiros, i);

        tiro->posicao.x += tiro->velocidade.x;
        tiro->posicao.y += tiro->velocidade.y;
    }

    // remove todos os tiros que sairam da tela
    removeTirosFora();

    // TODO: atualizar posicao e velocidade dos inimigos

    // TODO: remover tiros e inimigos que colidiram

    // TODO: checar se inimigo alcancou a nave e terminar o jogo

    // manda redesenhar a cena
    glutPostRedisplay();

    // manda chamar esta função novamente daqui 0,33s
    glutTimerFunc(33, atualizaCena, 0);
}

// atribui valores iniciais dos ObjetoJogo
void inicializa() {
    glClearColor(1, 1, 1, 1); // branco

    // cria nave
    nave = new_ObjetoJogo(
        new_Vetor(50, 5),   // posicao
        new_Vetor(0, 0),    // velocidade inicial
        new_Vetor(6, 6),    // tamanho
        carregaTextura("pixel-art-spacecraft.png")  // textura
    );

    // cria lista vazia de tiros
    tiros = new_ListaObjetos();

    // TODO: criar lista cheia de inimigos
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Movimentacao Horizontal");


    // Callback de desenhar cena
    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);

    // Callback de input do usuário
    glutKeyboardFunc(teclaPressionada);
    glutSpecialFunc(setaPressionada);
    glutSpecialUpFunc(setaNaoPressionada);

    // Callback para atualizacao de valores
    glutTimerFunc(33, atualizaCena, 0);

    // atribui valores iniciais dos ObjetoJogo
    inicializa();

    glutMainLoop();

    freeLista(&tiros);
    // TODO: liberar lista de inimigos

    return 0;
}
