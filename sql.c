#include <sql.h>

int conectar_mysql(){
	if(!mysql_init(&conectar)){
		printf("Não foi possível conectar\n");
		return 1;
	}
	unsigned int timeout = 60 * 60 * 24;
	int reconnect = 1;
	mysql_options(&conectar, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
	mysql_options(&conectar, MYSQL_OPT_RECONNECT, &reconnect);

	if(!mysql_real_connect(&conectar, server_confs.server_endereco, server_confs.server_user, server_confs.server_senha, server_confs.server_database, 0,NULL,0)){
		printf("Não foi possível conectar ao servidor\n");
		return 1;
	}

	char *character = strdup("utf8");
	char *msg = malloc(strlen(character) + 200);
	sprintf(msg, "setando caracete %s", character);
	if (mysql_set_character_set(&conectar, character)){
		printf("Não foi possível setar characteres UTF8\n");
	}
	server_confs.conectado = 1;

	return 0;
}
