#include "main_header.h"
/*second pass will create ob file,entry file and external file and go over file again and for each entry add address was used in and for external will add the dc of each extern.and code any line that used labels and wasnt fully coded */
void second_pass(FILE * file, table_line * table_instruction, table_line * table_data, int flag_ent, int flag_ext, int label_counter)
{
	FILE *file_ob ,*file_ent, *file_ext;
	int i, j, flag_correct = 0, flag_ext_use = 0;
	char first_word[WORD_LENGTH], second_word[WORD_LENGTH], file_ob_name[WORD_LENGTH], file_ext_name[WORD_LENGTH], file_ent_name[WORD_LENGTH];
	strcpy(file_ob_name, original_file_name);/*creating .ob, .ext ,.ent files name */
	strcpy(file_ext_name, original_file_name);
	strcpy(file_ent_name, original_file_name);
	strcat(file_ob_name,".ob");
	strcat(file_ent_name,".ent");
	strcat(file_ext_name,".ext");
	file_ob = fopen(file_ob_name, "w+");
	line_number = 0/*, error_flag = 0*/;
	if(flag_ext)/*moogdar here what is the dc of extern*/
		file_ext = fopen(file_ext_name, "w+");
		
	
	if(flag_ent)/*if has entry-*/
	{
		fseek(file, 0, SEEK_SET);/*go over file from beggining*/				
		file_ent = fopen(file_ent_name, "w+");
		while(!feof(file))
		{
			line_number++;
			fscanf(file, "%s", first_word);
			fscanf(file, "%s", second_word);/*!check nothing after second word*/
			if(!strcmp(first_word, ".entry"))/*if entry line*/
				{
					for(j = 0;(( j < label_counter)&&(strcmp(second_word, label_list[j].name))); j++)
						insert_to_ext_ent_file(file_ent, label_list[j].name, (unsigned int)label_list[j].address);
						
					if(j==label_counter)/*if saved not a label write in .ent file*/
						errors(33);
				}
		}
	}
	
	for(i = 0; i < IC; i++)/*go over instructions*/
		if(strncmp(table_instruction[i].operands, flag_addressing_done, 5))/*if not fully inserted it has label name thats coding has to be inserted*/
		{
			for(j = 0; j < label_counter; j++)/*find label coding according to label list*/
				if(!(strcmp(table_instruction[i].operands, label_list[j].name)))/**/
				{
					if(label_list[j].type != 'e')/*if its not extern then A R E is relocatable*/
						table_instruction[i].word.address.A_R_E = 2;
					else 
					{
						table_instruction[i].word.address.A_R_E = 1;/*if its extern then ARE is external and add to ext file */
						flag_ext_use = 1;
						insert_to_ext_ent_file(file_ext, label_list[j].name, (unsigned int)table_instruction[i].dec_add);
					}
					table_instruction[i].word.address.address = label_list[j].address;
					flag_correct = 1;/*found label in label list*/
				}
			if(!flag_correct /*&& flag_ext*/)/*label is not in label list will return error*/
			{
				line_number = table_instruction[IC].line_num;
				errors(34);
			}
		}	
	
	if(error_flag)/*if there were errors in second pass remove all files that were opend here*/
	{
		fclose(file_ob);/*if dont need to close!*/
		remove(file_ob_name);
		if(flag_ext)
		{
			fclose(file_ext);
			remove(file_ext_name);
		}
		if(flag_ent)
		{
			fclose(file_ent);
			remove(file_ent_name);
		}
		free(label_list);
		return;		
	}

		
	insert_to_ob_file(file_ob, (unsigned int)IC, (unsigned int)DC);/*if there were no errors will insert each line address and code in .ob file*/
	for(i = 0; i < IC; i++)	
		insert_to_ob_file(file_ob, (unsigned int)(table_instruction[i].dec_add), *(unsigned int *)(&(table_instruction[i].word)));		
	for(i = 0; i < DC; i++)	
		insert_to_ob_file(file_ob, (unsigned int)(table_data[i].dec_add), *(unsigned int *)(&(table_data[i].word)));		
	
	fclose(file_ob);
	if(flag_ent)
		fclose(file_ent);
	if(flag_ext)
	{
		fclose(file_ext);
		if(!flag_ext_use)
			remove(file_ext_name);
	}
	free(label_list);
}

