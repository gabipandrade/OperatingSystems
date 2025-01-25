all:

	gcc -o main main.c -pthread
	./main 500 10 2 3 5 5 4
