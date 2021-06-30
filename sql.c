#include <sql.h>
#include <conf.h>

int conectar_mysql(server_confs *server_conf){

	if(!mysql_init(&server_conf->conectar)){
		printf("Não foi possível conectar\n");
		return 1;
	}
	unsigned int timeout = 60 * 60 * 24;
	int reconnect = 1;
	mysql_options(&server_conf->conectar, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
	mysql_options(&server_conf->conectar, MYSQL_OPT_RECONNECT, &reconnect);

	if(!mysql_real_connect(
		&server_conf->conectar, 
		server_conf->server_endereco, 
		server_conf->server_user, 
		server_conf->server_senha, 
		server_conf->server_database, 0,NULL,0)){
		printf("Não foi possível conectar ao servidor\n%s\n", mysql_error(&server_conf->conectar));
		
		printf("User: %s, Pass: %s, DB: %s\n", 
			server_conf->server_user, 
			server_conf->server_senha, 
			server_conf->server_database);
		return 1;
	}

	char *character = strdup("utf8");
	char *msg = malloc(strlen(character) + 200);
	sprintf(msg, "setando caracete %s", character);
	if (mysql_set_character_set(&server_conf->conectar, character)){
		printf("Não foi possível setar characteres UTF8\n");
	}
	server_conf->conectado = 1;

	return 0;
}
