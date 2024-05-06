#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cycle = 1; 

void swapIntegers(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
//    swapIntegers(&num1, &num2);

//first instruction is instruction1
int NumberofInstructions = 7; 


int fetchInt; 
int fetchSave; 

int decodeInt; 
int executeInt; 
int memoryInt; 
int writeBackInt;

int main(){

    // while (1)
    // {
    //     if (0 != writeBackInt)
    //     {
    //         writeBackInt = 0; 
    //     }
    //     swapIntegers(&writeBackInt, &memoryInt); 
        
    //     // Odd cycle
    //     if (1 == cycle % 2)
    //     {
    //         if (fetchSave < NumberofInstructions)
    //         {
    //             fetchSave++; 
    //             fetchInt = fetchSave; 
    //         }
    //     }

    //     // Even cycle
    //     else{
    //         swapIntegers(&executeInt, &memoryInt); 
    //         swapIntegers(&decodeInt, &executeInt); 
    //         swapIntegers(&fetchInt, &decodeInt); 

            
    //     }
    //     printf("Cycle %d:  IF: %d, ID: %d, EX: %d, MEM: %d, WB:  %d \n",cycle, fetchInt, decodeInt, executeInt, memoryInt, writeBackInt); 
    //     cycle++; 


    //     //add logic for actual functions 
    //     // ID 7 then decode 7 for example


    //     //Stopping condition
    //     if (0 == fetchInt && 0 == decodeInt && 0 == executeInt && 0 == memoryInt && 0 != writeBackInt )
    //     {
    //         // Do the the last writeBack()
    //         break;
    //     }
        
        
        
    // }
    
    unsigned long long temp1 = 4294967295;
        unsigned long long temp2 = 1 ;

        // Perform the operation (change OP to the actual operation you want, e.g., +, -, *, etc.)
        unsigned long long result = temp1 + temp2;

        printf("%llu\n",result);

        // Check the 33rd bit (bit 32) using the MASK
        if ((result & 0xFFFFFFFF00000000) >  0xFFFFFFFF) {
            printf("Carry = 1\n");

        }

    return 0; 
}