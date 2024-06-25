/*second pass will recieve original file and table data, table instructions ,flag if has entry,flag if has extern and label conter
will return 3 files ; files of entries and where they were used files of externs and the whole file coded in special 32 base*/
void second_pass(FILE *, table_line *, table_line *, int, int, int);

/*this program will convert given number to special 32 base*/
void convert_to_32_base(FILE *, unsigned int );

/*will receive a file,address and coded word and insert into file in same line with tab in between*/
void insert_to_ob_file(FILE *, unsigned int, unsigned int);

/*will receive a file, label name and address and insert into file in same line with tab in between*/
void insert_to_ext_ent_file(FILE *, char *, unsigned int);
