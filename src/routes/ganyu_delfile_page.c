#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* delfile_page(CHTTPConnection* con, CHTTPRequest* request) {
    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Get request body is empty, returning not found page");
        return not_found_page(con, request);
    }

    CHTTPGetField* idField = chttp_get_request_parsed_find_field(get, "id");

    if(idField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Id field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }

    char* params[1] = { idField->fieldValue };

    PGresult *res = ganyu_make_sql_request(con, 
    "DELETE FROM ganyu.VirtualFile AS VF \
    WHERE VF.ID = $1;", (const char**) params, 1);

    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Ganyu viewer");
        }
        BODY("") {
            DIV("style='width:100%; height:100%; display:grid; place-items: center'") {
                DIV("style='text-align: center;'") {
                    H1("🎉 File successfully deleted 🎉");
                    A("href='/files' style='margin-right: 7px;'") { B("Ok 🌚"); }
                    A("href='/files' style='margin-left: 7px;'") { B("Thank you ! 💖"); }
                }
            }
        }
    } 

    PQclear(res);
    chttp_free_get_request_parsed(get);

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}