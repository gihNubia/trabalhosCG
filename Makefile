all: galaxian

galaxian: main.cpp utils.c
	gcc -o galaxian .\main.cpp .\utils.c -lglew32 -lSOIL -lopengl32 -lfreeglut