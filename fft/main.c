#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include "fourier.h"
#define  DDC_PI  (3.14159265358979323846)
#define CHECKPOINTER(p)  CheckPointer(p,#p)

 int MAXSIZE;
 int MAXWAVES;
float *RealIn=NULL;
float *RealOut=NULL;
float *ImagOut=NULL;
float *ImagIn=NULL;
float *goldReal=NULL;
float *goldImag=NULL;
void fft_float (
    unsigned  NumSamples,          /* must be a power of 2 */
    int       InverseTransform,    /* 0=forward FFT, 1=inverse FFT */
    float    *RealIn,              /* array of input's real samples */
    float    *ImaginaryIn,         /* array of input's imag samples */
    float    *RealOut,             /* array of output's reals */
    float    *ImaginaryOut );
static void CheckPointer ( void *p, char *name )
{
    if ( p == NULL )
    {
        fprintf ( stderr, "Error in fft_float():  %s == NULL\n", name );
        exit(1);
    }
}
#define TRUE  1
#define FALSE 0
#define NUM_EXEC 5
#define BITS_PER_WORD   (sizeof(unsigned) * 8)
int s;
struct sockaddr_in server;
unsigned int buffer[4];

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
    fread(goldReal,sizeof(float),MAXSIZE,fp);
    fread(goldImag,sizeof(float),MAXSIZE,fp);

    fclose(fp);
}
void initInput(char* input_file){
    FILE* fp;
    int i,j;
    fp = fopen(input_file,"rb");
    fread(RealIn,sizeof(float),MAXSIZE,fp);
    for(i=0;i<MAXSIZE;i++){
        ImagIn[i]=0.0;
    }
    fclose(fp);
}


int main(int argc, char *argv[]) {

	unsigned int i,j;
    int ex;

    int num_SDCs=0;
    int status_app=0;
    //printf("begin\n");
    unsigned int port = atoi(argv[2]);
    setup_socket(argv[1],port);
    int MAXSIZE=1<<(atoi(argv[3]));
    //printf("lol\n");
    RealIn=(float*)malloc(sizeof(float)*MAXSIZE);
    RealOut=(float*)malloc(sizeof(float)*MAXSIZE);
    ImagOut=(float*)malloc(sizeof(float)*MAXSIZE);
    ImagIn=(float*)malloc(sizeof(float)*MAXSIZE);
    goldReal=(float*)malloc(sizeof(float)*MAXSIZE);
    goldImag=(float*)malloc(sizeof(float)*MAXSIZE);
    //printf("socket set\n");
    initGold(argv[5]);
      initInput(argv[4]);
    while(1){
      

                



          /* regular*/
          fft_float (MAXSIZE,0,RealIn,ImagIn,RealOut,ImagOut);

          status_app=0;
        //printf("unsigned int goldRealOut[]={\n\r");

              for(i=0; i<MAXSIZE; i++)
              {
                  //printf("0x%lX,\n\r",*((uint32_t*)&RealOut[i]));
  		          if((*((unsigned int*)&RealOut[i]) != goldReal[i]) || (*((unsigned int*)&ImagOut[i]) != goldImag[i]))
                    {
                        if(status_app==0){
                            buffer[0] = 0xDD000000;

                        }else{
                            buffer[0] = 0xCC000000;
                        }

                        buffer[1] = *((uint32_t*)&i);
                        buffer[2] = *((uint32_t*)&RealOut[i]);
                        buffer[3] = *((uint32_t*)&ImagOut[i]); // u32, float has 32 bits

                        send_message(4);
                        status_app=1;
                    }

              }

                //printf("};");
                //return 0;
        

        if(status_app==0){
            buffer[0] = 0xAA000000; //sem erros
            send_message(1);
        }
    }
        return 0;



    }
