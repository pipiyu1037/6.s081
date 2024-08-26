#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), '\0', 1);
    return buf;
}

void findInDir(char *dir, char* fileName)
{
    struct dirent de;
    struct stat st;
    int fd;
    char buf[512], *p;

    if((fd = open(dir, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", dir);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", dir);
        close(fd);
        return;
    }


    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof(buf)){
        printf("ls: path too long\n");
        close(fd);
        return;
    }

    strcpy(buf, dir);
    p = buf + strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0) continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }

        char* name = fmtname(buf);

        if(st.type == T_DIR){
            if(strcmp(name, ".") != 0 && strcmp(name, "..") != 0){
                findInDir(buf, fileName);
            }
        }else if(st.type == T_FILE && strcmp(name, fileName) == 0){
            printf("%s\n", buf);
        }
    }

    return;
}

int
main(int argc, char *argv[])
{
    if(argc < 3){
        printf("argument less than 3.\n");
        exit(0);
    }
    findInDir(argv[1], argv[2]);

    exit(0);
}