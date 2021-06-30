#include <conf.h>

int rec_vars_from_file(){

	xmlDocPtr doc = xmlParseFile(SERVER_CONF);

	if(!doc){
		printf("Não foi possível ler arquivo de configuração");
		return 1;
	}

	xmlNodePtr endereco_tag = get_tag_by_namepath(doc,"/server_conf/address");
	xmlNodePtr user_tag = get_tag_by_namepath(doc,"/server_conf/username");
	xmlNodePtr senha_tag = get_tag_by_namepath(doc,"/server_conf/password");
	xmlNodePtr database_tag = get_tag_by_namepath(doc,"/server_conf/database");

	if(!endereco_tag || !xmlNodeGetContent(endereco_tag)){
		printf("Não foi possível receber endereço do server\n");
		return 1;
	}
	if(!user_tag || !xmlNodeGetContent(user_tag)){
		printf("Não foi possível receber usuario do server\n");
		return 1;
	}
	if(!senha_tag || !xmlNodeGetContent(senha_tag)){
		printf("Não foi possível receber senha do server\n");
		return 1;
	}
	if(!database_tag || !xmlNodeGetContent(database_tag)){
		printf("Não foi possível receber banco de dados do server\n");
		return 1;
	}

	server_confs.server_endereco = strdup((char*)xmlNodeGetContent(endereco_tag));
	server_confs.server_user = strdup((char*)xmlNodeGetContent(user_tag));
	server_confs.server_senha = strdup((char*)xmlNodeGetContent(senha_tag));
	server_confs.server_database = strdup((char*)xmlNodeGetContent(database_tag));
	return 0;
}


xmlNodePtr get_tag_by_namepath(xmlDoc *doc, char *namepath){
	xmlXPathContextPtr contxt = xmlXPathNewContext(doc);
	xmlXPathObjectPtr node_contxt= xmlXPathEval((xmlChar*)namepath,contxt);

	xmlNodePtr node=NULL;
	if(node_contxt &&
		node_contxt->nodesetval &&
		node_contxt->nodesetval->nodeNr &&
		node_contxt->nodesetval->nodeTab){

		node = node_contxt->nodesetval->nodeTab[0];
	}
	return node;
}