CC=g++
OS := $(shell uname)
ifeq ($(OS), Darwin)
  # Mac OS
  FRAMEWORKS=-framework GLUT -framework OpenGL
else
  # Not Mac OS
endif

all: PaintRobot

PaintRobot: PaintRobot.o PaintArm.o Matrix.o
	$(CC) PaintRobot.o PaintArm.o Matrix.o -o PaintRobot $(FRAMEWORKS)

PaintRobot.o: PaintRobot.cpp
	$(CC) -c -g PaintRobot.cpp 

PaintArm.o: PaintArm.cpp
	$(CC) -c -g PaintArm.cpp

Matrix.o: Matrix.cpp
	$(CC) -c -g Matrix.cpp 

clean:
	rm -rf *.o PaintRobot
