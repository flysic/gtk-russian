CC = gcc
FLAGS = -W -Wall -g -export-dynamic `pkg-config --cflags --libs gtk+-2.0` 

dataBase.o:dataBase.c dataBase.h support.h 
	$(CC) -c dataBase.c $(FLAGS)

diamond.o:diamond.c diamond.h dataBase.h support.h
	$(CC) -c dataBase.o diamond.c $(FLAGS)

mass.o:dataBase.o mass.c mass.h dataBase.h support.h
	$(CC) -c dataBase.o mass.c $(FLAGS)

russian:diamond.o mass.o dataBase.o russian.c support.h dataBase.h
	$(CC) diamond.o mass.o dataBase.o russian.c -o russian $(FLAGS)
