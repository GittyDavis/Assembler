
/*this function will recieve a file and an existing file name and will save the file according to the appropriate representation of assembly code in machine language other than lines that call labels and extern and entry commands that will remain uncoded and if file correct will send to second pass
@param file 
@param file_name
*/
void first_pass(FILE *);

/*this program will save the line that its given as a label if line is legal label 
*@param line is the label that has to be entered
*/
void insert_label(label **, char *, char, int *);

/*this program will determine,according to given instruction how many operands the given line gets and sends the rest of the line to appropriate functions .if is not an operand,will return an appropriate error
@param table of instructions
@param given instruction
@param input after operand
*/
void send_instruction_to_method(table_line *, char *, char **);

/*this program will insert a zero operand instruction
*@param table of instructions
@param the line to be coded 
*/
void insert_instruction_with_0_operands(table_line *, char **);

/*this program will insert a one operand instruction
*@param table of instructions
@param the line to be coded 
*/
void insert_instruction_with_1_operands(table_line *, char **,int);

/*this program will insert a two operand instruction
*@param table of instructions
@param the line to be coded 
@param instruction opcode??add!!
*/
void insert_instruction_with_2_operands(table_line *, char **,int);

/*this program will determine,according to given operand what operand method is it and send to appropriate function
@param table of instructions
@param number of method
@param given operand
@param 1 if first operand and 2 if is second operand after 
*/
void send_operands_to_method(table_line *, int, char *, int, int);

/*this program will address code operands that are immediate only if they are legal immidiates
@param table instruction
@param operand
*/
void insert_operand_method_0(table_line *, char *);
/*this program will address code operands that are labels
@param table instruction
@param operand
*/
void insert_operand_method_1(table_line *, char *);

/*this program will address code operands that are struct
@param table instruction
@param operand
*/
void insert_operand_method_2(table_line *, char *);

/*this program will address code operands that are registers
@param table instruction
@param operand
@param how many operans in this lines instruction
@param is first operand thats a register
*/
void insert_operand_method_3(table_line *, char *, int , int);

/*this program will send data to be inserted in function according to data type
@param data type (.data/.string/.struct)
@param data to be inserted
*/
void send_data_to_method(table_line *,  char *, char **);

/*this program will code the line that its given as data if line is legal data 
*@param table line 
*/
void insert_data(table_line *, char **);

/*this program will enter the given line as its representation of a string in machine language
@param table line
@param given line
*/
void insert_string(table_line *, char **);

/*this program will enter the given line as its representation of a struct in machine language
@param table line
@param given line
*/
void insert_struct(table_line *, char **);

/*this program will determine address coding method depending on operand type
@param operand
*/
int address_method(char *);

#define flag_addressing_done "#good"
#define FIRST_ADDRESS 100

