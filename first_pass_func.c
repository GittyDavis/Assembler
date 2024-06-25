#include "main_header.h"

/*checks if label name is correct with appropriate function if correct 
checks if label name exists already by comparing label name with all other label names that are saved in a label list.
if already declared will return appropriate error accroding to label type(if external or not). 
then proceeds to add label to list using label counter as index, 
saving its name that was given to function, 
type d if data label and i if instruction label 
and will update label counter by one and will reallocate space for the next label line in label list */
void insert_label(label ** label_list, char * name, char type, int * label_counter)
{
	int i;
	first_last_space(&name);
	if(is_correct_label(name, 'n'))
	{
		for(i = 0; i < (*label_counter); i++)/*checks if the label name exists already will return appropriate error*/
			if(!strncmp((*label_list)[i].name, name, strlen(name)))
			{
				if((*label_list)[i].type != 'e' && type != 'e')/*label exists already*/
					errors(4);
				else if(type != 'e')/*defining label that already declared as external*/
					errors(5);
				else/*declaring an already defined label as external*/
					errors(35);
			}
			strcpy((*label_list)[*label_counter].name, name);
			if(type == 'd')/*depending on label type will save address and type*/
				(*label_list)[*label_counter].address = DC;
			else if(type == 'i')
				(*label_list)[*label_counter].address = IC;
			else 
				(*label_list)[*label_counter].address = 0;
			(*label_list)[(*label_counter)].type = type;
			(*label_counter) += 1;	/*saves memory for the next label to be inserted and updates label counter*/
			*label_list = (label *)realloc(*label_list ,sizeof(label)*((*label_counter)+1));						
			if(!label_list)
				errors(2);
	}
}

/*the operations are arranged in an array according to how many operands they are supposed to get
we will compare the operand in given line to the operand list and stop when reached an equal operand.
then, according to given rules will send the rest of the line after the operand to correct insert function according to the amount of operands.
 if the word isnt a legal instruction, will return an error*/
void send_instruction_to_method(table_line table_instruction[],  char * name_operation, char ** line)
{	
	operation operands[16] = {{"mov", 0}, {"cmp", 1}, {"add", 2}, {"sub", 3}, {"lea", 6}, {"not", 4}, {"clr", 5}, {"inc", 7}, {"dec", 8}, {"jmp", 9}, 
	{"bne", 10}, {"get", 11}, {"prn", 12}, {"jsr", 13}, {"rts", 14}, {"hlt", 15}};
	int i = 0;
	while(i < 16 && strcmp(operands[i].name, name_operation))
		i++;
	table_instruction[IC].dec_add = IC;
	table_instruction[IC].word.code.opcode = operands[i].opcode;
	if(i < 5)
		insert_instruction_with_2_operands(table_instruction, line, operands[i].opcode);
	else if(i < 14)
		insert_instruction_with_1_operands(table_instruction, line, operands[i].opcode);
	else if(i < 16)
		insert_instruction_with_0_operands(table_instruction, line);
	else/*not legal operand*/
		errors(6);	
}	

/*this function will receive a line that is supposed to have 2 operands with a comma in between.
the function will devide into two strings make sure they dont start with a space and they end with a string termination character
and will send each operand to address method which will return what addressing method to use.
if addressing method is legal the function will code a line that that tells that there are two operands
 and saving each method.then will send each of the operand to be coded according to their method*/
