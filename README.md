# CS180_A02

//Compiling and Running the program in linux
The solution can be run in linux by using the command lines: 
gcc -o main main.c
./main input.csv

//Brief introduction
This program contains the implementation of 3 file allocation methods: Contiguous, Linked, and Index. The program reads inputs from the file, input.csv, then prompts the user
to choose between the 3 allocation methods. The program reads the command prompts (add, read, delete) within the input.csv file and executes functions appropriately.
After running all the commands within input.csv, the program prints out the disk map showing the current state of the disk.

//User input
Upon running the program, the user will be prompted to choose between the 3 allocation methods.
1 ----> Contiguous
2 ----> Linked
3 ----> Indexed