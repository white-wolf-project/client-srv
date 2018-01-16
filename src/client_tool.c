#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <libxml/parser.h>
#include <include/client_tool.h>

int init_client (int server, char *host, char *port, struct addrinfo **results)
{
	int    err;	
	struct addrinfo  hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (server)
		hints.ai_flags = AI_PASSIVE;
	if ((err = getaddrinfo(host, port, &hints, results)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		return -1;
	}
	return 0;
}

int send_data(int sock2server, const char* data2send){

	if (write(sock2server, data2send, strlen(data2send)) < 0) {
		perror("write");
		return -1;
	}
	return 0;
}


int is_leaf(xmlNode * node)
{
	xmlNode * child = node->children;
	while(child)
	{
		if(child->type == XML_ELEMENT_NODE) return 0;
		child = child->next;
	}
	return 1;
}

void print_xml(xmlNode * node, int indent_len)
{
	int i = 0;
	while(node)
	{
		if(node->type == XML_ELEMENT_NODE)
		{
			if (is_leaf(node)?xmlNodeGetContent(node):xmlGetProp(node, "id"))
			{
				switch(i) {
					case 0:
						ipaddr = is_leaf(node)?xmlNodeGetContent(node):xmlGetProp(node, "id");
						break;
					case 1:
						port = (is_leaf(node)?xmlNodeGetContent(node):xmlGetProp(node, "id"));
						break;
					case 2:
						data = is_leaf(node)?xmlNodeGetContent(node):xmlGetProp(node, "id");
						break;
				}
				/* you can print all values here if i == 2, usefull for debug, I keep it here*/
				/*if (i == 2)
				{
					printf("%s\n", ipaddr);
					printf("%s\n", port);
					printf("%s\n", bin2run);
				}*/
			} i++;
		}
		print_xml(node->children, indent_len + 1);
		node = node->next;
    }
}