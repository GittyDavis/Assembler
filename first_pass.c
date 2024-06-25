#include "main_header.h"

/*in the first pass we will go through given file, line by line determine what type of line it is .first if comment line, will continue.
if its a label will check what type of label and add to data lable image and instruction label image accordingly and send the rest of line it to appropriate functions to be inserted into data image and instruction image according to operand type.(for each instruction will send to determine,according to instruction name how many operands it has and then to a seperate function to determine what method type each operand is and then to another function that will code that operand according to its method)
otherwise, if not label then check if instruction or data line, 
then will insert according to instruction type and operant type. 
all lines with operands that are not labels will be inserted in data image
assuming file has its macros spread ,no multiple white spaces and no empty lines.
in this file,after assigning a new string will always send to first_last_space to determine the string wont start or end with a space*/

void first_pass(FILE * file)
{
	char *line, *first_word, *second_word, *ptr_line, *ptr_second_word;
	int  j, flag_ent = 0, flag_ext = 0, label_counter = 0;/*flags that will save if this file has entry or extern and that will count labels*/
	table_line table_data[155], table_instruction[155];/*tables that will save data and instructions*/
	label_list = (label *)malloc(sizeof(label));
	line = (char *)malloc(sizeof(char)*LINE_LENGTH);
	ptr_line = line;
	first_word = (char *)malloc(sizeof(char)*LINE_LENGTH);
	second_word = (char *)malloc(sizeof(char)*LINE_LENGTH);
	ptr_second_word = second_word;
	IC = 0,	DC = 0, line_number = 0, error_flag = 0;
	fseek(file, 0, SEEK_SET);				
	while(!feof(file))
	{	
		
		fscanf(file, "%s", first_word);
		if(fgets(line, LINE_LENGTH, file) == NULL)/*if reached end of file*/
			break;
		line_number++;/*update current line number*/
		first_last_space(&first_word);
		first_last_space(&line);
		if(first_word[0] == ';')/*if comment line*/
			continue;
		else if(first_word[(strlen(first_word)-1)] == ':')/*is label*/
		{
			first_word[strlen(first_word)-1] = '\0';
			if(!strcmp("",line))/*empty label*/
			{	
				is_correct_label(first_word,'n');
				errors(25);
				continue;
			}
			second_word = strtok(line, " \n\t\0");/*word after label*/
			first_last_space(&second_word);
			line = strtok(NULL, "\0\n");/*line after label*/
			first_last_space(&line);	
			if(line == NULL)
			{
				line = (char *)malloc(sizeof(char)*LINE_LENGTH);
				strcpy(line,"");
			}
				
			if(!strcmp(second_word, ".data") || !strcmp(second_word, ".string") || !strcmp(second_word, ".struct"))/*data label*/
			{
				insert_label(&label_list, first_word, 'd', &label_counter);
				if (strcmp("",line))/*line not empty*/
					send_data_to_method(table_data, second_word, &line);
				else errors(24);
			}
		
			else /*instruction label*/
			{	
				insert_label(&label_list, first_word, 'i', &label_counter);
				send_instruction_to_method(table_instruction, second_word, &line);	
			}
		}
		else if(!strcmp(first_word, ".extern"))
		{
			flag_ext = 1;
			if(!strcmp("",line))/*line is empty*/
			{
				errors(26);
				continue;
			}
			second_word = strtok(line, " \n\t\0");
			first_last_space(&second_word);
			insert_label(&label_list, second_word, 'e', &label_counter);
		}		
		else if(!strncmp(first_word, ".entry", 6))/*!! check if word after entry and problem*/
			{
				if(!strcmp("",line))/*line is empty*/
				{
					errors(32);
					continue;
				}
				flag_ent = 1;
			}
		else if(!strcmp(first_word, ".data") || !strcmp(first_word, ".string") || !strcmp(first_word, ".struct"))/*strn*/
			if (strcmp("",line))/*line not empty*/
				send_data_to_method(table_data, first_word, &line);
			else errors(24);
		else
			send_instruction_to_method(table_instruction, first_word, &line);	
				
		if(fgetc(file) == EOF)
			break;
		else
			fseek(file, -1, SEEK_CUR);				
	}

	for(j = 0; j < IC; j++)/*update addresses according to right order ;instructions first then data*/
		table_instruction[j].dec_add += FIRST_ADDRESS;
	for(j = 0; j < DC; j++)
		table_data[j].dec_add += (FIRST_ADDRESS+IC);
	
	for(j = 0; j < label_counter; j++)
	{
		if(label_list[j].type == 'i')
			label_list[j].address += FIRST_ADDRESS;
		else if(label_list[j].type == 'd')
			label_list[j].address += (FIRST_ADDRESS+IC);
	}
	
	if((!error_flag) && (IC || DC))/*if file was incorrect or was just comments and whitespaces wont continue to second pass*/
		second_pass(file, table_instruction, table_data, flag_ent, flag_ext, label_counter);
	else
		free(label_list);
		
	free(ptr_line);
	free(first_word);
	free(ptr_second_word);
}

