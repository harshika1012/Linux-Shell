#include "headers.h"
void proclore_print(){
    printf("PID = %d\n",getpid());
     char path[256], buffer[1024];
     char exe_path[1024];
     int len;
    int fd;
    ssize_t bytes_read;
    int pgrp;
    char status;
    unsigned long virtual_size;
    int field_count = 0;
    char *field_start;
    int pid = getpid();
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open stat file");
        return;
    }
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        perror("Failed to read from stat file");
        close(fd);
        return;
    }
    buffer[bytes_read] = '\0';
     field_start = buffer;
    for (int i = 0; i < 22; ++i) {
        field_start = strchr(field_start, ' ');
        if (field_start == NULL) {
            perror("Failed to parse the stat file");
            close(fd);
            return;
        }
        field_start++; 
    }
    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    len = readlink(path, exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("Failed to read the executable path");
        return;
    }
    exe_path[len] = '\0';
    sscanf(buffer, "%*d %*s %c", &status);
    sscanf(buffer, "%*d %*s %*c %*d %d", &pgrp);
    sscanf(field_start, "%lu", &virtual_size);
    close(fd);
    printf("Process Status for PID %d: %c\n", pid, status);
    printf("Process Group for PID %d: %d\n", pid, pgrp);
    printf("Virtual Memory Size for PID %d: %lu bytes\n", pid, virtual_size);
    printf("Executable Path for PID %d: %s\n", pid, exe_path);
    return;
}