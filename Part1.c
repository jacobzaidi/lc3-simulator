#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Converts each line of an assembly program into hexadecimal form.
 *
 * Author: Jacob Zaidi
*/

int main(int argc, char* argv[]){
	FILE* fp  = fopen(argv[1], "r");
	unsigned char readp[2];	
	while((fread(readp,2,1,fp) == 1)) {
		printf("0x%02x%02x\n", readp[0], readp[1]); 
	}
	fclose(fp);
	return 0;
}