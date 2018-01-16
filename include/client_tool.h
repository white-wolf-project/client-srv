#include <libxml/parser.h>

#define LG_BUFFER	1024

struct addrinfo *results;
char  *ipaddr, *port, *data;

int init_client (int server, char *host, char *port, struct addrinfo **result);
int send_data(int sock2server, const char* data2send);
int is_leaf(xmlNode * node);
void print_xml(xmlNode * node, int indent_len);