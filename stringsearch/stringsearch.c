#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include<arpa/inet.h>
#include<sys/socket.h>


#include "large_golden.h"

#define OUTPUT_SIZE 1332

static size_t table[UCHAR_MAX + 1];
static size_t len;
static char *findme;


#define MOD 1000

#define NUM_EXEC 1

#define US_TO_S 0.000001
#define US_TO_MS 0.001

#define APP_SUCCESS            0xAA000000
#define APP_SEND_DATA          0x11111111
#define APP_SDC            	   0xDD000000 //SDC

int s;
struct sockaddr_in server;
unsigned int buffer[4];



// TIMER instance
//XTmrCtr timer_dev;

/*
**  Call this with the string to locate to initialize the table
*/

void init_search(const char *string)
{
      size_t i;

      len = strlen(string);
      for (i = 0; i <= UCHAR_MAX; i++)                      /* rdg 10/93 */
            table[i] = len;
      for (i = 0; i < len; i++)
            table[(unsigned char)string[i]] = len - i - 1;
      findme = (char *)string;
}

/*
**  Call this with a buffer to search
*/

char *strsearch(const char *string)
{
      register size_t shift;
      register size_t pos = len - 1;
      char *here;
      size_t limit=strlen(string);

      while (pos < limit)
      {
            while( pos < limit &&
                  (shift = table[(unsigned char)string[pos]]) > 0)
            {
                  pos += shift;
            }
            if (0 == shift)
            {
                  if (0 == strncmp(findme,
                        here = (char *)&string[pos-len+1], len))
                  {
                        return(here);
                  }
                  else  pos++;
            }
      }
      return NULL;
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

//---------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int Status = 0;
    unsigned int port = atoi(argv[2]);
    setup_socket(argv[1],port);
    int status_app=0;


    char *here;

    int i = 0;

    char output[OUTPUT_SIZE];

    int count = 0;

    int cont = 0;
    while(1)
    {
    	int num_SDCs = 0;
    	//printf("0\n");
    	//########### control_dut ###########

    	status_app    = 0x00000000;
    	//########### control_dut ###########
    	//XTime_GetTime(&tStart);
    	//XTime tStart, tEnd;
    	//XTime_GetTime(&tStart);

    	//
        for (i = 0; find_strings[i]; i++)
        {
              init_search(find_strings[i]);
              here = strsearch(search_strings[i]);
              //printf("\"%s\" is%s in \"%s\"", find_strings[i],
              //      here ? "" : " not", search_strings[i]);
              if (here)
              {
                //printf(" [\"%s\"]   out = 1", here);
                output[i] = '1';
              }
              else
              {
                //printf("    out = 0");
                output[i] = '0';
              }
              //putchar('\n');
        }
        //XTime_GetTime(&endexec);
    	//XTime_GetTime(&tEnd);
    	//printf("%.8f us\n",1.0*(tEnd - tStart)/(COUNTS_PER_SECOND/1000000));

        //to test faults
    	//if (count == 5000)
    	//output[30] = 15;output[31] = 16;output[32] = 17; count=0;

        // check for errors
        for (i=0; i< OUTPUT_SIZE; i++)
        {
            if (output[i] != golden_large[i])
              {

            	if(num_SDCs == 0)
            	{
            		//printf ("SDC  ");
            		status_app= 1;
            		buffer[0] = 0xDD000000;
            		buffer[1] = *((uint32_t*)&i);
            		buffer[2] = *((char*)&output[i]);
            		send_message(3);
            		num_SDCs ++;
            	}
            	else
				{
            		//printf ("SDC  ");
            		status_app= 1;
            		buffer[0] = 0xCC000000;
            		buffer[1] = *((uint32_t*)&i);
                    buffer[2] = *((char*)&output[i]);
            		send_message(3);
            		num_SDCs ++;
				}

              }
        	//printf("a");
        }
        //printf("end");

    	//########### control_dut ###########
    	if (status_app == 0x00000000 && count==600) // sem erros
    	{
    		buffer[0] = APP_SUCCESS; //sem erros
    		send_message(1);
    		count=0;
        }
        if(cont==100){
          return 0;
        }
        cont++;
        //printf("%d\n",count);
        count++;
    }


    return 0;
}
