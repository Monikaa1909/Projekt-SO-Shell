#pragma once

#ifndef TOKENIZER_H
#define TOKENIZER_H

#define NO_ACTION 0
#define ACTION_PIPE 1
#define ACTION_REDIRECT 2
#define PROCESS_IN_BACKGROUND 3

#define READ_LINE_BUFSIZE 1024
#define TOKEN_BUFSIZE 64

typedef struct Commands_list Command;

struct Commands_list {
    char **node;                // argumenty konkretnej komendy (np.: ["ls", "-1"] )
    Command *next;              // wskaźnik na kolejną komendę
    int action;                 // rodzaj "połączenia" z kolejną komendą (NO_ACTION, PIPE, REDIRECT lub PROCESS_IN_BACKGROUND)
};

char *read_line();
char **tokenize();
Command *create_commands();

#endif
