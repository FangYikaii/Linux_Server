#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PERM S_IRUSR|S_IWUSR

int main(int argc,char** argv)
{
    int shmid,ret;
    char *p_addr,*c_addr;

    //创建共享内存：shmget,返回共享键值
    if((shmid=shmget(IPC_PRIVATE,1024,PERM))==-1)
    {
        fprintf(stderr,"Creat Share Memory Error:%s\n\a",strerror(errno));
        exit(1);
    }

    //创建子进程
    ret=fork();
    if(ret==-1)        //创建子进程错误
    {
        perror("Fork error:");
    }
    else if(ret==0)    //子进程：读
    {
        sleep(1);
        c_addr=shmat(shmid,0,0);    //映射
        printf("Client get %s \n",c_addr);
        exit(0);
    }
    else              //父进程：写
    {
        p_addr=shmat(shmid,0,0);  //映射
        memset(p_addr,'\0',1024);   //把p_addr开始的1024个字符长度的地址置为'\0'
        strncpy(p_addr,argv[1],1024);    //strncpy拷贝字符串：拷贝argv[1]到p_addr中，拷贝长度为1024
        wait(NULL);
        exit(0);
    }
}
