#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    int p2[2];
    pipe(p);
    pipe(p2);
    char buf[64];
    int pid = fork();
    
    if(pid == 0){
        int childPid = getpid();

        close(p[1]);
        read(p[0], (void *)buf, 1);
        printf("%d: received ping\n", childPid);
        
        write(p2[1], (void *)buf, 1);
        close(p[0]);
        close(p2[0]);
        close(p2[1]);
        exit(0);
    }else if(pid > 0){
        int parentPid = getpid();
        write(p[1], "a", 1);
        
        close(p2[1]);
        read(p2[0], (void *)buf, 1);
        printf("%d: received pong\n", parentPid);
        close(p[0]);
        close(p[1]);
        close(p2[0]);
        exit(0);
    }
    exit(0);
}