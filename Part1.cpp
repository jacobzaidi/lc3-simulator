#include <iostream>
#include <iomanip>

/* Converts each line of an assembly program into hexadecimal form.
 *
 * Author: Jacob Zaidi
*/

int main(int argc, char* argv[]){
	FILE* fp  = fopen(argv[1], "r");
	unsigned char readp[2];
	while((fread(readp,2,1,fp) == 1)) {
		std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)readp[0] << std::setfill('0') << std::setw(2) << (int)readp[1] << "\n";
	}
	fclose(fp);
	return 0;
}
