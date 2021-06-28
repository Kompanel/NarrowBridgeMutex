OBJ = main.o bridge.o queue.o
all: bridge
bridge: $(OBJ)
	gcc $(OBJ) -pthread -o bridge
$(OBJ): clean
clean:
	rm -f *.o bridge