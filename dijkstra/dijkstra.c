/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

//CONTROL_DUT configuration
#define CONTROL_DUT_ADDR       0x44000000
#define STATUS_RBK_CORE0_ADDR  0x44000010
#define STATUS_RBK_CORE1_ADDR  0x44000020
#define STATUS_APP_ADDR        0x44000054
#define DATA_SIZE_ARM_DUT_ADDR 0x440000E0
#define DATA_ARM_DUT_ADDR 	   0x44000110

#define APP_SUCCESS            0xAA000000
#define APP_SEND_DATA
#define APP_SDC            	   0xDD000000 //SDC
#define APP_SDC_MULTIPLE       0xCC000000

// 1 if using control_dut
int control_dut_inuse = 1;

// configuration of the algorithm


#define NONE                               9999
#define NUM_EXEC 1
#define MAX_HEAP 1*1024*1024
typedef  unsigned char uchar;
int result_heap[MAX_HEAP];
size_t heap_ptr=0;

int golden[]={ 0, 41, 45, 51, 50
		, 1, 58, 57, 20, 40, 17, 65, 73, 36, 46, 10, 38, 41, 45, 51
		, 2, 71, 47, 79, 23, 77, 1, 58, 57, 20, 40, 17, 52
		, 3, 53
		, 4, 85, 83, 58, 33, 13, 19, 79, 23, 77, 1, 54
		, 5, 26, 23, 77, 1, 58, 99, 3, 21, 70, 55
		, 6, 42, 80, 77, 1, 58, 99, 3, 21, 70, 55, 56
		, 7, 17, 65, 73, 36, 46, 10, 58, 57
		, 8, 37, 63, 72, 46, 10, 58
		, 9, 33, 13, 19, 79, 23, 77, 1, 59
		, 10, 60
		, 11, 22, 20, 40, 17, 65, 73, 36, 46, 10, 29, 61
		, 12, 37, 63, 72, 46, 10, 58, 99, 3, 21, 70, 62
		, 13, 19, 79, 23, 77, 1, 58, 99, 3, 21, 70, 55, 12, 37, 63
		, 14, 38, 41, 45, 51, 68, 2, 71, 47, 79, 23, 77, 1, 58, 33, 13, 92, 64
		, 15, 13, 92, 94, 11, 22, 20, 40, 17, 65
		, 16, 41, 45, 51, 68, 2, 71, 47, 79, 23, 77, 1, 58, 33, 32, 66
		, 17, 65, 73, 36, 46, 10, 58, 33, 13, 19, 79, 23, 91, 67
		, 18, 15, 41, 45, 51, 68
		, 19, 69
		, 20, 40, 17, 65, 73, 36, 46, 10, 58, 99, 3, 21, 70
		, 21, 70, 55, 12, 37, 94, 11, 90, 30, 2, 71
		, 22, 20, 40, 17, 65, 73, 36, 46, 10, 58, 99, 3, 21, 70, 55, 12, 37, 63, 72
		, 23, 77, 1, 58, 57, 20, 40, 17, 65, 73
		, 24, 11, 22, 20, 40, 17, 65, 73, 36, 46, 10, 29, 74
		, 25, 79, 23, 77, 1, 58, 99, 3, 21, 70, 62, 39, 75
		, 26, 23, 91, 67, 88, 76
		, 27, 30, 2, 71, 47, 79, 23, 77
		, 28, 20, 40, 17, 65, 73, 36, 46, 10, 58, 33, 13, 92, 94, 11, 22, 78
		, 29, 74, 85, 83, 58, 33, 13, 19, 79
		, 30, 2, 71, 47, 79, 23, 77, 1, 54, 80
		, 31, 50, 81
		, 32, 66, 20, 40, 17, 65, 73, 36, 46, 10, 58, 82
		, 33, 13, 92, 94, 11, 22, 20, 40, 17, 65, 85, 83
		, 34, 41, 45, 51, 68, 2, 71, 47, 84
		, 35, 13, 92, 94, 11, 22, 20, 40, 17, 65, 85
		, 36, 46, 10, 58, 99, 3, 21, 70, 62, 86
		, 37, 94, 11, 90, 30, 2, 71, 47, 84, 87
		, 38, 41, 45, 51, 68, 2, 71, 47, 79, 23, 91, 67, 88
		, 39, 13, 19, 79, 23, 77, 1, 58, 99, 3, 21, 70, 62, 89
		, 40, 17, 65, 73, 36, 46, 10, 58, 33, 13, 92, 94, 11, 90
		, 41, 45, 51, 68, 2, 71, 47, 79, 23, 91
		, 42, 80, 77, 1, 58, 33, 13, 92
		, 43, 76, 1, 58, 33, 13, 92, 93
		, 44, 23, 77, 1, 58, 33, 13, 92, 94
		, 45, 51, 68, 2, 71, 47, 79, 23, 77, 1, 58, 95
		, 46, 10, 58, 96
		, 47, 79, 23, 77, 1, 58, 33, 13, 97
		, 48, 30, 2, 71, 47, 79, 23, 77, 1, 58, 33, 13, 19, 98
		, 49, 36, 46, 10, 58, 99
		, 50, 36, 46, 10, 58, 33, 13, 92, 64, 0
		, 51, 68, 2, 71, 47, 79, 23, 77, 1
		, 52, 26, 23, 77, 1, 58, 57, 30, 2
		, 53, 89, 65, 73, 36, 46, 10, 58, 99, 3
		, 54, 79, 23, 77, 1, 58, 33, 13, 92, 93, 4
		, 55, 12, 37, 63, 72, 46, 10, 58, 82, 5
		, 56, 60, 17, 65, 73, 36, 46, 10, 58, 33, 13, 6
		, 57, 20, 40, 17, 65, 73, 36, 46, 10, 58, 99, 3, 21, 70, 62, 86, 7
		, 58, 33, 13, 19, 79, 23, 77, 1, 59, 8
		, 59, 8, 9
		, 60, 17, 65, 73, 36, 46, 10
		, 61, 86, 7, 99, 3, 21, 70, 55, 12, 37, 94, 11
		, 62, 86, 7, 99, 3, 21, 70, 55, 12
		, 63, 72, 46, 10, 58, 33, 13
		, 64, 0, 41, 45, 51, 68, 2, 71, 47, 79, 23, 77, 1, 58, 33, 13, 97, 14
		, 65, 73, 36, 46, 10, 58, 33, 13, 19, 79, 23, 77, 18, 15
		, 66, 20, 40, 17, 65, 73, 36, 46, 10, 58, 99, 3, 21, 70, 55, 12, 27, 16
		, 67, 63, 72, 46, 10, 20, 40, 17
		, 68, 2, 71, 47, 79, 23, 77, 18
		, 69, 39, 13, 19
		, 70, 55, 12, 37, 94, 11, 22, 20
		, 71, 47, 79, 23, 77, 1, 58, 99, 3, 21
		, 72, 46, 10, 58, 33, 13, 92, 94, 11, 22
		, 73, 36, 46, 10, 58, 33, 13, 19, 79, 23
		, 74, 85, 83, 58, 99, 3, 21, 70, 24
		, 75, 66, 20, 40, 17, 65, 73, 36, 46, 10, 58, 33, 13, 19, 79, 23, 77, 18, 15, 25
		, 76, 1, 58, 33, 13, 19, 79, 23, 77, 18, 26
		, 77, 1, 58, 99, 3, 21, 70, 55, 12, 27
		, 78, 80, 77, 1, 58, 33, 13, 19, 79, 23, 28
		, 79, 23, 77, 1, 58, 57, 20, 40, 17, 65, 73, 36, 46, 10, 29
		, 80, 77, 1, 58, 57, 30
		, 81, 71, 47, 79, 23, 77, 1, 58, 33, 13, 31
		, 82, 68, 2, 71, 47, 79, 23, 77, 1, 58, 33, 32
		, 83, 58, 33
		, 84, 92, 94, 11, 22, 34
		, 85, 83, 58, 33, 13, 19, 79, 23, 91, 67, 35
		, 86, 7, 17, 65, 73, 36
		, 87, 41, 45, 51, 68, 2, 71, 47, 79, 23, 77, 1, 58, 99, 3, 21, 70, 55, 12, 37
		, 88, 27, 30, 2, 71, 47, 79, 23, 77, 1, 58, 57, 20, 40, 17, 65, 73, 36, 46, 10, 38
		, 89, 65, 73, 36, 46, 10, 58, 99, 3, 21, 70, 62, 39
		, 90, 30, 2, 71, 47, 79, 23, 77, 1, 58, 57, 20, 40
		, 91, 67, 63, 72, 46, 10, 38, 41
		, 92, 94, 11, 90, 30, 2, 71, 47, 79, 23, 77, 18, 42
		, 93, 19, 79, 23, 91, 67, 35, 43
		, 94, 11, 22, 20, 40, 17, 65, 73, 36, 46, 10, 38, 44
		, 95, 81, 71, 47, 79, 23, 77, 1, 58, 57, 20, 40, 17, 65, 73, 36, 46, 10, 38, 41, 45
		, 96, 27, 30, 2, 71, 47, 79, 23, 77, 1, 58, 57, 20, 40, 17, 65, 73, 36, 46
		, 97, 14, 38, 41, 45, 51, 68, 2, 71, 47
		, 98, 46, 10, 20, 40, 17, 65, 48
		, 99, 3, 21, 70, 55, 12, 37, 63, 72, 46, 10, 58, 33, 13, 97, 49};
