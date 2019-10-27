#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * Implements an LC-3 simulator that is capable of executing the following instructions:
 * LD
 * LEA
 * LDI
 * AND
 * NOT
 * ADD
 *
 * Author: Jacob Zaidi
 */

int registerR[8] = {0,0,0,0,0,0,0,0}; 
int registerPC = 0;
int registerIR = 0;
char registerCC = 'Z';

void printRegisters();
void LD(int DR, int PCoffset9, char* thing);
void checkCC(int value); 

int main(int argc, char* argv[]){
	FILE* fp  = fopen(argv[1], "r"); //opening the file
	unsigned char readp[2]; //declaring a variable of two bytes
	int number; //a representation of a line(two bytes) as a integer value
	while((fread(readp,2,1,fp) == 1)) { //reading every line until end of file
        number = readp[1] + (readp[0] << 8); //converting a line to a number 
        registerIR = number;
		if (registerPC == 0){ 
            registerPC = number; //setting the registerPC to the value of the line
            registerPC--;
        }
		
        if (number >> 12 == 2){ //If the first 4 bits are 0010 then its a LD function
            LD((number >> 9) & 7, registerPC + (number & 0x1ff)+3, argv[1]); //makes the parameters to perform the load. Shifted the line 9 places to the right and masked away the OP code to get the DR. Added the PC to the value to load (line masked with 0111111111)  
        }
		
		if (number >> 12 == 14){        //LEA
            registerR[(number>>9) & 7] = registerPC + (number & 0x1ff) + 1;
            checkCC(registerR[(number >> 9) & 7]);
		}	
		if (number >> 12 == 10){       //LDI
            LD((number >> 9) & 0x7, registerPC + (number & 0x1ff)+4, argv[1]);
            LD((number >> 9) & 0x7,registerR[(number >> 9) & 0x7]+2, argv[1]);
        }
		
        if (number >> 12 == 5){       //AND
            if (((number >> 5) & 0x01) == 1){
                registerR[(number >> 9) & 7] = registerR[(number >> 6) & 7] & (number & 31) & 0xffff;
            } else{
                registerR[(number >> 9) & 7] = (registerR[(number >> 6) & 7] & registerR[number & 7]) & 0xffff;
            }
            checkCC(registerR[(number >> 9) & 7]);
        }
		
        if (number >> 12 == 9){
			registerR[(number >> 9) & 7] = ~(registerR[((number >> 6) & 7)]) & 0xffff;
            checkCC(registerR[(number >> 9) & 7]);
        }
		
        if (number >> 12 == 1){       //ADD
            if (((number >> 5) & 0x01) == 1){
                registerR[(number >> 9) & 7] = (registerR[(number >> 6) & 7] + (number & 31)) & 0xffff;
            } else{
                registerR[(number >> 9) & 7] = (registerR[(number >> 6) & 7] + registerR[number & 7]) & 0xffff;
            }

            checkCC(registerR[(number >> 9) & 7]);
			printf("after executing instruction\t0x%04x\n", registerIR);
            printRegisters();
        }

        

        registerPC++;
        
	}
	fclose(fp);
	return 0;
}

void LD(int DR, int PCoffset9, char* thing){
    FILE* fp2  = fopen(thing, "r");
    int number; //a representation of a line(two bytes) as a integer value
    int temporaryPC = 0; 
    unsigned char readp[2]; //declaring a variable of two bytes
    while((fread(readp,2,1,fp2) == 1) && (PCoffset9 != temporaryPC)) { //while not end of file and while the value to load is not equal
        number = readp[1] + (readp[0] << 8); //shifting to get the value of the line
        if (temporaryPC == 0){ 
            temporaryPC = number++; //setting the temporary PC to the value of the line + 1 
        }
        temporaryPC++; //then adds every next line so we end up with the value of the line we want to be at

    }
    registerR[DR] = number; //loading the number into the register and storing it into the register array
    checkCC(number); 		//printing the NZP
}

void checkCC(int value){ 
    if (((value >> 15) & 0x1) == 1){registerCC = 'N';}
    else    if ((value & 0x7fff) > 0){registerCC = 'P';}
    else    if ((value & 0x7fff) == 0){registerCC = 'Z';}
}

void printRegisters(){
    int i;
    for (i = 0; i < 8; i++){
        printf("R%d\t0x%04x\n", i, registerR[i]);
    }
    printf("PC\t0x%04x\n", registerPC);
    printf("IR\t0x%04x\n",  registerIR);
    printf("CC\t%c\n",  registerCC);
    printf("==================\n");
}
