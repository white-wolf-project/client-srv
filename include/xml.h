#include <libxml/parser.h>

int is_leaf(xmlNode * node);
void print_xml(xmlNode * node, int indent_len);
int parse_config_file(const char *xmlfile);