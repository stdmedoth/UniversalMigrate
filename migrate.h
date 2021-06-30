MYSQL conectar;

static struct {
  char *server_endereco;
  char *server_user;
  char *server_senha;
  char *server_database;
  int conectado;
}server_confs;

int rec_vars_from_file();

int rodar_migrate(char *migrate_path);

void help_message();

xmlNodePtr get_tag_by_namepath(xmlDoc *doc, char *namepath);

int check_migrate_exists(int migrate_id);

int list_migrates();

#define APP_NAME "."
#define APP_DIRNAME  APP_NAME "/data"
#define SERVER_CONF      APP_DIRNAME "/server.xml"
#define MIGRATE_FOLDER   APP_DIRNAME "/migrates"
#define MAX_DATE_LEN 15