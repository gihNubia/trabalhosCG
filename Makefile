all: galaxian

galaxian: main.c utils.c
	gcc -Wall -o galaxian main.c utils.c -lglew32 -lSOIL -lopengl32 -lfreeglut