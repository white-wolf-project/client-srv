#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/err.h>

SSL_CTX* InitServerCTX(void);
SSL_CTX* InitClientCTX(void);
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile);
void ShowCerts(SSL* ssl);
//void Servlet(SSL* ssl);	/* Serve the connection -- threadable */
