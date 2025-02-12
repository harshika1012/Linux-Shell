#include "headers.h"
void change_chdir(char* path){
    if (chdir(path) == 0) {
        // printf("Directory changed successfully to %s\n", path);
    } else {
        perror("chdir failed");
    }
}
void current_directory(char* path,const char* home){
    char curr_path[100];
    getcwd(curr_path,sizeof(curr_path));
    printf("%s\n",curr_path);
    return;
}

void previous_directory(char* previous_dir,char* path){
    char another_prev[4096];
    strcpy(another_prev,path);
    strcpy(path,previous_dir);
    change_chdir(path);
    printf("%s\n",path);
    strcpy(previous_dir,another_prev);
    return;
}
void to_parent_directory(char* path, char* previous_dir){
    strcpy(previous_dir,path);
    char pre_dir[4096];
    strcpy(pre_dir,path);
    if(chdir("..")!=0){
        printf("Chdir failed\n");
    }
    if(getcwd(pre_dir,sizeof(pre_dir))!=NULL){
        printf("%s\n",pre_dir);
        strcpy(path,pre_dir);
    }
    else {
        perror("getcwd() failed");
        printf("Error code: %d\n", errno);
    }
    return;
    
}
void substring(char *destination, const char *source, int start, int length) {
    strncpy(destination, source + start, length);
    destination[length] = '\0';
}
void dot_dot(char* path,char* token,char* previous_dir){
    to_parent_directory(path,previous_dir);
    char remaining_path[100];
    char dup_path[4096];
    if(token[5]=='\0'){
        return;
    }
    else{
        substring(remaining_path,token,5,100);
        strcpy(dup_path,path);
        strcat(dup_path,"/");
        strcat(dup_path,remaining_path);
        if(directory_exists(dup_path)){
            strcpy(previous_dir,path);
            printf("%s\n",dup_path);
            strcpy(path,dup_path);
            change_chdir(path);
        }
        else{
            printf("Directory doesn't exist\n");
        }
    }
}
void to_directory(char* previous_dir,char* path,char* token){
    char dup_path[100];
    char remaining_path[100];
    strcpy(dup_path,path);
    substring(remaining_path,token,4,100);
    if(strlen(path)!=1){
        strcat(dup_path,"/");
    }
    strcat(dup_path,remaining_path);
    if(directory_exists(dup_path)){
        strcpy(previous_dir,path);
        strcpy(path,dup_path);
        printf("%s\n",dup_path);
        change_chdir(path);
    }
    else{
        printf("Directory doesn't exists\n");
    }    
}
void delta_hop(char* previous_dir,char* path, char* shell_home){
    strcpy(previous_dir,path);
    strcpy(path,shell_home);
    change_chdir(path);
    printf("%s\n",path);
    return;
}
void slash_delta_path(char* shell_home, char* path, char* previous_dir, char* token){
    char remaining_path[100];
    char dup_path[100];
    if(strcmp(shell_home,path)!=0){
        delta_hop(previous_dir,path,shell_home);
    }
    substring(remaining_path,token,6,100);
    strcpy(dup_path,path);
    strcat(dup_path,"/");
    strcat(dup_path,remaining_path);
    if(directory_exists(dup_path)){
        strcpy(previous_dir,path);   
        strcpy(path,dup_path);
        printf("%s\n",path);
        change_chdir(path); 
    }
    else{
        printf("Directory doesn't exist\n");
    }

}
int directory_exists(char* path){
    struct stat stats;

    stat(path, &stats);
    if (S_ISDIR(stats.st_mode)){
        return 1;
    }
    return 0;
}