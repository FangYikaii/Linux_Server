#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
    int pipe_fd[2];
    int ret;
    pid_t pid;

    char buf[512]={0};

    memset(buf,0,sizeof(buf));

    ret=pipe(pipe_fd);

    if(pipe(pipe_fd)<0)
    {
        perror("pipe error:");
        exit(-1);
    }
    
    pid=fork();

    switch(pid)
    {
        case -1:   //fork错误
            perror("fork:");
            exit(-1);
        case 0:    //子进程
            printf("Child..\n");
            close(pipe_fd[1]);   //关闭写管道
            read(pipe_fd[0],buf,sizeof(buf));  //读管道
            printf("Read from pipe:%s",buf);
            exit(0);
        default:  //主进程
            printf("Parent..\n");
            close(pipe_fd[0]);  //关闭读管道
            sleep(2);   //
            write(pipe_fd[1],"Hello Linux\n",strlen("Hello Linux\n"));
    }
    wait(NULL);

    return 0;

}
