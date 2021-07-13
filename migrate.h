#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <libxml/xinclude.h>
#include <libxml/xpath.h>
#ifdef __linux__
#include <mysql/mysql.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <mysql/mysql.h>
#endif

int rodar_migrate(char *migrate_path, server_confs *server_conf);

int check_migrate_exists(int migrate_id, server_confs *server_conf);

int list_migrates();

int check_base_tables();

static server_confs *server_conf;

void set_is_verbose(int argc, char **argv, server_confs *server_conf);