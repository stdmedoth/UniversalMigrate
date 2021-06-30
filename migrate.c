#include <options.h>
#include <conf.h>
#include <sql.h>
#include <migrate.h>

char *get_version(server_confs *server_conf){
	MYSQL_RES *result;
	MYSQL_ROW row;

	if(!server_conf->conectado){
		if(conectar_mysql(server_conf)){
			printf("Não foi possível conectar\n");
			return NULL;
		}	
	}
	char query[50];
	sprintf(query, "SELECT versao FROM versao;");
	if((mysql_query(&server_conf->conectar,query))){
		printf("Não foi possível buscar versão\n");
		printf("Erro Mysql %s\n", mysql_error(&server_conf->conectar));
		return NULL;
	}
	if(!(result = mysql_store_result(&server_conf->conectar))){
		printf("Não foi possível buscar versao\n");
		printf("Erro Mysql %s\n", mysql_error(&server_conf->conectar));
		return NULL;
	}
	if(!(row = mysql_fetch_row(result))){
		printf("Versão não carregada no servidor\n");
		return NULL;
	}

	return row[0];
}


int check_base_tables(server_confs *server_conf){
	MYSQL_RES *result;
	MYSQL_ROW row;
	if(!server_conf->conectado){
		if(conectar_mysql(server_conf)){
			printf("Não foi possível conectar\n");
			return 1;
		}	
	}

	int query_len = 20, versao=0, migrate=0;
	char query[query_len];
	enum{
		CODE,
		DESCRICAO,
		ARQUIVO,
		VERSAO,
		LANCAMENTO
	};

	sprintf(query, "SHOW TABLES;");
	if((mysql_query(&server_conf->conectar,query))){
		printf("Não foi possível verificar tabelas\n");
		return 1;
	}
	if(!(result = mysql_store_result(&server_conf->conectar))){
		printf("Erro Mysql %s\n", mysql_error(&server_conf->conectar));
		return 1;
	}
	
	while((row = mysql_fetch_row(result))){
		if(!versao && !strcmp(row[0], "versao")){
			versao = 1;
		}
		if(!migrate && !strcmp(row[0], "migrate")){
			migrate = 1;
		}
	}

	if(!versao || !migrate){
		char *caminho_arquivo = malloc(strlen(MIGRATE_FOLDER) + 10 + 12 + 2);			
		sprintf(caminho_arquivo, "%s/migrate_00.sql",MIGRATE_FOLDER);
		if(rodar_migrate(caminho_arquivo, server_conf)){
			printf("Não foi possível executar migrate base\n");
			return 1;
		}	
	}	
	return 0;
}

int remove_migrate(int migrate_id, server_confs *server_conf){
	MYSQL_RES *result;
	MYSQL_ROW row;
	if(!server_conf->conectado){
		if(conectar_mysql(server_conf)){
			printf("Não foi possível conectar\n");
			return 1;
		}	
	}

	int query_len = 12 + 50;
	char query[query_len];
	enum{
		CODE,
		DESCRICAO,
		ARQUIVO,
		VERSAO,
		LANCAMENTO
	};

	sprintf(query, "SELECT * FROM migrate WHERE code = %i;", migrate_id);
	if((mysql_query(&server_conf->conectar,query))){
		printf("Não foi possível verificar migrate\n");
		return 1;
	}
	if(!(result = mysql_store_result(&server_conf->conectar))){
		printf("Erro Mysql %s\n", mysql_error(&server_conf->conectar));
		return 1;
	}
	if(!(row = mysql_fetch_row(result))){
		printf("Migrate não atualizado\n");
		return 1;	
	}
	
	const int path_len = strlen(MIGRATE_FOLDER) + 2 + 20;
	char path[path_len];
	printf("Atualizando %s\n", row[DESCRICAO]);
	sprintf(path, "%s/migrate_down_0%i.sql", MIGRATE_FOLDER, migrate_id);
	if(rodar_migrate(path, server_conf)){
		printf("Não foi possível rodar down do migrate %s\n", row[ARQUIVO]);
		return 1;
	}

	return 0;
}

int list_migrates(server_confs *server_conf){
	MYSQL_RES *result;
	MYSQL_ROW row;
	if(!server_conf->conectado){
		if(conectar_mysql(server_conf)){
			printf("Não foi possível conectar\n");
			return 1;
		}	
	}
	enum{
		CODE,
		DESCRICAO,
		ARQUIVO,
		VERSAO,
		LANCAMENTO
	};

	char query[50];
	sprintf(query, "SELECT * FROM migrate;");
	if((mysql_query(&server_conf->conectar,query))){
		printf("Não foi possível listar migrate\n");
		printf("Erro Mysql %s\n", mysql_error(&server_conf->conectar));
		return 1;
	}
	if(!(result = mysql_store_result(&server_conf->conectar))){
		printf("Não foi possível listar migrate\n");
		printf("Erro Mysql %s\n", mysql_error(&server_conf->conectar));
		return 1;
	}
	if(!mysql_num_rows(result)){
		printf("Não há migrates carregados no server\n");
		return 1;
	}
	while((row = mysql_fetch_row(result))){
		printf("%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
			row[CODE],
			row[DESCRICAO],
			row[ARQUIVO],
			row[VERSAO],
			row[LANCAMENTO]);
		
	}
	return 0;
}

