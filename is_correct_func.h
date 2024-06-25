#define correct 1
#define incorrect 0
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define BLUE "\x1b[34m"

/*this function will receive a number from 0 to 36 and return appropriate error*/
void errors(int);
/*this function will recieve a string and will check if the given string is empty.
if is correct will return 1 .
if not,will return appropriate error and 0*/
int is_empty_string(char *);
/*this function will recieve a string and will check if the given string is a number.
if is correct will return 1 .
if not,will return appropriate error and 0*/
int is_correct_number(char *);
/*this function will recieve a string and will check if the given string is a string(starts and ends with ")
if is correct will return 1 .
if not,will return appropriate error and 0*/
int is_correct_string(char *);
/*this function will recieve label and label type o for operand,m for macro and n for label declaration and will check if the given label is correct.
if is correct will return 1 .
if not,will return appropriate error and 0*/
int is_correct_label(char *, char);
/*this function will receive for each operand its instruction opcode ,method and if is source or destination.
it will check that that method for specific instruction is correct.
it will check all illegal options and return incorrect if illegal.otherwise will return correct.
*/
int is_correct_operand_type(int , int , int);
/*this function will receive a pointer to string and make sure string doesnt start or end with a space and that it has a termination character at the end */
void first_last_space(char **);
