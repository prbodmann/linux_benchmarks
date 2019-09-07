#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <stdlib.h>

#define UNLIMIT



#define MOD 1000

#define NUM_EXEC 100

#define US_TO_S 0.000001
#define US_TO_MS 0.001

#define APP_SUCCESS            0xAA000000
#define APP_SDC            	   0xDD000000 //SDC
#ifndef MAXARRAY
#define MAXARRAY 1000000
#endif
int s;
struct sockaddr_in server;
unsigned int buffer[4];
long long input[MAXARRAY];
double* array=(double*)input;
long long temp_gold[MAXARRAY];
double* gold=(double*)temp_gold;
// TIMER instance
//XTmrCtr timer_dev;

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

typedef struct my3DVertexStruct {
  int x, y, z;
  double distance;
} T_VERTEX;


void initGold(char* gold_file){
    FILE* fp = fopen(gold_file,"rb");
    int i;
    for(i=0;i<MAXARRAY;i++){
        fread(&gold[i],sizeof(double),1,fp);

    }
    fclose(fp);
}
void initInput(char* input_file){
    FILE* fp;
    int i;
    fp = fopen(input_file,"rb");
    for(i=0;i<MAXARRAY;i++){
        fread(&array[i],sizeof(double),1,fp);

    }
    fclose(fp);
}
void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));
int compare(const void *elem1, const void *elem2)
{
  /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
  /* sort based on distances from the origin... */

  double distance1, distance2;

  distance1 = (*((double *)elem1));
  distance2 = (*((double *)elem2));

  return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}



//---------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int Status = 0;
    unsigned int port = atoi(argv[2]);
    setup_socket(argv[1],port);
    int status_app=0;
    int i;
    int ex=0;
    uint64_t aux;
    int endexec=0;
    int count=0;

    int x,y,z;
	int cont=0,num_erros=0;
    initGold(argv[4]);

    while(1)
    {
    	initInput(argv[3]);
    	//printf("0\n");


    	//########### control_dut ###########


        printf("aha\n");
    	qsort(array,MAXARRAY,sizeof(double),compare);

    	//XTime_GetTime(&tEnd);
    	//printf("%.8f us\n",1.0*(tEnd - tStart)/(COUNTS_PER_SECOND/1000000));


        //to test faults
    	//if (count == 100){
        //    array[30].distance = 15.21;array[32].distance = 40.85;array[31].distance = 20.21;array[33].distance = 17.21; count=0;
        //}
        // check for errors
        num_erros=0;
        for (i=0;i<MAXARRAY;i++)
        {

            if (array[i] != gold[i])
              {
                  //printf("%lf - %lf;\n",array[i],gold[i]);
                 num_erros++;

              }
        	//printf("a");
        }
        //printf("end");
        //while(1);
    	//########### control_dut ###########
        if (num_erros == 0) // sem erros
    	{
            //printf("ok\n");
    		buffer[0] = APP_SUCCESS; //sem erros
    		send_message(1);

    	}
    	else{
    		buffer[0] = 0xDD000000; //sem erros
    		buffer[1] = *((uint32_t*)&num_erros);
            //printf("not ok %d\n",num_erros);
    		send_message(2);
    	}

        //printf("1\n");
        //ex++;
				//cont++;
    }

    return 0;
}