int reset_migrates(server_confs *server_conf){

	MYSQL_RES *result;
	MYSQL_ROW row;
	if(!server_conf->conectado){
		if(conectar_mysql(server_conf)){
			printf("Não foi possível conectar\n");
			return 1;
		}	
	}
	enum{
		CODE,
		DESCRICAO,
		ARQUIVO,
		VERSAO,
		LANCAMENTO
	};

	char query[50];
	sprintf(query, "SELECT * FROM migrate order by lancamento asc;");
	if((mysql_query(&server_conf->conectar,query))){
		printf("Não foi possível listar migrate\n");
		printf("Erro Mysql %s\n", mysql_error(&server_conf->conectar));
		return 1;
	}
	if(!(result = mysql_store_result(&server_conf->conectar))){
		printf("Não foi possível listar migrate\n");
		printf("Erro Mysql %s\n", mysql_error(&server_conf->conectar));
		return 1;
	}
	
	while((row = mysql_fetch_row(result))){
		if(remove_migrate(atoi(row[CODE]), server_conf))
			return 1;
	}
	return 0;
}

int check_migrate_exists(int migrate_id, server_confs *server_conf){
	MYSQL_RES *result;
	MYSQL_ROW row;
	if(!server_conf->conectado){
		if(conectar_mysql(server_conf)){
			printf("Não foi possível conectar\n");
			return -1;
		}	
	}

	int query_len = 12 + 50;
	char query[query_len];
	sprintf(query, "SELECT * FROM migrate WHERE code = %i;", migrate_id);
	if((mysql_query(&server_conf->conectar,query))){
		printf("Não foi possível verificar migrate\n");
		return -1;
	}
	if(!(result = mysql_store_result(&server_conf->conectar))){
		printf("Erro Mysql %s\n", mysql_error(&server_conf->conectar));
		return -1;
	}
	if((row = mysql_fetch_row(result)))
		return 1;

	return 0;
}

int rodar_migrate(char *migrate_path, server_confs *server_conf){

	FILE *fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int line_pos = 0;
	if(!server_conf->conectado){
		if(conectar_mysql(server_conf)){
			printf("Não foi possível conectar\n");
			return 1;
		}	
	}
	
	fp = fopen(migrate_path, "r");
	if(!fp){
		printf("Nâo foi possível abrir arquivo migrate: %s\n", migrate_path);
		return 1;
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		if((mysql_query(&server_conf->conectar,line))){
			int error_code = mysql_errno(&server_conf->conectar);
			switch(error_code){
				case 1050: 
				printf("Aviso: %s\n", mysql_error(&server_conf->conectar));
				break;
				case 1062: 
				printf("Aviso: %s\n", mysql_error(&server_conf->conectar));
				break;
				case 1065: 
	    			//empty query
				break;
				case 1060:
				printf("Aviso: %s\n", mysql_error(&server_conf->conectar));
				break;
				default:
				printf("Erro Mysql %i na linha %i %s\n", error_code, line_pos, mysql_error(&server_conf->conectar));
				printf("%s\n", line);
				return 1;	
			}

			
		}
		line_pos++;
	}

	return 0;
}

