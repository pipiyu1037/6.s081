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
            int writed = 0;
            if(fork() == 0){
                close(p[i][1]);
                while(read(p[i][0], buf + j, sizeof(int)) > 0){
                    if(j == 0){
                        printf("prime %d\n", buf[j]);
                    }else if(buf[j] % buf[0] != 0){
                        write(p[i + 1][1], (void *)(buf + j), sizeof(int));
                        writed = 1;
                    }
                    ++j;
                }
                close(p[i][0]);
            }else{
                close(p[i][0]);
                close(p[i][1]);
                close(p[i+1][0]);
                close(p[i+1][1]);
                wait(0);
                exit(0);
            }

            if(writed == 0){
                close(p[i+1][0]);
                close(p[i+1][1]);
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