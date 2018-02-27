#include <openssl/ssl.h>

#define LG_BUFFER	1024

struct addrinfo *results;
extern char  *ipaddr, *port, *iface;

int init_client (int server, char *host, char *port, struct addrinfo **result);
int exec_bin(SSL *ssl, const char* bin2exec);
