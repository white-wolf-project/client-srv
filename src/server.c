#define _GNU_SOURCE
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <include/server_tool.h>
#include <include/xml.h>

void usage(int argc, char* argv[]){
	printf("usage : %s [port]\n", argv[0]);
}

int main (int argc, char* argv[])
{
	char *xmlfile = NULL;
	if (argc == 3 && !strcmp(argv[1], "-x")){
		xmlfile = argv[2];
		parse_config_file(xmlfile);
		printf("%s\n", ipaddr);
		printf("%s\n", port);
		return 0;
	} else {
		if (argc == 1) {
			port = "50683";
		} else if (argc == 2) {
			port = argv[1];
		} else {
			usage(argc,argv);
			return -1;
		}
	}
	tcp_server(port);
	return 0;
}
