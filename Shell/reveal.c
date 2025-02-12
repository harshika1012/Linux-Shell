
// #include "headers.h"
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
// void print_file_details(const char *path, const FileInfo *fileInfo) {
//     const struct stat *fileStat = &fileInfo->fileStat;

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

//     print_file_info(fileInfo->name, fileStat);
// }

// // Comparator function for qsort
// int compare_file_info(const void *a, const void *b) {
//     return strcmp(((FileInfo *)a)->name, ((FileInfo *)b)->name);
// }

// // Function to handle the `reveal` command
// void reveal_fun(const char *flags, const char *path) {
//     DIR *dir;
//     struct dirent *entry;
//     char fullPath[4096];
//     FileInfo files[1024];
//     int fileCount = 0;

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

//     // First pass to read directory entries and calculate the total number of blocks
//     while ((entry = readdir(dir)) != NULL) {
//         if (!showAll && entry->d_name[0] == '.') continue;  // Skip hidden files if -a is not provided

//         snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
//         if (stat(fullPath, &files[fileCount].fileStat) == -1) {
//             perror("stat");
//             continue;
//         }

//         strcpy(files[fileCount].name, entry->d_name);  // Store the file name
//         totalBlocks += files[fileCount].fileStat.st_blocks;  // Accumulate blocks
//         fileCount++;
//     }

//     // Sort the files in lexicographical order
//     qsort(files, fileCount, sizeof(FileInfo), compare_file_info);

//     if (showLong) {
//         printf("Total: %ld\n", totalBlocks / 2);  // Dividing by 2 to convert 512-byte blocks to 1K blocks
//     }

//     // Print sorted files
//     for (int i = 0; i < fileCount; i++) {
//         if (showLong) {
//             print_file_details(path, &files[i]);
//         } else {
//             print_file_info(files[i].name, &files[i].fileStat);
//         }
//     }

//     closedir(dir);
// }


// void reveal(char* input,char* path){
//     //  printf("path + %s\n",path);
//     char flags[10] = "";
//     char temp_path[1024];
//     strcpy(temp_path,path);
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
//     reveal_fun(flags, temp_path);  
//     return;

// }



#include "headers.h"


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
void reveal_fun(const char *flags, const char *path) {
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
    static char previousPath[1024] = "";  // Stores previous path for "-"
    struct passwd *pw = getpwuid(getuid());

    if (strcmp(path, "~") == 0) {
        path = pw->pw_dir;  // Home directory
    } else if (strcmp(path, "-") == 0) {
        if (strlen(previousPath) == 0) {
            fprintf(stderr, "OLDPWD not set.\n");
            return;
        }
        path = previousPath;  // Use the previous path
    } else if (path[0] == '~') {
        snprintf(fullPath, sizeof(fullPath), "%s%s", pw->pw_dir, path + 1);  // Handle paths starting with ~
        path = fullPath;
    }

    // Update the previous path
    getcwd(previousPath, sizeof(previousPath));

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

// void reveal(char* input, char* path) {
//     char flags[10] = "";
//     char temp_path[1024];
  
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

//     reveal_fun(flags, path);  
// }


