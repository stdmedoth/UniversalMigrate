#ifndef __U_M_CONF__

#define __U_M_CONF__
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

#include <paths.h>

static struct _server_confs{
  char *server_endereco;
  char *server_user;
  char *server_senha;
  char *server_database;
  int conectado;
}server_confs;


int rec_vars_from_file();

xmlNodePtr get_tag_by_namepath(xmlDoc *doc, char *namepath);

#endif