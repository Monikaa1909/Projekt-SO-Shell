#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"
#include "tokenizer.h"
#include "execute.h"
#include "builtins.h"

void loop(){
    char* line;
    char** args;
    int status;

    do{
        printf("> ");
        line = read_line();
        args = split_line(line, " ");
        args = tokenize(args, "|");
        //args = tokenize(args, ">");

        for(int j = 0; j < sizeof(args) * sizeof(char*); j++){
            if(args[j]!=NULL)printf("%s,", args[j]);
        }
        printf("\n");

        status = execute(args);

        free(line);
        free(args);
    } while(status);
}


int main(int argc, char *argv[]){
	loop();

	return EXIT_SUCCESS;
}