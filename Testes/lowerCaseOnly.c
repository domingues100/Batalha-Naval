#include <stdio.h>
#include <string.h>

void aux_lowercaseOnly (char *str) {
	
	short k;
	for (k = 0; k < strlen(str); k++) 
		if (str[k] >= 'A' && str[k] <= 'Z')
			str[k] += 32;
}

int main () {
	
	char nome[10];
	
	//scanf ("%s", nome);
	gets (nome);
	
	aux_lowercaseOnly (nome);
	
	printf ("%s", nome);
	
	return 0;
}
