run: main.o pre_passes.o first_pass.o first_pass_func.o second_pass.o second_pass_func.o  is_correct_func.o
	gcc -g -ansi -pedantic -Wall main.o pre_passes.o first_pass.o first_pass_func.o second_pass.o second_pass_func.o is_correct_func.c -o run
main.o: main.c main_header.h
	gcc -c -ansi -pedantic -Wall main.c -o main.o
pre_passes.o: pre_passes.c main_header.h
	gcc -c -ansi -pedantic -Wall pre_passes.c -o pre_passes.o
first_pass.o: first_pass.c first_pass_func.c main_header.h
	gcc -c -ansi -pedantic -Wall first_pass.c -o first_pass.o
first_pass_func.o: first_pass_func.c main_header.h
	gcc -c -ansi -pedantic -Wall first_pass_func.c -o first_pass_func.o
second_pass.o: second_pass.c second_pass_func.c main_header.h
	gcc -c -ansi -pedantic -Wall second_pass.c -o second_pass.o
second_pass_func.o: second_pass_func.c second_pass.h main_header.h
	gcc -c -ansi -pedantic -Wall second_pass_func.c -o second_pass_func.o
is_correct_func.o: is_correct_func.c is_correct_func.c main_header.h
	gcc -c -ansi -pedantic -Wall is_correct_func.c -o is_correct_func.o
