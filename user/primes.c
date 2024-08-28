#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int upBound = 35;
    int p[35][2];
    pipe(p[0]);
    int buf[35];

    if(fork() == 0){
        for(int i = 0; ; ++i){
            pipe(p[i+1]);
            int j = 0;
            int prime;

            close(p[i][1]);
            if(read(p[i][0], &prime, sizeof(prime)) > 0){
                printf("prime %d\n", prime);
            }else{
                close(p[i][0]);
                close(p[i+1][0]);
                close(p[i+1][1]);
                exit(0);
            }

            if(fork() == 0){
                close(p[i][0]);
            }else{
                while(read(p[i][0], buf + j, sizeof(int)) > 0){
                    if(buf[j] % prime != 0){
                        write(p[i + 1][1], (void *)(buf + j), sizeof(int));
                    }
                    ++j;
                }
                close(p[i][0]);
                close(p[i+1][0]);
                close(p[i+1][1]);

                wait(0);
                exit(0);
            }            
        }
    }else{
        for(int i = 2; i <= upBound; ++i){
            write(p[0][1], &i, sizeof(int));
        }
        close(p[0][0]);
        close(p[0][1]);
        wait(0);
        exit(0);
    }
}