# 		   MAKEFILE 
#		  --Authors--
#		  Wesley Tang
# Department of Computer Science
#	  Texas A&M University
#


CC=g++

all: main

matrix.o: matrix.h matrix.cpp
	$(CC) -c -g matrix.cpp 

main: main.cpp matrix.o
	$(CC) main.cpp matrix.o -o main

clean:
	rm -rf *.o main