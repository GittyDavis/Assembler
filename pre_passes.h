
/*this program will receieve a file and return that file with erased double spaces and no empty lines and if has a line longer than 80 will return error*/
void erase_space(FILE * , FILE * );

/*this program will go over the given file and return that file with spread macros*/
void macrospread(FILE * , FILE * );

/*this struct saves macros in this file.saves the macro name and the macro data that the macro holds that should be spread when the macro is called*/
typedef struct
{
	char *name;
	char *macro;
}macros;
