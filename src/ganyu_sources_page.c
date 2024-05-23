#include "ganyu_app.h"

CHTTPResponse* sources_page(CHTTPConnection* con, CHTTPRequest* request) {
    return chttp_ok_response_file(HTTP_1_1, "directory.html");
}
