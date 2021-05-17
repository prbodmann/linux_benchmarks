/*
 * =====================================================================================
 *
 *       Filename:  suite.c
 *
 *    Description:  The main wrapper for the suite
 *
 *        Version:  1.0
 *        Created:  10/22/2009 08:40:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liang Wang (lw2aw), lw2aw@virginia.edu
 *        Company:  CS@UVa
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include "common.h"


static int do_verify = 0;
int omp_num_threads = 1;

static struct option long_options[] = {
    /* name, has_arg, flag, val */
    {"input", 1, NULL, 'i'},
    {"gold", 1, NULL, 'g'},
    {"size", 1, NULL, 's'},
    {0,0,0,0}
};
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
extern void
lud_omp(FP *m, int matrix_dim);

int iteractions = 100000;
int
main ( int argc, char *argv[] )
{

    int matrix_dim = atoi(argv[3]); /* default size */
    int opt, option_index=0;
    func_ret_t ret;
    const char *input_file = argv[4];
    const char *gold_file = argv[5];
    FP *m, *gold;
    stopwatch sw;
    FILE *f_a, *f_gold;
    int n = matrix_dim;

    unsigned int port = atoi(argv[2]);
    setup_socket(argv[1],port);
    if (input_file && gold_file && matrix_dim>0) {
       
        
        f_a = fopen(input_file, "rb");
        f_gold = fopen(gold_file, "rb");

        if ((f_a == NULL) || (f_gold == NULL)) {
            printf("Error opening files\n");
            exit(EXIT_FAILURE);
        }

        m = (FP*) malloc(sizeof(FP)*n*n);
        gold = (FP*) malloc(sizeof(FP)*n*n);
        //std::cout << "read...";

        //fclose(f_a);
        //fclose(f_gold);
    }
    else {
        printf("No input, gold, or matrix_dim specified!\n");
        exit(EXIT_FAILURE);
    }

    fread(gold, sizeof(FP) * n * n, 1, f_gold);
    while(1) {
        rewind(f_a);
        fread(m, sizeof(FP) * n * n, 1, f_a);
        

        lud_omp(m, matrix_dim);

        int i, flag = 0;        
        for ( i = 0; i < matrix_dim; i++) {
            int j;
            for ( j = 0; j < matrix_dim; j++) {
               
                if(m[i + matrix_dim * j] != gold[i + matrix_dim * j])
                {
                     if(flag==0){
                        buffer[0] = 0xDD000000;
                        flag=1;
                    }else{
                        buffer[0] = 0xCC000000;
                    }
                    buffer[1]=*((unsigned int*)&i); 
                    buffer[2]=*((unsigned int*)&j); 
                    unsigned long long aux=*((unsigned long long*)&m[i + matrix_dim * j]);
                    buffer[3] = (unsigned int)((aux & 0xFFFFFFFF00000000LL) >> 32);                      
                    buffer[4] = (unsigned int)(aux & 0xFFFFFFFFLL);
                    send_message(5); 
                }
                    
            }
        }
         if(flag==0){
            buffer[0] = 0xAA000000;
              send_message(1); 
        }

       

        // read inputs again


    }


    free(m);
    free(gold);

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