void insert_instruction_with_2_operands(table_line table_instruction[], char ** line, int instruction_opcode)
{
	int num_method_1, num_method_2, i = 0, j = 0;
	char first_operand[WORD_LENGTH] = "", second_operand[WORD_LENGTH] = "";
	first_last_space(line);
	while((*line)[i] != ',' && (*line)[i] != '\0')/*save first operand*/
		first_operand[j++] = (*line)[i++];	
	first_operand[strlen(first_operand)] = '\0';
	if((*line)[i] == ',')
	{
		j = 0; i++;
		/*while?*/if(isspace((*line)[i]))
			i++;
		while((*line)[i] != ' ' && (*line)[i] != '\0' && (*line)[i] != '\n'&& (*line)[i] != ',')/*save second operand*/
			second_operand[j++] = (*line)[i++];
		second_operand[strlen(second_operand)] = '\0';
		num_method_1 = address_method(first_operand);/*find addressing method for each operand*/
		num_method_2 = address_method(second_operand);
		if((is_correct_operand_type(instruction_opcode, num_method_1, 's')) && (is_correct_operand_type(instruction_opcode,num_method_2,'d')))/*if correct operand type will insert line of source ,destination address method and send each operand to be coded*/
		{
			table_instruction[IC].word.code.src_register = num_method_1;
			table_instruction[IC].word.code.des_register = num_method_2;
			table_instruction[IC].word.code.A_R_E = 0;
			strcpy(table_instruction[IC].operands, flag_addressing_done);
			IC++;
			send_operands_to_method(table_instruction, num_method_1, first_operand, 1, 0);
			send_operands_to_method(table_instruction, num_method_2, second_operand, 2, num_method_1);
		}
	} 
	else 
		errors(29);/*only one operand and missing commas in between*/
	if(((strlen(*line)) - i) > 1)/*have another argument */
		errors(30);	    	
}

/*this function will receive a line that is supposed to have one operand.
the function will make sure they end with a string termination character.
will send the operand to address method which will return what addressing method to use.
if addressing method is legal the function will code a line that that tells that there is one operand and saving this operands method.
then will send the operand to be coded to its method*/
void insert_instruction_with_1_operands(table_line table_instruction[], char ** line, int instruction_opcode)
{
	int num_method, i = 0, j = 0;
	char operand[WORD_LENGTH] = "";
	if((*line)[i] == '\0')
	{
		errors(19);
		return;
	}
	while((*line)[i] != '\0' && (*line)[i] != '\n' && (*line)[i] != ',')/*will find operand and find address method*/
		operand[j++] = (*line)[i++];
	operand[strlen(operand)] = '\0';
	num_method = address_method(operand);
	if(is_correct_operand_type(instruction_opcode,num_method,'d'))/*if correct operand type will insert line of source (no source),destination address method and send operand to be coded*/
	{
		table_instruction[IC].word.code.src_register = 0;
		table_instruction[IC].word.code.des_register = num_method;
		table_instruction[IC].word.code.A_R_E = 0;
		strcpy(table_instruction[IC].operands, flag_addressing_done);
		IC++;
		send_operands_to_method(table_instruction, num_method, operand, 1, 4);
	}
	if(((strlen(*line))-i)>1)/*have more than one argument */
		errors(30);
}

/*this function will receive a line that is supposed to have zero operands. 
will make sure the string is empty and will code the line*/
void insert_instruction_with_0_operands(table_line table_instruction[], char ** line)
{
	if(!is_empty_string(*line))/*if has arguments after instruction incorrect,otherwise will code line*/
		errors(31);
	table_instruction[IC].word.code.src_register = 0;
	table_instruction[IC].word.code.des_register = 0;
	table_instruction[IC].word.code.A_R_E = 0;
	strcpy(table_instruction[IC].operands, flag_addressing_done);
	IC++;
}

/*this function recieves operand and checks it.
if it starts with a #  will send to immediate method- 0, 
if has a . in the string will send to struct method-2,
if is a register that starts with a 'r' and then has a number from 1-7 and nothing else and will send to register method -3 
if wasnt all three will check if its correct label if its isnt it will return error statement special for operand-operand not legal*/
int address_method(char * operand)
{
	int i = 0 ,j = 0;
	if(isspace(operand[0]))
		i++;
	j = i;
	while( i < strlen(operand) && operand[i] != '.')
		i++;
	if(operand[0] == '#')/*if operand starts with # with an legal immidiate after is address method 0 */
	{operand++;
		if(is_correct_number(operand))
			return 0;/*if operand starts with # with an illegal is incorrect operand*/
		else return 1;
	}
	else if(i < strlen(operand))/*if has . in it it might be a struct*/
		return 2;
	else if((operand[j] == 'r') && (operand[j+1] >= '0') && (operand[j+1] <= '7') && (operand[j+2] <= '\0'))/*is a register*/
		return 3;	
	else/*either label or illegal operand*/
		return 1;
}

