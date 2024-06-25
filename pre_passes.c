#include "main_header.h"

/*this program will go over the file and check if the current line is a call to a  saved macro ,if it is, it will spread that macro in newfile .if not ,will check its (part of) a macro definition and will save it. otherwise,will just copy line to newfile .assuming macro definition will always come before a call to saved macro,this program will copy over given file and return the same file with spread macros*/
void macrospread(FILE * file, FILE * newfile)
{
	int macro_counter = 0, i = 0, is_macro = 0;/*macro counter will save the number of macros that were saved so far is macro saves if currently saving a macro*/
	char *line, *first_word ;
	macros *macro_list;
	macro_list = (macros *)malloc(sizeof(macros));
	line = (char *)malloc(sizeof(char)*LINE_LENGTH);
	first_word = (char *)malloc(sizeof(char)*LINE_LENGTH);
	if(!first_word || !line || !macro_list)
		errors(2);
	fseek(file, 0, SEEK_SET);
	line_number = 0, error_flag = 0;
	while(!feof(file))/*reads the whole file, line by line*/
	{
		fscanf(file, "%s", first_word);
		fgets(line, LINE_LENGTH, file);
		line_number++;	
		while(i < macro_counter && strncmp(macro_list[i].name, first_word, strlen(first_word)))/*checks if first word is a saved macro*/
			i++;
		if(i < macro_counter)
		{
			if(!is_empty_string(line))/*something is written after the macro-rest of string isn't empty*/
			{
				fputs(first_word, newfile);/*insert line*/
				fputs(line, newfile);
			}
			else
				fputs(macro_list[i].macro, newfile);/*spread macro*/
		}	
		else if(!strcmp("macro", first_word))/*if its a start of macro decleration*/
		{
			macro_list[macro_counter].name = (char *)malloc(sizeof(char)*LINE_LENGTH);/*memory allocation to save macro*/
			macro_list[macro_counter].macro = (char *)malloc(sizeof(char)*LINE_LENGTH);
			if(!macro_list[macro_counter].macro || !macro_list[macro_counter].name)/*memory allocation failed*/
				errors(2);
			for(i = 0; i < macro_counter; i++)
				if(!strcmp(macro_list[i].name, strtok(line, " \n")))/*if macro name was already used*/
					errors(3);	
			is_macro = 1;/*saving macro process currently*/
			strcpy(macro_list[macro_counter].name, strtok(line, " \n"));
			if(is_empty_string(line))/*if line empty save in macro data an enter*/
				macro_list[macro_counter].macro = strtok(NULL, "\n");
		}	
		else if(!strncmp("endmacro", first_word, 8))
		{
			is_macro = 0;/*stopped saving macro process*/	
			macro_counter++;
			macro_list = (macros *)realloc(macro_list ,sizeof(macros)*(macro_counter+1));
		}
		else if(is_macro)/*saving macro process currently*/
		{
			macro_list[macro_counter].macro = (char *)realloc(macro_list[macro_counter].macro, sizeof(char)*((macro_counter+1)*LINE_LENGTH));	
			strcat(macro_list[macro_counter].macro, first_word);/*save current in macro data*/
			strcat(macro_list[macro_counter].macro, line);
		}	
		else/*not calling a macro and not saving current macro just copy line to file*/
		{
			fputs(first_word, newfile);
			fputs(line, newfile);
		}
		i = 0;
		strcpy(first_word, "");
		strcpy(line, "");	
		if(fgetc(file) == EOF)
			break;
		else
			fseek(file, -1, SEEK_CUR);
	}
	while(macro_counter)
	{	
		free(macro_list[macro_counter].macro);
		free(macro_list[macro_counter].name);
		macro_counter--;
	}
	free(macro_list);
	free(first_word);
	free(line);
}

/*this program will copy given file into the other file without multiple spaces.will read given file letter by letter if isnt space will copy to other file and if is space will run as long as all there is are spaces on same line and copy one space to the other file and return error if line is longer than 80 chars*/
void erase_space(FILE * file, FILE * newfile)
{
	char curr, counter = 0;
	line_number = 1;
	while((curr = fgetc(file)) != EOF)
	{
		if(isspace(curr))
		{
			while(isspace(curr) && (curr != '\n'))/*multiple spaces in a row on same line replace with one space*/
				curr = fgetc(file);
			if(curr != '\n')
				fputc(' ',newfile);
			else
			{
				counter = 0;
				line_number++;
			}
		}
		fputc(curr,newfile);/*if not a space will copy char to other file*/
		counter++;
		if(counter > 80)
		{
			errors(0);
			counter=0;	
			continue;
		}
	}
}
