CARGS=-lSDL2 -lSDL2_image

compile: Game.cpp
	g++ Game.cpp $(CARGS) -o Game

test: compile
	./Game