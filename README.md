# Galaxian
Trabalho 01 de Computação Gráfica

## Preparando o ambiente
Antes de começar, tenha certeza de que tem instalado: freeglut, glew, SOIL
```shell
# Compile o projeto
$ gcc -o galaxian .\main.c .\utils.c -lfreeglut -lglew32 -lopengl32

# Rode o projeto
$ .\galaxian.exe
```

## Documentação

**utils.h**
----
A biblioteca utils.h possui implementações para facilitar a manipulação de listas e objetos de jogo.

* **Structs**
    
    [Vetor](#vetor)
  
    [ObjetoJogo](#objetojogo)

    [No](#no)

    [ListaObjetos](#listaobjetos)

* **Funções**
    
    [new_Vetor(float x, float y): Vetor](#new_vetor)

    [new_ObjetoJogo(Vetor pos, Vetor vel, Vetor dim, int id): ObjetoJogo](#new_objetojogo)

    [new_ObjetoJogo_Vazio(): ObjetoJogo](#new_objetojogo_vazio)

    [new_ListaObjetos(): ListaObjetos](#new_listaobjetos)

    [getObjetoJogo(ListaObjetos l, int pos): ObjetoJogo*](#getobjetojogo)

    [append(ListaObjetos * l, ObjetoJogo obj): void](#append)

    [pop(ListaObjetos * l, int pos): void](#pop)

    [freeLista(ListaObjetos * l): void](#freelista)

    [getSize(ListaObjetos l): int](#getsize)

    [isEmpty(ListaObjetos l): int](#isempty)

**Vetor**
----
Armazena dois valores float. Pode ser usado para guardar posição, velocidade, força, aceleração...
* **Variáveis:**

    `x=[float]`
  
    `y=[float]`

**ObjetoJogo**
----
Representação genérica de um objeto do jogo. Pode ser usado para representar nave, tiros e inimigos.
* **Variáveis:**

    `posicao=[Vetor]`
  
    `velocidade=[Vetor]`

    `dimensoes=[Vetor]`

    `idTextura=[int]`

**No**
----
Estrutura auxiliar para construir listas. NÃO CRIE ESSAS VARIÁVEIS POR CONTA PRÓPRIA, USE AS FUNÇÕES.
* **Variáveis:**

    `proximo=[No*]`
  
    `obj=[ObjetoJogo]`

**ListaObjetos**
----
Representa uma lista de Objetos de tamanho dinâmico. NÃO CRIE ESSAS VARIÁVEIS POR CONTA PRÓPRIA, USE AS FUNÇÕES. NÃO ALTERE NEM ACESSE AS VARIÁVEIS DA LISTA DIRETAMENTE, USE AS FUNÇÕES.
* **Variáveis:**

    `primeiro=[No*]`
  
    `ultimo=[No*]`

    `count=[int]`

**new_Vetor**
----
* **Vetor new_Vetor(float x, float y)**
Retorna um vetor com os parâmetros fornecidos.
* **Parâmetros:**
  
    `x=[float]`

    `y=[float]`

* **Resposta:**
    ```
    Vetor{x=x, y=y}
    ```
      
* **Exemplo de uso:**

    ```
    // criando vetores
    Vetor pos = new_Vetor(65, 77);
    Vetor vel = new_Vetor(1.5, 3.0);
    Vetor dim = new_Vetor(10, 7);
    ```

**new_ObjetoJogo**
----
* **ObjetoJogo new_ObjetoJogo(Vetor pos, Vetor vel, Vetor dim, int id)**
Retorna um ObjetoJogo com os parâmetros fornecidos.
* **Parâmetros:**
  
    `pos=[Vetor]`
  
    `vel=[Vetor]`

    `dim=[Vetor]`

    `id=[int]`

* **Resposta:**
    ```
    ObjetoJogo{posicao=pos, velocidade=vel, dimensoes=dim, idTextura=id}
    ```
      
* **Exemplo de uso:**

    ```
    // criando novo objeto
    ObjetoJogo meuObjeto = new_ObjetoJogo(pos, vel, dim, 55);
    ```

**new_ObjetoJogo_Vazio**
----
* **ObjetoJogo new_ObjetoJogo_Vazio()**
Retorna um ObjetoJogo com os parâmetros zerados.
* **Parâmetros:**
  
    Nenhum

* **Resposta:**
    ```
    ObjetoJogo{posicao=Vetor{x=0,y=0}, velocidade=Vetor{x=0,y=0}, dimensoes=Vetor{x=0,y=0}, idTextura=0}
    ```
      
* **Exemplo de uso:**

    ```
    // criando novo objeto zerado
    ObjetoJogo meuObjeto = new_ObjetoJogo_Vazio();
    ```

**new_ListaObjetos**
----
* **ListaObjetos new_ListaObjetos()**
Retorna uma Lista de Objetos vazia.
* **Parâmetros:**
  
    Nenhum

* **Resposta:**
    ```
    ListaObjetos{primeiro=NULL, ultimo=NULL, count=0}
    ```
      
* **Exemplo de uso:**

    ```
    // criando nova lista
    ListaObjetos minhaLista = new_ListaObjetos();
    ```

**getObjetoJogo**
----
* **(ObjetoJogo*) getObjetoJogo(ListaObjetos l, int pos)**

Retorna um PONTEIRO para o Objeto na posição informada. Se posição estiver fora do limite da lista retorna NULL.
* **Parâmetros:**
  
    `l=[ListaObjetos]`
  
    `pos=[int]`

* **Resposta:**
    ```
    0x04232 (nesse endereço existe um Objeto)
    ```
      
* **Exemplo de uso:**

    ```
    // recuperando segundo Objeto da lista (posicao 1)
    ObjetoJogo * segundoObjeto = getObjetoJogo(minhaLista, 1);

    // editando o objeto
    segundoObjeto->idTextura = 23;
    // dobrando a velocidade
    segundoObjeto->velocidade = new_Vetor(segundoObjeto->velocidade.x*2, segundoObjeto->velocidade.y*2);
    ```

**append**
----
* **void append(ListaObjetos * l, ObjetoJogo obj)**

Adiciona um novo ObjetoJogo ao final da lista.
* **Parâmetros:**
  
    `l=[ListaObjetos*]`
  
    `obj=[ObjetoJogo]`

* **Resposta:**
    Nenhum
      
* **Exemplo de uso:**

    ```
    // adicionando obj ao final da minhaLista
    append(&minhaLista, obj);
    ```

**pop**
----
* **void pop(ListaObjetos * l, int pos)**

Remove o objeto da posição informada. Se posição estiver fora do limite da lista não faz nada.
* **Parâmetros:**
  
    `l=[ListaObjetos*]`
  
    `pos=[int]`

* **Resposta:**
    Nenhum
      
* **Exemplo de uso:**

    ```
    // removendo segundo objeto (posição 1) da minhaLista
    pop(&minhaLista, 1);
    ```

**freeLista**
----
* **void freeLista(ListaObjetos * l)**

Remove todos os objetos da lista. DEVE SER CHAMADO SEMPRE ANTES DO FIM DO PROGRAMA.
* **Parâmetros:**
  
    `l=[ListaObjetos*]`

* **Resposta:**
    Nenhum
      
* **Exemplo de uso:**

    ```
    // SEMPRE libere a lista quando terminar de usar
    freeLista(&minhaLista);
    ```

**getSize**
----
* **int getSize(ListaObjetos l)**

Retorna a quantidade de objetos da lista.
* **Parâmetros:**
  
    `l=[ListaObjetos]`

* **Resposta:**
    ```
    10
    ```
      
* **Exemplo de uso:**

    ```
    // i varia de 0 até tamanho da minhaLista
    for (int i = 0; i < getSize(minhaLista); i++){...
    ```

**isEmpty**
----
* **int isEmpty(ListaObjetos l)**

Checa se lista está vazia. Se estiver vazia retorna 1, senão retorna 0.
* **Parâmetros:**
  
    `l=[ListaObjetos]`

* **Resposta:**
    ```
    1
    ```
      
* **Exemplo de uso:**

    ```
    if(isEmpty(minhaLista))
        printf("Minha lista esta vazia\n");
    ```