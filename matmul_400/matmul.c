
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
#include "golden.h"
#define MOD 1000

#define NUM_EXEC 1
#define PI 3.1415926535897932384626433
#define MATRIX_SIZE 400 // matrix size

#define US_TO_S 0.000001
#define US_TO_MS 0.001
#define SIZE        ((MATRIX_SIZE)*(MATRIX_SIZE))

float mA[MATRIX_SIZE][MATRIX_SIZE];
float mB[MATRIX_SIZE][MATRIX_SIZE];
float mCS0[MATRIX_SIZE][MATRIX_SIZE];
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


int s;
//struct sockaddr_in server;
unsigned int buffer[4];

//---------------------------------------------------------------------------
int main(int argc, char **argv)
{
	//int Status = 0;
    unsigned int port = atoi(argv[2]);
    setup_socket(argv[1],port);

    int i = 0;
    int j = 0;
    int k = 0;
    ///int e = 0;
    int status_app;
    int ex;
    //int count = 0;

    for(i=0; i<MATRIX_SIZE; i++)
    {
    	for(j=0; j<MATRIX_SIZE; j++)
    	{
        	//xil_printf("i");
        	mA[i][j] = (float)(i * PI+4.966228736338716478); // generate nice longs/longs
        	mB[i][j] = (float)(i / PI+2.726218736218716238);
        	mCS0[i][j] = 0;
    	}
    }
    //XTime tStart, tEnd, endexec;
    //int cont=0;


        	//XTime_GetTime(&tStart);
        	//XTime tStart, tEnd;
        	//XTime_GetTime(&tStart);
        	//printf("0\n");
        	//########### control_dut ###########
        while(1){
            for(ex=0;ex<NUM_EXEC;ex++){
                status_app    = 0x00000000;
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

                for (i=0; i<MATRIX_SIZE; i++)
                {
                    //printf("{");
                	for(j=0; j<MATRIX_SIZE; j++)
                	{///
                        //printf("0x%X,",*((unsigned int*)&mCS0[i][j]));
                		if((*(unsigned int*)&mCS0[i][j] != float_golden[i][j]))
                		{
                			if(status_app==0){
                				buffer[0] = 0xDD000000;

                			}else{
                				buffer[0] = 0xCC000000;
                			}


                            status_app = 1;
                			buffer[1] = *((unsigned int*)&i);
                			buffer[2] = *((unsigned int*)&j);
                			buffer[3] = *((unsigned int*)&mCS0[i][j]); // u32, float has 32 bits
                			send_message(4);
                    		}
                	}
                    //printf("},\n");
                	///printf("a");
                }
                //return 0;
                    //printf("end");
            }

    	//########### control_dut ###########
    	if (status_app == 0x00000000) // sem erros
    	{
    		buffer[0] = APP_SUCCESS; //sem erros
    		send_message(1);
    	}
    }
			return 0;

}
