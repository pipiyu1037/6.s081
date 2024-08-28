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
            *p++ = *suffix++;
        }
    }

    return i;
}

int
main(int argc, char* argv[])
{
    char* com = argv[1];

    printf("command: %s argc: %d\n", com, argc);
    char* fixedArg[MAXARG];

    char suffixArg[64], *p;

    for(int i = 1; i < argc; ++i){
        fixedArg[i-1] = argv[i];
        printf("fix: %s \n", fixedArg[i-1]);
    }

    p = suffixArg;

    while(read(0, (void *)p, 1)){
        if(*p == '\0'){
            break;
        }

        if(*p == '\n'){
            *p = '\0';

            p = suffixArg;
            continue;
        }
        ++p;
    }

    printf("suffix: %s \n", suffixArg);
    
    // for(int num = parseLine(&suffixArg, fixedArg + argc -1); num != 0; ){
    //     fixedArg[argc + num -1] = 0;

    //     printf("num: %d\n", num);

    //     if(fork() == 0){
    //         for(int i = 0; i < argc + num - 1; i++){
    //             printf("%s\n", fixedArg[i]);
    //         }
    //         exec(com, fixedArg);
    //     }else{
    //         wait(0);
    //         exit(0);
    //     }
        
    // }

    exit(0);
}
