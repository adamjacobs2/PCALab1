CC = mpicc
CFLAGS = -Wall
TARGETS = hello_world send_recv mpi_bcast


.PHONY: all clean


all: $(TARGETS)


mat_mult: mat_mult.c
	$(CC) $(CFLAGS) -o mat_mult mat_mult.c



clean:
	rm -f mat_mult

all: 
	mpicc -o hello_world hello_world.c
