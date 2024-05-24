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

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}
