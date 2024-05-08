#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// const int R0=0;    // zero register
int *memoryfile = NULL;
int registerFile[32];     // 31 gprs
int NumberofInstructions; // number of instructions in memory
int pc = 0;
int zeroFlag = 0;

int cycle = 1;

int* temparr1 = NULL;
int* temparr2 = NULL;
int* IFID = NULL; 
int* IDEX = NULL; 
int* EXMEM = NULL; 
int* MEMWB = NULL; 

int fetchInt;
int fetchSave;
int decodeInt;
int executeInt;
int memoryInt;
int writeBackInt;

int overflowFLag;
int carryFlag;

void fetch()
{
    unsigned int ins = memoryfile[pc];
    pc++;
    printf("Fetch output : %i\n", ins);
    *IFID = pc;
    *(IFID+1)=ins; 
}

void decode()
{

    printf("Decode input : %i\n", *(IFID+1));
    if (cycle % 2 == 0)
    {
        *temparr1=*IFID; //forward the pc of te instruction
        printf("Decode output : Instruction=%i ", pc);
        unsigned int opcode = *(IFID+1) & 0b11110000000000000000000000000000;
        opcode = opcode >> 28;
        printf("opcode=%i ", opcode);
        *(temparr1+1) = opcode;
        int rs = *(IFID+1) & 0b00001111100000000000000000000000;
        rs = rs >> 23;
        printf("rs=%i ", rs);
        *(temparr1+2) = rs;
        int rt = *(IFID+1) & 0b00000000011111000000000000000000;
        rt = rt >> 18;
        printf("rt=%i \n", rt);
        *(temparr1+3) = rt;
    }
    else
    {
        *(IDEX)=*(temparr1);
        *(IDEX+1)=*(temparr1+1);
        *(IDEX+2)=*(temparr1+2);
        *(IDEX+3)=*(temparr1+3);
        int rd = *(IFID+1) & 0b00000000000000111110000000000000;
        rd = rd >> 13;
        printf("Decode output : rd=%i ", rd);
        *(IDEX+4) = rd;
        int shamt = *(IFID+1) & 0b00000000000000000001111111111111;
        printf("shamt=%i ", shamt);
        *(IDEX+5) = shamt;
        int imm = *(IFID+1) & 0b00000000000000111111111111111111;
        printf("immediate=%i ", imm);
        if (imm >= 131072)
        {
            imm = imm | 0b11111111111111000000000000000000;
        }
        *(IDEX+6) = imm;
        int address = *(IFID+1) & 0b00001111111111111111111111111111;
        printf("address=%i\n", address);
        *(IDEX+7) = address;
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

}

int ALU(int operandA, int operandB, int operation)
{
    int output = 0;
    zeroFlag = 0;
    overflowFLag = 0;
    carryFlag = 0;

    if (operation == 0)
    {
        output = operandA + operandB;

        // Extract sign bits
        int signA = operandA >> 31; // Right shift to get the sign bit
        int signB = operandB >> 31;
        int signResult = output >> 31;

        // Check for overflow
        if (signA ^ signB ^ signResult)
        {
            printf("Overflow detected!\n");
            overflowFLag = 1;
        }

        unsigned long temp1 = operandA;
        unsigned long temp2 = operandB;

        // Perform the operation (change OP to the actual operation you want, e.g., +, -, *, etc.)
        unsigned long result = temp1 + temp2;

        // Check the 33rd bit (bit 32) using the MASK
        if ((result & 0xFFFFFFFF00000000) > 0xFFFFFFFF)
        {
            printf("Carry = 1\n");
        }
    }
    else if (operation == 1)
    {
        output = operandA - operandB;
    }
    else if (operation == 2)
    {
        output = operandA * operandB;

        // Extract sign bits
        int signA = operandA >> 31; // Right shift to get the sign bit
        int signB = operandB >> 31;
        int signResult = output >> 31;

        // Check for overflow
        if (signA ^ signB ^ signResult)
        {
            printf("Overflow detected!\n");
            overflowFLag = 1;
        }

        unsigned long temp1 = operandA;
        unsigned long temp2 = operandB;

        // Perform the operation (change OP to the actual operation you want, e.g., +, -, *, etc.)
        unsigned long result = temp1 * temp2;

        // Check the 33rd bit (bit 32) using the MASK
        if ((result & 0xFFFFFFFF00000000) > 0xFFFFFFFF)
        {
            printf("Carry = 1\n");
        }
    }
    else if (operation == 3)
    {
        output = operandA & operandB;
    }
    else if (operation == 4)
    {
        output = operandA | operandB;
    }
    else if (operation == 5)
    {
        output = operandA << operandB; // A is reg B is shamt
    }
    else
    {
        output = operandA >> operandB; // A is reg B is shamt
    }

    if (output == 0)
        zeroFlag = 1;

    // printf("Operation = %d\n", operation);
    // printf("First Operand = %d\n", operandA);
    // printf("Second Operand = %d\n", operandB);
    // printf("Result = %d\n", output);
    // printf("Zero Flag = %d\n", zeroFlag);

    return output;
}

void execute()
{

    int writeflag = 0;
    int loadflag = 0;
    int storeflag = 0;
    int tmpresult = 0;

    unsigned int opcode = *(IDEX+1);
    int rs = *(IDEX+2);
    int rt = *(IDEX+3);
    int rd = *(IDEX+4);
    int shamt = *(IDEX+5);
    int imm = *(IDEX+6);
    int address = *(IDEX+7);
    printf("Execute input : opcode=%i rs=%i rt=%i rd=%i shamt=%i imm=%i address=%i\n", opcode, rs, rt, rd, shamt, imm, address);
   
    if (cycle % 2 == 0)
    {

        if (opcode == 0)
        {
            tmpresult = ALU(registerFile[rs], registerFile[rt], 0);
        }
        else if (opcode == 1)
        {
            tmpresult = ALU(registerFile[rs], registerFile[rt], 1);
        }
        else if (opcode == 2)
        {
            tmpresult = ALU(registerFile[rs], imm, 2);
        }
        else if (opcode == 3)
        {
            tmpresult = ALU(registerFile[rs], imm, 0);
        }
        else if (opcode == 4)
        {
            ALU(registerFile[rs], registerFile[rt], 1);
        }
        else if (opcode == 5)
        {
            tmpresult = ALU(registerFile[rs], imm, 3);
        }
        else if (opcode == 6)
        {
            tmpresult = ALU(registerFile[rs], imm, 4);
        }
        else if (opcode == 7)
        {
        }
        else if (opcode == 8)
        {
            tmpresult = ALU(registerFile[rs], shamt, 5);
        }
        else if (opcode == 9)
        {
            tmpresult = ALU(registerFile[rs], shamt, 6);
        }
        else if (opcode == 10)
        {
            tmpresult = ALU(registerFile[rs], imm, 0);
        }
        else
        {
            tmpresult = ALU(registerFile[rs], imm, 0);
        }
        *(temparr2) = tmpresult;
        printf("Execute output : ALU result=%i\n", tmpresult);
    }
    else
    {
        *(EXMEM)=*(temparr2);

        int branchflag = 0;
        int jumpFlag = 0;
        if (opcode == 0)
        {
            writeflag = 1;
            *(EXMEM+4) = rd;
        }
        else if (opcode == 1)
        {
            writeflag = 1;
            *(EXMEM+4) = rd;
        }
        else if (opcode == 2)
        {
            writeflag = 1;
            *(EXMEM+4) = rt;
        }
        else if (opcode == 3)
        {
            writeflag = 1;
            *(EXMEM+4) = rt;
        }
        else if (opcode == 4)
        {
            if (zeroFlag != 1)
            {
                pc = *IDEX + imm;
                branchflag = 1;
            }
        }
        else if (opcode == 5)
        {
            writeflag = 1;
            *(EXMEM+4) = rt;
        }
        else if (opcode == 6)
        {
            writeflag = 1;
            *(EXMEM+4) = rt;
        }
        else if (opcode == 7)
        { 
            printf("address %i",address);
            address = address & 0b00001111111111111111111111111111;
            printf("address %i",address);
            int tmp = *IDEX & 0b11110000000000000000000000000000;
            printf("tmp %i",tmp);
            pc = tmp | address;
            printf("pc %i",pc);
            jumpFlag = 1;
        }
        else if (opcode == 8)
        {
            writeflag = 1;
            *(EXMEM+4) = rt;
        }
        else if (opcode == 9)
        {
            writeflag = 1;
            *(EXMEM+4) = rt;
        }
        else if (opcode == 10)
        {
            loadflag = 1;
            writeflag = 1;
            *(EXMEM+4) = rt;
        }
        else if (opcode == 11)
        {
            storeflag = 1;
            *(EXMEM+4) = rt;
        }
        *(EXMEM+1) = writeflag;
        *(EXMEM+2) = loadflag;
        *(EXMEM+3) = storeflag;

        if (jumpFlag)
        {
            fetchInt = 0;
            decodeInt = 0;
        }
        if (branchflag)
        {
            fetchInt = 0;
            decodeInt = 0;
        }
        printf("Execute output : writeflag=%i loadflag=%i storeflag=%i\n", writeflag, loadflag, storeflag);
    }
}

void memory()
{
    *MEMWB = *EXMEM;
    printf("Memory access input : ALU result=%i loadflag=%i storeflag=%i rt/rd=%i\n", *EXMEM, *(EXMEM+2), *(EXMEM+3), *(EXMEM+4));
    if (*(EXMEM+3))
    {
        printf("Updating memory location %i value from %i to %i\n", *EXMEM, memoryfile[*EXMEM], registerFile[*(EXMEM+4)]);
        memoryfile[*EXMEM] = registerFile[*(EXMEM+4)];
    }
    if (*(EXMEM+2))
    {
        *MEMWB = memoryfile[*EXMEM];
        printf("Memory access output : Value to load = %i\n", *MEMWB);
    }
    *(MEMWB+1) = *(EXMEM+1);
    *(MEMWB+2) = *(EXMEM+4);
}

void writeback()
{
    printf("Write back input : Value to load=%i writeflag=%i rt/rd=%i\n", *MEMWB, *(MEMWB+1), *(MEMWB+2));
    if (*(MEMWB+1))
    {
        if (*(MEMWB+2) != 0)
        {
            printf("Updating R%i value from %i to %i\n",*(MEMWB+2), registerFile[*(MEMWB+2)], *MEMWB);
            registerFile[*(MEMWB+2)] = *MEMWB;
        }
        else
        {
            printf("Writing to R0 failed.\n");
        }
    }
}

int LineToBinary(char *line)
{
    char *token = strtok(line, " ");
    int b = 0;
    int insnum = 0;
    if (strcmp(token, "ADD") == 0)
    {
        insnum = 0;
        b = 0b00000000000000000000000000000000;
    }
    else if (strcmp(token, "SUB") == 0)
    {
        insnum = 1;
        b = 0b00010000000000000000000000000000;
    }
    else if (strcmp(token, "MULI") == 0)
    {
        insnum = 2;
        b = 0b00100000000000000000000000000000;
    }
    else if (strcmp(token, "ADDI") == 0)
    {
        insnum = 3;
        b = 0b00110000000000000000000000000000;
    }
    else if (strcmp(token, "BNE") == 0)
    {
        insnum = 4;
        b = 0b01000000000000000000000000000000;
    }
    else if (strcmp(token, "ANDI") == 0)
    {
        insnum = 5;
        b = 0b01010000000000000000000000000000;
    }
    else if (strcmp(token, "ORI") == 0)
    {
        insnum = 6;
        b = 0b01100000000000000000000000000000;
    }
    else if (strcmp(token, "J") == 0)
    {
        insnum = 7;
        b = 0b01110000000000000000000000000000;
    }
    else if (strcmp(token, "SLL") == 0)
    {
        insnum = 8;
        b = 0b10000000000000000000000000000000;
    }
    else if (strcmp(token, "SRL") == 0)
    {
        insnum = 9;
        b = 0b10010000000000000000000000000000;
    }
    else if (strcmp(token, "LW") == 0)
    {
        insnum = 10;
        b = 0b10100000000000000000000000000000;
    }
    else if (strcmp(token, "SW") == 0)
    {
        insnum = 11;
        b = 0b10110000000000000000000000000000;
    }
    int rs1 = 0;
    int rt1 = 0;
    int rd1 = 0;
    int imm1 = 0;
    int shamt1 = 0;
    int address1 = 0;
    if (insnum == 0 || insnum == 1)
    {
        token = strtok(NULL, " ");
        char *c = token;
        c++;
        rd1 = atoi(c);
        token = strtok(NULL, " ");
        c = token;
        c++;
        rs1 = atoi(c);
        token = strtok(NULL, " ");
        c = token;
        c++;
        rt1 = atoi(c);
        rs1 = rs1 << 23;
        rt1 = rt1 << 18;
        rd1 = rd1 << 13;
        b = b | rs1 | rt1 | rd1;
    }
    else if (insnum == 2 || insnum == 3 || insnum == 4 || insnum == 5 || insnum == 6 || insnum == 10 || insnum == 11)
    {
        token = strtok(NULL, " ");
        char *c = token;
        c++;
        rt1 = atoi(c);
        token = strtok(NULL, " ");
        c = token;
        c++;
        rs1 = atoi(c);
        token = strtok(NULL, " ");
        c = token;
        imm1 = atoi(c);
        // printf("%i\n",imm1);
        imm1 = imm1 & 0b00000000000000111111111111111111;
        // printf("%i\n",imm1);
        rs1 = rs1 << 23;
        rt1 = rt1 << 18;
        b = b | rs1 | imm1 | rt1;
    }
    else if (insnum == 8 || insnum == 9)
    {
        token = strtok(NULL, " ");
        char *c = token;
        c++;
        rt1 = atoi(c);
        token = strtok(NULL, " ");
        c = token;
        c++;
        rs1 = atoi(c);
        token = strtok(NULL, " ");
        c = token;
        shamt1 = atoi(c);
        rs1 = rs1 << 23;
        rt1 = rt1 << 18;
        b = b | rs1 | shamt1 | rt1;
    }
    else
    {
        token = strtok(NULL, " ");
        char *c = token;
        address1 = atoi(c);
        b = b | address1;
    }
    return b;
}

int loadInstToMemory(char *fileName)
{
    FILE *txtfile;
    memoryfile = (int *)malloc(2048 * sizeof(int));
    char line[1000];
    txtfile = fopen(fileName, "r");
    if (txtfile == NULL)
    {
        printf("Error opening file \n");
        return 1;
    }
    while (fgets(line, sizeof(line), txtfile) != NULL)
    {
        memoryfile[NumberofInstructions] = LineToBinary(line);
        NumberofInstructions++;
    }

    fclose(txtfile);
    return 0;
}

void swapIntegers(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void printarray(int *arr)
{
    printf("[");
    for (int i = 0; i < 7; i++)
    {
        printf("%i,", arr[i]);
    }
    printf("]\n");
}

int main()
{
    if (loadInstToMemory("mips.txt"))
    {
        return 1;
    }

    temparr1=malloc(4*sizeof(int));
    temparr2=malloc(1*sizeof(int));
   
    IFID=malloc(1*sizeof(int));
    IDEX=malloc(8*sizeof(int));
    EXMEM=malloc(5*sizeof(int));
    MEMWB=malloc(3*sizeof(int));

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
            fetchSave = pc + 1;
            if (fetchSave <= NumberofInstructions)
            {
                fetchInt = fetchSave;
            }
        }
        // Even cycle
        else
        {
            swapIntegers(&executeInt, &memoryInt);
            swapIntegers(&decodeInt, &executeInt);
            swapIntegers(&fetchInt, &decodeInt);
        }
        printf("Cycle %d:  IF: %d, ID: %d, EX: %d, MEM: %d, WB:  %d \n", cycle, fetchInt, decodeInt, executeInt, memoryInt, writeBackInt);

        if(writeBackInt){
            writeback();
        }

        if(memoryInt){
            memory();
        }

        if(executeInt){
            execute();
        }

        if(decodeInt){
            decode();
        }

        if(fetchInt){
            fetch();
        }



        


        cycle++;
        // Stopping condition
        if (0 == fetchInt && 0 == decodeInt && 0 == executeInt && 0 == memoryInt)
        {
            break;
        }
        printf("\n");
    }
    free(IFID);
    free(IDEX);
    free(EXMEM);
    free(MEMWB);
    free(temparr1);
    free(temparr2);

    printf("Register File:\n");
    for (int i = 0; i < 32; i++)
    {
        printf("R%i:%i, ", i, registerFile[i]);
    }
    printf("\n");

    printf("Main Memory:\n");
    for (int i = 0; i < 2048; i++)
    {
        printf("%i:%i, ", i, memoryfile[i]);
    }
    printf("\n");
    return 0;
}
