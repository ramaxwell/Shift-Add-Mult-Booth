Shift-Add multiplier with Booth Algorithm for shift-over and Carry Completion Adder for the Add/Subtract
---------------------------------------------------------
SAmult-booth is a program used to multiply two N-bit
numbers using shift and add method (shift-over 0's and
1's) with Carry completion for the add/subtract.

Overall, the program performs multiplication of 
randomly generated 2,3,4,...,32 bit (MAX_BITS)
values multiple times as denoted by the MAX_RUNS 
constant. The total delay (in gate delays) as 
well as cycle count for each set of bits is 
accumulated and reported upon completion.

Please refer to the diagram in this repo for
overall algorithm block diagram.

Compilation and execution is straight forward but a 
usage file is included in the repo.

List of files in this repo:

--> usage.txt :                        Description of how to compile and run

--> Shift-Add-mult-Booth-diagram.png : Algorithm Diagram

--> SAmult-booth-cca.c :               Simulation source file

--> CCAdder.h :                        Carry Completion header file

--> CCAdder.c :                        Carry Completion methods

--> run.txt :                          Typical output

--> README.md :                        This readme file