/*will receive table instruction, num method the operand and is its a source or destination operand.
and will send the operand to be coded into machine language according to method*/
void send_operands_to_method(table_line table_instruction[], int num_method, char * operand, int first_or_second, int is_first_reg)
{
	if(num_method == 0)
		insert_operand_method_0(table_instruction, operand);	
	else if(num_method == 1)
		insert_operand_method_1(table_instruction, operand);
	else if(num_method == 2)
		insert_operand_method_2(table_instruction, operand);
	else if(num_method == 3)
		insert_operand_method_3(table_instruction, operand, first_or_second, is_first_reg);
}

/*this program will receive an operand that is an immediate make sure it has a string termination character and code into machine language.
into a line of the instruction table according to given rules, will mark as dine coding and update instruction counter*/
void insert_operand_method_0(table_line table_instruction[], char * operand)
{
	int i = 1, j = 0;
	char number[5];
	while((i < strlen(operand) || isdigit(operand[i]) || operand[i] == '-' ) && operand[i] != '\n' && operand[i] != '\0')
		number[j++] = operand[i++];
	number[j] = '\0';
	if(is_correct_number(number))
	{
		table_instruction[IC].word.address.A_R_E = 0;
		table_instruction[IC].word.address.address = atoi(number);
		table_instruction[IC].dec_add = IC;
		strcpy(table_instruction[IC].operands, flag_addressing_done);
		IC++;
	}
	else errors(9);
}

/*this program will receive an operand that is a label,
 will code into machine language line number and instruction counter and update instruction counter.but wont be fully coded beacuse will wait for second pass until we know all label addresses*/
void insert_operand_method_1(table_line table_instruction[], char *operand)
{
	if(is_correct_label(operand, 'o'))
	{
		strcpy(table_instruction[IC].operands, operand);
		table_instruction[IC].line_num = line_number;
		table_instruction[IC].dec_add = IC;
		IC++;
	}
}

/*this program will receive an operand that is a struct will save the line until the period and after.
before the period should be label name and after should be a number;either 1 or 2.
then code into machine language.into a line of the instruction table according to given rules, 
will mark as dine coding and update instruction counter*/
void insert_operand_method_2(table_line table_instruction[], char *operand)
{
	char * label_name;
	label_name = strtok(operand, ".");
	operand = strtok(NULL, "\0");
	if(is_correct_label(label_name, 'o'))
	{
		if((operand[0] == '1' || operand[0] == '2')&&(strlen(operand)==1))/*check if correct call to struct and insert to table*/
		{
			strcpy(table_instruction[IC].operands, label_name);
			table_instruction[IC].line_num = line_number;
			table_instruction[IC].dec_add = IC;
			IC++;
			table_instruction[IC].dec_add = IC;
			table_instruction[IC].word.address.address = (operand[0]-'0');
			table_instruction[IC].word.address.A_R_E = 0;
			strcpy(table_instruction[IC].operands, flag_addressing_done);
			IC++;
		}
		else
			errors(23);/*incorrect struct*/
	}	
}

/*this program will receive an operand that is a register.
will insert into a line of the instruction table according to given rules, 
will mark as dine coding and update instruction counter*/
void insert_operand_method_3(table_line table_instruction[], char * operand, int num_operand, int is_first_operand_reg)
{
	int i = 0;
	table_instruction[IC].word.rgister.src_register = 0;
	table_instruction[IC].word.rgister.des_register = 0;
	while(i < strlen(operand) && isspace(operand[i]))
		i++;
	if(is_first_operand_reg == 4)
		table_instruction[IC].word.rgister.des_register = operand[i+1];
	else if(num_operand == 1)
		table_instruction[IC].word.rgister.src_register = operand[i+1];
	else if(num_operand == 2)
	{
		if(is_first_operand_reg == 3)
			IC = IC - 1;
		table_instruction[IC].word.rgister.des_register = operand[i+1];
	}
	table_instruction[IC].word.address.A_R_E = 0;
	table_instruction[IC].dec_add = IC;
	strcpy(table_instruction[IC].operands, flag_addressing_done);
	IC++;
}

/*this program will receive data tabel, a data operation name and line that will be sent to appropriate insert function only after made sure it has a string termination character.
we sent to this function and know that name operation is correct and will only be one of the three legal ones.*/
void send_data_to_method(table_line table_data[],  char * name_operation, char ** line)
{	
	first_last_space(line);
	first_last_space(&name_operation);
	if((*line)[strlen(*line)] == '\n')
    		(*line)[strlen(*line)-1] = '\0';
	if(!strncmp(name_operation, ".data", 5))			
		insert_data(table_data, line);	
	if(!strncmp(name_operation, ".string", 7))
		insert_string(table_data, line);
	if(!strncmp(name_operation, ".struct", 7))
		insert_struct(table_data, line);
}

