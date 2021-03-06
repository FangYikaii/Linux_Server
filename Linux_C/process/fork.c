#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void main()
{
    int value=0;
    int childpid;

    fprintf(stderr,"The process before creating pid=%d\n",getpid());

    if((childpid=fork())==0)       //子进程
    {
        fprintf(stderr,"the initial valude=%d\n",value);
        value=1;
        fprintf(stderr,"the child process pid=%d\n",getpid());
        fprintf(stderr,"the value is %d after being changed by the child process\n",value);
    }
    else if(childpid>0)  //父进程
    {
        fprintf(stderr,"the parent process pid=%d\n",getpid());
        fprintf(stderr,"value in parent process is %d\n",value);
    }
    else    //fork失败
    {
        fprintf(stderr,"the fork system called fail");
    }
}
