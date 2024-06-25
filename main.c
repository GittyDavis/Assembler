#include "main_header.h"
/*this project imitates the operation of a computer assembler.coding assembly language into machine language.every line in the given file (filename) that is assembly language will be coded into memory cells called words and will create sevral files
filename.am is the original file with speaded macros
filename.ob is the original file in machine language in special 32 base
this program will open given files and for each file create filename.as and filename.am and send to erase space ,then to macrospread and then to first pass which eventually sends to second  pass
assuming original file has .as ending*/
int main(int argc, char * argv[])
{
	int i;
	char file_name[WORD_LENGTH], file_am_name[WORD_LENGTH], file_temp_name[WORD_LENGTH];
	FILE *file, *file_am, *file_temp;
	error_flag = 0;
	for(i = 1; i < argc; i++)
	{
		error_flag = 0;
		
		strcpy(original_file_name, argv[i]);
		strcpy(file_name, original_file_name);
		strcpy(file_am_name, original_file_name);
		strcpy(file_temp_name, original_file_name);
		strcat(file_name,".as");
		strcat(file_temp_name,".txt");
		strcat(file_am_name,".am");
		/*memcpy(0,table_data,sizeof(table_data));
		memcpy(0,table_instructions,sizeof(table_instructions));*/
		file_am=NULL;
		label_list=NULL;
		/*memcpy(label_list,0,sizeof(*label_list));*/
		if(!(file = fopen(file_name , "r")))/*open original file to read .if file doesnt exist return error*/
		{
			errors(1);
			continue;
		}
		file_temp = fopen(file_temp_name, "w+");/*opening files to write in them*/
		file_am = fopen(file_am_name, "w+");
		erase_space(file, file_temp);
		if(!error_flag)
			macrospread(file_temp, file_am);/*send to macro spread to spread macros*/
		else printf("macro %s\n",file_name);		
		if(!error_flag)
			first_pass(file_am);
		else printf("first pass %s\n",file_name);
		fclose(file_am);/*closing files at end*/
		fclose(file_temp);
		fclose(file);
		remove(file_temp_name);/*remove erase space file*/
	}
	return 0;
}