#define NUM_NODES                          100
int AdjMatrix[NUM_NODES][NUM_NODES];
// num of alg execp per run (to better the simulation/experiments)

double x0 = 99000;
double allerr = 0.00000001;
int maxmitr = 10;
int s;
struct sockaddr_in server;
unsigned int buffer[4];


//int used_iter[NUM_EXEC];
//double golden_value = 2.740646095973692908;
//int golden_iter = 10;

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

struct _NODE
{
  int iDist;
  int iPrev;
};
typedef struct _NODE NODE;

struct _QITEM
{
  int iNode;
  int iDist;
  int iPrev;
  struct _QITEM *qNext;
};
typedef struct _QITEM QITEM;

QITEM *qHead = NULL;

int g_qCount = 0;
NODE rgnNodes[NUM_NODES];
int ch;
int iPrev, iNode;
int i, iCost, iDist;

void savenode(int node){
  result_heap[heap_ptr++]=node;
}

void print_path (NODE *rgnNodes, int chNode)
{
  if (rgnNodes[chNode].iPrev != NONE)
    {
      print_path(rgnNodes, rgnNodes[chNode].iPrev);
    }
  savenode( chNode);

  //fflush(stdout);
}


void enqueue(int iNode, int iDist, int iPrev)
{
  QITEM *qNew = (QITEM *)malloc(sizeof(QITEM));
  QITEM *qLast = qHead;

  qNew->iNode = iNode;
  qNew->iDist = iDist;
  qNew->iPrev = iPrev;
  qNew->qNext = NULL;

  if (!qLast)
    {
      qHead = qNew;
    }
  else
    {
      while (qLast->qNext) qLast = qLast->qNext;
      qLast->qNext = qNew;
    }
  g_qCount++;
  //               ASSERT(g_qCount);
}


