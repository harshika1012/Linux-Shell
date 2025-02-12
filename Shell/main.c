#include "headers.h"
int main(){
    struct utsname uts;
    uid_t uid=getuid();
    struct passwd *pw = getpwuid(uid);
    char shell_home[100];
    char path[4096];
    char str[4096];
    char file_path_abs[4096];
    char add[4096];
    char previous_dir[4096];
    char remaining_path[4096];
    char after_home[4096];
    char rel_path[4096];
    int back_gr[100];
    char *tokens_arr[4096];
    char *token_final[4096];
    int status;
    int token_arr_size=0;
    int is_sleep_there=0;
    char add_sleep[1034];
    char* token;
    char original_str[4096];
    int is_background=0;
    getcwd(shell_home,sizeof(shell_home));
    const char* home=pw->pw_dir;
    strcpy(path,shell_home);
    strcpy(previous_dir,path);              //// set this to current dir if in case it is the first command(-)
    const char* filepath="/home/samaharshika/Documents/osn/ass2/log.txt";
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;  // Restart system calls if interrupted
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }


    while(1){
        
        if(uname(&uts)<0){
            printf("Error in uname function\n");
            exit(1);
        }
        if(!pw){
            printf("Error in getpwuid function\n");
            return 1;
        }
        if(path==NULL){
            printf("Error in getcwd function\n");

        }
        // printf("%s\n",path);
        if(strcmp(path,shell_home)==0){
            // printf("Home directory\n");
            if(add_sleep[0]!='\0'){
                printf("<%s@%s:~ %s >",pw->pw_name,uts.nodename,add_sleep);
                is_sleep_there=0;
                add_sleep[0]='\0';
            }
            else{
                printf("<%s@%s:~>",pw->pw_name,uts.nodename);
            }
        }
        else{
            // printf("Some path\n");
            int len=strlen(shell_home);
            if( strncmp(path,shell_home,len)==0){
                // printf("I need to print relative path\n");
                substring(rel_path,path,len,100);
                if(add_sleep[0]!='\0'){
                    // printf("Enterred\n");
                    printf("<%s@%s:~%s %s>",pw->pw_name,uts.nodename,rel_path,add_sleep);
                    is_sleep_there=0;
                    add_sleep[0]='\0';
                }
                else{
                    // printf("no1\n");
                    printf("<%s@%s:~%s>",pw->pw_name,uts.nodename,rel_path);
                }
            }   
            else{
                // printf("Outside shell code  - so absolute path\n");
                if(add_sleep[0]!='\0'){
                    // printf("Enter\n");
                    printf("<%s@%s:%s %s>",pw->pw_name,uts.nodename,path,add_sleep);
                    is_sleep_there=0;
                    add_sleep[0]='\0';
                }
                else{
                    // printf("no2\n");
                    printf("<%s@%s:%s>",pw->pw_name,uts.nodename,path);
                }
            }
        }
        fgets(str,sizeof(str),stdin);
        int len=strlen(str);
        char given_ip[1046];
        strcpy(given_ip,str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
        }
        strcpy(original_str,str);
        int token_arr_size=0;
        //  printf("%c\n",str[len-2]);
        // printf("%s\n",str);
        memset(back_gr, 0, sizeof(back_gr));
        tokens_arr[token_arr_size]=strtok(str,";");
        while(tokens_arr[token_arr_size]!=NULL){
            token_arr_size++;
            tokens_arr[token_arr_size]=strtok(NULL,";");
        }
        int no_of_com=0;
        int final_size=0;
        int size_back_gr=0;
        if(checking_in_out(tokens_arr[0],strlen(tokens_arr[0]))){
            printf("hello\n");
            // return 0;
        }
        else{
            for (int no_of_com = 0; no_of_com < token_arr_size; no_of_com++) {
            char *command = trim(tokens_arr[no_of_com]);
                if (strlen(command) == 0) continue;

                char *parts = strchr(command, '&');
                while (parts != NULL) {
                    int cmd_length = parts - command;
                    if (cmd_length > 0) {
                        char *sub_command = strndup(command, cmd_length); 
                        sub_command = trim(sub_command); 

                        if (strlen(sub_command) > 0) {
                            token_final[final_size] = sub_command;
                            back_gr[final_size] = 1;  
                            final_size++;
                        } else {
                            free(sub_command);  
                        }
                    }
                    command = parts + 1;
                    command = trim(command);
                    parts = strchr(command, '&');
                }
                command = trim(command); 
                if (*command != '\0') {
                    token_final[final_size] = command;
                    back_gr[final_size] = 0; 
                    final_size++;
                }
            }
        }
        int size=0;
        char tk[100];
        char *for_back[100];
        token_final[final_size]=NULL;
        int check=check_log(token_final,final_size);
        if(check==0){
            // printf("String = %s\n",original_str);
            write_strings_to_file(filepath,original_str);
        }
        while(size<final_size){
            print_completed_processes();
            strcpy(tk,token_final[size]);
            if(back_gr[size]==1){
                run_background(tk);
            }
            else{
                // printf("This is a foreground process\n");
            
                if(strcmp(tk,"hop.")==0){
                    current_directory(path,home);
                    // printf("Path after %s\n",path);
                }
                else if(strcmp(tk,"hop~")==0){
                    delta_hop(previous_dir,path,shell_home);
                    // printf("Path after %s\n",path);
                
                }
                else if(strcmp(tk,"hop-")==0){
                    previous_directory(previous_dir,path);
                    // printf("Path after %s\n",path);
                }
                else if(strncmp(tk,"hop..",5)==0){
                    dot_dot(path,tk,previous_dir);
                    // printf("Path after %s\n",path);
                }
                else if(strncmp(tk,"hop ~/",6)==0){
                    slash_delta_path(shell_home,path,previous_dir,tk);
                }
                else if(strncmp(tk,"hop /",5)==0){
                    strcpy(previous_dir,path);
                    substring(file_path_abs, tk, 4,100);
                    if(directory_exists(file_path_abs)){    //// for absolute paths
                        printf("%s\n",file_path_abs);
                        strcpy(path,file_path_abs);

                    }
                    else{
                        printf("Directory doesn't exist\n");
                    }
                }
                else if(strncmp(tk,"hop ",4)==0){                   // hop dir_name
                    to_directory(previous_dir,path,tk);
                }
                else if(strcmp(tk,"proclore")==0){
                    proclore_print();
                }
                else if(strncmp(tk,"log execute ",11)==0){
                    int num;
                    if (sscanf(tk + 11, "%d", &num) == 1) { 
                        print_specific_line(filepath, num);  
                    }
                }
                else if(strcmp(tk,"log purge")==0){
                    clear_file(filepath);
                }
                else if(strcmp(tk,"log")==0){
                    log_print(filepath);
                }
                else if(strncmp(tk,"seek ",4)==0){
                    seek_categories(tk);
                }
                else if(strncmp(tk,"reveal",6)==0){
                    // printf("%s %s %s\n",tk,str,tk);
                    printf("path in main %s\n",path);
                    // reveal(given_ip,path);
                }
                else{
                    is_sleep_there=sleep_compare(tk);
                    add_prompt_sleep(tk,is_sleep_there,add_sleep);
                    run_foreground(tk);
                }
            }
            size++;
        }

    }

}

