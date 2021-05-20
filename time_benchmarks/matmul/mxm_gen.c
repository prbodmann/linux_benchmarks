#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.1415926535897932384626433

float *mA;
float *mB;
float *mCS0;
int MATRIX_SIZE;
char name[2000];
int main(int argc, char** argv){

    int i,j,k;
    FILE* fd,out;

    MATRIX_SIZE=atoi(argv[1]);
    mA=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);   
    if(mA==NULL){
        printf("error at mA\n");
        exit(-1);
    }
    mB=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);
     if(mB==NULL){
        printf("error at mB\n");
        exit(-1);
    }
    mCS0=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);
     if(mCS0==NULL){
        printf("error at mCS0\n");
        exit(-1);
    }
    sprintf(name,"matmul_input_%d.bin",MATRIX_SIZE);
    fd =fopen(name,"wb");
    if(fd==NULL){
        printf("error at fopen 1\n");
        exit(-1);
    }
    printf("lol\n");
    for (i=0;i<MATRIX_SIZE;i++){
        for (j=0;j<MATRIX_SIZE;j++){
            //printf("lol %d %d", i ,j);
            mA[i*MATRIX_SIZE+j] = (float)(i * PI+4.966228736338716478); // generate nice longs/longs
            mB[i*MATRIX_SIZE+j] = (float)(i / PI+2.726218736218716238);
        }
    }
    fwrite(mA,sizeof(float),MATRIX_SIZE*MATRIX_SIZE,fd);
    fwrite(mB,sizeof(float),MATRIX_SIZE*MATRIX_SIZE,fd);
    fclose(fd);
    for (i=0; i<MATRIX_SIZE; i++)
        {
            for(j=0; j<MATRIX_SIZE; j++)
            {
                mCS0[i*MATRIX_SIZE+j] = 0.0;
                for (k=0; k<MATRIX_SIZE; k++)
                    mCS0[i*MATRIX_SIZE+j] += mA[i*MATRIX_SIZE+k] * mB[k*MATRIX_SIZE+j];
            }
        }
    sprintf(name,"matmul_gold_%d.bin",MATRIX_SIZE);
    fd =fopen(name,"wb");
    fwrite(mCS0,sizeof(float),MATRIX_SIZE*MATRIX_SIZE,fd);
    fclose(fd);


}
