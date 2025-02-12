#include "headers.h"
int is_background=0;
typedef struct {
    pid_t pid;
    int status;
} ProcessInfo;
ProcessInfo completed_processes[1024];
int completed_count = 0;  
void sigchld_handler(int sig) {
    int status;
    pid_t pid;

    // Wait for all children that have terminated (non-blocking)
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (completed_count < 1024) {
            // Store completed process information
            completed_processes[completed_count].pid = pid;
            completed_processes[completed_count].status = status;
            completed_count++;
        } else {
            fprintf(stderr, "Too many completed processes to track!\n");
        }
    }
}

void print_completed_processes() {
    for (int i = 0; i < completed_count; i++) {
        if (WIFEXITED(completed_processes[i].status)) {
            printf("Background process with PID %d completed with exit status %d.\n", completed_processes[i].pid, WEXITSTATUS(completed_processes[i].status));
        } else if (WIFSIGNALED(completed_processes[i].status)) {
            printf("Background process with PID %d terminated by signal %d.\n", completed_processes[i].pid, WTERMSIG(completed_processes[i].status));
        } else {
            printf("Background process with PID %d terminated abnormally.\n", completed_processes[i].pid);
        }
    }
    completed_count = 0;  // Reset the count after printing
}

void run_background(char* input) {
    int i = 0;
    char* args[1024];

    input = strtok(input, " ");
    while (input != NULL && i < 1023) {  // Limit to 1023 to leave space for NULL
        args[i] = input;
        i++;
        input = strtok(NULL, " ");
    }
    args[i] = NULL;  // Null-terminate the argument list

    pid_t child_pid = fork();
    if (child_pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // In child process
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // In parent process
        printf("Started background process with PID %d.\n", child_pid);
    }
}
void run_foreground(char* input){
     int i = 0;
    char* args[1024];
    input = strtok(input, " "); 
    int len=sizeof(args);
    while (input != NULL && i < len - 1) {
        args[i] = input;
        i++;
        input = strtok(NULL, " ");
    }
    args[i] = NULL;
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else {
        is_background=0;
        wait(NULL);
    }

}