int main(int argc, char **argv){
	if(argc < 2){
		help_message();
		return 1;
	}
	
	server_conf = malloc(sizeof(server_confs));
	int option = get_options(argv[1]);
	if(rec_vars_from_file(server_conf)){
		printf("Não foi possível receber informações de configuração\n");
		return 1;
	}

	if(check_base_tables(server_conf))
		return 1;

	switch(option){
		case 1: 

		printf("Criando migrate...\n");
		FILE *fp, *fp_down;
		int numero = 0;
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		char *nome_arquivo_base = "migrate_0";
		char *nome_arquivo_basedown = "migrate_down_0";

		char *nome_arquivo = malloc(strlen(nome_arquivo_base) + 12 + 5);
		char *nome_arquivo_down = malloc(strlen(nome_arquivo_basedown) + 12 + 5);

		char *caminho_arquivo = malloc(strlen(MIGRATE_FOLDER) + strlen(nome_arquivo) + 12 + 2);			
		char *caminho_arquivo_down = malloc(strlen(MIGRATE_FOLDER) + strlen(nome_arquivo_down) + 12 + 2);			

		sprintf(nome_arquivo, "%s%i.sql", nome_arquivo_base, numero);
		sprintf(caminho_arquivo, "%s/%s",MIGRATE_FOLDER, nome_arquivo);

		while( fopen(caminho_arquivo, "r") ){
			numero++;
			
			printf("Arquivo %s já existente\n", nome_arquivo);
			sprintf(nome_arquivo, "%s%i.sql", nome_arquivo_base, numero);
			sprintf(caminho_arquivo, "%s/%s",MIGRATE_FOLDER, nome_arquivo);

			sprintf(nome_arquivo_down, "%s%i.sql", nome_arquivo_basedown, numero);
			sprintf(caminho_arquivo_down, "%s/%s",MIGRATE_FOLDER, nome_arquivo_down);

		}

		printf("Gerando migrate %s...\n", nome_arquivo);
		fp = fopen(caminho_arquivo, "w");
		fp_down = fopen(caminho_arquivo_down, "w");
		if(!fp){
			printf("Não foi possível abrir arquivo migrate %s\n", caminho_arquivo);
			return 1;
		}
		if(!fp_down){
			printf("Não foi possível abrir arquivo down %s\n", caminho_arquivo_down);
			return 1;
		}
		char *dt_lancamento = malloc(MAX_DATE_LEN);
		sprintf(dt_lancamento, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
		fprintf(fp, "-- migrate made with UniversalMigrate script  \n\n\n");
		fprintf(fp, "update versao set versao = 'x.x', lancamento = '%s';\n", dt_lancamento);
		fprintf(fp, "insert into migrate values(%i, 'Versao x.x', '%s', 'x.x', '%s');", numero, nome_arquivo, dt_lancamento);
		fclose(fp);

		fprintf(fp_down, "-- downgrade template made with UniversalMigrate script  \n\n\n");
		fprintf(fp_down, "update versao set versao = 'x.x', lancamento = '%s';\n", dt_lancamento);
		fprintf(fp_down, "DELETE FROM migrate WHERE code = %i;", numero);
		fclose(fp_down);
		printf("Migrate criado com sucesso\n");
		break;
		case 2: 

		printf("Iniciando atualização do migrate...\n");

		struct dirent **namelist;
		    //FILE    *migrate_file;
		int n, i=0;

		n = scandir(MIGRATE_FOLDER, &namelist, NULL, alphasort);
		if (n < 0){
			perror("scandir");
		}else {
			while (i<n) {
				if(strstr(namelist[i]->d_name, "down")){
					i++;
					continue;
				}
				int ignore_files_pos=0;
				char *ignore_files[] = {
					".",
					"..",
					NULL
				};
				while(ignore_files[ignore_files_pos]){
					if(!strcmp(namelist[i]->d_name, ignore_files[ignore_files_pos])){
						i++;
						continue;
					}	
					ignore_files_pos++;
				}

				const int path_len = strlen(MIGRATE_FOLDER) + 2 + strlen(namelist[i]->d_name);
				char path[path_len];
				printf("Atualizando %s\n", namelist[i]->d_name);
				sprintf(path, "%s/%s", MIGRATE_FOLDER, namelist[i]->d_name);
				int migrate_id = 0;
				if(sscanf(namelist[i]->d_name,"migrate_0%i.sql", &migrate_id)!=1){
					printf("Arquivo migrate %s não identificado\n", namelist[i]->d_name);
					return 1;
				}
				int migrate_exists = check_migrate_exists(migrate_id, server_conf);
				if(migrate_exists == -1){
					printf("Erro ao verificar migrate");
					return 1;
				}
				if(!migrate_exists){
					if(rodar_migrate(path, server_conf))
						return 1;	
				}else{
					printf("Migrate %s já atualizado!\n", namelist[i]->d_name);
				}

				free(namelist[i]);
				i++;
			}
			free(namelist);
		}

		break;		
		case 3: 
		printf("Removendo migrate...\n");	
		if(argc < 3){
			help_message();
			return 1;
		}
		int migrate = atoi(argv[2]);
		if(remove_migrate(migrate, server_conf))
			return 1;
		printf("Migrate %i removido\n", migrate);
		break;		
		case 4: 
		printf("Resetando migrates...\n");			
		reset_migrates(server_conf);
		break;
		case 5: 
		printf("Listagem de migrates...\n");			
		list_migrates(server_conf);
		break;
		case 6:
		printf("Buscando versão...\n");
		char *versao = get_version(server_conf);
		if(!versao)
			return 1;

		printf("Versão app: %s\n", versao);
		break;
		default:
		help_message();
		break;		
	}
	return 0;
}