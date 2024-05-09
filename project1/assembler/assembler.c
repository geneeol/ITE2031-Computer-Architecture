/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

// TODO: zero register 값 고정!
// TODO: 우분투에서 돌려보기

// my code
#define MAX_LABEL_LENGTH 1000
#define INITIAL_LABEL_SIZE 10
typedef struct label
{
	char name[MAX_LABEL_LENGTH];
	int addr;
} t_label;

typedef struct label_arr
{
	t_label *arr;
	int size;
	int capacity;
} t_label_arr;

t_label_arr labels;

typedef int (*t_validator)(char * label, char * opcode, char * arg0, 
							char * arg1, char * arg2);

char *instruction_tabel[] = {
	"add", "nor",
	"lw", "sw", "beq",
	"jalr",
	"halt", "noop",
	".fill"
};

t_validator validator_tabel[] = {
	rtype_validator, rtype_validator,
	itype_validator, itype_validator, itype_validator,
	jtype_validator,
	otype_validator, otype_validator

};

enum
{
	ADD = 0, NOR,
	LW, SW, BEQ,
	JALR,
	HALT, NOOP,
	FILL
};

// R-type add, nor
// I-type lw, sw, beq
// J-type jalr
// O-type halt, noop

// bit usage, bit 0 is the least significant bit
// 31-25 not used, should be 0
// 24-22 opcode
// more details in handout page2

// TODO: label 이름 다양하게 테스트, label 개수 많이 넣기
// TODO: beq에 same register

// assembly code format
// label<white>instruction<white>fld0<white>fld1<white>fld2<white>comments
// (<white> means a series of tabs and/or spaces)
// unused fileds should be ignored

int rtype_validator(char *label, char *opcode, char *arg0,
					char *arg1, char *arg2);
int itype_validator(char *label, char *opcode, char *arg0,
					char *arg1, char *arg2);
int jtype_validator(char *label, char *opcode, char *arg0,
					char *arg1, char *arg2);
int otype_validator(char *label, char *opcode, char *arg0,
					char *arg1, char *arg2);
int directive_validator(char *label, char *opcode, char *arg0,
						char *arg1, char *arg2);
