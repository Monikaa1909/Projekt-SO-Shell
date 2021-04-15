#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"

#define READ_LINE_BUFSIZE 1024

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