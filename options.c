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
		"version",
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
	printf("./migrate <opcao> [--verbose]:\n");
	printf("Opção desconhecida, lista:\n");
	printf(" - new\n");
	printf(" - update\n");
	printf(" - remove id_migrate\n");
	printf(" - reset\n");
	printf(" - list\n");
	printf(" - version\n");
}
