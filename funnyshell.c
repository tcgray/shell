// Author - Thomas Gray

// The following code was designed by the author and is NOT allowed to be distributed, shared,
// or otherwise referenced without direct, explicit permission from the author.

// This file processes creates a custom shell that utilizes features from built-in commands, as well
// as introducing some of our own commands that have unique and practical uses.

// Background processing, a background process converter, an output redirection algorithm, and a child
// process tracker are all some of the extra implementations this program offers.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_SIZE 64

struct childprocs { // struct used in array for tracking listchildprocs
    int pid;
    char command[MAX_SIZE];
};

void reverseArray(char * args[], int start, int end) { // reverses array to make easier to use with execlp() and overall command processing
    char *temp;
    while (start < end) {
        temp = args[start];
        args[start] = args[end];
        args[end] = temp;
        start++;
        end--;
    }
}

void redirectOutput(char *fileName, char *args[], int lenOfArg, struct childprocs CHILDPROC[], int *childProcLoc) {
    // uses very similar logic to executeArgs but replaces stdout with a file
    int fd = open(fileName, O_CREAT|O_WRONLY|O_TRUNC, 0644);
    if (fd==-1) {
        printf("File could not be opened..\n");
        return;
    }
    pid_t pid = fork();
    if (pid == -1) { // check for successful fork
        close(fd);
        printf("Fork failed..\n");
        return;
    }
    if (pid==0) { // child process
        dup2(fd, 1);
        close(fd);

        if (lenOfArg == 3) {
            if (execlp(args[0], args[0], NULL) < 0) { // remove last 2 arguments (">*" and fileName)
                printf("Could not execute command..\n");
            } 
        }
        else if (lenOfArg == 4) {
            if (execlp(args[0], args[0], args[1], NULL) < 0) {
                printf("Could not execute command..\n");
                //printf("4ARGS[0]: %s",args[0]);
            }
        }
        else {
            if (execlp(args[0], args[0], args[1], args[2], NULL) < 0) {
                printf("Could not execute command..\n");
            }
        }
        exit(0);

    }
    else {
        close(fd);

        // updates child process tracker
        struct childprocs CHILD;
        strncpy(CHILD.command, args[0], strlen(args[0]));
        CHILD.pid = (int) pid;
        if (*childProcLoc==20) {
            *childProcLoc = 0;
        }
        CHILDPROC[*childProcLoc] = CHILD;
        *childProcLoc+=1;
        wait(NULL); // this line waits for the child to terminate
        return;

    }
}

static void executeArgsInBackground(char **args, int lenOfArg, struct childprocs CHILDPROC[], int *childProcLoc) {
    pid_t pid = fork();
    if (pid == -1) {
        printf("Fork failed, exiting...\n");
        return;
    }
    args[lenOfArg-1] = NULL;
    if (pid == 0) {
        if (lenOfArg == 1) {
            if (execlp(args[0], args[0], NULL) < 0) {
                printf("Could not execute command..\n");
            }
        }
        else if (lenOfArg == 2) {
            if (execlp(args[0], args[0], args[1], NULL) < 0) {
                printf("Could not execute command..\n");
            }
        }
        else if (lenOfArg == 3) {
            if (execlp(args[0], args[0], args[1], args[2], NULL) < 0) {
                printf("Could not execute command..\n");
            }
        }
        else if (lenOfArg == 4) {
            if (execlp(args[0], args[0], args[1], args[2], args[3], NULL) < 0) {
                printf("Could not execute command..\n");
            }
        }
        else {
            if (execlp(args[0], args[0], args[1], args[2], args[3], args[4], NULL) < 0) {
                printf("Could not execute command..\n");
            }
        }
    }
    else {
        // notice how a wait(NULL) is not called. this is why this background processor works!
        if (lenOfArg==2) {
            printf("[1]+ Running: (PID: %d) %s %s\n", pid, "&", args[0]);
        }
        else if (lenOfArg==3) {
            printf("[1]+ Running: (PID: %d) %s %s %s\n", pid, "&", args[1], args[0]);
        }
        else if (lenOfArg==4) {
            printf("[1]+ Running: (PID: %d) %s %s %s %s\n", pid, "&", args[2], args[1], args[0]);
        }
        else {
            printf("[1]+ Running: (PID: %d) %s %s %s %s %s\n", pid, "&", args[3], args[2], args[1], args[0]);
        }

        // updates child process tracker
        struct childprocs CHILD;
        strncpy(CHILD.command, args[0], strlen(args[0]));
        CHILD.pid = (int) pid;
        if (*childProcLoc==20) {
            *childProcLoc = 0;
        }
        CHILDPROC[*childProcLoc] = CHILD;
        *childProcLoc+=1;
    }
    return;
}

