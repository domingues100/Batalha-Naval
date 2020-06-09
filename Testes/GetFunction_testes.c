#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int aux_getFunction (unsigned char str[]) {
	
	short k;
	char _str[10];
	
	for (k = 0; k < strlen(str); k++) //Remove case sensitivity
		if (str[k] >= 'A' && str[k] <= 'Z') 
			_str[k] = str[k] + 32;
		else 
			_str[k] = str[k];
		
	char functionTitle[][10] = {"pow", "ajuda", "reset", "sair", "acaso", "gravar", "carregar", "revelar"};

	for (k = 0; k < 8 && strcmp (_str, functionTitle[k]); k++);

	return k;
}

int main () {
	
	printf ("-- %d --", aux_getFunction ("acAso"));
	
	return 0;
}
