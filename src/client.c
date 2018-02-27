#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <libxml/parser.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <include/client_tool.h>
#include <include/xml.h>
#include <include/ssl.h>

void usage(int argc, char *argv[]){
	printf("usage : %s -i [ip] -p [port] -c <cmd>\n",argv[0]);
	printf("-i, --ip [ip]\t\t set ip server\n");
	printf("-p, --port [port]\t set port server\n");
	printf("-c, --cmd [cmd]\t\t set command to run\n");
	printf("-x, --xml [xmlfile]\t set parameters in xml file\n");
	printf("-h, --help\t\t print help\n");
}

int main (int argc, char *argv[])
{
	int sock;
	int i, xml = 0;
	SSL_CTX *ctx;
	SSL *ssl;

	char *xmlfile = NULL;
	if (argc > 6 || argc <= 2)
	{
		usage(argc, argv);
		return 0;
	}

	for(i = 0; i < argc; i++) {
		if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--ip")) {
			ipaddr = argv[i + 1];
		} else if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")) {
			port = argv[i + 1];
		} else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--cmd")) {
			iface = argv[i + 1];
		} else if (!strcmp(argv[i], "-x") || !strcmp(argv[i], "--xml"))
		{
			xml = 1;
			xmlfile = argv[i+1];
		} else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			usage(argc, argv);
			return 0;
		}
	}
	if (xml)
	{
		parse_config_file(xmlfile);
		printf("%s\n", ipaddr);
		printf("%s\n", port);
		printf("%s\n", iface);
	}

	SSL_library_init();
	ctx = InitClientCTX();
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
	/* create new SSL connection state */
	ssl = SSL_new(ctx);

	/* attach the socket descriptor */
	SSL_set_fd(ssl, sock);

	/* perform the connection */
	if (SSL_connect(ssl) == -1 )
		ERR_print_errors_fp(stderr);
	else {
		printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
		ShowCerts(ssl);
		freeaddrinfo(results);
		exec_bin(ssl, iface);
	}
	close(sock);
	SSL_CTX_free(ctx);
	return 0;
}