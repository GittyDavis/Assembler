#include "main_header.h"
/*this function will receive error number and return appropriate error*/
void errors(int error_number)
{
	char file_name_am[WORD_LENGTH], file_name_as[WORD_LENGTH];
	strcpy(file_name_am, original_file_name);
	strcat(file_name_am, ".am");
	strcpy(file_name_as, original_file_name);
	strcat(file_name_as, ".as");
	error_flag = 1;
	if(error_number < 4)
		printf("%s%s%s%s%s%s%s%d%s%s%s", RED, "Error!\tin file: ",  BLUE, file_name_as, RED, " line: ", BLUE,  line_number, RED, " - ", RESET_COLOR);
	else
		printf("%s%s%s%s%s%s%s%d%s%s%s", RED, "Error!\tin file: ", BLUE, file_name_am, RED, " line: ", BLUE,  line_number, RED, " - ", RESET_COLOR);
	switch (error_number)
	{
		case 0: printf("the line is too long (without counting the white spaces)\nthe program will continue to next file\n"); break;
		case 1: printf("no such file\n"); break;
		case 2: printf("memory allocation failed\n"); exit(0); break;
		case 3:	printf("macro name exists already\n"); break;
		case 4:	printf("label name exists already\n"); break;
		case 5: printf("label name defined already as an external label\n"); break;
		case 6: printf("instruction not found\n"); break;
		case 7: printf("num of operands not correct\n"); break;
		case 8: printf("characters after end of instruction\n"); break;
		case 9: printf("operand not legal\n"); break;
		case 10: printf("label name is not legal\n"); break;
		case 11: printf("number is not legal\n"); break;
		case 12: printf("string is not legal\n"); break;
		case 13: printf("incorrect .data input\n"); break;
		case 14: printf("struct not legal\n"); break;
		case 15: printf("incorrect insert data command\n"); break;
		case 16: printf("multiple commas in a row\n"); break;
		case 17: printf("arguments not correct acording to command\n"); break;
		case 18: printf("operand not legal for command\n"); break;
		case 19: printf("no operands after command\n"); break;
		case 20: printf("comma with no data afterwards\n"); break;
		case 21: printf("incorrect struct input\n"); break;
		case 22: printf("no input to enter\n"); break;
		case 23: printf("illegal access to struct\n"); break;
		case 24: printf("no data after data instruction\n"); break;
		case 25: printf("empty label\n"); break;
		case 26: printf("missing label after extern\n"); break;
		case 27: printf("comma before data\n"); break;
		case 28: printf("illegal data insert\n"); break;
		case 29: printf("too few arguments for instruction\n"); break;
		case 30: printf("too many arguments for instruction\n"); break;
		case 31: printf("this instruction doesn't get arguments\n"); break;
		case 32: printf("missing label after entry\n"); break;
		case 33: printf("label is set as entry but not defined\n"); break;
		case 34: printf("operand not legal, use of label that is not defined\n"); break;
		case 35: printf("tried to define extern label that is declared allready\n"); break;
		case 36: printf("macro name is not legal\n"); break;
	}
}		
/*this function will assign flag to be correct and will change if identifies an illegal char in the string.if its a saved word flag will changed to incorrrect.
if the length of label is longer than legal or its first char isnt a letter the flag is changed to incorrect.
then will run on string and if char isnt letter or digit th flag is changed to incorrect 
at the end will return appropriate error (depending which function is using it) if flag incorrect and will return flag */
int is_correct_label(char * label_name, char type_label)
{	
	int i, flag_correct = 1;
	char saved_words [28] [6] = {"data","struct","string","extern","entry","r1","r2","r3","r4","r5","r6","r7","mov","cmp","add", "sub", "lea", "not", "clr","inc", "dec","jmp","bne","get","prn","jsr","rts", "hlt"};
	for(i=0;i<29;i++)
		if(!strcmp((saved_words[i]),label_name))/*if is saved word that cant be used as label*/
			flag_correct=0;
	if((strlen(label_name) > WORD_LENGTH) || !isalpha(label_name[0]))/*if not alphabetical first letter or label too long*/
		flag_correct = 0;
	for(i = 1; i < strlen(label_name); i++)
		if(!isalpha(label_name[i]) && (!isdigit(label_name[i])))/*if not alphabetical letter or digit*/
			flag_correct = 0;
	if(!flag_correct)
	{
		if(type_label == 'n')
			errors(10);
		if(type_label == 'm')
			errors(36);
		else if(type_label == 'o')
			errors(9);
	}
	return flag_correct;
}
/*this function will assign flag to be correct and will change if identifies an illegal char in the string.
if the its first char isnt a number the flag is changed to incorrect.unless its + or - and has legal numbers after in which case flag will be correct.
then will run on string and if char isnt digit the flag is changed to incorrect 
at the end will retun error if flag incorrect and will return flag */
int is_correct_number(char * number)
{	
	int flag_correct = 1, i;
	first_last_space(&number);
	/*printf("|%s|\n",number);*/
	if ((isdigit(number[0]) == 0))/*if doesnt start with a number*/
		flag_correct = 0;
	if(((number[0] == '-') || (number[0] != '+')) && (strlen(number) > 1))/*unless it starts with plus or minus*/
		flag_correct = 1;
	for(i = 1; i < strlen(number); i++)
		if(!(isdigit((int)number[i])))/*if not digit*/
			flag_correct = 0;
	if(flag_correct == 0)
		errors(11);
	return flag_correct;
} 
/*this function receives string,assuming it doesnt have a space at the begenning and has no emty spaces at end .
checks if the first and last char in line are not quotation marks will return error and incorrect.otherwise will return correct  */
int is_correct_string(char * string)
{
	first_last_space(&string);
	if ((string[0] != '"') || (string[strlen(string)-1] != '"')||(strlen(string)<2))/*if doesnt start and end like a string*/
	{
		errors(12);
		return incorrect;
	}
	return correct;
}

/*this function will receive for each operand its instruction opcode ,method and if is source or destination.
it will check that that method for specific instruction is correct.
it will check all illegal options and return incorrect if illegal.otherwise will return correct.
*/
int is_correct_operand_type(int instruction_opcode, int num_method, int source_destination)
{
	if((((instruction_opcode >= 0) && (instruction_opcode <= 13) && (instruction_opcode != 1) && (instruction_opcode != 12)) && (source_destination == 'd') && 			(num_method == 0)) || ((instruction_opcode == 6) && (source_destination == 's') && ((num_method == 0) || (num_method == 3))))
	{
		errors(18);
		return incorrect;
	}
	return correct;
}

/*this function will check if the string is empty if it is will return correct ,else will return incorrect*/
int is_empty_string(char * string)
{
	if(string != NULL && strcmp(string, "\0") && strcmp(string, "\n") && strcmp(string, " ") && strcmp(string, ""))/*if string is not empty*/
		return incorrect;
	return correct;
}
/*this function will make sure string doesnt start or end with space*/
void first_last_space(char ** string)
{ 
	if((*string)!=NULL)
	{
		if(((*string)[strlen(*string)-1] == '\n') || (isspace((*string)[strlen(*string)-1])))/*if ends with space*/
			(*string)[strlen(*string)-1] = '\0';
		else 
			(*string)[strlen(*string)] = '\0';
		if (isspace((*string)[0]))/*if starts with space*/
			(*string)++;
	}
}

