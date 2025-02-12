#include "headers.h"
int count=0;
void list_files_in_directory(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char path[1024];

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    printf("Files in directory '%s':\n", dir_path);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
                printf("  %s\n", entry->d_name);  
            }
        }
    }

    closedir(dir);
}
void search_directories(const char *dir_path, const char *prefix, char found_directories[][1024], int *found_count) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char path[1024];
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
                if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
                    strcpy(found_directories[*found_count], path); 
                    (*found_count)++;
                }
                search_directories(path, prefix, found_directories, found_count);
            }
        }
    }

    closedir(dir);
}

void search_in_directory(const char *dir_path, const char *file_name) {
    struct dirent *entry;
    struct stat buffer;
    char path[1024];

    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        if (stat(path, &buffer) == 0) {
            if (S_ISDIR(buffer.st_mode)) {
                search_in_directory(path, file_name);
            } else if (S_ISREG(buffer.st_mode) && strncmp(entry->d_name, file_name,strlen(file_name)) == 0) {
                printf("Found '%s' in '%s'\n", file_name, path);
            }
        }
    }

    closedir(dir);
}

void search_with_directory(const char *dir_path, const char *prefix) {
    struct dirent *entry;
    struct stat buffer;
    char path[1024];

    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        if (stat(path, &buffer) == 0) {
            if (S_ISDIR(buffer.st_mode)) {
                search_with_directory(path, prefix);
            } else if (S_ISREG(buffer.st_mode) && strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
                printf("%s\n", path);
            }
        }
    }

    closedir(dir);
}
void check_directory(const char *dir_path, const char *prefix) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char path[1024];
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
                if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
                    printf("%s\n", path);
                }
                check_directory(path, prefix);
            }
        }
    }
    closedir(dir);
}
void check_with_directory(const char* dir_path, const char* prefix) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char path[1024];

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
                if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
                    printf("%s\n", path);  
                }
                check_with_directory(path, prefix);
            }
        }
    }

    closedir(dir);
}
void search_in_directory_for_e(const char *dir_path, const char *file_name,int* count) {
    struct dirent *entry;
    struct stat buffer;
    char path[1024];

    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        if (stat(path, &buffer) == 0) {
            if (S_ISDIR(buffer.st_mode)) {
                search_in_directory_for_e(path, file_name,count);
            } else if (S_ISREG(buffer.st_mode) && strcmp(entry->d_name, file_name) == 0) {
                count++;
                printf("Found '%s' in '%s'\n", file_name, path);
                
            }
        }
    }

    closedir(dir);
}
void print_file_content(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
}

void search_and_print_file(const char *dir_path, const char *prefix, char *found_file, int *found_count) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char path[1024];
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            if (stat(path, &st) == 0) {
                if (S_ISDIR(st.st_mode)) {
                    search_and_print_file(path, prefix, found_file, found_count);
                } else if (S_ISREG(st.st_mode) && strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
                    (*found_count)++;
                    strcpy(found_file, path);  
                }
            }
        }
    }
    closedir(dir);
}
void seek_categories(char* input){
    char* parts[4096];
    char found_file[1024];
    int found_count = 0;
    char found_directories[100][1024];  
    int found_count_dir = 0;
    int size=0;
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }

    parts[size] = strtok(input, " ");

    while (parts[size] != NULL) {
        size++;
        parts[size] = strtok(NULL, " ");
    }
    if(size==2){
        search_in_directory(".", parts[1]); 
        check_directory(".",parts[1]);
    }
    else if(strcmp(parts[1],"-e")==0){
        search_in_directory(".", parts[2]); 
        check_directory(".",parts[2]);
    }
    else if(strncmp(parts[2],"-e",2)==0){

       if(strcmp(parts[1],"-f")==0){
            if(size==5){
                char str[100];
                substring(str,parts[4],2,100);
                search_and_print_file(str, parts[3], found_file, &found_count);
                if (found_count == 1) {
                    print_file_content(found_file);
                    printf("\n");
                }
                else {
                    search_with_directory(str, parts[3]); 
                } 
            }
            else{
                search_and_print_file(".", parts[3], found_file, &found_count);
                if (found_count == 1) {
                    print_file_content(found_file);
                    printf("\n");
                }
                else {
                    search_in_directory(".", parts[3]); 
                    check_directory(".",parts[3]);
                } 
            }
        }
        else if(strcmp(parts[1],"-d")==0){
            if(size==5){
                char str[100];
                substring(str,parts[4],2,100);
                search_directories(str, parts[3], found_directories, &found_count_dir);
                if (found_count_dir == 1) {
                    list_files_in_directory(found_directories[0]);
                    printf("\n");
                }
                else{
                    check_directory(str,parts[3]);
                }
            }
            else{
                search_directories(".", parts[3], found_directories, &found_count_dir);
                if (found_count_dir == 1) {
                    list_files_in_directory(found_directories[0]);
                    printf("\n");
                }
                else{
                    check_directory(".",parts[3]);
                }
            
            } 
        }
    }
    else if(strcmp(parts[1],"-f")==0){
        if(size==3){
            search_in_directory(".",parts[2]); 
        }
        else if(strlen(parts[3])!=0){
            char str[100];
            substring(str,parts[3],2,100);
            search_with_directory(str,parts[2]);
        }
    }
    
    else if(strcmp(parts[1],"-d")==0){
        if(size==3){
            check_directory(".",parts[2]);
        }
        else if(strlen(parts[3])!=0){
            char str[100];
            substring(str,parts[3],2,100);
            check_with_directory(str,parts[2]);
        }
       
    }
    else if(strcmp(parts[1],"-e")==0){
        search_in_directory_for_e(".",parts[2],&count);
        if(count==1){
            printf("Data\n");
        }

    }
    else if(size==2){
        search_in_directory(".", parts[1]); 
        check_directory(".",parts[1]);
    }
    else{
        printf("Invalid command\n");
    }
}
