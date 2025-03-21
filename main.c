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

// variaveis de controle da nave
int vidas;

// variaveis de controle de tiro
int podeAtirar;

// variaveis de controle do movimento dos inimigos
int contadorMax = 63;
int contador = 0;
int fase;
Vetor direcoes[4];

//game state
enum {InGame, GameOver, Pause} gameState;

// recebe o id de uma textura e desenha na tela
void desenhaFundo(int idTextura){
    int tamanhoMundo = 100;
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
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

// remove inimigos que foram acertados
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

// Recebe um ObjetoJogo e printa ele na tela
void desenhaObjeto(ObjetoJogo obj) {
    glColor3f(1, 1, 1);
    glPushMatrix();
        // Move o sistema de coordenadas para a posição onde deseja-se desenhar
        glTranslatef(obj.posicao.x, obj.posicao.y, 0);
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

// printa a pontuacao na tela
void informarPontuacao() {
    glRasterPos3f(65, 95, 0);

    char buf[10];

    gcvt(pontos, 10, buf);

    for (int i = 0; i < strlen(buf); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
    }
}

// printa a fase na tela
void informarFase() {
    glRasterPos3f(5, 95, 0);

    char buf[10];

    gcvt(fase, 10, buf);

    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'F');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');

    for (int i = 0; i < strlen(buf); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
    }
}

// printa a vidas na tela
void informarVidas() {
    glRasterPos3f(25, 95, 0);

    char buf[10];

    gcvt(vidas, 10, buf);

    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'V');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'I');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'D');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');

    for (int i = 0; i < strlen(buf); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
    }
}

// Callback: Cena deve ser desenhada
void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);

    // se estiver jogando e vivo
    if(gameState == InGame){
        desenhaFundo(idTexturaFundo);

        // desenha nave
        desenhaObjeto(nave);

        // desenha tiros
        for(int i = 0; i < getSize(tiros); i++)
            desenhaObjeto(*getObjetoJogo(tiros, i));
        
        // desenhar inimigos
        for(int i = 0; i < getSize(inimigos); i++)
            desenhaObjeto(*getObjetoJogo(inimigos, i));

        informarPontuacao();
        informarFase();
        informarVidas();
    }
    // se estiver morto
    else if(gameState == GameOver){
        desenhaFundo(idTexturaGameOver);
    }
    // se estiver pausado
    else if(gameState == Pause){
        desenhaFundo(idTexturaPause);
    }
    
    glutSwapBuffers();
}

// Callback: tela foi redimensionada
void redimensionada(int width, int height) {
   glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -1, 1);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float) 100)/ 100;
    // se a janela está menos larga do que o mundo
    if (razaoAspectoJanela < razaoAspectoMundo) {
        // vamos colocar barras verticais (acima e abaixo)
        float hViewport = width / razaoAspectoMundo;
        float yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    // se a janela está mais larga (achatada) do que o mundo
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        // vamos colocar barras horizontais (esquerda e direita)
        float wViewport = ((float)height) * razaoAspectoMundo;
        float xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// quando o jogador reiniciar o jogo
void reiniciar(){
    contador=0;
    podeAtirar=1;
    fase=1;
    gameState=InGame;

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

    vidas = 3;

    // zera a pontuacao
    pontos = 0;
}

// atribui valores iniciais dos ObjetoJogo
void inicializa() {
    glClearColor(0, 0, 0, 1); // branco

    // habilita mesclagem de cores, para termos suporte a texturas
    // com transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    idTexturaNave = carregaTextura("assets/spaceship.png");
    idTexturaInimigo = carregaTextura("assets/fogo.png");
    idTexturaTiro = carregaTextura("assets/tiro.png");
    idTexturaFundo = carregaTextura("assets/space.jpg");
    idTexturaGameOver = carregaTextura("assets/game-over.png");
    idTexturaPause = carregaTextura("assets/pause.png");

    contador=0;
    podeAtirar=1;
    fase=1;
    gameState=InGame;

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
    
    vidas = 3;

    pontos = 0;
}

// Callback: usuario pressionou uma tecla
void teclaPressionada(unsigned char key, int x, int y) {
    switch(key) {
        case 27:    // esc
            exit(0);
            break;
        case 32:    // espaco
            // se estiver jogando e pode atirar
            if(gameState == InGame && podeAtirar == 1){
               // cria um tiro e o adiciona na lista de tiros
                ObjetoJogo novoTiro = new_ObjetoJogo(
                    new_Vetor(nave.posicao.x + nave.dimensoes.x/2, nave.posicao.y + nave.dimensoes.y),  // tiro sai do meio da nave
                    new_Vetor(0, 2),    // velocidade apenas na vertical
                    new_Vetor(1, 3),    // (largura, altura) do tiro
                    idTexturaTiro   // Textura do tiro
                );
                append(&tiros, novoTiro);

                // nao pode atirar ate tirar o dedo do espaco
                podeAtirar=0;
            }
            

            break;
        case 114: // r
            liberarMemoria();
            reiniciar();
            break;

        case 112: // p
            // se estiver em jogo, pause
            if(gameState == InGame){
                gameState = Pause;
            }
            // se estiver em pause, play
            else if(gameState == Pause){
                gameState = InGame;
            }
            break;

        default:
            break;
    }
}

// Callback: usuario pressionou uma seta
void setaPressionada(int key, int x, int y) {
    // so muda se estiver em jogo
    if(gameState == InGame){
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

// Callback: usuario tirou o dedo de uma tecla
void teclaNaoPressionada(unsigned char key, int x, int y) {
    switch(key) {
        case 32:    // espaço
            podeAtirar = 1;
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
    // se estiver em jogo
    if(gameState == InGame){
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

        // contador eh uma variavel que controla o tempo para
        // que os inimigos mudem de direcao

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
            inimigo->velocidade.y = direcao.y * (0.25 * ((float)(fase)/3));
        }

        // remove todos os tiros que sairam da tela
        removeTirosFora();

        // remover tiros e inimigos que colidiram
        removeInimigosAcertados();

        // se nao tem mais inimigos, passa para proxima fase
        if (isEmpty(inimigos)){
            contador = 0;
            criarInimigos();
            fase += 1;
        }

        // se inimigo alcancou a nave ou a base no mundo, perder uma vida e destruir inimigo
        for(int i = getSize(inimigos) - 1; i >= 0; i--){
            ObjetoJogo * inimigo = getObjetoJogo(inimigos, i);
            if(objetoColideCom(nave, *inimigo) || inimigo->posicao.y < 0){
                vidas -= 1;
                pop(&inimigos, i);
            }
        }

        // se acabou as vidas, game over
        if (vidas <= 0){
            gameState=GameOver;
            liberarMemoria();
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
    glutKeyboardUpFunc(teclaNaoPressionada);
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
