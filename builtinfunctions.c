#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "builtinfunctions.h"

// deklaracje funkcji dla wbudowanych poleceń powłoki:
int builtin_cd(char** args);
int builtin_history (char** args);
int builtin_exit(char **args);

// lista wbudowanych poleceń z odpowiadającymi im funkcjami:
char* builtin_commands_str[] = {
        "cd",
        "history",
        "exit"
};

int(*builtin_functions[]) (char**) = {
        &builtin_cd,
        &builtin_history,
        &builtin_exit
};

int number_builtins(){
    return sizeof(builtin_commands_str) / sizeof(char*);
}

char* str_builtins(int i){
    return  builtin_commands_str[i];
}

int builtin_function(int i, char** args){
    return(*builtin_functions[i])(args);
}

int builtin_cd(char** args){
    if(args[1] == NULL){
        fprintf(stderr, "Brakuje argumentu przy komendzie cd.");
    }
    else{
        if(chdir(args[1]) != 0){
            perror("Blad przy wykonywaniu chdir.");
        }
    }
    return 1;
}

int builtin_history(char** args){
    FILE *fp = fopen ("~/history.txt","r");;
    if (fp == NULL) {
        fprintf (stderr , "Nie mozna otworzyc pliku. \n");
        return 1;
    }

    char buffer[255];
    printf("\n");
    while (fgets(buffer, 255,fp) != NULL) {
        printf("%s", buffer);
    }
    fclose(fp);
    return 1;
}

int builtin_exit(char** args){
    return 0;
}