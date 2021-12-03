VPATH = include:src:glm:glm/gtc:glad

#OBJS = $(shell find ./src -name "*.cpp")
OBJS = main.o scene.o staticFurniture.o wall.o uglyBall.o blueGhost.o candel.o door.o fBullet.o fruit.o gun.o key.o pGun.o player.o whiteGhost.o

DEP = glm.hpp matrix_transform.hpp type_ptr.hpp glad.h

CC = g++

CFLAGS = -std=c++14 -c
LFLAGS = -Wall

LINKER_FLAGS = -L/usr/lib -Lglad
GL_FLAGS = -ldl -lSDL2

APP_NAME = GhostHouse

all: $(OBJS)
	$(CC) $(LINKER_FLAGS) $(LFLAGS) $(OBJS) ./glad/glad.c -o $(APP_NAME) $(GL_FLAGS)

main.o: main.cpp MyStruct.h Player.h StaticFurniture.h $(DEP)
	$(CC) $(CFLAGS) ./src/main.cpp

scene.o: Scene.cpp Scene.h GlobalVar.h $(DEP)
	$(CC) $(CFLAGS) ./src/Scene.cpp

staticFurniture.o: StaticFurniture.cpp StaticFurniture.h
	$(CC) $(CFLAGS) ./src/StaticFurniture.cpp

wall.o: Wall.cpp Wall.h
	$(CC) $(CFLAGS) ./src/Wall.cpp

uglyBall.o: UglyBall.cpp UglyBall.h
	$(CC) $(CFLAGS) ./src/UglyBall.cpp

blueGhost.o: BlueGhost.cpp BlueGhost.h
	$(CC) $(CFLAGS) ./src/BlueGhost.cpp

candel.o: Candel.cpp Candel.h
	$(CC) $(CFLAGS) ./src/Candel.cpp

door.o: Door.cpp Door.h $(DEP)
	$(CC) $(CFLAGS) ./src/Door.cpp

fBullet.o: FBullet.cpp FBullet.h
	$(CC) $(CFLAGS) ./src/FBullet.cpp

fruit.o: Fruit.cpp fruit.h $(DEP)
	$(CC) $(CFLAGS) ./src/Fruit.cpp

gun.o: Gun.cpp Gun.h
	$(CC) $(CFLAGS) ./src/Gun.cpp

key.o: Key.cpp Key.h $(DEP)
	$(CC) $(CFLAGS) ./src/Key.cpp

pGun.o: PGun.cpp PGun.h
	$(CC) $(CFLAGS) ./src/PGun.cpp

player.o: Player.cpp Player.h
	$(CC) $(CFLAGS) ./src/Player.cpp

whiteGhost.o: WhiteGhost.cpp WhiteGhost.h
	$(CC) $(CFLAGS) ./src/WhiteGhost.cpp


.PHONY: clean run

clean:
	rm *.o GhostHouse
