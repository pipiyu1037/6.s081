#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

char whitespace[] = " \t\r\v";

int parseLine(char* suffix, char* args[])
{
    while(strchr(whitespace, *suffix)){
        ++suffix;
    }
    int i = 0;

    for(i = 0; *suffix != '\0'; ++i) {    
        if(!args[i]){
            args[i] = (char *)malloc(32);
        }

        char* p = args[i];
        
        while(strchr(whitespace, *suffix) != 0){
            ++suffix;
        }

        while(strchr(whitespace, *suffix) == 0){
            *p = *suffix;
            if(*suffix == '\0'){
                break;
            }
            ++p;
            ++suffix;
        }
    }


    return i;
}

int
main(int argc, char* argv[])
{
    char* com = argv[1];
    char* fixedArg[MAXARG];
    char suffixArg[32], *p;

    for(int i = 1; i < argc; ++i){
        fixedArg[i-1] = argv[i];
    }

    p = suffixArg;

    while(read(0, (void *)p, 1) == 1){
        if(*p == '\0'){
            int num = parseLine(suffixArg, fixedArg + argc -1);
            fixedArg[argc + num -1] = 0;

            if(fork() == 0){
                exec(com, fixedArg);
                exit(0);
            }else{
                wait(0);
            }

            p = suffixArg;
            break;
        }

        if(*p == '\n'){
            *p = '\0';
            
            int num = parseLine(suffixArg, fixedArg + argc -1);
            fixedArg[argc + num -1] = 0;

            if(fork() == 0){
                exec(com, fixedArg);
                exit(0);
            }else{
                wait(0);
            }

            p = suffixArg;
            continue;
        }
        ++p;
    }
    exit(0);
}