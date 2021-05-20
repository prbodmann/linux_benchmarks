#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>
#include "./main.h"
#include "./kernel/kernel_cpu.h"
#include<arpa/inet.h>
#include<sys/socket.h>


void usage()
{
    printf("Usage: lavamd <IP> <port> <# boxes 1d> <input_distances> <input_charges> <gold_output> \n");
    printf("  # boxes 1d is the input size, 15 is reasonable\n");
}


int iteractions = 100000;

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



int main( int argc, char *argv [])
{
    char * input_distance;
    char * input_charges;
    char * output_gold;

    int i, j, k, l, m, n;

    par_str par_cpu;
    dim_str dim_cpu;
    box_str* box_cpu;
    FOUR_VECTOR* rv_cpu;
    fp* qv_cpu;
    FOUR_VECTOR* fv_cpu;
    FOUR_VECTOR* fv_cpu_GOLD;
    int nh;

    dim_cpu.boxes1d_arg = 1;

    if(argc == 5) {
 
        dim_cpu.boxes1d_arg = atoi(argv[1]);
        input_distance = argv[2];
        input_charges = argv[3];
        output_gold = argv[4];
    } else {
        usage();
        exit(1);
    }


    //printf("Configuration used: cores = %d, boxes1d = %d\n", dim_cpu.cores_arg, dim_cpu.boxes1d_arg);

    par_cpu.alpha = 0.5;

    dim_cpu.number_boxes = dim_cpu.boxes1d_arg * dim_cpu.boxes1d_arg * dim_cpu.boxes1d_arg;

    dim_cpu.space_elem = dim_cpu.number_boxes * NUMBER_PAR_PER_BOX;
    dim_cpu.space_mem = dim_cpu.space_elem * sizeof(FOUR_VECTOR);
    dim_cpu.space_mem2 = dim_cpu.space_elem * sizeof(fp);

    dim_cpu.box_mem = dim_cpu.number_boxes * sizeof(box_str);

    box_cpu = (box_str*)malloc(dim_cpu.box_mem);

    nh = 0;

    for(i=0; i<dim_cpu.boxes1d_arg; i++) {

        for(j=0; j<dim_cpu.boxes1d_arg; j++) {

            for(k=0; k<dim_cpu.boxes1d_arg; k++) {

                box_cpu[nh].x = k;
                box_cpu[nh].y = j;
                box_cpu[nh].z = i;
                box_cpu[nh].number = nh;
                box_cpu[nh].offset = nh * NUMBER_PAR_PER_BOX;

                box_cpu[nh].nn = 0;

                for(l=-1; l<2; l++) {

                    for(m=-1; m<2; m++) {

                        for(n=-1; n<2; n++) {

                            if((((i+l)>=0 && (j+m)>=0 && (k+n)>=0)==true && ((i+l)<dim_cpu.boxes1d_arg && (j+m)<dim_cpu.boxes1d_arg && (k+n)<dim_cpu.boxes1d_arg)==true) && (l==0 && m==0 && n==0)==false) {

                                box_cpu[nh].nei[box_cpu[nh].nn].x = (k+n);
                                box_cpu[nh].nei[box_cpu[nh].nn].y = (j+m);
                                box_cpu[nh].nei[box_cpu[nh].nn].z = (i+l);
                                box_cpu[nh].nei[box_cpu[nh].nn].number = (box_cpu[nh].nei[box_cpu[nh].nn].z * dim_cpu.boxes1d_arg * dim_cpu.boxes1d_arg) + (box_cpu[nh].nei[box_cpu[nh].nn].y * dim_cpu.boxes1d_arg) + box_cpu[nh].nei[box_cpu[nh].nn].x;
                                box_cpu[nh].nei[box_cpu[nh].nn].offset = box_cpu[nh].nei[box_cpu[nh].nn].number * NUMBER_PAR_PER_BOX;

                                box_cpu[nh].nn = box_cpu[nh].nn + 1;

                            }
                        }
                    }
                }

                nh = nh + 1;
            }
        }
    }


    srand(time(NULL));

    FILE *file;

    if( (file = fopen(input_distance, "rb" )) == 0 ) {
        printf( "The file 'input_distances' was not opened\n" );
        exit(1);
    }

    rv_cpu = (FOUR_VECTOR*)malloc(dim_cpu.space_mem);
    for(i=0; i<dim_cpu.space_elem; i=i+1) {
        fread(&(rv_cpu[i].v), 1, sizeof(double), file);
        fread(&(rv_cpu[i].x), 1, sizeof(double), file);
        fread(&(rv_cpu[i].y), 1, sizeof(double), file);
        fread(&(rv_cpu[i].z), 1, sizeof(double), file);
    }

    fclose(file);

    if( (file = fopen(input_charges, "rb" )) == 0 ) {
        printf( "The file 'input_charges' was not opened\n" );
        exit(1);
    }

    qv_cpu = (fp*)malloc(dim_cpu.space_mem2);
    for(i=0; i<dim_cpu.space_elem; i=i+1) {
        fread(&(qv_cpu[i]), 1, sizeof(double), file);
    }
    fclose(file);

    fv_cpu = (FOUR_VECTOR*)malloc(dim_cpu.space_mem);
    fv_cpu_GOLD = (FOUR_VECTOR*)malloc(dim_cpu.space_mem);
    if( (file = fopen(output_gold, "rb" )) == 0 ) {
        printf( "The file 'output_forces' was not opened\n" );
        exit(1);
    }
    for(i=0; i<dim_cpu.space_elem; i=i+1) {
        fv_cpu[i].v = 0;
        fv_cpu[i].x = 0;
        fv_cpu[i].y = 0;
        fv_cpu[i].z = 0;

        fread(&(fv_cpu_GOLD[i].v), 1, sizeof(double), file);
        fread(&(fv_cpu_GOLD[i].x), 1, sizeof(double), file);
        fread(&(fv_cpu_GOLD[i].y), 1, sizeof(double), file);
        fread(&(fv_cpu_GOLD[i].z), 1, sizeof(double), file);
    }

    fclose(file);



        for(i=0; i<dim_cpu.space_elem; i=i+1) {
            fv_cpu[i].v = 0;
            fv_cpu[i].x = 0;
            fv_cpu[i].y = 0;
            fv_cpu[i].z = 0;
        }

        clock_t begin = clock();
        kernel_cpu(	par_cpu,
                    dim_cpu,
                    box_cpu,
                    rv_cpu,
                    qv_cpu,
                    fv_cpu);

                          clock_t  middle = clock();
        double time_spent1 = (double)(middle - begin) / CLOCKS_PER_SEC;
        int flag=0;
        for(i=0; i<dim_cpu.space_elem; i++) {
            int thread_error=0;
            if (fv_cpu[i].v != fv_cpu_GOLD[i].v || fv_cpu[i].x != fv_cpu_GOLD[i].x || fv_cpu[i].y != fv_cpu_GOLD[i].y || fv_cpu[i].z != fv_cpu_GOLD[i].z) {
                //if(fv_cpu_GOLD[i].v != fv_cpu[i].v) {
                if(flag==0){
                    buffer[0] = 0xDD000000;
                    flag=1;
                }else{
                    buffer[0] = 0xCC000000;
                }
                buffer[1]=*((unsigned int*)&i);
                unsigned long long aux=*((unsigned long long*)&fv_cpu[i].v);
                    buffer[2] = (unsigned int)((aux & 0xFFFFFFFF00000000LL) >> 32);                      
                    buffer[3] = (unsigned int)(aux & 0xFFFFFFFFLL);
                     	
                 m=*((unsigned long long*)&fv_cpu[i].x);
                    buffer[4] = (unsigned int)((aux & 0xFFFFFFFF00000000LL) >> 32);                      
                    buffer[5] = (unsigned int)(aux & 0xFFFFFFFFLL);
                      

                 m=*((unsigned long long*)&fv_cpu[i].y);
                    buffer[6] = (unsigned int)((aux & 0xFFFFFFFF00000000LL) >> 32);                      
                    buffer[7] = (unsigned int)(aux & 0xFFFFFFFFLL);
                       
                 m=*((unsigned long long*)&fv_cpu[i].z);
                    buffer[8] = (unsigned int)((aux & 0xFFFFFFFF00000000LL) >> 32);                      
                    buffer[9] = (unsigned int)(aux & 0xFFFFFFFFLL);
                      
                 send_message(10);  
            }
            
                
        }	
               clock_t end = clock();
        double time_spent2 = (double)(end - middle) / CLOCKS_PER_SEC;
       printf("time: %lf %lf ",time_spent1,time_spent2);
        
        


    


    free(rv_cpu);
    free(qv_cpu);
    free(fv_cpu);
    free(box_cpu);

    return 0;
}
