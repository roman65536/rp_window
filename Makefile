CC = gcc -g -O6 
LIBS=-L/usr/X11R6/lib -lX11 
SRC= X11.c win.c button.c slider.c main.c
OBJ= X11.o win.o button.o slider.o main.o

.c.o:
	$(CC) -c $<

all:	$(OBJ) 
	$(CC) -g -o test $(OBJ) $(LIBS)


