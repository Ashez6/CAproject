// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// // const int R0=0;    // zero register 
// int* memory1;
// int registerFile [32] ; //31 gprs
// int NumberofInstructions; // number of instructions in memory
// int pc = 0;
// int instruction;   //instruction fetched from memory
// int zeroFlag=0;



// // Enumerated types for state codes and return codes
// enum state_codes {
//     IF,
//     ID,
//     EX,
//     MEM,
//     WB,
// };

// enum ret_codes {
//     OK,
//     OK2,
//     END
// };

// // Struct for representing transitions
// struct transition {
//     enum state_codes src_state;
//     enum ret_codes ret_code;
//     enum state_codes dst_state;
// };

// // Array of state function pointers
// typedef void (*state_fn)(void);

// // State functions

// void fetch() {
//     instruction= memory1[pc];
//     pc++;
// }

// void decode() {
        
//         unsigned int opcode = 0;  
//         int rs = 0;      
//         int rt = 0;      
//         int rd = 0;      
//         int shamt = 0;   
//         int imm = 0;     
//         int address = 0; 
        
//         int valueRS = 0;
//         int valueRT = 0;
        
//         opcode=instruction & 0b11110000000000000000000000000000;
//         opcode=opcode>>28;
//         rs = instruction & 0b00001111100000000000000000000000;
//         rs = rs>>23;
//         rt = instruction & 0b00000000011111000000000000000000;
//         rt=rt>>18;
//         rd = instruction & 0b00000000000000111110000000000000;
//         rd=rd>>13;
//         shamt = instruction & 0b00000000000000000001111111111111;
//         imm= instruction & 0b00000000000000111111111111111111;
//         address = instruction & 0b00001111111111111111111111111111;

//         valueRS= registerFile[rs];
//         valueRT= registerFile[rt];

        
//         // Printings
        
//         printf("Instruction %i\n",pc);
// 		printf("opcode = %i\n",opcode);
// 		printf("rs = %i\n",rs);
// 		printf("rt = %i\n",rt);
// 		printf("rd = %i\n",rd);
// 		printf("shift amount = %i\n",shamt);
//         printf("immediate = %i\n",imm);
// 		printf("address = %i\n",address);
// 		printf("value[rs] = %i\n",valueRS);
// 		printf("value[rt] = %i\n",valueRT);
// 		printf("---------- \n");
                  
// }
// void execute(void) { /* ... */ }
// void memory(void) { /* ... */ }
// void writeBack(void) { /* ... */ }


// // Array of state function pointers
// state_fn state[] = {
//     fetch,
//     decode,
//     execute,
//     memory, 
//     writeBack
// };

// // Transition rules
// struct transition state_transitions[] = {
//      {IF, OK, ID},
//      {ID, OK, ID},
//      {ID, OK2, EX},
//      {EX, OK, EX},
//      {EX, OK2, MEM},
//      {MEM, OK, WB}
// };

// // Function to look up the next state based on the current state and return code
// enum state_codes lookup_transitions(enum state_codes cur_state, enum ret_codes rc) {
//     for (int i = 0; i < sizeof(state_transitions) / sizeof(state_transitions[0]); ++i) {
//         if (state_transitions[i].src_state == cur_state && state_transitions[i].ret_code == rc) {
//             return state_transitions[i].dst_state;
//         }
//     }
//     return END; // Default to END state
// }

// int main(void) {
//     enum state_codes cur_state = fetch;
//     enum ret_codes rc;
//     state_fn state_fun;

//     while (1) {
//         state_fun = state[cur_state];
//         state_fun();

//         if (cur_state == IF) {
//             fetch(); 
//             break;
//         }
//          if (cur_state == ID) {
//             decode(); 
//             break;
//         }
//          if (cur_state == EX) {
//             execute(); 
//             break;
//         }
//          if (cur_state == MEM) {
//             memory(); 
//             break;
//         }
//          if (cur_state == WB) {
//             writeBack(); 
//             break;
//         }


//         rc = OK;

//         cur_state = lookup_transitions(cur_state, rc);
//     }

//     return 0;
// }


#include <stdio.h>


void simulatePipeline(int instructions[][5], int numInstructions) {
    for (int cycle = 0; cycle < 7 + ((numInstructions - 1) * 2); cycle++) {
        for (int i = 0; i < numInstructions; i++) {
            int type = instructions[i][0];
            int operand1 = instructions[i][1];
            int operand2 = instructions[i][2];

            switch (cycle % 5) {
                case 0: // IF stage
                    printf("Cycle %d: Fetch instruction %d\n", cycle, i);
                    break;
                case 1: // ID stage
                    printf("Cycle %d: Decode instruction %d (Type: %d, Op1: %d, Op2: %d)\n",
                           cycle, i, type, operand1, operand2);
                    break;
                case 2: // EX stage
                    printf("Cycle %d: Execute instruction %d\n", cycle, i);
                    break;
                case 3: // MEM stage
                    printf("Cycle %d: Memory access for instruction %d\n", cycle, i);
                    break;
                case 4: // WB stage
                    printf("Cycle %d: Write back result for instruction %d\n", cycle, i);
                    break;
               
               //could create additional cycles
            }
        }
    }
}

int main() {
    int numInstructions = 6;

    //simulatePipeline(instructions, numInstructions);

    return 0;
}

