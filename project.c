static int R0=0;    // zero register 
int* memory;
int registerFile [31] ; //31 gprs
int NumberofInstructions; // number of instructions in memory
int pc = 0;
int instruction;   //instruction fetched from memory


void decode(long instruction) {
        
        int opcode = 0;  // bits31:26
        int rs = 0;      // bits25:21
        int rt = 0;      // bit20:16
        int rd = 0;      // bits15:11
        int shamt = 0;   // bits10:6
        int funct = 0;   // bits5:0
        int imm = 0;     // bits15:0
        int address = 0; // bits25:0
        
        int valueRS = 0;
        int valueRT = 0;
        
        opcode=instruction & 0b11111100000000000000000000000000;
        opcode=opcode>>26;
        rs = instruction & 0b00000011111000000000000000000000;
        rs = rs>>21;
        rt = instruction & 0b00000000000111110000000000000000;
        rt=rt>>16;
        rd = instruction & 0b00000000000000001111100000000000;
        rd=rd>>11;
        shamt = instruction & 0b00000000000000000000011111000000;
        shamt=shamt>>6;
        funct = instruction & 0b00000000000000000000000000111111;
        imm= instruction & 0b00000000000000001111111111111111;
        address = instruction & 0b00000011111111111111111111111111;

        valueRS= registerFile[rs];
        valueRT= registerFile[rt];

        
        // Printings
        
        printf("Instruction %i\n",pc);
		printf("opcode = %i\n",opcode);
		printf("rs = %i\n",rs);
		printf("rt = %i\n",rt);
		printf("rd = %i\n",rd);
		printf("shift amount = %i\n",shamt);
		printf("function = %i\n",funct);
		printf("immediate = %i\n",imm);
		printf("address = %i\n",address);
		printf("value[rs] = %i\n",valueRS);
		printf("value[rt] = %i\n",valueRT);
		printf("---------- \n");
                  
}

void fetch() {
        
        for(int i=0;i<NumberofInstructions;i++){
         //   instruction=instructionMemory[pc];
            pc++;
        }
             
}






int zeroFlag=0;
int ALU(int operandA, int operandB, int operation) {
    int output = 0;
    zeroFlag = 0;

   
if(operation==0){
output= operandA & operandB;

}
else if (operation==1) {
output=operandA | operandB;


}
else if (operation==2) {
output=operandA + operandB;

}
else if (operation==3) {
output=operandA - operandB;

}
else if (operation==4)  {
    if(operandA<operandB)
    output=1;
} 
else{
output= ~(operandA | operandB);

}

if (output==0)
   zeroFlag=1;

    printf("Operation = %d\n", operation);
    printf("First Operand = %d\n", operandA);
    printf("Second Operand = %d\n", operandB);
    printf("Result = %d\n", output);
    printf("Zero Flag = %d\n", zeroFlag);

    return output;
}

