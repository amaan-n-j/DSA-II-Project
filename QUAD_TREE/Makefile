SIM  = sim

GFLAGS = -lglut -lGLU -lGL
IFLAGS = -I
MFLAGS = -lm
CFLAGS = -Wall -Ofast
#MEMFLAGS = -fsanitize=address

CC = gcc $(IFLAGS) $(CFLAGS)
SOURCE = $(wildcard *.c)
OBJECT = $(subst .c,.o,$(SOURCE))

run: $(OBJECT)
	$(CC) $(SOURCE) $(GFLAGS) $(MFLAGS) $(MEMFLAGS) -o $(SIM)

clean:
	rm *.o

