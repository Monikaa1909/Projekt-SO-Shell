#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "tokenizer.h"

#define SPLIT_TOKEN_BUFSIZE 64
#define TOKEN_DELIMETERS " |>"

// dzieli wczytaną linię na listę argumentów
char** split_line(char* line, char* delimeter){
    int buffer_size = SPLIT_TOKEN_BUFSIZE;
    char** tokens = malloc(sizeof(char*) * buffer_size);
    char* token;

    if(!tokens){
        fprintf(stderr, "Blad alokacji\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    token = strtok(line, delimeter);
    tokens[i++] = token;

    token = strtok(NULL, delimeter);

    while (token != NULL) {
        if(i + 2 >= buffer_size){
            buffer_size += SPLIT_TOKEN_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * buffer_size);
            if(!token) {
                fprintf(stderr, "Blad alokacji\n");
                exit(EXIT_FAILURE);
            }
        }
        tokens[i++] = delimeter;
        tokens[i++] = token;
        token = strtok(NULL, delimeter);
    }
    return tokens;
}

// dzieli wczytaną linię na listę argumentów
char** tokenize(char** tokens, char* delimeter){
    int buffer_size = SPLIT_TOKEN_BUFSIZE;
    char** newtokens = malloc(sizeof(char*) * buffer_size);
    char* token;

    if(!tokens){
        fprintf(stderr, "Blad alokacji\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
//    for(int j = 0; j < sizeof(tokens); j++){
//        if(tokens[j] != NULL){
//            token = strtok(tokens[j], delimeter);
//            newtokens[i++] = token;
//
//            token = strtok(NULL, delimeter);
//            while(token != NULL){
//                if(i + 2 >= buffer_size){
//                    buffer_size += SPLIT_TOKEN_BUFSIZE;
//                    newtokens = realloc(tokens, sizeof(char*) * buffer_size);
//                    if(!token) {
//                        fprintf(stderr, "Blad alokacji\n");
//                        exit(EXIT_FAILURE);
//                    }
//                }
//                newtokens[i++] = delimeter;
//                newtokens[i++] = token;
//                token = strtok(NULL, delimeter);
//            }
//        }
//    }


    int j = 0;
    while(tokens[j] != NULL){
        token = strtok(tokens[j], delimeter);
        newtokens[i++] = token;

        token = strtok(NULL, delimeter);
        while(token != NULL){
            if(i + 2 >= buffer_size){
                buffer_size += SPLIT_TOKEN_BUFSIZE;
                newtokens = realloc(tokens, sizeof(char*) * buffer_size);
                if(!token) {
                    fprintf(stderr, "Blad alokacji\n");
                    exit(EXIT_FAILURE);
                }
            }
            newtokens[i++] = delimeter;
            newtokens[i++] = token;
            token = strtok(NULL, delimeter);
        }
        j++;
    }

//    printf("Tokens: ");
//    for(int j = 0; j < sizeof(newtokens); j++){
//        if(newtokens[j] != NULL) printf("%s,", newtokens[j]);
//    }
//    printf("\n");

    return newtokens;
}
