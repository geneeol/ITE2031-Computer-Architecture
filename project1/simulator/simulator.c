/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

enum
{
	ADD = 0, NOR,
	LW, SW, BEQ,
	JALR,
	HALT, NOOP,
	FILL
};

void init_state(stateType *);
void printState(stateType *);
int convertNum(int num);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    // my code
    int instr;
    int n_instr = 0;
    int opcode, reg_a, reg_b, dest_reg, offset;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

	/* TODO: */

    init_state(&state);

    while (state.pc < state.numMemory)
    {
        printState(&state);
        n_instr++;
        instr = state.mem[state.pc];
        opcode = instr >> 22 & 0x7;
        reg_a = instr >> 19 & 0x7;
        reg_b = instr >> 16 & 0x7;

        state.pc++;
        switch (opcode)
        {
            case ADD:
                dest_reg = instr & 0x7;
                state.reg[dest_reg] = state.reg[reg_a] + state.reg[reg_b];
                break ;
            case NOR:
                dest_reg = instr & 0x7;
                state.reg[dest_reg] = ~(state.reg[reg_a] | state.reg[reg_b]);
                break ;
            case LW:
                offset = convertNum(instr & 0xffff);
                state.reg[reg_b] = state.mem[state.reg[reg_a] + offset];
                break ;
            case SW:
                offset = convertNum(instr & 0xffff);
                state.mem[state.reg[reg_a] + offset] = state.reg[reg_b];
                break ;
            case BEQ:
                offset = convertNum(instr & 0xffff);
                if (state.reg[reg_a] == state.reg[reg_b])
                    state.pc += offset;
                break ;
            case JALR:
                state.reg[reg_b] = state.pc;
                state.pc = state.reg[reg_a];
                break ;
            case HALT:
                printf("machine halted\n");
                goto end;
            case NOOP:
                break ;
        }
    }
end:
    printf("total of %d instructions executed\n", n_instr);
    printf("final state of machine:\n");
    printState(&state);
    return(0);
}

void init_state(stateType *statePtr)
{
    statePtr->pc = 0;
    int i;
    for (i = 0; i < NUMREGS; i++) {
        statePtr->reg[i] = 0;
    }
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}
