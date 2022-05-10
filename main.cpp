#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>

struct pontoNave{
    int x;
    int y;
};

GLuint texturaNave;


struct pontoNave posNave;

GLuint carregaTextura(char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
        arquivo,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    return idTextura;
}

void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 1, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaNave);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(posNave.x-5, posNave.y-5);
        glVertex3f(posNave.x-6, posNave.y-6, 0); //baixo direito

        glTexCoord2f(posNave.x-5, posNave.y+5);
        glVertex3f(posNave.x-6, posNave.y+6, 0); //cima direito

        glTexCoord2f(posNave.x+5, posNave.y+5);
        glVertex3f(posNave.x+6, posNave.y+6, 0); //cima esquerdo

        glTexCoord2f(posNave.x+5, posNave.y-5);
        glVertex3f(posNave.x+6, posNave.y-6, 0); //baixo esquerdo
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //glutSwapBuffers();
    glFlush();
}

void inicializa() {
    glClearColor(1, 1, 1, 1); // branco
    posNave.x = 50;
    posNave.y = 5;

    texturaNave = carregaTextura("pixel-art-spacecraft.png");
}

void redimensionada(int width, int height) {
   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 100, 0, 100, -1, 1);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y) {
    switch(key) {
    case 27:
        exit(0);
        break;
    case 100:  //seta direita
        posNave.x+=5;
        //tratar para quando as coordenadas ultrapassarem o gortho
        desenhaMinhaCena();
        break;
    case 97: //seta esquerda
        //tratar para quando as coordenadas ultrapassarem o gortho
        posNave.x-=5;
        desenhaMinhaCena();
        break;

    default:
        break;
    }
    //glutPostRedisplay();
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);

   glutInitContextVersion(1, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);

   glutCreateWindow("Movimentacao Horizontal");

   glutDisplayFunc(desenhaMinhaCena);
   glutReshapeFunc(redimensionada);
   glutKeyboardFunc(teclaPressionada);

   inicializa();

   glutMainLoop();
   return 0;
}
