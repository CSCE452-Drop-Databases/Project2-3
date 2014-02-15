# 		   MAKEFILE 
#		  --Authors--
#		Colton Williams
#	   Charles Kernstock
#		  Chris Murray
#	 	  Paul Gaughan
#		  Wesley Tang
# Department of Computer Science
#	  Texas A&M University
#


CC=g++

all: main

main: main.o paintarm.o matrix.o
	$(CC) main.o paintarm.o matrix.o -o main

main.o: main.cpp
	$(CC) -c -g main.cpp 

paintarm.o: paintarm.cpp
	$(CC) -c -g paintarm.cpp

matrix.o: matrix.cpp
	$(CC) -c -g matrix.cpp 

clean:
	rm -rf *.o main

	