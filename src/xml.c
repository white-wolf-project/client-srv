#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <libxml/parser.h>
#include <include/xml.h>
#include <include/client_tool.h>

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
						iface = is_leaf(node)?xmlNodeGetContent(node):xmlGetProp(node, "id");
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


int parse_config_file(const char *xmlfile){

	if (xmlfile == NULL || access(xmlfile, F_OK) == -1)
	{
		fprintf(stderr, "%s : %s\n", xmlfile, strerror(errno));
		return -1;
	} else {
		xmlDoc *doc = NULL;
		xmlNode *root_element = NULL;
		doc = xmlReadFile(xmlfile, NULL, 0);

		if (doc == NULL) {
			printf("Could not parse the XML file");
			return -2;
		}

		root_element = xmlDocGetRootElement(doc);
		print_xml(root_element, 1);
		xmlFreeDoc(doc);
		xmlCleanupParser();
		/*debug("%s\n", ipaddr);
		debug("%s\n", port);*/
	}

	return 0;
}