int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
void init_labels_arr();
int duplicated_label(char *name);
void add_label(char *name, int addr);
void print_labels();

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	int pc = -1;
	int opcode_bit;
	int machine_code = 0;
	int offset;

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* here is an example for how to use readAndParse to read a line from
		 inFilePtr */

	// if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
	// 	/* reached end of file */
	// }

	init_labels_arr();
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
	{
		printf("label: %s, opcode: %s, arg0: %s, arg1: %s, arg2: %s\n", label, opcode, arg0, arg1, arg2);
		pc++;

		if (strlen(label) == 0)
			continue ;

		// TODO: print addr
		if (isNumber(label) || strlen(label) > 6)
		{
			printf("error in line %d: label is invalid\n", pc + 1);
			exit(1);
		}

		if (duplicated_label(label))
		{
			printf("error in line %d: label is duplicated\n", pc + 1);
			exit(1);
		}
		add_label(label, pc);
	}
	print_labels();



	/* TODO: Phase-1 label calculation */

	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	rewind(inFilePtr);

	/* TODO: Phase-2 generate machine codes to outfile */

	/* after doing a readAndParse, you may want to do the following to test the
		 opcode */

	pc = -1;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
	{
		pc++;
		opcode_bit = get_opcode(opcode);
		if (opcode_bit < 0)
		{
			printf("error in line %d: opcode is invalid\n", pc + 1);
			exit(1);
		}
		if (!validator_tabel[opcode_bit](label, opcode, arg0, arg1, arg2))
		{
			printf("error in line %d: validator is invalid\n", pc + 1);
			exit(1);
		}
	// 	switch(opcode_bit)
	// 	{
	// 		// check register number range, zero register
	// 		case ADD:
	// 		case NOR:
	// 			// if (strlen(arg0) == 0 || strlen(arg1) == 0 || strlen(arg2) == 0
	// 			// 	|| is_int(arg0) || is_int(arg1) || is_int(arg2))
	// 			// {
	// 			// 	printf("error in line %d: register number is invalid\n", pc + 1);
	// 			// 	exit(1);
	// 			// }
	// 			// if (!in_range(arg0, 0, 7) || !in_range(arg1, 0, 7) || !in_range(arg2, 0, 7))
	// 			// {
	// 			// 	printf("error in line %d: register number is invalid\n", pc + 1);
	// 			// 	exit(1);
	// 			// }
	// 			// machine_code = opcode_bit << 22 | atoi(arg0) << 19 | atoi(arg1) << 16 | atoi(arg2);
	// 		case LW:
	// 		case SW:
	// 			// if (strlen(arg0) == 0 || strlen(arg1) == 0 || strlen(arg2) == 0)
	// 			// {
	// 			// 	printf("error in line %d: arg is invalid\n", pc + 1);
	// 			// 	exit(1);
	// 			// }
	// 			// if (in_range(arg0, 0, 7) || in_range(arg1, 0, 7))
	// 			// {
	// 			// 	printf("error in line %d: register number is invalid\n", pc + 1);
	// 			// 	exit(1);
	// 			// }
	// 			// if (!is_int(arg2))
	// 			// {
	// 			// 	offset = get_label_addr(arg2);
	// 			// 	if (offset < 0)
	// 			// 	{
	// 			// 		printf("error in line %d: label is undefined\n", pc + 1);
	// 			// 		exit(1);
	// 			// 	}
	// 			// }
	// 			// else
	// 			// {
	// 			// 	offset = atoi(arg2);
	// 			// }

	// 			// break ;
	// 		case BEQ:
	// 			break ;
	// 		case JALR:
	// 			break ;
	// 		case HALT:
	// 		case NOOP:
	// 			break ;
	// 		case FILL:
	// 			break ;
	// 		default:
	// 			printf("error in line %d: opcode is invalid\n", pc + 1);
	// 			exit(1);
	// 	}

	// }


	if (!strcmp(opcode, "add")) {
		/* do whatever you need to do for opcode "add" */
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int is_int(char *str)
{
	char str2[100];

	sprintf(str2, "%d", atoi(str));
	return strcmp(str, str2) == 0;
}

int in_range(char *str, int min, int max)
{
	int num = atoi(str);

	return num >= min && num <= max;
}

void init_labels_arr()
{
	labels.size = 0;
	labels.capacity = INITIAL_LABEL_SIZE;
	labels.arr = (t_label *)malloc(sizeof(t_label) * labels.capacity);
}

int duplicated_label(char *name)
{
	for (int i = 0; i < labels.size; i++)
	{
		if (strcmp(labels.arr[i].name, name) == 0)
			return 1;
	}
	return 0;
}

void add_label(char *name, int addr)
{
	if (labels.size == labels.capacity)
	{
		labels.capacity *= 2;
		labels.arr = (t_label *)realloc(labels.arr, sizeof(t_label) * labels.capacity);
	}
	strcpy(labels.arr[labels.size].name, name);
	labels.arr[labels.size].addr = addr;
	labels.size++;
}

int get_label_addr(char *name)
{
	for (int i = 0; i < labels.size; i++)
	{
		if (strcmp(labels.arr[i].name, name) == 0)
			return labels.arr[i].addr;
	}
	return -1;
}

void print_labels()
{
	for (int i = 0; i < labels.size; i++)
	{
		printf("name: %s, addr: %d\n", labels.arr[i].name, labels.arr[i].addr);
	}
}

int get_opcode(char *opcode)
{
	for (int i = 0; i < sizeof(instruction_tabel) / sizeof(char *); i++)
	{
		if (strcmp(opcode, instruction_tabel[i]) == 0)
			return i;
	}
	return -1;
}