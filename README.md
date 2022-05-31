# Galaxian
Trabalho 01 de Computação Gráfica

## Integrantes
- Erick Rocha
- Gisele Núbia

## Preparando o ambiente
Antes de começar, tenha certeza de que tem instalado: freeglut, glew, SOIL
```shell
# Compile o projeto
$ gcc -Wall -o galaxian main.c utils.c -lglew32 -lSOIL -lopengl32 -lfreeglut

# ou use Makefile
$ make

# Rode o projeto
$ .\galaxian.exe
```

## Lista de Itens Adicionais
- Relativas ao **jogo**:
  1. :star: **Fases infinitas (4%)**: gere uma nova fase toda vez que o jogador
     morrer ou que ele tenha êxito em aterrisar
  1. :star: **Pontuação (5%)**: determine uma forma de pontuar o jogador, por exemplo,
     cada inimigo vale uma quantidade de pontos dependendo do seu tipo e
     da circunstância (eg, quanto mais próximo do chão, mais pontos).
     - O placar deve ser exibido em uma _head-up display_ (**HUD**)
- Relativas à **nave**:
  1. :star: **Vidas (5%)**: em vez de morrer na primeira colisão, o jogador pode
     ter vidas
     - Deve haver uma _head-up display_ (**HUD**) mostrando a vida atual do jogador.
- Outras funcionalidades:
  1. :star: **Manter razão de aspecto (6%)**: faça com que a razão de aspecto do jogo
     seja sempre mantida, independente das dimensões da janela, mas que o
     jogo ocupe a maior área possível da janela e ficando centralizado
  1. :star: **Lista infinita de tiros**: não existe um limite para a quantidade de tiros que a nave pode dar simultaneamente