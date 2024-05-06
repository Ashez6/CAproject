#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// const int R0=0;    // zero register 
int* memoryfile=NULL;
int registerFile [32] ; //31 gprs
int NumberofInstructions; // number of instructions in memory
int pc = 0;
int zeroFlag=0;

int cycle = 1;
int* tmparr=NULL;
int* tmparr1=NULL;

int fetchInt; 
int fetchSave; 
int decodeInt; 
int executeInt; 
int memoryInt; 
int writeBackInt;
int branchflag;

int* fetch() {
    int* instruction=malloc(7*sizeof(int));
    unsigned int ins=memoryfile[pc];
    *instruction=ins;
    pc++;
    return instruction;
}

int* decode(int* instruction) {
        int* arr=malloc(7*sizeof(int));
        for(int i=0;i<7;i++){
            arr[i]=tmparr[i];
        }
        if(cycle%2==0){
            // printf("Instruction %i\n",pc);
            unsigned int opcode=*instruction & 0b11110000000000000000000000000000;
            opcode=opcode>>28;
            // printf("opcode = %i\n",opcode);
            arr[0]=opcode;
            int rs = *instruction & 0b00001111100000000000000000000000;
            rs = rs>>23;
            // printf("rs = %i\n",rs);
            arr[1]=rs;
            int rt = *instruction & 0b00000000011111000000000000000000;
            rt=rt>>18;
            // printf("rt = %i\n",rt);
            arr[2]=rt;
        }
        else{
            int rd = *instruction & 0b00000000000000111110000000000000;
            rd=rd>>13;
            // printf("rd = %i\n",rd);
            arr[3]=rd;
            int shamt = *instruction & 0b00000000000000000001111111111111;
            // printf("shift amount = %i\n",shamt);
            arr[4]=shamt;
            int imm= *instruction & 0b00000000000000111111111111111111;
            // printf("immediate = %i\n",imm);
            if(imm>=131072){
                imm=imm | 0b11111111111111000000000000000000;
            }
            arr[5]=imm;
            int address = *instruction & 0b00001111111111111111111111111111;
            // printf("address = %i\n",address);
            arr[6]=address;
            // printf("---------- \n");
            // Printings
        
            // printf("Instruction %i\n",pc);
            // printf("opcode = %i\n",opcode);
            // printf("rs = %i\n",rs);
            // printf("rt = %i\n",rt);
            // printf("rd = %i\n",rd);
            // printf("shift amount = %i\n",shamt);
            // printf("immediate = %i\n",imm);
            // printf("address = %i\n",address);
            // printf("---------- \n");

            
            // arr[0]=opcode;
            // arr[1]=rs;
            // arr[2]=rt;
            // arr[3]=rd;
            // arr[4]=shamt;
            // arr[5]=imm;
            // arr[6]=address;
        }
        return arr;
        
        
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

    // printf("Operation = %d\n", operation);
    // printf("First Operand = %d\n", operandA);
    // printf("Second Operand = %d\n", operandB);
    // printf("Result = %d\n", output);
    // printf("Zero Flag = %d\n", zeroFlag);

    return output;
}