/*this program will receive data that will be sure it has a string termination character.
will run until string termination character and add current character to number when reached end of string or comma will check if its a double comma and will return error and check if is correct number and add it to table data then will nullify number and run on the rest of the string
.*/
void insert_data(table_line table_data[], char ** line)
{
	
	int i = 0, j = 0, flag_comma = 0,flag_correct=1;/*flag_comma will save if is the char before current char was a comma and flag correct will be incorrect for any error*/
	char number[LINE_LENGTH] = "";
	if((*line)[strlen(*line)-1]=='\n')
		(*line)[strlen(*line)-1]='\0';
	if((*line)[0]==',')/*if comma before data*/
		{
			errors(27);
			flag_correct=0;
			j++;
		}
	
	while((*line)[j] != '\0')
	{
		if(isspace((*line)[j]))
			j++;
		while((*line)[j] != '\0' && (*line)[j] != ',')
		{
			number[i++] = (*line)[j++];
			flag_comma = 0;
		}		
		if((*line)[j] ==',')/*the number is a comma*/
		{
			j++;
			if(!flag_comma)/*there isnt a comma right before*/
				flag_comma = 1;
			else/*two commas in a row*/
			{
				errors(16);
				flag_correct = 0;
				strcpy(number,"");				
				continue;
			}
		}
		if(is_correct_number(number))/*number before comma correct,will save*/
		{
			table_data[DC].word.data.address = atoi(number);	
			table_data[DC].dec_add = DC;
			DC++;	
		}
		else flag_correct=0;
		strncpy(number,"",strlen(number));
		i = 0;
	}
	if(flag_comma)/*if comma with no data after*/
	{
		errors(20);
		flag_correct = 0;
	}
	if(!flag_correct)
		errors(28);
}

/*this function makes sure the string doent end with a space and if its correct string (starts and ends with apostrophes) 
will copy what is inside the apostrophes and save coded in machine code,
a coded word for every char in the string and one line for string termination character*/
void insert_string(table_line table_data[], char ** line)
{
	int i;	
	if(is_correct_string(*line))
	{
		for(i = 1; i < ((strlen(*line))-1); i++)/*insert string without quotation marks, a word for every char*/
		{
			table_data[DC].word.data.address = (*line)[i];	
			table_data[DC].dec_add = DC;	
			DC++;
		}	
		table_data[DC].word.data.address = '\0';
		table_data[DC].dec_add = DC;	
		DC++;
	}	
}

/*this function recieves a line we assume has a string termination character.
will run until reach comma or end of line and if is correct number,
add as number then will check if the rest of the string after the comma 
(not including a space,if there is) if its a correct string and add it as a string*/
void insert_struct(table_line table_data[], char ** line)
{					
	int i = 0, j = 0, is_correct = 1;
	char string[LINE_LENGTH], number[WORD_LENGTH];
	
	while((*line)[j] != '\0' && (*line)[j] != ',' )/*save number,first argument in struct ,insert if correct*/
		number[i++] = (*line)[j++];
	number[i] = '\0';
	if(is_correct_number(number))
	{
		table_data[DC].word.data.address = atoi(number);	
		table_data[DC].dec_add = DC;				
		DC++;
	}
	else 
		is_correct = 0;	
	j++;
	if(isspace((*line)[j]))
		(*line)++;
	i = 0;

	while((*line)[j] != '\0' )
		string[i++] = (*line)[j++];
	string[i] = '\0';
	j = 1;
	if(is_correct_string(string))/*save string,second argument in struct ,insert if correct*/
	{	
		while(j < i-1)
		{
			table_data[DC].word.data.address = string[j];	
			table_data[DC].dec_add = DC;	
			j++;
			DC++;
		}
		table_data[DC].word.data.address = '\0';
		table_data[DC].dec_add = DC;	
		DC++;
	}
	else is_correct = 0;
	if(!is_correct)/*incorrect struct arguments*/
		errors(21);				
}
