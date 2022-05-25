#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

// Aqui declaramos os objetos e listas de objeto
ObjetoJogo nave;
ListaObjetos tiros;
ListaObjetos inimigos;

// id das texturas
int idTexturaNave;
int idTexturaInimigo;
int idTexturaTiro;
int idTexturaFundo;
int idTexturaGameOver;
int idTexturaPause;

// variaveis de pontuacao
// BUG: tente declarar essa variavel embaixo de direcoes
float pontos;


// variaveis de controle do movimento dos inimigos
int contadorMax = 63;
int contador = 0;
Vetor direcoes[4];

//game state
int gameState =1;

void desenhaGameOver(){
    int tamanhoMundo = 100;
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaPause);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(1, 0);
        glVertex3f(tamanhoMundo, 0, 0); //baixo direito

        glTexCoord2f(1, 1);
        glVertex3f(tamanhoMundo, tamanhoMundo, 0); //cima direito

        glTexCoord2f(0, 1);
        glVertex3f(0, tamanhoMundo, 0); //cima esquerdo

        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0); //baixo esquerdo
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void pause(){
    int tamanhoMundo = 100;
    glColor3f(0.5, 0.5, 0.5);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaPause);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(1, 0);
        glVertex3f(tamanhoMundo, 0, 0); //baixo direito

        glTexCoord2f(1, 1);
        glVertex3f(tamanhoMundo, tamanhoMundo, 0); //cima direito

        glTexCoord2f(0, 1);
        glVertex3f(0, tamanhoMundo, 0); //cima esquerdo

        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0); //baixo esquerdo
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void liberarMemoria(){
    freeLista(&tiros);
    freeLista(&inimigos);
}

// preenche lista de inimigos
void criarInimigos(){
    float linha = 0;
    float coluna = 90;

    int tamanhoInimigo = 4;
    float espacoEntreInimigos = 8.5;
    int qtdInimigos = 7;
    int tamanhoDoMundo = 100;
    int espacoLateral = (contadorMax + 1)/4;

    for(int i =0; i<2; i++){
        for(int j=0; j<qtdInimigos; j++){
            ObjetoJogo inimigo = new_ObjetoJogo(
                new_Vetor(linha, coluna),   // posicao
                new_Vetor(0, 0),    // velocidade inicial
                new_Vetor(tamanhoInimigo, tamanhoInimigo),    // tamanho
                idTexturaInimigo  // textura
            ); 

            append(&inimigos, inimigo);

            //incremento
            linha += tamanhoInimigo + espacoEntreInimigos;
        }

        linha = tamanhoDoMundo - (tamanhoInimigo*qtdInimigos + espacoEntreInimigos*(qtdInimigos-1) + espacoLateral);
        coluna-=15;
    }
    
}

// carrega textura do arquivo indicado e retorna seu id
GLuint carregaTextura(char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
        arquivo,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    return idTextura;
}



// remove os tiros que estao fora do gortho
void removeTirosFora(){
    // percorrer lista ao contrario para evitar erros de posicao ao remover itens
    for (int i = getSize(tiros) - 1; i >= 0; i--){
        if (getObjetoJogo(tiros, i)->posicao.y > 100){
            pop(&tiros, i);
        }
    }
    return;
}

void removeInimigosAcertados(){
    // percorrer listas ao contrario para evitar erros de posicao ao remover itens
    for (int i = getSize(tiros) - 1; i >= 0; i--){
        for (int j = getSize(inimigos) - 1; j >= 0; j--){
            if (objetoColideCom(*getObjetoJogo(tiros, i), *getObjetoJogo(inimigos, j))){
                pontos = pontos + 100 - (*getObjetoJogo(inimigos, j)).posicao.y;
                pop(&tiros, i);
                pop(&inimigos, j);
                break;
            }
        }
    }
}

void desenhaFundo(){
    int tamanhoMundo = 100;
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaFundo);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(1, 0);
        glVertex3f(tamanhoMundo, 0, 0); //baixo direito

        glTexCoord2f(1, 1);
        glVertex3f(tamanhoMundo, tamanhoMundo, 0); //cima direito

        glTexCoord2f(0, 1);
        glVertex3f(0, tamanhoMundo, 0); //cima esquerdo

        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0); //baixo esquerdo
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Recebe um ObjetoJogo e printa ele na tela
void desenhaObjeto(ObjetoJogo obj) {
    glColor3f(1, 1, 1);
    glPushMatrix();
        // Move o sistema de coordenadas para a posição onde deseja-se desenhar
        glTranslatef(obj.posicao.x, obj.posicao.y, 0);
        // TODO: Fazer a textura funcionar
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, obj.idTextura);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(1, 0);
            glVertex3f(obj.dimensoes.x, 0, 0); //baixo direito

            glTexCoord2f(1, 1);
            glVertex3f(obj.dimensoes.x, obj.dimensoes.y, 0); //cima direito

            glTexCoord2f(0, 1);
            glVertex3f(0, obj.dimensoes.y, 0); //cima esquerdo

            glTexCoord2f(0, 0);
            glVertex3f(0, 0, 0); //baixo esquerdo
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void informarPontuacao() {
    glRasterPos3f(5, 95, 0);

    char buf[10];
  
    gcvt(pontos, 10, buf);

    for (int i = 0; i < strlen(buf); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
    }
}

// Callback: Cena deve ser desenhada
void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);
    if(gameState == 1){
        desenhaFundo();

        // desenha nave
        desenhaObjeto(nave);

        // desenha tiros
        for(int i = 0; i < getSize(tiros); i++)
            desenhaObjeto(*getObjetoJogo(tiros, i));
        
        // desenhar inimigos
        for(int i = 0; i < getSize(inimigos); i++)
            desenhaObjeto(*getObjetoJogo(inimigos, i));

        informarPontuacao();
    }
    else if(gameState == 2){
        desenhaGameOver();
    }
    else if(gameState ==3){
        pause();
    }
    
    
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

