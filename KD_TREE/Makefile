VPATH = source  \
		

SIM  = sim

GFLAGS = -lglut -lGLU -lGL
IFLAGS = -I include
MFLAGS = -lm
CFLAGS = -Wall -g3
#MEMFLAGS = -fsanitize=address

CC = gcc $(IFLAGS) $(CFLAGS)
SOURCE = $(wildcard source/*.c)
OBJECT = $(subst .c,.o,$(SOURCE))

run: $(OBJECT) 
	$(CC) $(^) $(GFLAGS) $(MFLAGS) $(MEMFLAGS) -o $(SIM)

%.o : %.c
	$(CC) -c $< -o $(@) 
clean:
	rm source/*.o