void executeArgs(char** args, int lenOfArg, struct childprocs CHILDPROC[], int *childProcLoc) { //maybe do in main idfk

    // default function for executing arguments, used for mainly built-in functions
    int temp = *childProcLoc;
    if (lenOfArg>2) {
        if (strcmp(args[lenOfArg-2], "<*") == 0) {
            redirectOutput(args[lenOfArg-1], args, lenOfArg, CHILDPROC, &temp);
            return;
        }
    }

    pid_t pid = fork();

    if (pid == -1) {
        printf("Fork failed, exiting...\n");
        return;
    }
    else if (pid==0) {

        if (lenOfArg == 1) {
            if (execlp(args[0], args[0], NULL) < 0) {
                printf("Could not execute command..\n");
            }
        }
        else if (lenOfArg == 2) {
            if (execlp(args[0], args[0], args[1], NULL) < 0) {
                printf("Could not execute command..\n");
            }
        }
        else if (lenOfArg == 3) {
            if (execlp(args[0], args[0], args[1], args[2], NULL) < 0) {
                printf("Could not execute command..\n");
            } 
        }
        else if (lenOfArg == 4) {
            if (execlp(args[0], args[0], args[1], args[2], args[3], NULL) < 0) {
                printf("Could not execute command..\n");
                //printf("4ARGS[0]: %s",args[0]);
            }
        }
        else {
            if (execlp(args[0], args[0], args[1], args[2], args[3], args[4], NULL) < 0) {
                printf("Could not execute command..\n");
            }
        }
        exit(0);
    }
    else {
        struct childprocs CHILD;
        strncpy(CHILD.command, args[0], strlen(args[0]));
        CHILD.pid = (int) pid;
        if (*childProcLoc==20) {
            *childProcLoc = 0;
        }
        CHILDPROC[*childProcLoc] = CHILD;
        *childProcLoc+=1;
        wait(NULL); // this line waits for the child to terminate
        return;
    }

}

void fg(struct childprocs CHILDPROC[], int *childProcLoc) {
    // this function is used when "fg" is called to convert a background process to foreground
    struct childprocs CHILD;
    strncpy(CHILD.command, "fg", strlen("fg"));
    CHILD.pid = 0;
    if (*childProcLoc==20) {
        *childProcLoc = 0;
    }
    CHILDPROC[*childProcLoc] = CHILD;
    *childProcLoc++;
    
    wait(NULL);
}

void listChildProcs(struct childprocs CHILDPROC[], int childProcLoc) {
    // simple for loop that prints data from childProc array
    for (int i = 0; ((i < 20) && (CHILDPROC[i].pid>0)); i++) {
        printf("cmd: %s\t\tpid: %d\n", CHILDPROC[i].command, CHILDPROC[i].pid);
    }
}

void prompt(char *argument) { // prompts user for input
    printf("tcgray2> ");
    fgets(argument, MAX_SIZE, stdin);
}

int main() {
    struct childprocs CHILDPROC[20]; // array of child process tracker objects
    int childProcLoc = 0;
    for (int i = 0; i < 20; i++) { // loops through and sets each pid's value to -1: a successful pid will never be negative (helps with error checking later)
        CHILDPROC[i].pid = -1;
    }
    char *argStr;
    char *command[5];
    printf("\nWelcome to Funny Shell! The shell where you write arguments in reverse order!\nMax: 4 arguments\n");
    int shouldRun = 1;
    while (shouldRun==1) {

        for (int i = 0; i < 5; i++) { // sets each command to null, would have leftover arguments if this loop did not exist
            command[i] = '\0';
        }
        char argument[MAX_SIZE];
        char *split;

        prompt(argument);
        if (strlen(argument) != 0) { // parses argument
            split = strtok(argument, "\t\r\n\v\f ");
            int counter = 0;
            while (split!=NULL) {
                if (counter==5) {
                    printf("Error: 4 arguments max\n");
                    break;
                }
                command[counter]=split;
                split = strtok(NULL, "\t\r\n\v\f ");
                counter++;
            }

        }

        int length = 0;
        for (int i = 0; (command[i] != NULL && i < 5); i++) {
            length++;
        }
        reverseArray(command, 0, length-1);
        
        // command parser
        if (command[0] != NULL) {
            if (strcmp(command[0], "fg") == 0) {
                fg(CHILDPROC, &childProcLoc);
            }
            else if (*command[length-1] == '&') {
                executeArgsInBackground(command, length, CHILDPROC, &childProcLoc);
            }
            else if (strcmp(command[0], "listchildprocs") == 0) {
                listChildProcs(CHILDPROC, childProcLoc);
            }
            else if (strcmp(command[0], "exit") == 0) {
                shouldRun=2;
            }
            else {
                executeArgs(command, length, CHILDPROC, &childProcLoc);
            }
        }
    }
}

// use a read/write output algorithm (fread, fwrite) for redirecting output