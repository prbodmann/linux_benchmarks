#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include<arpa/inet.h>
#include<sys/socket.h>


int s;
struct sockaddr_in server;
unsigned int buffer[10];
void setup_socket(char* ip_addr, int port){
    s=socket(PF_INET, SOCK_DGRAM, 0);
    //memset(&server, 0, sizeof(struct sockaddr_in));
    //printf("port: %d",port);
    //printf("ip: %s", ip_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip_addr);

}

void send_message(size_t size){
    //printf("message sent\n");
    sendto(s,buffer,4*size,0,(struct sockaddr *)&server,sizeof(server));
}
#define ARRAY_SIZE 4096
volatile unsigned int array[ARRAY_SIZE];

int main(int argc, char **argv)
{

    int i;
    unsigned int port = atoi(argv[2]);
    setup_socket(argv[1],port);
    /* allocate memory for the temperature and power arrays	*/
   

    while(1)
    {
        for(i=0;i<ARRAY_SIZE;i++)
        {
            array[i]=0xA5A5A5A5;
        }

        sleep(1);
         int flag=0;
        for(i=0;i<ARRAY_SIZE;i++)
        {
            if(array[i]!=0xA5A5A5A5){
                 if(flag==0){
                buffer[0] = 0xDD000000;
                flag=1;
                }else{
                    buffer[0] = 0xCC000000;
                }
                buffer[1] = *((unsigned int*)&i);
                unsigned int temp=array[i]^(0xA5A5A5A5);
                buffer[2] = *((unsigned int*)&temp);
                send_message(3);
            }
           
        }       
      if(flag==0){
        buffer[0] = 0xAA000000;
          send_message(1); 
        }


    }


    return 0;
}