void dequeue (int *piNode, int *piDist, int *piPrev)
{
  QITEM *qKill = qHead;

  if (qHead)
    {
      //                 ASSERT(g_qCount);
      *piNode = qHead->iNode;
      *piDist = qHead->iDist;
      *piPrev = qHead->iPrev;
      qHead = qHead->qNext;
      free(qKill);
      g_qCount--;
    }
}


int qcount (void)
{
  return(g_qCount);
}

void dijkstra(int chStart, int chEnd)
{

  for (ch = 0; ch < NUM_NODES; ch++)
    {
      rgnNodes[ch].iDist = NONE;
      rgnNodes[ch].iPrev = NONE;
    }

  if (chStart == chEnd)
    {
    //  printf("Shortest path is 0 in cost. Just stay where you are.\n");
    }
  else
    {
      rgnNodes[chStart].iDist = 0;
      rgnNodes[chStart].iPrev = NONE;

      enqueue (chStart, 0, NONE);

     while (qcount() > 0)
  	{
  	  dequeue (&iNode, &iDist, &iPrev);
  	  for (i = 0; i < NUM_NODES; i++)
  	    {
  	      if ((iCost = AdjMatrix[iNode][i]) != NONE)
  		      {
  		          if ((NONE == rgnNodes[i].iDist) ||
  		        (rgnNodes[i].iDist > (iCost + iDist)))
  		          {
  		              rgnNodes[i].iDist = iDist + iCost;
  		              rgnNodes[i].iPrev = iNode;
  		              enqueue (i, iDist + iCost, iNode);
  		          }
  		       }
  	    }
  	}
      print_path(rgnNodes, chEnd);

    }
}

