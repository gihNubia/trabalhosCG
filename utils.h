#ifndef UTILS
#define UTILS

typedef struct
{
    float x;
    float y;
}Vetor;


typedef struct
{
    Vetor posicao;
    Vetor velocidade;
    Vetor dimensoes;
    int idTextura;
}ObjetoJogo;

typedef struct No
{
    struct No *proximo;
    ObjetoJogo obj;
}No;

typedef struct
{
    No *primeiro;
    No *ultimo;
    int count;
}ListaObjetos;

ListaObjetos new_ListaObjetos();
ObjetoJogo new_ObjetoJogo(Vetor pos, Vetor vel, Vetor dim, int id);
ObjetoJogo new_ObjetoJogo_Vazio();
Vetor new_Vetor(float x, float y);

ObjetoJogo get(ListaObjetos l, int pos);
void append(ListaObjetos * l, ObjetoJogo obj);
void pop(ListaObjetos * l, int pos);
void freeLista(ListaObjetos * l);
int getSize(ListaObjetos l);
int isEmpty(ListaObjetos l);

void printObjetoJogo(ObjetoJogo obj);
void printVetor(Vetor vetor);
#endif