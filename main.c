#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "arguments.h"
#include "execute.h"
#include "history.h"
#include "builtinfunctions.h"

void SIGQUIT_handler(int signum) {
    builtin_function(1, NULL);
    printf("\nNacisnij spacje, by kontynuowac..\n");
}


void loop() {
    if (signal(SIGQUIT, SIGQUIT_handler) == SIG_ERR) {
        printf("Blad przy obsludze sygnalu. ");
        switch (errno) {
            case EINVAL: printf("Bledny numer sygnalu. \n");
                break;
        }
    }

    int status;
    char *line;
    char **args;

    History *history_head = NULL;

    do {
        printf("> ");
        line = read_line();

        int fd = open ("~/history.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        if (fd == -1) {
            fprintf (stderr , "Nie mozna otworzyc pliku. \n");
            exit(EXIT_FAILURE);
        }
        history_head = update_history(history_head, line, fd);
        close(fd);

        args = tokenize(line, " |>&#");
        Command *c = create_commands(args);
        status = execute(c);
        free(line);
        free(args);
    } while(status);
}


int main(int argc, char *argv[]) {
    loop();

	return EXIT_SUCCESS;
}

