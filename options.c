#include <options.h>

int get_options(char *option){
	int pos=0;
	char *options [] = {
		"empty",
		"new",
		"update",
		"remove",
		"reset",
		"list",
		0
	};
	while(options[pos]){
		if(!strcmp(option,options[pos])){
			return pos;
		}
		pos++;
	}
	return 0; 
}

void help_message(){
	printf("Opção desconhecida, lista:\n");
	printf(" - new\n");
	printf(" - update\n");
	printf(" - remove id_migrate\n");
	printf(" - reset\n");
	printf(" - list\n");
}
