all: clean my_shell

my_shell: main.o arguments.o execute.o history.o builtinfunctions.o
	gcc -g -Wall -o my_shell main.o execute.o history.o builtinfunctions.o arguments.o

main.o: main.c arguments.h execute.h history.h builtinfunctions.h
	gcc -g -Wall -c main.c

arguments.o: arguments.c arguments.h
	gcc -g -Wall -c arguments.c

execute.o: execute.c execute.h arguments.h builtinfunctions.h
	gcc -g -Wall -c execute.c

history.o: history.c history.h
	gcc -g -Wall -c history.c

builtinfunctions.o: builtinfunctions.c builtinfunctions.h
	gcc -g -Wall -c builtinfunctions.c

clean:
	rm -f my_shell *.o