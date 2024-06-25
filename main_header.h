#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_LENGTH 80
#define WORD_LENGTH 30



/*this struct is the representation of a code word in machine language*/
typedef struct 
{
	unsigned int A_R_E:2;
	unsigned int des_register:2;
	unsigned int src_register:2;
	unsigned int opcode:4;
}code_word;

/*this struct is the representation of a register in machine language*/
typedef struct 
{
	unsigned int A_R_E:2;
	unsigned int des_register:4;
	unsigned int src_register:4;
}register_word;

/*this struct is the representation of an address in machine language*/
typedef struct 
{
	unsigned int A_R_E:2;
	unsigned int address:8;
}address_word;

/*this struct is the representation of a data word in machine language*/
typedef struct 
{
	unsigned int address:10;
}data_word;

/*this struct is the representation of each line of code in the memory*/
typedef struct 
{
	 int dec_add;
	 char operands[LINE_LENGTH];
	 int line_num;
	 union word {
		data_word data;
		code_word code;
		register_word rgister;
		address_word address;
	 }word;
}table_line;

/*this struct is the representation of a label saving each label's name what number label and what type*/
typedef struct
{
	char name[WORD_LENGTH];
	int address;
	char type;
}label;

/*this struct is the representation of an operation saving its name and opcode*/
typedef struct
{
	char name[WORD_LENGTH];
	int opcode;
}operation;

#include "pre_passes.h"
#include "first_pass.h"
#include "second_pass.h"
#include "is_correct_func.h"

int line_number, error_flag, DC, IC;/*line number will save current line number in .am file and will be updated with every line,error flag will turn on if there is an error in file and not continue. data counter, instruction counter will count */
char original_file_name[WORD_LENGTH];
label *label_list;

