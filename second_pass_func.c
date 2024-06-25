#include "main_header.h"
/*this function will convert numbers to special 32 base.which ,in binary is at most five bits.the given number has 10 bits with bits masks and basic operations we can find the 5 significant bits and  the 5 less significant bits and find their representation in the special 32 base according to the array of the 32 bits where each value is saved according to index*/
void convert_to_32_base(FILE * file, unsigned int word)
{
	char base_32[] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
	unsigned int first_half, second_half, high_bits = 992, low_bits = 31;
	first_half = (word & high_bits)>>5;
	second_half = word & low_bits;
	fputc(base_32[first_half], file);
	fputc(base_32[second_half], file);
}
/*will receive a file,address and coded word and insert into file in same line with tab in between*/
void insert_to_ob_file(FILE * file, unsigned int address, unsigned int coded_word)
{
	convert_to_32_base(file, address);
	fputc('\t' , file);
	convert_to_32_base(file, coded_word);
	fputc('\n' , file);
}
/*will receive a file, label name and address and insert into file in same line with tab in between*/
void insert_to_ext_ent_file(FILE * file, char * label_name, unsigned int address)
{
	fputs(label_name, file);
	fputc('\t' , file);
	convert_to_32_base(file, address);
	fputc('\n' , file);
}
