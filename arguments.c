#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

#include "arguments.h"

char *read_line(){
    int buffer_size = READ_LINE_BUFSIZE;
    char* buffer = malloc(sizeof(char) * buffer_size);

    if(!buffer){
        fprintf(stderr, "Blad alokacji.\n");
        exit(EXIT_FAILURE);
    }

    int c, i = 0;

    while (1 == 1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[i] = '\0';
            return buffer;
        }
        else {
            buffer[i] = c;
        }

        i++;

        if (i >= buffer_size) {
            buffer_size += READ_LINE_BUFSIZE;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Blad alokacji.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}


char **tokenize(char *line, char *delimeters) {
    if (line == NULL) {
        return NULL;
    }

    int buffer_size = TOKEN_BUFSIZE;
    char** tokens = malloc(sizeof(char*) * buffer_size);
    if(!tokens){
        fprintf(stderr, "Blad alokacji\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    size_t length = strlen(line);
    size_t index = strcspn(line, delimeters);       // index pierwszego wystąpienia, któregoś z 'delimetrów' w linii
    while (index != length) {
        if (index + 1 >= buffer_size) {
            buffer_size += TOKEN_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * buffer_size);
            if(!tokens) {
                fprintf(stderr, "Blad alokacji\n");
                exit(EXIT_FAILURE);;
            }
        }

        char* left = malloc(sizeof(char) * index);
        strncpy(left, line, index);

        if (left[0] != '\0') {
            tokens[i++] = left;
        }

        tokens[i] = malloc(sizeof(char) * 2);
        tokens[i][0] = line[index];
        tokens[i++][1] = '\0';

        length -= index + 1;
        char* right = malloc(sizeof(char) * length);
        strncpy(right, line + index + 1, length);

        line = right;

        index = strcspn(line, delimeters);
    }

    if (line[0] != '\0') {
        tokens[i++] = line;
    }

    tokens[i] = NULL;

    return tokens;
}


Command *create_commands(char** args){
    Command *head, *current;
    head = NULL;
    int buffer_size = TOKEN_BUFSIZE;
    bool fault = false;

    current = (Command*)malloc(sizeof(Command));
    current->next = NULL;
    current->action = NO_ACTION;
    current->node = malloc(sizeof(char*) * buffer_size);
    if (!current->node) {
        fprintf(stderr, "Blad alokacji.\n");
        exit(EXIT_FAILURE);
    }

    int i = 0, node_lenght = 0;
    while(args[i] != NULL){
        // spacje pomijam:
        if (strcmp(args[i], " ") == 0) {
            i++;
        }

        // jeśli to 'znak specjalny' -> ustawiam odpowiednią akcję i dodaję dotychczas stworzony element do listy:
        else if (strcmp(args[i], "|") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], "&") == 0) {

            if (strcmp(args[i], "|") == 0) {

                // obsłużenie błędów składniowych w podanym przez użytkownika poleceniu:
                int j = i + 1;
                fault = true;
                while (args[j] != NULL) {
                    if (args[j] != NULL && strcmp(args[j], " ") != 0 && strcmp(args[j], "&") != 0) fault = false;
                    j++;
                }
                if (fault == true) {
                    fprintf(stderr, "Blad w skladni. Brakuje argumentow po \"|\"\n");
                    if (head == NULL) {
                        head = (Command*)malloc(sizeof(Command));
                        head->node = malloc(sizeof(char*) * buffer_size);
                    }
                    head->node[0] = NULL;
                    return head;
                }

                current->action = ACTION_PIPE;
                i++;
            }

            else if (strcmp(args[i], ">") == 0) {

                // obsłużenie błędów składniowych w podanym przez użytkownika poleceniu:
                int j = i + 1;
                fault = true;
                while (args[j] != NULL) {
                    if (args[j] != NULL && strcmp(args[i+1], " ") != 0 && strcmp(args[i+1], "&") != 0) fault = false;
                    j++;
                }

                if (fault == true) {
                    fprintf(stderr, "Blad w skladni1. Nie zapomniales o \">\"?\n");
                    if (head == NULL) {
                        head = (Command*)malloc(sizeof(Command));
                        head->node = malloc(sizeof(char*) * buffer_size);
                    }
                    head->node[0] = NULL;
                    return head;
                }

                if (strcmp(args[i+1], ">") != 0) {
                    fprintf(stderr, "Blad w skladni2. Nie zapomniales o \">\"?\n");
                    if (head == NULL) {
                        head = (Command*)malloc(sizeof(Command));
                        head->node = malloc(sizeof(char*) * buffer_size);
                    }
                    head->node[0] = NULL;
                    return head;
                }

                else {
                    int j = i + 2;
                    fault = true;
                    while (args[j] != NULL) {
                        if (args[j] != NULL && strcmp(args[j], " ") != 0 && strcmp(args[j], "&") != 0) fault = false;
                        j++;
                    }

                    if (fault == true) {
                        fprintf(stderr, "Blad w skladni. Nie podales pliku, do ktorego chcesz przekierowac wyjscie.\n");
                        if (head == NULL) {
                            head = (Command*)malloc(sizeof(Command));
                            head->node = malloc(sizeof(char*) * buffer_size);
                        }
                        head->node[0] = NULL;
                        return head;
                    }
                }

                current->action = ACTION_REDIRECT;
                i += 2;
            }

            else if (strcmp(args[i], "&") == 0) {
                current->action = PROCESS_IN_BACKGROUND;
                break;
            }

            current->node[node_lenght] = NULL;
            node_lenght = 0;

            if (head == NULL) {
                head = current;
                head->node = current->node;
            }

            else {
                Command *pointer = head;
                while (pointer->next != NULL) {
                    pointer = pointer->next;
                }
                pointer->next = current;
                pointer->next->node = current->node;
            }

            current = (Command*)malloc(sizeof(Command));
            current->next = NULL;
            current->action = NO_ACTION;
            current->node = malloc(sizeof(char*) * buffer_size);
            if(!current->node){
                fprintf(stderr, "Blad alokacji\n");
                exit(EXIT_FAILURE);
            }
        }

        else {
            if (node_lenght + 1 >= buffer_size){
                buffer_size += TOKEN_BUFSIZE;
                current->node = realloc(current->node, sizeof(char*) * buffer_size);
                if(!current->node) {
                    fprintf(stderr, "Blad alokacji\n");
                    exit(EXIT_FAILURE);;
                }
            }
            current->node[node_lenght] = args[i];
            node_lenght++;
            i++;
        }
    }

    // skończyły się argumenty, więc dodaje ostatni element do listy:
    if (head == NULL) {
        head = current;
        head->node = current->node;
    }

    else if(head != NULL) {
        Command *pointer = head;
        while (pointer->next != NULL) {
            pointer = pointer->next;
        }
        pointer->next = current;
        pointer->next->node = current->node;
    }

    return head;
}

// tylko do testów:
//void WypiszAST(Command *head){
//    Command* current = head;
//    int i = 0;
//    while (current != NULL) {
//        printf("Element nr %d: \n", i++);
//        printf("Nodes: [");
//        int j = 0;
//        while (current->node[j] != NULL) {
//            printf("`%s`", current->node[j++]);
//        }
//        printf("], Actions: %d\n", current->action);
//        current = current->next;
//    }
//}