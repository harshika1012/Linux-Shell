
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>

#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[1;34m"
#define GREEN_COLOR "\033[1;32m"
#define WHITE_COLOR "\033[1;37m"

#define MAX_FILES 1024  // Maximum number of files to handle

// Struct to store file information
typedef struct {
    char name[256];
    struct stat fileStat;
} FileInfo;

// #include "headers.h"
// Function to get the file type and color code
void print_file_info(const char *name, const struct stat *fileStat) {
    if (S_ISDIR(fileStat->st_mode)) {
        printf(BLUE_COLOR "%s\n" RESET_COLOR, name);
    } else if (fileStat->st_mode & S_IXUSR) {
        printf(GREEN_COLOR "%s\n" RESET_COLOR, name);
    } else {
        printf(WHITE_COLOR "%s\n" RESET_COLOR, name);
    }
}

// Function to print file details like `ls -l`
void print_file_details(const char *path, const FileInfo *fileInfo) {
    const struct stat *fileStat = &fileInfo->fileStat;

    printf((S_ISDIR(fileStat->st_mode)) ? "d" : "-");
    printf((fileStat->st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat->st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat->st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat->st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat->st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat->st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat->st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat->st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat->st_mode & S_IXOTH) ? "x" : "-");

    printf(" %ld", (long)fileStat->st_nlink);

    struct passwd *pw = getpwuid(fileStat->st_uid);
    struct group *gr = getgrgid(fileStat->st_gid);
    printf(" %s %s", pw->pw_name, gr->gr_name);

    printf(" %5ld", (long)fileStat->st_size);

    char timebuf[80];
    struct tm *timeinfo = localtime(&fileStat->st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", timeinfo);
    printf(" %s ", timebuf);

    print_file_info(fileInfo->name, fileStat);
}

// Comparator function for qsort
int compare_file_info(const void *a, const void *b) {
    return strcmp(((FileInfo *)a)->name, ((FileInfo *)b)->name);
}

// Function to handle the `reveal` command
void reveal(const char *flags, const char *path) {
    DIR *dir;
    struct dirent *entry;
    char fullPath[4096];
    FileInfo files[1024];
    int fileCount = 0;

    int showAll = 0, showLong = 0;
    long totalBlocks = 0;  // To store total blocks count

    // Check flags for -a and -l
    for (int i = 0; i < strlen(flags); i++) {
        if (flags[i] == 'a') showAll = 1;
        if (flags[i] == 'l') showLong = 1;
    }

    // Handle special paths (., .., ~, -)
    if (strcmp(path, "~") == 0) {
        path = getenv("HOME");
    } else if (strcmp(path, "-") == 0) {
        path = getenv("OLDPWD");
    } else if (path[0] == '~') {
        snprintf(fullPath, sizeof(fullPath), "%s%s", getenv("HOME"), path + 1);
        path = fullPath;
    }

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

    // First pass to read directory entries and calculate the total number of blocks
    while ((entry = readdir(dir)) != NULL) {
        if (!showAll && entry->d_name[0] == '.') continue;  // Skip hidden files if -a is not provided

        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
        if (stat(fullPath, &files[fileCount].fileStat) == -1) {
            perror("stat");
            continue;
        }

        strcpy(files[fileCount].name, entry->d_name);  // Store the file name
        totalBlocks += files[fileCount].fileStat.st_blocks;  // Accumulate blocks
        fileCount++;
    }

    // Sort the files in lexicographical order
    qsort(files, fileCount, sizeof(FileInfo), compare_file_info);

    if (showLong) {
        printf("Total: %ld\n", totalBlocks / 2);  // Dividing by 2 to convert 512-byte blocks to 1K blocks
    }

    // Print sorted files
    for (int i = 0; i < fileCount; i++) {
        if (showLong) {
            print_file_details(path, &files[i]);
        } else {
            print_file_info(files[i].name, &files[i].fileStat);
        }
    }

    closedir(dir);
}


// void reveal(char* input,char* path){
//     //  printf("path + %s\n",path);
//     char flags[10] = "";
//     char temp_path[1024];
//     // input[strcspn(input, "\n")] = '\0';
//     if (strncmp(input, "reveal", 6) == 0) {
//         char *token = strtok(input + 6, " ");
        
//         while (token) {
//             if (token[0] == '-') {  // Flags part
//                 strcpy(flags, token + 1);
//             } else {  // Path part
//                 strcpy(temp_path, token);
//             }
//             token = strtok(NULL, " ");
//         }
//     } else {
//         printf("Invalid command. Use 'reveal <flags> <path/name>'.\n");
//         return;
//     }
//     // printf("path + %s\n",path);
//     reveal_fun(flags, path);  
//     return;

// }
int main() {
    char input[1024];
    char flags[10] = "";
    char path[1024] = ".";
    
    printf("Enter the reveal command: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove newline character

    // Parse the input command
    if (strncmp(input, "reveal", 6) == 0) {
        char *token = strtok(input + 6, " ");
        
        while (token) {
            if (token[0] == '-') {  // Flags part
                strcpy(flags, token + 1);
            } else {  // Path part
                strcpy(path, token);
            }
            token = strtok(NULL, " ");
        }
    } else {
        printf("Invalid command. Use 'reveal <flags> <path/name>'.\n");
        return 1;
    }
    printf("%s\n",path);
    reveal(flags, path);
    return 0;
}




// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <dirent.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <pwd.h>
// #include <grp.h>
// #include <time.h>
// #include <errno.h>

// #define RESET_COLOR "\033[0m"
// #define BLUE_COLOR "\033[1;34m"
// #define GREEN_COLOR "\033[1;32m"
// #define WHITE_COLOR "\033[1;37m"

// // Function to get the file type and color code
// void print_file_info(const char *name, const struct stat *fileStat) {
//     if (S_ISDIR(fileStat->st_mode)) {
//         printf(BLUE_COLOR "%s\n" RESET_COLOR, name);
//     } else if (fileStat->st_mode & S_IXUSR) {
//         printf(GREEN_COLOR "%s\n" RESET_COLOR, name);
//     } else {
//         printf(WHITE_COLOR "%s\n" RESET_COLOR, name);
//     }
// }

// // Function to print file details like `ls -l`
// void print_file_details(const char *path, const char *name, const struct stat *fileStat) {
//     char fullPath[4096];
//     snprintf(fullPath, sizeof(fullPath), "%s/%s", path, name);

//     printf((S_ISDIR(fileStat->st_mode)) ? "d" : "-");
//     printf((fileStat->st_mode & S_IRUSR) ? "r" : "-");
//     printf((fileStat->st_mode & S_IWUSR) ? "w" : "-");
//     printf((fileStat->st_mode & S_IXUSR) ? "x" : "-");
//     printf((fileStat->st_mode & S_IRGRP) ? "r" : "-");
//     printf((fileStat->st_mode & S_IWGRP) ? "w" : "-");
//     printf((fileStat->st_mode & S_IXGRP) ? "x" : "-");
//     printf((fileStat->st_mode & S_IROTH) ? "r" : "-");
//     printf((fileStat->st_mode & S_IWOTH) ? "w" : "-");
//     printf((fileStat->st_mode & S_IXOTH) ? "x" : "-");

//     printf(" %ld", (long)fileStat->st_nlink);

//     struct passwd *pw = getpwuid(fileStat->st_uid);
//     struct group *gr = getgrgid(fileStat->st_gid);
//     printf(" %s %s", pw->pw_name, gr->gr_name);

//     printf(" %5ld", (long)fileStat->st_size);

//     char timebuf[80];
//     struct tm *timeinfo = localtime(&fileStat->st_mtime);
//     strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", timeinfo);
//     printf(" %s ", timebuf);

//     print_file_info(name, fileStat);
// }

// // Function to handle the `reveal` command
// void reveal(const char *flags, const char *path) {
//     DIR *dir;
//     struct dirent *entry;
//     struct stat fileStat;
//     char fullPath[4096];

//     int showAll = 0, showLong = 0;
//     // Check flags for -a and -l
//     for (int i = 0; i < strlen(flags); i++) {
//         if (flags[i] == 'a') showAll = 1;
//         if (flags[i] == 'l') showLong = 1;
//     }

//     // Handle special paths (., .., ~, -)
//     if (strcmp(path, "~") == 0) {
//         path = getenv("HOME");
//     } else if (strcmp(path, "-") == 0) {
//         path = getenv("OLDPWD");
//     } else if (path[0] == '~') {
//         snprintf(fullPath, sizeof(fullPath), "%s%s", getenv("HOME"), path + 1);
//         path = fullPath;
//     }

//     if ((dir = opendir(path)) == NULL) {
//         perror("opendir");
//         return;
//     }

//     while ((entry = readdir(dir)) != NULL) {
//         // Skip hidden files unless -a is provided
//         if (!showAll && entry->d_name[0] == '.') continue;

//         snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
//         if (stat(fullPath, &fileStat) == -1) {
//             perror("stat");
//             continue;
//         }

//         if (showLong) {
//             print_file_details(path, entry->d_name, &fileStat);
//         } else {
//             print_file_info(entry->d_name, &fileStat);
//         }
//     }

//     closedir(dir);
// }

// int main() {
//     char input[1024];
//     char flags[10] = "";
//     char path[1024] = ".";
    
//     printf("Enter the reveal command: ");
//     fgets(input, sizeof(input), stdin);
//     input[strcspn(input, "\n")] = '\0';  // Remove newline character

//     // Parse the input command
//     if (strncmp(input, "reveal", 6) == 0) {
//         char *token = strtok(input + 6, " ");
        
//         while (token) {
//             if (token[0] == '-') {  // Flags part
//                 strcpy(flags, token + 1);
//             } else {  // Path part
//                 strcpy(path, token);
//             }
//             token = strtok(NULL, " ");
//         }
//     } else {
//         printf("Invalid command. Use 'reveal <flags> <path/name>'.\n");
//         return 1;
//     }

//     reveal(flags, path);
//     return 0;
// }




// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <dirent.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <pwd.h>
// #include <grp.h>
// #include <time.h>
// #include <errno.h>

// #define RESET_COLOR "\033[0m"
// #define BLUE_COLOR "\033[1;34m"
// #define GREEN_COLOR "\033[1;32m"
// #define WHITE_COLOR "\033[1;37m"

// // Function to get the file type and color code
// void print_file_info(const char *name, const struct stat *fileStat) {
//     if (S_ISDIR(fileStat->st_mode)) {
//         printf(BLUE_COLOR "%s\n" RESET_COLOR, name);
//     } else if (fileStat->st_mode & S_IXUSR) {
//         printf(GREEN_COLOR "%s\n" RESET_COLOR, name);
//     } else {
//         printf(WHITE_COLOR "%s\n" RESET_COLOR, name);
//     }
// }

// // Function to print file details like `ls -l`
// void print_file_details(const char *path, const char *name, const struct stat *fileStat) {
//     char fullPath[4096];
//     snprintf(fullPath, sizeof(fullPath), "%s/%s", path, name);

//     printf((S_ISDIR(fileStat->st_mode)) ? "d" : "-");
//     printf((fileStat->st_mode & S_IRUSR) ? "r" : "-");
//     printf((fileStat->st_mode & S_IWUSR) ? "w" : "-");
//     printf((fileStat->st_mode & S_IXUSR) ? "x" : "-");
//     printf((fileStat->st_mode & S_IRGRP) ? "r" : "-");
//     printf((fileStat->st_mode & S_IWGRP) ? "w" : "-");
//     printf((fileStat->st_mode & S_IXGRP) ? "x" : "-");
//     printf((fileStat->st_mode & S_IROTH) ? "r" : "-");
//     printf((fileStat->st_mode & S_IWOTH) ? "w" : "-");
//     printf((fileStat->st_mode & S_IXOTH) ? "x" : "-");

//     printf(" %ld", (long)fileStat->st_nlink);

//     struct passwd *pw = getpwuid(fileStat->st_uid);
//     struct group *gr = getgrgid(fileStat->st_gid);
//     printf(" %s %s", pw->pw_name, gr->gr_name);

//     printf(" %5ld", (long)fileStat->st_size);

//     char timebuf[80];
//     struct tm *timeinfo = localtime(&fileStat->st_mtime);
//     strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", timeinfo);
//     printf(" %s ", timebuf);

//     print_file_info(name, fileStat);
// }

// // Function to handle the `reveal` command
// void reveal(const char *flags, const char *path) {
//     DIR *dir;
//     struct dirent *entry;
//     struct stat fileStat;
//     char fullPath[4096];

//     int showAll = 0, showLong = 0;
//     long totalBlocks = 0;  // To store total blocks count

//     // Check flags for -a and -l
//     for (int i = 0; i < strlen(flags); i++) {
//         if (flags[i] == 'a') showAll = 1;
//         if (flags[i] == 'l') showLong = 1;
//     }

//     // Handle special paths (., .., ~, -)
//     if (strcmp(path, "~") == 0) {
//         path = getenv("HOME");
//     } else if (strcmp(path, "-") == 0) {
//         path = getenv("OLDPWD");
//     } else if (path[0] == '~') {
//         snprintf(fullPath, sizeof(fullPath), "%s%s", getenv("HOME"), path + 1);
//         path = fullPath;
//     }

//     if ((dir = opendir(path)) == NULL) {
//         perror("opendir");
//         return;
//     }

//     // First pass to calculate the total number of blocks
//     while ((entry = readdir(dir)) != NULL) {
//         if (!showAll && entry->d_name[0] == '.') continue;  // Skip hidden files if -a is not provided

//         snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
//         if (stat(fullPath, &fileStat) == -1) {
//             perror("stat");
//             continue;
//         }

//         totalBlocks += fileStat.st_blocks;  // Accumulate blocks
//     }

//     // Reset directory pointer for the second pass
//     rewinddir(dir);

//     if (showLong) {
//         printf("Total: %ld\n", totalBlocks / 2);  // Dividing by 2 to convert 512-byte blocks to 1K blocks
//     }

//     // Second pass to display file information
//     while ((entry = readdir(dir)) != NULL) {
//         if (!showAll && entry->d_name[0] == '.') continue;  // Skip hidden files if -a is not provided

//         snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
//         if (stat(fullPath, &fileStat) == -1) {
//             perror("stat");
//             continue;
//         }

//         if (showLong) {
//             print_file_details(path, entry->d_name, &fileStat);
//         } else {
//             print_file_info(entry->d_name, &fileStat);
//         }
//     }

//     closedir(dir);
// }

// int main() {
//     char input[1024];
//     char flags[10] = "";
//     char path[1024] = ".";
    
//     printf("Enter the reveal command: ");
//     fgets(input, sizeof(input), stdin);
//     input[strcspn(input, "\n")] = '\0';  // Remove newline character

//     // Parse the input command
//     if (strncmp(input, "reveal", 6) == 0) {
//         char *token = strtok(input + 6, " ");
        
//         while (token) {
//             if (token[0] == '-') {  // Flags part
//                 strcpy(flags, token + 1);
//             } else {  // Path part
//                 strcpy(path, token);
//             }
//             token = strtok(NULL, " ");
//         }
//     } else {
//         printf("Invalid command. Use 'reveal <flags> <path/name>'.\n");
//         return 1;
//     }

//     reveal(flags, path);
//     return 0;
// }

// int main() {
//     char input[1024];
//     char flags[10] = "";
//     char path[1024] = ".";
    
//     printf("Enter the reveal command: ");
//     fgets(input, sizeof(input), stdin);
//     input[strcspn(input, "\n")] = '\0';  // Remove newline character

//     // Parse the input command
//     if (strncmp(input, "reveal", 6) == 0) {
//         char *token = strtok(input + 6, " ");
        
//         while (token) {
//             if (token[0] == '-') {  // Flags part
//                 strcpy(flags, token + 1);
//             } else {  // Path part
//                 strcpy(path, token);
//             }
//             token = strtok(NULL, " ");
//         }
//     } else {
//         printf("Invalid command. Use 'reveal <flags> <path/name>'.\n");
//         return 1;
//     }

//     reveal(flags, path);
//     return 0;
// }