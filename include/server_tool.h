#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

char  *ipaddr, *port;
extern SSL_CTX *ctx;
int tcp_server(const char* service_port);
int  leave_srv(void);
void manage_co(int sock, SSL* ssl);