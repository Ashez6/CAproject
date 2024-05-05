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


int fetch; 
int fetchSave; 

int decode; 
int execute; 
int memory; 
int writeBack;

int main(){

    while (1)
    {
        if (0 != writeBack)
        {
            writeBack = 0; 
            swapIntegers(&writeBack, &memory); 
        }
        else{
            swapIntegers(&writeBack, &memory); 

        }
        
        // Odd cycle
        if (1 == cycle % 2)
        {
            if (fetch < NumberofInstructions && fetchSave < NumberofInstructions)
            {
                fetchSave++; 
                fetch = fetchSave; 
            }
        }

        // Even cycle
        else{
            swapIntegers(&execute, &memory); 
            swapIntegers(&decode, &execute); 
            swapIntegers(&fetch, &decode); 

            
        }
        printf("Cycle %d:  IF: %d, ID: %d, EX:  %d, MEM: %d, WB:  %d \n",cycle, fetch, decode, execute, memory, writeBack); 
        cycle++; 

        //Stopping condition
        if (0 == fetch && 0 == decode && 0 == execute && 0 == memory && 0 != writeBack )
        {
            break;
        }
        
        
        
    }
    


    return 0; 
}