#ifndef GANYU_PAGES_H
#define GANYU_PAGES_H

#include "chtml.h"
#include "chttp/chttp.h"

CHTTPResponse* index_page(CHTTPConnection* con, CHTTPRequest* request);
CHTTPResponse* files_page(CHTTPConnection* con, CHTTPRequest* request);
CHTTPResponse* file_page(CHTTPConnection* con, CHTTPRequest* request);
CHTTPResponse* directories_page(CHTTPConnection* con, CHTTPRequest* request);
CHTTPResponse* sources_page(CHTTPConnection* con, CHTTPRequest* request);

CHTTPResponse* not_found_page(CHTTPConnection* con, CHTTPRequest* request);

void navigation_element(FILE* HTML_STREAM);

char* ganyu_file_extension_to_icon(char* ext);

#endif