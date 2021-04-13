#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"

#define READ_LINE_BUFSIZE 1024
#define SPLIT_TOKEN_BUFSIZE 64
#define TOKEN_DELIMETERS " \n\t\r\v\f#&"
// \n - nowy wiersz; \t - tablulacja; \f - nowa strona ...

// wczytuje całą linię wejścia
char* read_line(){
    // startowy blok pamięci;
    // jeśli zostanie przekroczony, trzeba będzie przydzielić
    // więcej miejsca
	int buffer_size = READ_LINE_BUFSIZE;
	char* buffer = malloc(sizeof(char) * buffer_size);

	if(!buffer){
        fprintf(stderr, "Blad alokacji\n");
        exit(EXIT_FAILURE);
	}

	int c;
	int i = 0;

	while(1 == 1){
	    c = getchar();

	    // jeśli koniec linii: przerwij bieżący ciąg pustym znakiem i zwróc go
	    if(c == EOF || c == '\n'){
	        buffer[i] = '\0';
	        return buffer;
	    }
	    // lub dodaj znak do aktualnego ciągu
	    else{
	        buffer[i] = c;
	    }
	    i++;

        // w razie przekroczenia przydzielonego bloku pamięci
        // dla naszego bufora dokonuje realokacji:
        if(i >= buffer_size){
            buffer_size += READ_LINE_BUFSIZE;
            buffer = realloc(buffer, buffer_size);
            if(!buffer){
                fprintf(stderr, "Blad alokacji\n");
                exit(EXIT_FAILURE);
            }
        }
	}
}

// dzieli wczytaną linię na listę argumentów
char** split_line(char* line){
    int buffer_size = SPLIT_TOKEN_BUFSIZE;
    char** tokens = malloc(sizeof(char*) * buffer_size);
    char* token;

    if(!tokens){
        fprintf(stderr, "Blad alokacji\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    token = strtok(line, TOKEN_DELIMETERS);


    while(token != NULL){
        tokens[i] = token;
        i++;
        if(i >= buffer_size){
            buffer_size += SPLIT_TOKEN_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * buffer_size);
            if(!token){
                fprintf(stderr, "Blad alokacji\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOKEN_DELIMETERS);
    }
    tokens[i] = NULL;

    return tokens;
}