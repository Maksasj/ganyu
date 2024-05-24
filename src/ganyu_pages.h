#ifndef GANYU_PAGES_H
#define GANYU_PAGES_H

#include <libpq-fe.h>

#include "chtml.h"
#include "chttp/chttp.h"

CHTTPResponse* index_page(CHTTPConnection* con, CHTTPRequest* request);
CHTTPResponse* files_page(CHTTPConnection* con, CHTTPRequest* request);

CHTTPResponse* file_page(CHTTPConnection* con, CHTTPRequest* request);
CHTTPResponse* directory_page(CHTTPConnection* con, CHTTPRequest* request);
CHTTPResponse* source_page(CHTTPConnection* con, CHTTPRequest* request);

CHTTPResponse* directories_page(CHTTPConnection* con, CHTTPRequest* request);
CHTTPResponse* sources_page(CHTTPConnection* con, CHTTPRequest* request);

CHTTPResponse* not_found_page(CHTTPConnection* con, CHTTPRequest* request);

void navigation_element(FILE* HTML_STREAM);

char* ganyu_file_extension_to_icon(char* ext);
char* ganyu_source_type_to_icon(char* ext);
PGresult* ganyu_make_sql_request(CHTTPConnection* con, const char* query, const char** params, unsigned int count);

#endif