#include "headers.h"
char* lstrip(char* str){
    if (str == NULL) {
        return str;
    }
    while (*str != '\0' && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}
char* rstrip(char* str){
    char *end = str + strlen(str) - 1;  
    while (end >= str && isspace((unsigned char)*end)) {  
        *end = '\0';  
        end--; 
    }
    return str;
}
char *trim(char *str) {
    while (*str == ' ') str++;
    char *end = str + strlen(str) - 1;
    while (end > str && *end == ' ') {
        *end = '\0';
        end--;
    }
    return str;
}
int sleep_compare(char* token){
    char command[100];
    int time;
    int len=strlen(token);
    sscanf(token,"%s %d",command,&time);
    if(strcmp(command,"sleep")==0 && time>2){
        return 1;
    }
    else{
        return 0;
    }

}
void add_prompt_sleep(char* token,int is_sleep_there,char* add_sleep){
    if(is_sleep_there){
        strcat(add_sleep,token);
        strcat(add_sleep,"s : ");
    }
    else{
        // return 0;
    }
}