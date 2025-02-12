#include "headers.h"
void clear_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file for clearing");
        exit(EXIT_FAILURE);
    }
    fclose(file);
    printf("The file '%s' has been cleared.\n", filename);
}

void remove_line(const char* filename,char* last_line){
    int line1,size;
    int total=15;
    line1=1;
    size=1;
    char str1[100];
    char str2[100];
    char all_strings[100][100];
    FILE* file=fopen(filename,"r");
     if (file == NULL) {
        perror("Failed to open file");  
        exit(EXIT_FAILURE);
    }
    fgets(str1,sizeof(str1),file);
    while(fgets(str2,sizeof(str2),file)!=NULL){
            strcpy(all_strings[size],str2);
            size++;
    }
    fclose(file);
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < 15; i++) {
        // printf("%s\n",all_strings[i]);
        fputs(all_strings[i], file);
    }
    fputs(last_line,file);
    fclose(file);
}
int count_lines_in_file(const char *filename) {
    FILE *file = fopen(filename, "r");  
    if (file == NULL) {
        perror("Failed to open file"); 
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char buffer[256]; 
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        count++; 
    }
    if (fclose(file) != 0) {
        perror("Failed to close file");
    }

    return count;  
}
void print_specific_line(const char *filename, int line_number) {
    FILE *file = fopen(filename, "r");  
    int total=count_lines_in_file(filename);
    int given_num=line_number;
    line_number=total-line_number+1;
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char buffer[256];
    int current_line = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (current_line == line_number) {
            printf("Line %d: %s", given_num, buffer);  
            break;
        }
        current_line++;
    }

    if (current_line < line_number) {
        printf("The file has less than %d lines.\n", line_number);
    }
    if (fclose(file) != 0) {
        perror("Failed to close file");
    }
}
int check_log(char** tokens,int len){
    int i=0;
    while(i<len){
        if(strncmp(tokens[i],"log",3)==0){
            return 1;
        }
        i++;
    }
    return 0;
}
void log_print(const char* filepath){
    read_strings_from_file(filepath);
}
void write_strings_to_file(const char *filename,char* str) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);  
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    strcat(str,"\n");
    int total=count_lines_in_file(filename);
    if(total>=15){
        remove_line(filename,str);
        return;
    }
    else{
        if (write(fd, str, strlen(str)) == -1) {
            perror("Failed to write to file");
            close(fd);
            return;
        }
    }
    if (close(fd) == -1) {
        perror("Failed to close file");
    }
}
void read_strings_from_file(const char *filename) {
    int fd = open(filename, O_RDONLY); 
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    char buffer[256];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';  
        printf("%s", buffer);
    }

    if (bytes_read == -1) {
        perror("Failed to read from file");
    }
    if (close(fd) == -1) {
        perror("Failed to close file");
    }
}