int* execute(int* arr){

    int writeflag=0;
    int loadflag=0;
    int storeflag=0;
    int tmpresult=0;


    unsigned int opcode = arr[0];  
    int rs = arr[1];      
    int rt = arr[2];      
    int rd = arr[3];      
    int shamt = arr[4];   
    int imm = arr[5];
    int address = arr[6];
    int* flags=malloc(7*sizeof(int));
        for(int i=0;i<7;i++){
            flags[i]=tmparr1[i];
        }

    if(cycle%2==0){
        if(opcode==0){
            tmpresult= ALU(registerFile[rs],registerFile[rt],0);
        }
        else if(opcode==1){
            tmpresult= ALU(registerFile[rs],registerFile[rt],1);
        }
        else if(opcode==2){
            tmpresult= ALU(registerFile[rs],imm,2);
        }
        else if(opcode==3){
            tmpresult= ALU(registerFile[rs],imm,0);
        }
        else if(opcode==4){
            ALU(registerFile[rs],registerFile[rt],1);
        }
        else if(opcode==5){
            tmpresult= ALU(registerFile[rs],imm,3);
        }
        else if(opcode==6){
            tmpresult= ALU(registerFile[rs],imm,4);
        }
        else if(opcode==7){
            address=address & 0b00001111111111111111111111111111;
            int tmp=pc & 0b11110000000000000000000000000000;
            pc = tmp | address;
        }
        else if(opcode==8){
            tmpresult= ALU(registerFile[rs],shamt,5);
        }
        else if(opcode==9){
            tmpresult= ALU(registerFile[rs],shamt,6);
        }
        else if(opcode==10){
            tmpresult= ALU(registerFile[rs],imm,0);
        }
        else{
            tmpresult= ALU(registerFile[rs],imm,0);
        }
        flags[0]=tmpresult;
        return flags;
    }
    else{
        branchflag=0;
        if(opcode==0){
            writeflag=1;
            flags[4]=rd;
        }
        else if(opcode==1){
            writeflag=1;
            flags[4]=rd;
        }
        else if(opcode==2){
            writeflag=1;
            flags[4]=rt;
        }
        else if(opcode==3){
            writeflag=1;
            flags[4]=rt;
        }
        else if(opcode==4){
            if(zeroFlag!=1){
                pc=pc+imm-2;//-2 since 2 fetches have occured incrementing the pc
                branchflag=1;
            }
            
        }
        else if(opcode==5){
            writeflag=1;
            flags[4]=rt;
        }
        else if(opcode==6){
            writeflag=1;
            flags[4]=rt;
        }
        else if(opcode==8){
            writeflag=1;
            flags[4]=rt;
        }
        else if(opcode==9){
            writeflag=1;
            flags[4]=rt;
        }
        else if(opcode==10){
            loadflag=1;
            writeflag=1;
            flags[4]=rt;
        }
        else if(opcode==11){
            storeflag=1;
            flags[4]=rt;
        }
        flags[1]=writeflag;
        flags[2]=loadflag;
        flags[3]=storeflag;
        return flags;
    }
    
    

}

int* memory(int* arr){
    if(arr[3]){
        printf("Updating memory location %i value to %i\n",arr[0],registerFile[arr[4]]);
        memoryfile[arr[0]]=registerFile[arr[4]];
    }
    if(arr[2]){
        arr[0]=memoryfile[arr[0]];
    }
    if(branchflag){
        decodeInt=0;
        executeInt=0;
        memoryInt=0;
    }
    return arr;
}

void writeback(int* arr){
    if(arr[1]){  
        if(arr[4]!=0){
            printf("Updating R%i value to %i\n",arr[4],arr[0]);
            registerFile[arr[4]]=arr[0];
        }
    }
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
        rt1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        c++;
        rs1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        imm1= atoi(c);
        printf("%i\n",imm1);
        imm1= imm1 & 0b00000000000000111111111111111111;
        printf("%i\n",imm1);
        rs1=rs1<<23;
        rt1=rt1<<18;
        b=b | rs1 | imm1 | rt1;
    }
    else if(insnum==8 || insnum==9 ){
        token=strtok(NULL," ");
        char* c=token;
        c++;
        rt1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        c++;
        rs1= atoi(c); 
        token=strtok(NULL," ");
        c=token;
        shamt1= atoi(c);
        rs1=rs1<<23;
        rt1=rt1<<18;
        b=b | rs1 | shamt1 | rt1;
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
    memoryfile=(int*)malloc(2048*sizeof(int));
    char line[1000];
    txtfile=fopen(fileName,"r");
    if(txtfile==NULL){
        printf("Error opening file \n");
        return 1;
    }
    while(fgets(line,sizeof(line),txtfile)!=NULL){
        memoryfile[NumberofInstructions]= LineToBinary(line);
        NumberofInstructions++;
    }
    
    fclose(txtfile);
    return 0;

}

