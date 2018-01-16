#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <include/client_tool.h>
#include <libxml/parser.h>

int main (int argc, char *argv[])
{
	int sock;
	char *xmlfile;

	if (argc == 2)
		xmlfile = argv[1];
	else
		xmlfile = "config.xml";

	if (xmlfile == NULL || access(xmlfile, F_OK) == -1)
	{
		fprintf(stderr, "%s : %s\n", xmlfile, strerror(errno));
		return -1;
	}
	else {
		xmlDoc *doc = NULL;
		xmlNode *root_element = NULL;
		doc = xmlReadFile(xmlfile, NULL, 0);

		if (doc == NULL) {
			printf("Could not parse the XML file");
		}

		root_element = xmlDocGetRootElement(doc);
		print_xml(root_element, 1);
		xmlFreeDoc(doc);
		xmlCleanupParser();
		printf("%s\n", ipaddr);
		printf("%s\n", port);
		printf("%s\n", data);
	}

	if (init_client(0,ipaddr, port, &results) < 0){
		exit(EXIT_FAILURE);
	}
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if (connect(sock, results->ai_addr, results->ai_addrlen) < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(results);
	send_data(sock, data);
	close(sock);
	return 0;
}

