all:
	gcc -Wall -Werror -fPIC -c dll.c
	gcc -shared dll.o -lrt -o dll.so
	gcc -rdynamic -pthread -o exec main.c -ldl