#define US_TO_MS 0.001

int main(int argc, char **argv)
{
	int Status = 0;
    unsigned int port = atoi(argv[2]);
    setup_socket(argv[1],port);
    FILE* fp;
    int i,j,k;
    int ex;
   //int count;
    int DD_CC_flag=0;
    //XTime tStart, tEnd, endexec;
    int cont=0;
    int status_app;
    while(1)
    {
    	//while(1);
    	//printf("0\n");
    	//########### control_dut ###########
        for(ex=0;ex<NUM_EXEC;ex++){
        	status_app = 0;
            fp = fopen (argv[3],"r");

            /* make a fully connected matrix */
            for (i=0;i<NUM_NODES;i++) {
              for (j=0;j<NUM_NODES;j++) {
                /* make it more sparce */
                fscanf(fp,"%d",&k);
          			AdjMatrix[i][j]= k;
              }
            }

          DD_CC_flag=0;
          heap_ptr=0;
    	  for (i=0,j=NUM_NODES/2;i<100;i++,j++) {

    		  j=j%NUM_NODES;
    		  dijkstra(i,j);
    	   }
    	   //result_heap[10]=1;
    	   //result_heap[50]=0;
    	  /*if (cont==10){

    		  cont=0;
    	  }*/
    	  //XTime_GetTime(&endexec);
    	  for(k=0;k<heap_ptr;k++){
    		  if(result_heap[k]!=golden[k]){
    			  if(DD_CC_flag==0){
    				  buffer[0]=APP_SDC;
    				  DD_CC_flag=1;
    			  }else{
    				  buffer[0]=APP_SDC_MULTIPLE;
    			  }
    			  status_app = 1;
    			  //buffer[0] = APP_SDC;//1
    			  buffer[1] = (result_heap[k]);//4
    			  buffer[2] = k;//4

    			  send_message(3);
    		  }
    	  }
          fclose(fp);
      }
	  if (status_app == 0) // sem erros
		{
			buffer[0] = APP_SUCCESS; //sem erros
			send_message(1);			
		}



			 /*XTime_GetTime(&tEnd);
			 long long unsigned exectime1 = (endexec - tStart);

			 //printf("%llu c.c. \n",exectime);
			 //printf("%5f ms\n", (1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000000))*US_TO_MS);
			 long long unsigned exectime2 = (tEnd - tStart);
			 //printf("%llu c.c. \n",exectime);
			 printf("%5f\t%5f\n", (1.0 * (exectime1) / (COUNTS_PER_SECOND/1000000))*US_TO_MS, (1.0 * (exectime2) / (COUNTS_PER_SECOND/1000000))*US_TO_MS);
			 */if(cont==100){
				 return 0;
			 }
			 cont++;




        //printf("end");

    	//########### control_dut ###########



    }

    return 0;
}