void swapIntegers(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void printarray(int* arr){
    printf("[");
    for(int i=0;i<7;i++){
        printf("%i,",arr[i]);
    }
    printf("]\n");
}

int main(){
    if(loadInstToMemory("mips.txt")){
        return 1;
    }
    tmparr=(int*)malloc(7*sizeof(int));
    tmparr1=(int*)malloc(7*sizeof(int));

    int* firstInstructionData = malloc(7*sizeof(int));
    int* secondInstructionData = malloc(7*sizeof(int));
    int* thirdInstructionData = malloc(7*sizeof(int));
    int* fourthInstructionData = malloc(7*sizeof(int));

    while (1)
    {
        if (0 != writeBackInt)
        {
            writeBackInt = 0; 
        }
        swapIntegers(&writeBackInt, &memoryInt); 
        
        // Odd cycle
        if (1 == cycle % 2)
        {
            if (fetchSave < NumberofInstructions)
            {
                fetchSave=pc+1; 
                fetchInt = fetchSave; 
            }
        }
        // Even cycle
        else{
            swapIntegers(&executeInt, &memoryInt); 
            swapIntegers(&decodeInt, &executeInt); 
            swapIntegers(&fetchInt, &decodeInt); 

            
        }
        printf("Cycle %d:  IF: %d, ID: %d, EX: %d, MEM: %d, WB:  %d \n",cycle, fetchInt, decodeInt, executeInt, memoryInt, writeBackInt); 

        if(fetchInt%4==1){
            firstInstructionData=fetch();
        }
        else if(fetchInt%4==2){
            secondInstructionData=fetch();
        }
        else if(fetchInt%4==3){
            thirdInstructionData=fetch();
        }
        else if(fetchInt%4==0 && fetchInt!=0){
            fourthInstructionData=fetch();
        }


        if(cycle%2==0){
            if(decodeInt%4==1){
                tmparr=decode(firstInstructionData);
            }
            else if(decodeInt%4==2){
                tmparr=decode(secondInstructionData);
            }
            else if(decodeInt%4==3){
                tmparr=decode(thirdInstructionData);
            }
            else if(decodeInt%4==0 && decodeInt!=0){
                tmparr=decode(fourthInstructionData);
            }
        }
        else{
            if(decodeInt%4==1){
                firstInstructionData=decode(firstInstructionData);
            }
            else if(decodeInt%4==2){
                secondInstructionData=decode(secondInstructionData);
            }
            else if(decodeInt%4==3){
                thirdInstructionData=decode(thirdInstructionData);
            }
            else if(decodeInt%4==0 && decodeInt!=0){
                fourthInstructionData=decode(fourthInstructionData);
            }
        }
        

        if(cycle%2==0){
            if(executeInt%4==1){
                tmparr1=execute(firstInstructionData);
            }
            else if(executeInt%4==2){
                tmparr1=execute(secondInstructionData);
            }
            else if(executeInt%4==3){
                tmparr1=execute(thirdInstructionData);
            }
            else if(executeInt%4==0 && executeInt!=0){
                tmparr1=execute(fourthInstructionData);
            }
            
        }
        else{
            if(executeInt%4==1){
                firstInstructionData=execute(firstInstructionData);
            }
            else if(executeInt%4==2){
                secondInstructionData=execute(secondInstructionData);
            }
            else if(executeInt%4==3){
                thirdInstructionData=execute(thirdInstructionData);
            }
            else if(executeInt%4==0 && executeInt!=0){
                fourthInstructionData=execute(fourthInstructionData);
            }
        }

        if(memoryInt%4==1){
            firstInstructionData=memory(firstInstructionData);
        }
        else if(memoryInt%4==2){
            secondInstructionData=memory(secondInstructionData);
        }
        else if(memoryInt%4==3){
            thirdInstructionData=memory(thirdInstructionData);
        }
        else if(memoryInt%4==0 && memoryInt!=0){
            fourthInstructionData=memory(fourthInstructionData);
        }

        if(writeBackInt%4==1){
            writeback(firstInstructionData);
        }
        else if(writeBackInt%4==2){
            writeback(secondInstructionData);
        }
        else if(writeBackInt%4==3){
            writeback(thirdInstructionData);
        }
        else if(writeBackInt%4==0 && writeBackInt!=0){
            writeback(fourthInstructionData);
        }

        
        cycle++; 
        //Stopping condition
        if (0 == fetchInt && 0 == decodeInt && 0 == executeInt && 0 == memoryInt && !branchflag)
        {
            break;
        }
    }

    printf("Register File:\n");
    for(int i=0;i<32;i++){
            printf("R%i:%i, ",i,registerFile[i]);
    }
    printf("\n");

    // printf("Main Memory:\n");
    // for(int i=0;i<2048;i++){
    //         printf("%i:%i, ",i,memoryfile[i]);
    // }
    // printf("\n");
    return 0;
}

