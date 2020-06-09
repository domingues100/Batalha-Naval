#include <stdlib.h>
#include <stdio.h>

unsigned char binario () {
	
	static bool j = 1;
	
	return j ^= 1;
}

unsigned char check () {
	
	unsigned char report = 0;
	unsigned char pack[3] = {0, 0, 0};
	bool feedback;	
	short k;
	
	for (k = 0; k < 8; k++) {
		
		report |= binario() << k;
	
	}	
	
	return report;
}


int main () {
	
 	short k;
	for (k = 0; k < 8; k++)
		printf ("%d\n", binario());
	
	printf ("%x", check());
	
	return 0;
}
