#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>     // uint32_t
#include <inttypes.h>   // %"PRIu32"
#include <unistd.h>     // Sleep
#include <time.h>       // Time
#include <math.h>       // pow
#include <omp.h>        // OpenMP
#include <sched.h>      // sched_getcpu

#define MAX_ERROR       32              // Max. number of errors per repetition



//======================================================================

#define LOOP_MUL {\
        value_int = ref_int3; \
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        asm volatile("mul %0,$0x2, %0" : "+r" (value_int));\
        \

                }


//======================================================================
// Linear Feedback Shift Register using 32 bits and XNOR. Details at:
// http://www.xilinx.com/support/documentation/application_notes/xapp052.pdf
// http://www.ece.cmu.edu/~koopman/lfsr/index.html

void ref_word( uint32_t *ref_int3){
    static uint32_t counter = 0;

    counter++;
    if (counter == 1){
        *ref_int3  = 0x00CCCCCC;   // 0000 0000 1100 1100 | 1100 1100 1100 1100
        return;
    }
    else if (counter == 2){
        *ref_int3  = 0x06666666;   // 0000 0000 0110 0110 | 0110 0110 0110 0110
        return;
    }
    else if (counter == 3){
        *ref_int3  = 0x00333333;   // 0000 0000 0011 0011 | 0011 0011 0011 0011
        return;
    }
    else if (counter == 4){
        *ref_int3  = 0x00AAAAAA;   // 0000 0000 1010 1010 | 1010 1010 1010 1010
        return;
    }
    else if (counter == 5){
        
        *ref_int3  = 0x00555555;   // 0000 0000 0101 0101 | 0101 0101 0101 0101
        return;
    }
    else if (counter == 6) {

        *ref_int3  = 0x00999999;   // 0000 0000 1001 1001 | 1001 1001 1001 1001
        return;
    }
    else if (counter == 7){

        *ref_int3  = 0x00888888;   // 0000 0000 1000 1000 | 1000 1000 1000 1000
        return;
    }
    else if (counter == 8){
        *ref_int3  = 0x00444444;   // 0000 0000 0100 0100 | 0100 0100 0100 0100
        return;
    }
    else if (counter == 9){
        *ref_int3  = 0x00222222;   // 0000 0000 0010 0010 | 0010 0010 0010 0010
        return;
    }
    else {
        *ref_int3  = 0x00111111;  // 0000 0000 0001 0001 | 0001 0001 0001 0001
        counter = 0;
        return;
    }
}

// =============================================================================


//======================================================================
int main (int argc, char *argv[]) {

  
    

    //==================================================================
    // Benchmark variables
    uint64_t i = 0;
    uint64_t j = 0;
    uint32_t errors = 0;

    uint32_t x;
    uint32_t y;


    //==================================================================
    // Benchmark
    while(1) {

        errors = 0;

        //==============================================================
        // Initialize the variables with a new REFWORD
        uint32_t  ref_int3;
        ref_word(&ref_int3);

        //======================================================================P
        // Parallel region
         
                asm volatile ("nop");
                asm volatile ("nop");

                uint32_t value_int = 0;


                //==============================================================
                // ADD
                    for(j = (repetitions == 0); j < BUSY; j++) {
                        LOOP_MUL
                        LOOP_MUL
                        LOOP_MUL
                        LOOP_MUL
                        LOOP_MUL
                        LOOP_MUL
                        LOOP_MUL
                        LOOP_MUL
                    }


            
            asm volatile ("nop");
            asm volatile ("nop");
   

    }

    exit(EXIT_SUCCESS);
}