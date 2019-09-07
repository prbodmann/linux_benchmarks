#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#ifndef MATRIX_SIZE
#define MATRIX_SIZE 256
#endif
float mA[MATRIX_SIZE][MATRIX_SIZE];
float mB[MATRIX_SIZE][MATRIX_SIZE];
float mCS0[MATRIX_SIZE][MATRIX_SIZE];
float float_golden[MATRIX_SIZE][MATRIX_SIZE];
int main(int argc, char** argv){
    time_t t;
    int i,j,k;
    FILE* fd,out;
    float a = 123456789.0;
    char name[200];
    srand((unsigned) time(&t));
    sprintf(name,"matmul_input_%d.bin",MATRIX_SIZE);
    fd =fopen(name,"wb");
    for (i=0;i<MATRIX_SIZE;i++){
        for (i=0;i<MATRIX_SIZE;i++){
            mA[i][j] = (float)(i * PI+4.966228736338716478); // generate nice longs/longs
            mB[i][j] = (float)(i / PI+2.726218736218716238);
        }
    }
    fwrite(mA,sizeof(float),MATRIX_SIZE*MATRIX_SIZE,fd);
    fwrite(mB,sizeof(float),MATRIX_SIZE*MATRIX_SIZE,fd);
    fclose(fd);
    for (i=0; i<MATRIX_SIZE; i++)
    {
        for(j=0; j<MATRIX_SIZE; j++)
        {
            mCS0[i][j] = 0.0;
            for (k=0; k<MATRIX_SIZE; k++)
                mCS0[i][j] += mA[i][k] * mB[k][j];

        }
    }
    sprintf(name,"matmul_gold_%d.bin",MATRIX_SIZE);
    fd =fopen(name,"wb");
    fwrite(mCS0,sizeof(float),MATRIX_SIZE*MATRIX_SIZE,fd);
    fclose(fd);


}
