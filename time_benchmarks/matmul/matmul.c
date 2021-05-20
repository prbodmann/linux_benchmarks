
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
#include <time.h>

#define MOD 1000

#define NUM_EXEC 1
#define PI 3.1415926535897932384626433

int MATRIX_SIZE;
float *mA;
float *mB;
float *mCS0;
float *float_golden;
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

    int i = 0;
    int j = 0;
    int k = 0;
    int p = 0;
    int status_app;
    //int count = 0;
    MATRIX_SIZE=atoi(argv[3]);
    mA=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);   
    mB=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);
    mCS0=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);
    float_golden=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);
    //XTime tStart, tEnd, endexec;
    
    initGold(argv[2]);
 

        initInput(argv[1]);
    	//########### control_dut ###########
        clock_t begin = clock();
    	for (i=0; i<MATRIX_SIZE; i++)
    	{
    		for(j=0; j<MATRIX_SIZE; j++)
    		{
    			mCS0[i*MATRIX_SIZE+j] = 0.0;
    			for (k=0; k<MATRIX_SIZE; k++)
    				mCS0[i*MATRIX_SIZE+j] += mA[i*MATRIX_SIZE+k] * mB[k*MATRIX_SIZE+j];
    		}
    	}
    	clock_t middle = clock();
		double time_spent1 = (double)(middle - begin) / CLOCKS_PER_SEC;
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

        		if((mCS0[i*MATRIX_SIZE+j] != float_golden[i*MATRIX_SIZE+j]))
        		{
        			if(flag==0){
        				buffer[0] = 0xDD000000;
        				flag=1;
        			}else{
        				buffer[0] = 0xCC000000;
        			}
        			//printf("\ni=%d j=%d \n %20.18f vs %20.18f\n",i,j,mCS0[i][j],float_golden[i][j]);


        			buffer[1] = *((uint32_t*)&i);
        			buffer[2] = *((uint32_t*)&j);
        			buffer[3] = *((uint32_t*)&mCS0[i*MATRIX_SIZE+j]); // u32, float has 32 bits
        			send_message(4);

        		}
        	}
        	//printf("a");
        }
        //printf("end");
        clock_t end = clock();
        double time_spent2 = (double)(end - middle) / CLOCKS_PER_SEC;
    	//########### control_dut ###########
        printf("time: %lf %lf ",time_spent1,time_spent2);


    return 0;
}
