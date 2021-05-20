#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <sys/resource.h>
#include <time.h>

int s;

unsigned int buffer[4];
int MAXARRAY;

double *distance;
long long *temp_gold;
struct sockaddr_in server;
// TIMER instance
//XTmrCtr timer_dev;



void initInput(char* input_file){
    FILE* fp;
    int i;
    fp = fopen(input_file,"rb");
    if(fp==NULL){
        printf("error opening input\n");
        exit(1);
    }
    fread(distance,sizeof(double)*MAXARRAY,1,fp);
    fclose(fp);
}
void initGold(char* input_file){
    FILE* fp;
    fp = fopen(input_file,"rb");
    if(fp==NULL){
        printf("error opening gold\n");
        exit(1);
    }
    fread(temp_gold,sizeof(long long)*3*MAXARRAY,1,fp);
    fclose(fp);
}
void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));
//---------------------------------------------------------------------------


//void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));
int compare(const void *elem1, const void *elem2);
int compare(const void *elem1, const void *elem2)
{
  /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
  /* sort based on distances from the origin... */
 // printf("hello\n\r");
  double distance1, distance2;

  distance1 = *((double*)elem1);
  distance2 = *((double*)elem2);
//printf("%f %f %d",distance1,distance2,(distance1 > distance2) ? 1 : ((distance1 < distance2) ? -1 : 0));
  return (distance1 > distance2) ? 1 : ((distance1 < distance2) ? -1 : 0);
}

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


int main(int argc, char **argv)
{

    int i;
    MAXARRAY=atoi(argv[3]);    
    
    distance=(double*)malloc(sizeof(double)*MAXARRAY);
    if(distance==NULL){
        printf("error malloc distance\n");
        exit(1);
    }
    temp_gold=(long long *)malloc(sizeof(long long )*MAXARRAY);
     if(temp_gold==NULL){
        printf("error malloc gold\n");
        exit(1);
    }
    initGold(argv[1]);


        initInput(argv[2]);

        
        //printf("0\n");

        //status_app    = 0x00000000;
        //########### control_dut ###########
        clock_t begin = clock();
        qsort(distance,MAXARRAY,sizeof(double),compare);
        clock_t middle = clock();
        double time_spent1 = (double)(middle - begin) / CLOCKS_PER_SEC;
        int num_SDCs = 0;
        for (i=0;i<MAXARRAY;i++)
        {
            /*char test[200];
            long *test1=(long*)&distance[i];
            long *test2=(long*)&temp_gold[i];
            sprintf(test,"gold[%d]=0x%08lx%08lx;\n\r",i,test1[1],test1[0]);
            //sprintf(test,"gold[%d]=0x%lx%lx;\n\r",i,test1[1],test1[0]);
            printf (test);*/
            //printf("gold[%d]=0x%llx;\n",i,*((long long *)&distance[i]));
            if (*((long long *)&distance[i]) != temp_gold[i])
            {

                num_SDCs++;


            }
            //printf("a");
        }
        clock_t end = clock();
        double time_spent2 = (double)(end - middle) / CLOCKS_PER_SEC;
       printf("time: %lf %lf ",time_spent1,time_spent2);
    //while(1);
    //########### control_dut ###########


    return 0;
}
