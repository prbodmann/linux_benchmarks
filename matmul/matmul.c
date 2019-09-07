
#include<arpa/inet.h>
#include<sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include<sys/socket.h>

#define MOD 1000

#define NUM_EXEC 1
#define PI 3.1415926535897932384626433
#ifndef MATRIX_SIZE
#define MATRIX_SIZE 256 // matrix size
#endif
#define SIZE        ((MATRIX_SIZE)*(MATRIX_SIZE))

float mA[MATRIX_SIZE][MATRIX_SIZE];
float mB[MATRIX_SIZE][MATRIX_SIZE];
float mCS0[MATRIX_SIZE][MATRIX_SIZE];
float float_golden[MATRIX_SIZE][MATRIX_SIZE];
int s;
struct sockaddr_in server;
unsigned int buffer[4];

#define US_TO_S 0.000001
#define US_TO_MS 0.001


#define APP_SUCCESS            0xAA000000

#define APP_SDC            	   0xDD000000 //SDC

// 1 if using control_dut
int control_dut_inuse = 1;

#define US_TO_MS 0.001

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

void initGold(char* gold_file){
    FILE* fp = fopen(gold_file,"rb");
    int i,j;
    i=fread(float_golden,sizeof(float),MATRIX_SIZE*MATRIX_SIZE,fp);
    if(i!=MATRIX_SIZE*MATRIX_SIZE){
        exit(-3);
    }
    fclose(fp);
}
void initInput(char* input_file){
    FILE* fp;
    int i,j;
    fp = fopen(input_file,"rb");
    i=fread(mA,sizeof(float),MATRIX_SIZE*MATRIX_SIZE,fp);
    if(i!=MATRIX_SIZE*MATRIX_SIZE){
        exit(-1);
    }
    i=fread(mB,sizeof(float),MATRIX_SIZE*MATRIX_SIZE,fp);
    if(i!=MATRIX_SIZE*MATRIX_SIZE){
        exit(-2);
    }
    fclose(fp);
}
//---------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int Status = 0;
    unsigned int port = atoi(argv[2]);
    setup_socket(argv[1],port);


    int i = 0;
    int j = 0;
    int k = 0;
    int p = 0;
    int status_app;
    //int count = 0;
    printf("%d",MATRIX_SIZE);
    //XTime tStart, tEnd, endexec;
    initInput(argv[3]);
    initGold(argv[4]);
    while(1){
    	status_app    = 0;
    	//########### control_dut ###########

    	for (i=0; i<MATRIX_SIZE; i++)
    	{
    		for(j=0; j<MATRIX_SIZE; j++)
    		{
    			mCS0[i][j] = 0.0;
    			for (k=0; k<MATRIX_SIZE; k++)
    				mCS0[i][j] += mA[i][k] * mB[k][j];
    		}
    	}
    	//XTime_GetTime(&endexec);
    	//if (count == 5)
    	//{mCS0[30][47] = 2.35; count=0;}

        // check for errors
    	//mCS0[10][20]--;
    	//mCS0[30][20]--;
    	int flag=0;
        for (i=0; i<MATRIX_SIZE; i++)
        {
        	for(j=0; j<MATRIX_SIZE; j++)
        	{

        		if((mCS0[i][j] != float_golden[i][j]))
        		{
                    status_app = 1;
        			if(flag==0){
        				buffer[0] = 0xDD000000;
        				flag=1;
        			}else{
        				buffer[0] = 0xCC000000;
        			}
        			//printf("\ni=%d j=%d \n %20.18f vs %20.18f\n",i,j,mCS0[i][j],float_golden[i][j]);


        			buffer[1] = *((uint32_t*)&i);
        			buffer[2] = *((uint32_t*)&j);
        			buffer[3] = *((uint32_t*)&mCS0[i][j]); // u32, float has 32 bits
        			send_message(4);

        		}
        	}
        	//printf("a");
        }
        //printf("end");

    	//########### control_dut ###########
    	if (status_app == 0) // sem erros
    	{
    		buffer[0] = APP_SUCCESS; //sem erros
    		send_message(1);
    	}
    }


    return 0;
}
