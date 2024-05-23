#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* index_page(CHTTPConnection* con, CHTTPRequest* request) {
    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Ganyu viewer");
        }
        BODY("") {
            H1("Ganyu viewer");
            navigation_element(HTML_STREAM);

            P("Hello world !");
            P("This is a test article, number %d", 1);
        }
    } 

    char* string = HTML_COMPILE();
    return chttp_ok_response_flag(HTTP_1_1, string, CHTTP_FREE_MESSAGE);
}
