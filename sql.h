#ifndef __U_M_SQL__

#define __U_M_SQL__


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

#include <conf.h>

static MYSQL conectar;

int conectar_mysql();

#endif