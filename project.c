#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// const int R0=0;    // zero register 
int* memory;
int registerFile [32] ; //31 gprs
int NumberofInstructions; // number of instructions in memory
int pc = 0;
int instruction;   //instruction fetched from memory
int zeroFlag=0;

unsigned int opcode = 0;  
int rs = 0;      
int rt = 0;      
int rd = 0;      
int shamt = 0;   
int imm = 0;
int address = 0;

void decode() {
        
        int valueRS = 0;
        int valueRT = 0;
        
        opcode=instruction & 0b11110000000000000000000000000000;
        opcode=opcode>>28;
        rs = instruction & 0b00001111100000000000000000000000;
        rs = rs>>23;
        rt = instruction & 0b00000000011111000000000000000000;
        rt=rt>>18;
        rd = instruction & 0b00000000000000111110000000000000;
        rd=rd>>13;
        shamt = instruction & 0b00000000000000000001111111111111;
        imm= instruction & 0b00000000000000111111111111111111;
        address = instruction & 0b00001111111111111111111111111111;
        
        // Printings
        
        printf("Instruction %i\n",pc);
		printf("opcode = %i\n",opcode);
		printf("rs = %i\n",rs);
		printf("rt = %i\n",rt);
		printf("rd = %i\n",rd);
		printf("shift amount = %i\n",shamt);
        printf("immediate = %i\n",imm);
		printf("address = %i\n",address);
		printf("---------- \n");
        
}

void fetch() {
    instruction=memory[pc];
    pc++;
}

void execute(){
    if(opcode==0){

    }
    else if(opcode==1){

    }
    else if(opcode==2){
        
    }
    else if(opcode==3){
        
    }
    else if(opcode==4){
        
    }
    else if(opcode==5){
        
    }
    else if(opcode==6){
        
    }
    else if(opcode==7){
        
    }
    else if(opcode==8){
        
    }
    else if(opcode==9){
        
    }
    else if(opcode==10){
        
    }
    else{
        
    }
}







int ALU(int operandA, int operandB, int operation) {
    int output = 0;
    zeroFlag = 0;

    if(operation==0){
        output= operandA + operandB;
    }
    else if (operation==1) {
        output=operandA - operandB;
    }
    else if (operation==2) {
        output=operandA * operandB;
    }
    else if (operation==3) {
        output=operandA & operandB;
    }
    else if (operation==4)  {
        output=operandA | operandB;
    } 
    else if (operation==5)  {
        output= operandA<<operandB; //A is reg B is shamt
    } 
    else {
        output=operandA >> operandB; //A is reg B is shamt
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

int LineToBinary(char* line){
    char* token=strtok(line," ");
    int b=0;
    int insnum=0;
    if(strcmp(token,"ADD")==0 ){
        insnum=0;
        b=0b00000000000000000000000000000000;
    }else if(strcmp(token,"SUB")==0){
        insnum=1;
        b=0b00010000000000000000000000000000;
    }else if(strcmp(token,"MULI")==0){
        insnum=2;
        b=0b00100000000000000000000000000000;
    }
    else if(strcmp(token,"ADDI")==0){
        insnum=3;
        b=0b00110000000000000000000000000000;
    }
    else if(strcmp(token,"BNE")==0){
        insnum=4;
        b=0b01000000000000000000000000000000;
    }
    else if(strcmp(token,"ANDI")==0){
        insnum=5;
        b=0b01010000000000000000000000000000;
    }
    else if(strcmp(token,"ORI")==0){
        insnum=6;
        b=0b01100000000000000000000000000000;
    }
    else if(strcmp(token,"J")==0){
        insnum=7;
        b=0b01110000000000000000000000000000;
    }
    else if(strcmp(token,"SLL")==0){
        insnum=8;
        b=0b10000000000000000000000000000000;
    }
    else if(strcmp(token,"SRL")==0){
        insnum=9;
        b=0b10010000000000000000000000000000;
    }
    else if(strcmp(token,"LW")==0){
        insnum=10;
        b=0b10100000000000000000000000000000;
    }
    else if(strcmp(token,"SW")==0){
        insnum=11;
        b=0b10110000000000000000000000000000;
    }
    int rs1=0;
    int rt1=0;
    int rd1=0;
    int imm1=0;
    int shamt1=0;
    int address1=0;
    if(insnum==0 || insnum==1){
        token=strtok(NULL," ");
        char* c=token;
        c++;
        rd1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        c++;
        rs1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        c++;
        rt1= atoi(c);
        rs1=rs1<<23;
        rt1=rt1<<18;
        rd1=rd1<<13;
        b=b | rs1 | rt1 | rd1;
    }
    else if(insnum==2 || insnum==3 || insnum==4 || insnum==5 || insnum==6 || insnum==10 || insnum==11){
        token=strtok(NULL," ");
        char* c=token;
        c++;
        rd1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        c++;
        rs1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        imm1= atoi(c);
        rs1=rs1<<23;
        rd1=rd1<<18;
        b=b | rs1 | imm1 | rd1;
    }
    else if(insnum==8 || insnum==9 ){
        token=strtok(NULL," ");
        char* c=token;
        c++;
        rd1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        c++;
        rs1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        shamt1= atoi(c);
        rs1=rs1<<23;
        rd1=rd1<<18;
        b=b | rs1 | shamt1 | rd1;
    }
    else{
        token=strtok(NULL," ");
        char* c=token;
        b= b | address1;
        address1= atoi(c); 
    }
    return b;
    
}

int loadInstToMemory(char* fileName){
    FILE * txtfile;
    memory=(int*)malloc(2048*sizeof(int));
    char line[1000];
    txtfile=fopen(fileName,"r");
    if(txtfile==NULL){
        printf("Error opening file \n");
        return 1;
    }
    while(fgets(line,sizeof(line),txtfile)!=NULL){
        memory[NumberofInstructions]= LineToBinary(line);
        NumberofInstructions++;
    }
    
    fclose(txtfile);
    return 0;

}

int main(){
    if(loadInstToMemory("mips.txt")){
        return 1;
    }

    for(int i=0;i<NumberofInstructions;i++){
        fetch();
        decode();
        // execute();
        // memory();
        // writeback();
        //cycle++
    }
}

