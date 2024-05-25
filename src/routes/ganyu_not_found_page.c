#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* not_found_page(CHTTPConnection* con, CHTTPRequest* request) {
    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Not found");
        }
        BODY("") {
            DIV("style='width:100%; height:100%; display:grid; place-items: center'") {
                H1("🤬 Requested resource not found 🥵");
            }
        }
    } 

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}
