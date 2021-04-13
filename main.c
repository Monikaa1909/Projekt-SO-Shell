#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"
#include "execute.h"

void loop(){
    char* line;
    char** args;
    int status;

    do{
        printf("> ");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    }while(status);
}

int main(int argc, char *argv[]){
	//printf("Hello from main!\n");

	loop();

	return EXIT_SUCCESS;
}
