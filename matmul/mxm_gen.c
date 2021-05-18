#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.1415926535897932384626433

float *mA;
float *mB;
float *mCS0;
int MATRIX_SIZE;
int main(int argc, char** argv){
    time_t t;
    int i,j,k;
    FILE* fd,out;
    float a = 123456789.0;
    char name[200];
    MATRIX_SIZE=atoi(argv[3]);
    mA=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);   
    mB=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);
    mCS0=(float*)malloc(sizeof(float)*MATRIX_SIZE*MATRIX_SIZE);

    sprintf(name,"matmul_input_%d.bin",MATRIX_SIZE);
    fd =fopen(name,"wb");
    for (i=0;i<MATRIX_SIZE;i++){
        for (j=0;i<MATRIX_SIZE;j++){
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
