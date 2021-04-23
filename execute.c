#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "execute.h"
#include "builtinfunctions.h"
#include "arguments.h"

int execute(Command* head) {
    if (head->node[0] == NULL){
        return 1;
    }

    for(int i = 0; i < number_builtins(); i++){
        if(strcmp(head->node[0], str_builtins(i)) == 0){
            return builtin_function(i, head->node);
        }
    }

    Command* current = head;

    // tu jest (powinien?) być proces w tle:
//    if (current->action == PROCESS_IN_BACKGROUND) {
//        pid_t pid = fork();
//
//        if (pid < 0) {
//            printf("Fork failed!\n");
//            exit(EXIT_FAILURE);
//        }
//
//        if (pid > 0) {
//            printf("Successfully forked\nPID: %d\n", pid);
//            exit(EXIT_SUCCESS);
//        }
//
//        else {
//            if (execvp(current->node[0], current->node) < 0) {
//                perror("Blad przy execvp.");
//                exit(EXIT_FAILURE);
//            }
//        }
//
//
//        return 1;
//    }


    int relations = 0, redirect_relations = 0;
    while (current->next != NULL) {
        relations++;
        if (current->action == ACTION_REDIRECT) {
            redirect_relations++;
        }
        current = current->next;
    }

    int status;
    int pipes_number = relations;
    if (redirect_relations > 0) {
        pipes_number = relations - 1;
    }

    int pipes[2 * pipes_number];

    for (int j = 0; j < pipes_number; j++) {
        pipe(pipes + j * 2);
    }

    int out;
    current = head;

    pid_t pid = fork();
    if (pid == 0) {
        if (pipes_number > 0 && current->action == ACTION_PIPE) {
            dup2(pipes[1], 1);
            for (int i = 0; i < 2 * pipes_number; i++) {
                close(pipes[i]);
            }
        }

        if (pipes_number == 0 && current->action == ACTION_REDIRECT) {
            if (strcmp(current->next->node[0], " ") == 0) {
                out = open(current->next->node[1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            }
            else {
                out = open(current->next->node[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            }
            dup2(out, 1);
            close(out);
        }

        if (execvp(current->node[0], current->node) == -1) {
            printf ("%s\n", current->node[0]);
            perror("Blad przy execvp.");
            exit(EXIT_FAILURE);
        }
    }

    else if (pid < 0) {
        perror("Blad przy forku.");
        exit(EXIT_FAILURE);
    }

    else {
        for (int i = 2; i <= pipes_number + 1; i++) {
            pid_t pid = fork();
            current = current->next;
            if (pid == 0) {
                if (i < pipes_number + 1 && current->action == ACTION_PIPE) {
                    dup2(pipes[2*(i-2)], 0);
                    dup2(pipes[2*(i-1) + 1], 1);
                }

                else if (i == pipes_number + 1 && current->action == NO_ACTION) {
                    dup2(pipes[2*(i-2)], 0);
                }

                else if (i == pipes_number + 1 && current->action == ACTION_REDIRECT) {
                    if (strcmp(current->next->node[0], " ") == 0) {
                        out = open(current->next->node[1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                    }
                    else {
                        out = open(current->next->node[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                    }
                    dup2(out, 1);
                    dup2(pipes[2*(i-2)], 0);
                    close(out);
                }
                for (int i = 0; i < 2 * pipes_number; i++) {
                    close(pipes[i]);
                }

                if(execvp(current->node[0], current->node) == -1){
                    perror("Blad przy execvp.");
                }
                exit(EXIT_FAILURE);
            }

            else if (pid < 0) {
                perror("Blad przy forku.");
                exit(EXIT_FAILURE);
            }
        }
    }

    // only the parent gets here and waits for 3 children to finish
    for (int i = 0; i < 2 * pipes_number; i++) {
        close(pipes[i]);
    }

    for (int i = 0; i < pipes_number + 1; i++)
        wait(&status);

    return 1;
}


//    tablica pipe:
//    wyjscie  wejście
//    pipe[0], pipe[1] -> 1. część     [2*(k-1))]   [2*(k-1) + 1] pipe_relations = k-1
//    pipe[2], pipe[3] -> 2. część
//    pipe[4], pipe[5] -> 3. część
//    pipe[6], pipe[7] -> 4. część
//    ...
//    pipe[2 * pipe_relations - 1], pipe[2 * pipe_relations - 1 = ] -> k. część

