#ifndef HEADERS_H_
#define HEADERS_H_

#include<stdio.h>
#include<sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pwd.h>
#include<string.h>
#include<sys/stat.h>
#include<ctype.h>
#include <sys/wait.h>
#include<signal.h>
#include <fcntl.h>  
#include <dirent.h>
#include <grp.h>
#include <time.h>
#include <errno.h>


#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[1;34m"
#define GREEN_COLOR "\033[1;32m"
#define WHITE_COLOR "\033[1;37m"

typedef struct {
    char name[256];
    struct stat fileStat;
} FileInfo;

void current_directory(char* path,const char* home);
void previous_directory(char* previos_dir,char* path);
void to_parent_directory(char* path, char* previous_dir);
void dot_dot(char* path,char* token,char* previous_dir);
void substring(char *destination, const char *source, int start, int length);
void slash_delta_path(char* shell_home, char* path, char* previous_dir, char* token);
void delta_hop(char* previous_dir,char* path, char* shell_home);
int directory_exists(char* path);
void change_chdir(char* path);
int directory_exists(char* path);
void to_directory(char* previous_dir,char* path,char* token);
char* lstrip(char* str);
char* rstrip(char* str);
char *trim(char *str);
void sigchld_handler(int sig);
void run_background(char* input);
void run_foreground(char* input);
void parseInput(char* input, char** args, int* background);
void checkBackgroundProcesses(int* bg_pids, int* bg_count) ;
void executeCommand(char** args, int background, int* bg_pids, int* bg_count);
int sleep_compare(char* token);
void add_prompt_sleep(char* token,int is_sleep_there,char* add_sleep);
void proclore_print();
void log_print(const char* filepath);
void write_strings_to_file(const char *filename,char* str);
void read_strings_from_file(const char *filename);
int check_log(char** tokens,int len);
void print_specific_line(const char *filename, int line_number);
void remove_line(const char* filename,char* last_line);
int count_lines_in_file(const char *filename);
void clear_file(const char *filename);
void list_files_in_directory(const char *dir_path);
void search_directories(const char *dir_path, const char *prefix, char found_directories[][1024], int *found_count);
void search_in_directory(const char *dir_path, const char *file_name) ;
void search_with_directory(const char *dir_path, const char *prefix);
void check_directory(const char *dir_path, const char *prefix);
void check_with_directory(const char* dir_path, const char* prefix);
void search_in_directory_for_e(const char *dir_path, const char *file_name,int* count);
void print_file_content(const char *file_path);
void search_and_print_file(const char *dir_path, const char *prefix, char *found_file, int *found_count);
void seek_categories(char* input);
void print_completed_processes();
void print_file_info(const char *name, const struct stat *fileStat) ;
void print_file_details(const char *path, const FileInfo *fileInfo) ;
int compare_file_info(const void *a, const void *b);
void reveal_fun(const char *flags, const char *path);
void reveal(char* input,char* path);
int checking_in_out(char* input,int len);

#endif