#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include "message.h"
#include <stdlib.h>

static int msg_que_id=-1;      //消息队列ID

void main()
{
    int ret,send_len,rece_len;            //发送消息标志位；发送消息长度；接受消息长度
    Message send_msg,rece_msg;            //发送信息结构体，接受信息结构体
    int ticket_number;                    //所需票数

    //ser_list将创建消息队列，cli_list只需要赋值获取已经存在的消息队列
    msg_que_id=msgget(BOOKING_KEY,0);            //检查消息队列是否存在
    if(msg_que_id==-1)   //获取消息队列失败
    {
        perror("get message list ID");
        exit(1);
    }
    while(1)
    {
        //接收用户的输入
        printf("How many tickets do you want?");
        scanf("%d",&ticket_number);
        printf("the user wants %d tickets.\n",ticket_number);

        //填写消息的标志
        send_msg.m_type=CLIENT_INPUT;
        
        //填写消息的内容，从process_id开始都是消息的具体数据
        send_msg.process_id=getpid();
        send_msg.m_buffer[0]=ticket_number;

        //消息的长度不包括消息的标志，所以只包含两项process_id和m_buffer[0]
        send_len=sizeof(long)+sizeof(int);

        if((ret=msgsnd(msg_que_id,&send_msg,send_len,0))<0)         //发送消息队列的消息
        {
            perror("send message");
            exit(1);
        }

        //从消息队列中接收消息的回应，消息的标志是进程号
        rece_len=msgrcv(msg_que_id,&rece_msg,sizeof(Message)-sizeof(long),getpid(),0);
        if(rece_len<0)
        {
            perror("receive message");
            exit(1);
        }
        if(rece_msg.m_buffer[0]==1)
            printf("booking ticket sucess.\n");
        else
            printf("booking ticket failed.\n");
    }
}
