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
    
    printf("%i\n",atoi("3"));
    printf("%i\n",atoi("-5"));

    return 0; 
}