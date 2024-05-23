#include "ganyu_app.h"

CHTTPResponse* not_found_page(CHTTPConnection* con, CHTTPRequest* request) {
    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Not found");
        }
        BODY("") {
            H1("🤬 Requested resource not found 🥵");
        }
    } 

    char* string = HTML_COMPILE();
    return chttp_ok_response_flag(HTTP_1_1, string, CHTTP_FREE_MESSAGE);
}
