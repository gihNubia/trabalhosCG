#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


// ------------ CONSTRUTORES ----------------//
ListaObjetos new_ListaObjetos(){
    ListaObjetos l;
    l.primeiro = NULL;
    l.ultimo = NULL;
    l.count = 0;

    return l;
}

ObjetoJogo new_ObjetoJogo(Vetor pos, Vetor vel, Vetor dim, int idTex){
    ObjetoJogo obj;
    obj.posicao = pos;
    obj.velocidade = vel;
    obj.dimensoes = dim;
    obj.idTextura = idTex;

    return obj;
}

ObjetoJogo new_ObjetoJogo_Vazio(){
    ObjetoJogo obj;
    obj.posicao = new_Vetor(0,0);
    obj.velocidade = new_Vetor(0,0);
    obj.dimensoes = new_Vetor(0,0);
    obj.idTextura = 0;

    return obj;
}

Vetor new_Vetor(float x, float y){
    Vetor v;
    v.x = x;
    v.y = y;

    return v;
}

// --------- METODOS DE ListaObjetos -------- //

ObjetoJogo * getObjetoJogo(ListaObjetos l, int posicao){
    if (posicao >= l.count || posicao < 0)
        return NULL;
    
    int contador = 0;
    No *noAtual = l.primeiro;

    while(contador < posicao)
    {
        noAtual = noAtual->proximo;
        contador += 1;
    }

    return &(noAtual->obj);
}

void append(ListaObjetos *l, ObjetoJogo obj){
    No *novoNo = malloc(sizeof(No));
    novoNo->proximo = NULL;
    novoNo->obj = obj;

    if(l->count == 0)
    {
        l->primeiro = novoNo;
        l->ultimo = novoNo;
    } else {
        l->ultimo->proximo = novoNo;
        l->ultimo = novoNo;
    }
    l->count += 1;
}

void pop(ListaObjetos *l, int posicao){
    if (posicao >= l->count || posicao < 0)
        return;
    
    if (posicao == 0){
        No *noDeletado = l->primeiro;
        if (l->count == 1)
            l->ultimo = NULL;
        l->primeiro = l->primeiro->proximo;

        free(noDeletado);
    } else {
        int contador = 0;
        No *noAtual = l->primeiro;

        while(contador < posicao - 1)
        {
            noAtual = noAtual->proximo;
            contador += 1;
        }

        No *noDeletado = noAtual->proximo;
        noAtual->proximo = noAtual->proximo->proximo;
        if(noAtual->proximo == NULL)
            l->ultimo = noAtual;

        free(noDeletado);
    }
    l->count -= 1;
}

void freeLista(ListaObjetos *l){
    No *noAtual = l->primeiro;
    while(noAtual != NULL){
        No *noDeletado = noAtual;
        noAtual = noAtual->proximo;
        free(noDeletado);
    }
    l->primeiro = NULL;
    l->ultimo = NULL;
    l->count = 0;
}

int getSize(ListaObjetos l){
    return l.count;
}

int isEmpty(ListaObjetos l){
    return l.count == 0 ? 1 : 0;
}

// --------- METODOS DE ObjetoJogo -------- //

void printObjetoJogo(ObjetoJogo obj){
    printf("Velocidade: ");
    printVetor(obj.velocidade);
    printf("\n");

    printf("Dimensoes: ");
    printVetor(obj.dimensoes);
    printf("\n");

    printf("Posicao: ");
    printVetor(obj.posicao);
    printf("\n");

    printf("Id: %i \n", obj.idTextura);
}

int objetoColideCom(ObjetoJogo obj1, ObjetoJogo obj2){
    if (obj1.posicao.x < obj2.posicao.x + obj2.dimensoes.x  // se houve colisao no eixo x
     && obj1.posicao.x + obj1.dimensoes.x > obj2.posicao.x
     && obj1.posicao.y < obj2.posicao.y + obj2.dimensoes.y  // e colisao no eixo y
     && obj1.posicao.y + obj1.dimensoes.y > obj2.posicao.y)
        return 1;
    return 0;
}

// --------- METODOS DE Vetor -------- //

void printVetor(Vetor vetor){
    printf("(%f, %f)", vetor.x, vetor.y);
}