// atribui valores iniciais dos ObjetoJogo
void inicializa() {
    glClearColor(0, 0, 0, 1); // branco
    contador =0;

    // habilita mesclagem de cores, para termos suporte a texturas
    // com transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    idTexturaNave = carregaTextura("assets/spaceship.png");
    idTexturaInimigo = carregaTextura("assets/inimigo.png");
    idTexturaTiro = carregaTextura("assets/tiro.png");
    idTexturaFundo = carregaTextura("assets/fundo.png");
    idTexturaGameOver = carregaTextura("assets/game-over.png");
    idTexturaPause = carregaTextura("assets/pause2.png");

    // cria nave
    nave = new_ObjetoJogo(
        new_Vetor(50, 5),   // posicao
        new_Vetor(0, 0),    // velocidade inicial
        new_Vetor(10, 10),    // tamanho
        idTexturaNave  // textura
    );


    // cria lista vazia de tiros
    tiros = new_ListaObjetos();

    // cria lista cheia de inimigos
    inimigos = new_ListaObjetos();
    criarInimigos();

    direcoes[0] = new_Vetor(1,0);   //direita
    direcoes[1] = new_Vetor(0, -1); //baixo
    direcoes[2] = new_Vetor(-1, 0); //esquerda
    direcoes[3] = new_Vetor(0, -1); //baixo
    
    pontos = 0;
}

// Callback: usuario pressionou uma tecla
void teclaPressionada(unsigned char key, int x, int y) {
    switch(key) {
        case 27:
            exit(0);
            break;
        case 32:    // espaço
            if(gameState == 1){
               // cria um tiro e o adiciona na lista de tiros
                ObjetoJogo novoTiro = new_ObjetoJogo(
                    new_Vetor(nave.posicao.x + nave.dimensoes.x/2, nave.posicao.y + nave.dimensoes.y),  // tiro sai do meio da nave
                    new_Vetor(0, 2),    // velocidade apenas na vertical
                    new_Vetor(1, 3),    // (largura, altura) do tiro
                    idTexturaTiro   // TODO: Textura do tiro
                );
                append(&tiros, novoTiro); 
            }
            

            break;
        case 114: // r
        liberarMemoria();
        gameState = 1;
        inicializa();
        break;

        case 112: // p
        if(gameState==1){
            gameState=3;
        }
        else if(gameState == 3){
            gameState=1;
        }
        break;

        default:
            break;
    }
}

// Callback: usuario pressionou uma seta
void setaPressionada(int key, int x, int y) {
    if(gameState == 1){
        switch(key) {
        
        case GLUT_KEY_RIGHT:  //seta direita
            // nave tem velocidade para direita ->
            nave.velocidade.x=1.25;
            nave.velocidade.y=0;
            break;
        case GLUT_KEY_LEFT: //seta esquerda
            // nave tem velocidade para esquerda <-
            nave.velocidade.x=-1.25;
            nave.velocidade.y=0;
            break;

        default:
            break;
        }  
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
    if(gameState == 1){
        // atualiza posicao da nave
        nave.posicao.x += nave.velocidade.x;
        nave.posicao.y += nave.velocidade.y;

        // checar se posicao da nave esta fora do gortho
        if (nave.posicao.x < 0 || nave.posicao.x + nave.dimensoes.x > 100) // se eixo x saiu do mundo
            nave.posicao.x -= nave.velocidade.x;    // desfazer movimento no eixo x
        if (nave.posicao.y < 0 || nave.posicao.y + nave.dimensoes.y > 100) // se eixo y saiu do mundo
            nave.posicao.y -= nave.velocidade.y;    // desfazer movimento no eixo y

        // atualiza posicao dos tiros
        for (int i = 0; i < getSize(tiros); i++){
            ObjetoJogo * tiro = getObjetoJogo(tiros, i);

            tiro->posicao.x += tiro->velocidade.x;
            tiro->posicao.y += tiro->velocidade.y;
        }

        // atualizar posicao dos inimigos
        for (int i = 0; i < getSize(inimigos); i++){
            ObjetoJogo * inimigo = getObjetoJogo(inimigos, i);

            inimigo->posicao.x += inimigo->velocidade.x;
            inimigo->posicao.y += inimigo->velocidade.y;
        }

        // contador nunca eh maior que o maximo...
        contador += 1;
        if (contador > contadorMax)
            contador = 0;
        // ...portanto essa posicao nunca eh maior que 3
        Vetor direcao = direcoes[contador/(contadorMax/4)];

        // atualizar direcao dos inimigos
        for (int i = 0; i < getSize(inimigos); i++){
            ObjetoJogo * inimigo = getObjetoJogo(inimigos, i);
            inimigo->velocidade.x = direcao.x * 1;
            inimigo->velocidade.y = direcao.y * 0.25;
        }

        // remove todos os tiros que sairam da tela
        removeTirosFora();

        // remover tiros e inimigos que colidiram
        removeInimigosAcertados();

        // TODO: checar se inimigo alcancou a nave e terminar o jogo
        for(int i = 0; i< getSize(inimigos); i++){
            ObjetoJogo * inimigo = getObjetoJogo(inimigos, i);
            if(objetoColideCom(nave, *inimigo)){
                gameState=2;
            }
            
        }
    }
    

    // manda redesenhar a cena
        glutPostRedisplay();

        // manda chamar esta função novamente daqui 0,33s
        glutTimerFunc(33, atualizaCena, 0);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("----------------Galaxian----------------");


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

    // quando programa parar de executar libera a memoria
    atexit(liberarMemoria);

    glutMainLoop();

    return 0;
}
