CC=gcc
CFLAGS=-I. -g
DEPS = common.h
OBJ = common.o matmul.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

matrixmul: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf ./*.o ./matrixmul
