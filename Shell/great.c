#include "headers.h"
int checking_in_out(char* input,int len){
    for(int i=0;i<len;i++){
        if(input[i]=='<' || input[i]=='>'){
            return 1;
        }
    }
    return 0;
}
void find_less_great(char* input,int len){
    char command[100];
    for(int i=0;i<len;i++){
        if(input[i]=='<'){
            substring(command,input,0,i);
            trim(command);
            read_input();
        }
    }
}