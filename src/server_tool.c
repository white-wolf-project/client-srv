#define _GNU_SOURCE
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>

#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <include/server_tool.h>
#include <include/ssl.h>

SSL_CTX *ctx;
int tcp_server(const char* service_port)
{
	int err;
	int sock_server;
	int sock;
	struct addrinfo  hints;
	struct addrinfo *results;

	struct sockaddr_in* addr_in;
	socklen_t length = 0;
	char hostname [NI_MAXHOST];
	char servname [NI_MAXSERV];


	SSL *ssl;
	SSL_library_init();
	ctx = InitServerCTX();

	// better not hardcode
	LoadCertificates(ctx, "newreq.pem", "newreq.pem");
	// Create srv socket & attribute service number 
	if ((sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket");
		return -1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((err = getaddrinfo(NULL, service_port,  &hints, &results)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		close(sock_server);
		return -2;
	}

	err = bind(sock_server, results->ai_addr, results->ai_addrlen);
	freeaddrinfo(results);
	if (err < 0) {
		perror("bind");
		close(sock_server);
		return -3;
	}

	length = sizeof(struct sockaddr_in);
	if (getsockname(sock_server, (struct sockaddr *) &addr_in, &length) < 0) {
		perror ("getsockname");
		return -4;
	}
	if (getnameinfo((struct sockaddr *) &addr_in, length, hostname, NI_MAXHOST, servname, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV) == 0){
		#ifdef DEBUG
		fprintf (stdout, "IP = %s, Port = %s \n", hostname, servname);
		#endif
	}
	listen(sock_server, 20);

	while (!leave_srv()) {
		SSL *ssl;
		sock = accept(sock_server, NULL, NULL);

		/* get new SSL state with context */
		ssl = SSL_new(ctx);

		/* set connection socket to SSL state */
		SSL_set_fd(ssl, sock);

		if (sock < 0) {
			perror("accept");
			return -1;
		}
		switch (fork()) {
			case 0 : // son
				close(sock_server);
				manage_co(sock, ssl);
				exit(EXIT_SUCCESS);
			case -1 :
				perror("fork");
				return -1;
			default : // father
				close(sock);
		}
	}
	return 0;
}

int leave_srv(void)
{
	return 0;
}

void manage_co(int sock, SSL* ssl)
{
	struct sockaddr * sockaddr;
	socklen_t length = 0;
	char hostname [NI_MAXHOST];
	char port [NI_MAXSERV];
	char buffer[256];
	int  buf_len;
	int sd;

	getpeername(sock, NULL, &length);
	if (length == 0)
		return;
	sockaddr = malloc(length);
	if (getpeername(sock, sockaddr, & length) < 0) {
		perror ("getpeername");
		free(sockaddr);
		return;
	}

	FILE *fp = fopen("server.log" ,"a+"); //w+
	if (getnameinfo(sockaddr, length, hostname, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
		sprintf (buffer, "IP:%s\tPort: %s\n", hostname, port);
		fprintf(stdout, "%s\n", buffer);
		fprintf(fp, "%s\n", buffer);
	}
	free(sockaddr);

	/* FIN */
	while (1) {


		/* do SSL-protocol accept */
		if (SSL_accept(ssl) == -1)
			ERR_print_errors_fp(stderr);
		else {
			buf_len = SSL_read(ssl, buffer, 256);

			if (buf_len < 0) {
				perror("read");
				exit(EXIT_SUCCESS);
			}
			if (buf_len == 0)
				break;
			printf("%s\r\n",buffer);
			fprintf(fp, "%s\n", buffer);

			memset(buffer, 0, 256);
			buffer[buf_len] = '\0';
		}
	}
	fclose(fp);
	close(sock);
}
