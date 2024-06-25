;bs"d errors_input. the output is in screenshots in the main folder
;label errors
myLabel: .string "This should be fine"
1myLabel: .string "This shouldn't"
thisIsAveryVeryVeryVeryVeryLongLabel: .data 12, 4, 56, 78, 10
myLabel: .string "This label was already used on first line"
;extra commas in instruction
mov, r3, K
cmp K, , #-6
;data errors
myArray: .data 12,6, -9, 10,
myArray3: .data ,12,6, -9, , 10
.data
.data lost, 4, 8, 15, 16, 23, 42
.data --433, 653, 30
;string errors
.string
.string za wardo
.string za wardo"
.string "za wardo
.string za "wardo".
.string za ."wardo"
;extra words after commend
mov A, r1, r2
clr r2, r6
;illegal operands
jmp %ROOF %FLOOR
sub #3
mov #1, #4
;illegal numbers
mov #23a, r1
mov r1, #23a


