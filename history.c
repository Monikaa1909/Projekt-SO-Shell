#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "history.h"

int command_counter = 0;

History *update_history(History *head, char *line, int fd) {
    if (head == NULL) {
        head = (History*) malloc(sizeof(History));
        head->node = malloc(sizeof(char) * strlen(line));
        if(!head->node) {
            fprintf(stderr, "Blad alokacji.\n");
            exit(EXIT_FAILURE);
        }
        strncpy(head->node, line, strlen(line));
        head->next = NULL;
        command_counter++;
    }

    else {
        History *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = (History*) malloc(sizeof(History));
        current->next->node = malloc(sizeof(char) * strlen(line));
        if(!current->node) {
            fprintf(stderr, "Blad alokacji.\n");
            exit(EXIT_FAILURE);
        }
        strncpy(current->next->node, line, strlen(line));
        current->next->next = NULL;

        command_counter++;
    }

    if (command_counter > 20) {
        head = head->next;
    }

    History *current = head;
    while (current != NULL) {
        write(fd, current->node, strlen(current->node));
        write(fd, "\n", 1);

        current = current->next;
    }

    return head;
}