CC = gcc
CFLAGS = -g
CLIBS = -lSDL2 -lSDL2_image
CINCLUDE = -I include

game: src/main.c vector.o entity.o
	$(CC) $(CFLAGS) src/main.c bin/vector.o bin/entity.o $(CLIBS) -o bin/game $(CINCLUDE)

vector.o: src/vector.c include/vector.h
	$(CC) $(CFLAGS) -c src/vector.c $(CLIBS) -o bin/vector.o $(CINCLUDE)

entity.o: src/entity.c include/entity.h
	$(CC) $(CFLAGS) -c src/entity.c $(CLIBS) -o bin/entity.o $(CINCLUDE)

clean:
	rm bin/*
