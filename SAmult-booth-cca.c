/*

Robert A. Maxwell
Project 2 for comparison with Shift-Add_RCA
Computer Arithmetic
University of Texas at San Antonio
------------------------------------------------------
 n-bit shift and add multiplier with CCA and Shift-Over

 This application simulates an n-bit shift-add multipler
using a carry completion adder and Booth algorithm.

 Multiplication operands are generated randomly, and 
the operation is carried out in an asynchronous manner. 
Many runs for each multiplication are carried out so 
as to generate smooth delay averages.

https://github.com/ramaxwell/Shift-Add-Mult-Booth.git
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CCAdder.h"

#define MAX_BITS 32
#define MAX_RUNS 1000
#define ARRAY_SIZE MAX_BITS
#define PROD_ARRAY_SIZE ARRAY_SIZE*2 +1

void printOperand( int *oper, int max_bits ){

	int c;

	for ( c = 0; c < max_bits; c++ ){

		printf("%u", oper[c]);

	}
	printf("\n");
}
//
//Generate random binary numbers
//
void generateOperand( unsigned int *op , int num_bits ){

	int pos;
	unsigned int val;

	for(pos = 0; pos < num_bits; pos++){
		val = rand() % 2;
		
		if(val){
			op[pos] = val;			
		}
	}
}
//
//Insert product into upper half of product array
//
void insertUpperProd(int *p_reg, int num_bits, int * op /*, int c*/){

	int m = 0;

	for(m = 0; m < num_bits; m++){

		p_reg[m] = op[m];

	}
}
//
//Insert multiplier into lower n-bits of product array
//
void insertLowerProd(int *p_reg, int num_bits, int * op){

	int m = 0;

	for(m = 0; m < num_bits; m++){

		p_reg[m+num_bits] = op[m];

	}
}
//
//retrieve upper portion of product for input into adder
//
void getUpperProd(int *p_reg, int *pProdReg, int num_bits){

	int m;

	for(m = 0; m < num_bits; m++){

		pProdReg[m] = p_reg[m];

	}
}
//
//for copying to mux_out
//
void arrayCopy( int * arr1, int * arr2, int size){
	
	int m;

	for(m = 0; m < size; m++){

		arr2[m] = arr1[m];

	}
}
//
//Register shift
//
void shiftREG( int *prodReg, int shamt ){

	int m;
	unsigned int temp = prodReg[0];

	for(m = PROD_ARRAY_SIZE-1; m >= 0; m--){

		prodReg[m] = prodReg[m-1];

	} 
	
	prodReg[0] = prodReg[1];
}
//
//compliment all bits and add 1 at the end
//
void comp2(int * val, int num_bits){

	int m;

	for(m = 0; m < num_bits; m++){			//loop for complimenting all bits

		if(val[m])
			val[m] = 0;
		else val[m] = 1;

	}

	for(m = num_bits-1; m >= 0; m--){
		
		if(val[m]){
			val[m] = 0;
		}else{
			val[m] = 1;
			break;
		}	
	}
}
//
//for checking product of multiplication
// -- returns integer value
//
double bin2dec(int *bin, int num_bits){

	int m;
	int cnt = 0;
	double dec = 0;	

	for(m = num_bits-1; m >=0; m--){
		
		if(bin[m] == 1)
			dec += pow(2,cnt);

		cnt++;	
	}

return dec;
}

//
// Main Function
//
int main(void){

	int i, j;
	int delay;

	double avgDelay[MAX_BITS];
	float delayPerCycle;
	float avgDelayPerCycle[MAX_BITS];
	int A[ARRAY_SIZE];				
	int B[ARRAY_SIZE];				
	int S[ARRAY_SIZE];
	int MUX_IN1[ARRAY_SIZE];	//for holding a zero variable
	int pProd[ARRAY_SIZE];

	srand(time(NULL));			//seed the random number generator

	for ( i = 2; i <= MAX_BITS; i++ ) {

		avgDelay[i-2] = 0;
		avgDelayPerCycle[i-2] = 0;

		for(j = 0; j < MAX_RUNS; j++){

			int k;
			int l;
			int cycles;
			int c_complete = 0;
			int mux_out[ARRAY_SIZE];		
			int prod[PROD_ARRAY_SIZE];
			
			for(k = 0; k < ARRAY_SIZE; k++){
				A[k] = 0;						//initialize storage arrays
				B[k] = 0;
				S[k] = 0;
				MUX_IN1[k] = 0;
				mux_out[k] = 0;
				pProd[k] = 0;
			}

			for(k = 0; k < PROD_ARRAY_SIZE; k++){
				prod[k] = 0;
			}

			delay = 0;
			
			generateOperand(A, i);			//generate operands
			generateOperand(B, i);			// A and B

			//Initialize product register
			insertLowerProd(prod, i, B);		//B goes in lower half of product

			for(l = 0; l < i; l++){
				cycles++;
				delay += 2;		//XOR delay
				if(prod[2*i-1] ^ prod[2*i]){			// 1) evaluate XOR 
														//    for b_i and b_i-1
					
														//Since (b_i XOR b_i-1) == 1
														//can copy A to mux output
					arrayCopy(A, mux_out, i);			// 2.) eval MUX

					delay += 2;		//MUX delay
			
					getUpperProd(prod, pProd, i);						
										
					if(prod[2*i-1])						// if(bi) then ~a+1
						comp2(mux_out, i);						
														// 3.) eval CCA						

					c_complete = CCAdder(pProd, mux_out, S, i, &delay);
					
					insertUpperProd(prod, i, S);		// 4.) store sum		
				}
										// 5.) shift reg
				shiftREG(prod, 1);
				delay += 2;				//shift delay				
				delay += 2;				//initiate next cycle			
			}

			avgDelay[i-2] += delay;
			delayPerCycle = delay/cycles;
			avgDelayPerCycle[i-2] += delayPerCycle;
			cycles = 0;
			delay = 0;
		}//END FOR LOOP (Each sim run)

		avgDelay[i-2] = avgDelay[i-2]/MAX_RUNS;	
		avgDelayPerCycle[i-2] = avgDelayPerCycle[i-2]/MAX_RUNS;	
		printf("Number of bits: %i\n", i);
		printf("Average Delay = %.3f\n", avgDelay[i-2]);	
		printf("Average Delay Per Cycle = %.3f\n", avgDelayPerCycle[i-2]);

	}//END FOR LOOP (Each Set of number of bits)

	FILE *fp;									//save average delay to file for 
	fp=fopen("run.txt", "w");					//input into spreadsheet for graphing
	fprintf(fp, "# Bits \t Avg Delay \t Delay/cycle\n");
	for(i = 0; i < MAX_BITS-1; i++){
		fprintf(fp, "%i\t\t%.3f\t\t%.3f\n", i+2, avgDelay[i], avgDelayPerCycle[i]);
	}
	fclose(fp);

return 0;
}
