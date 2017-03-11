/*

	Redo of carry completion adder to handle arrays

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void printSum(int * val, int size){

	int i;

	printf("Sum = ");
	for(i = 0; i < size; i++){
		printf("%i", val[i]);
	}

	printf("\n");

}

unsigned int CCAdder(int * in1, int * in2, int * sum, int num_bits, int *d_accum){

	unsigned int cin1, cin0, ai, bi;	

	int C0[num_bits];
	int C1[num_bits];

	unsigned int cc = 0;
	int k;
	int l;
	int num_done = 0;
			
	for(k = 0; k < num_bits; k++){
		C0[k] = 0;
		C1[k] = 0;
	}

			
	for(k = num_bits-1; k >= 0; k--){			//initial cycle 
				
		if(!(in1[k] ^ in2[k])){					//checking for [0,0] or [1,1]
					
			if(in1[k] & in2[k])					//set bits in initial cycle
				C1[k] = 1;
			else if (~in1[k] & ~in2[k]){
				C0[k] = 1;
			}
		}else if(k == num_bits-1){
			C0[k] = 1;							//initialize C0 "IN"
		}
				
		if(in1[k] ^ in2[k] ^ 0){				//get intermediate Sum
			sum[k] = 1;
		}
	}

		*d_accum += 2;							//initial cycle/delay
		//printf("\t(2d) CCA Intermediate ");
		//printSum(sum, num_bits);

/*
 * This is the main looping structure for carrying out 
 * C0,C1 propagation
 */	
			while(!cc){

				cin1 = 0;				 //Cin buffers init
				cin0 = 1;				 //
				
				for(k = num_bits-1; k >= 0; k--){				//for each bit from the "right"
					
					ai = in1[k];
					bi = in2[k];
																//if current bit carry isn't complete yet
					if(C0[k] == C1[k]){						//ie, if C0,C1 = 0,0

						if(cin0 ^ cin1){							//check if previous carry complete

							if( (~ai & bi & cin1) | (ai & ~bi & cin1) ){
								C1[k] = 1;								//set C1 bit
								C0[k] = 0;
							}else if( ((~ai & bi & cin0) | (ai & ~bi & cin0)) +2 ){
								C0[k] = 1;								//set C0 bit
								C1[k] = 0;
							}

							//while next C0,C1 are equal to 0,0
							//keep moving along array looking next 0,1 or 1,0
							if(k != 0){						
								while(!(C0[k-1] ^ C1[k-1])){		
									if(k-1 <= 0){
										k = 0;
										break;
									}else
										k--;
								}
							}	
						}						
					}

					if(ai ^ bi ^ cin1){				//get intermediate Sum
						sum[k] = 1;
					}else
						sum[k] = 0;
					
					cin0 = C0[k];			//send C0,C1 to buffers for
					cin1 = C1[k];			//next bit
						
				}//END FOR LOOP (Each bit)
						
				*d_accum += 2;							//end of cycle delay accum
				//printf("\t(2d) CCA Intermediate ");
				//printSum(sum, num_bits);
		
				for(l = num_bits-1; l >= 0; l--){
					
					if(C0[l] ^ C1[l])					//if C0,C1 are 0,1 or 1,0
						num_done++;						//then this bit is done
				}
				
				if(num_done >= num_bits){
														//if all bits are done
					cc = 1;								//set while loop sentinel					
				}
				num_done = 0;
					
			}//END WHILE LOOP (carry-complete)
		
		//printf("\t(2d) Carry Complete\n");
		*d_accum += 2;				//carry complete
		//printf("\t(2d) Final Sum Bit\n");
		*d_accum += 2;				//final sum bit 
		//printf("\tFinal ");
		//printSum(sum, num_bits);

return cc;
}
