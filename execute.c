#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "execute.h"
#include "builtins.h"

typedef struct Nazwa {
    // definicja struktury
} Nazwa;

int launch(char **args){
    pid_t pid;
    int status;

    // tworzę 'potomny' proces:
    pid = fork();

    if(pid == 0){
        // jeśli to "child process" chcę wykonać komendy dane przez
        // użytkownika, więc za pomocą exec() zastępuję ten proces nowym
        // -----
        // execvp() oczekuje nazwę programu i wektor z argumentami (utworzony
        // za pomocą funkcji split_line()); podajemy samą nazwę programu
        // bez pełnej ścieżki
        if(execvp(args[0], args) == -1){
            perror("Blad przy execvp");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0){
        perror("Blad przy forku");
    }
    else{
        // "parent process"; czeka na komendę od dziecka, że może zakończyć działanie;
        // ponieważ proces potomny może zmienić stan na różne sposoby (zakończony, zakończony
        // z błędem, zabity przez sygnał) używamy makra dostarczane przez waitpid()
        // żeby zaczekać do odpowiedniego momentu i przekazać funkcji wywołującej wartość 1
        // jako sygnał żeby ponownie poprosiła o wprowadzenie danych
        do{
            waitpid(pid, &status, WUNTRACED);
            // WUNTRACED - flaga określa, że waitpid powinien zgłosić stan wszystkich procesów,
            // które zostały zatrzymany, ale też tych, które zostały zakończone
        }while(!WIFEXITED(status) && !WIFSIGNALED(status));
            // WIFEXITED - zwraca wartość różną od 0, jeśli proces potomny zakończyło polecenie exit lub _exit
            // WIFSIGNALED - zwraca wartość różną od 0, jeśli proces potomny został zakończony, ponieważ
            // otrzymał sygnał, który nie został obsłużony
    }
    return 1;
}

int execute(char **args){
    if(args[0] == NULL){
        return 1;
    }
    for(int i = 0; i < number_builtins(); i++){
        if(strcmp(args[0], str_builtins(i)) == 0){
            return builtin_function(i,args);
        }
    }
    return launch(args);
}