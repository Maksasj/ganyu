#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* rmfilestore_page(CHTTPConnection* con, CHTTPRequest* request) {
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

    CHTTPGetField* targetField = chttp_get_request_parsed_find_field(get, "target");
    if(targetField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Target field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }

    char* params[2] = { idField->fieldValue, targetField->fieldValue };

    PGresult *res = ganyu_make_sql_request(con, 
    "DELETE FROM ganyu.VirtualFileStored AS VFS \
    WHERE (VFS.VirtualFileID = $1) AND (VFS.VirtualDirectoryID = $2);", (const char**) params, 2);

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
                    H1("🎉 Successfully removed file from directory 🎉");
                    A("href='/directory?id=%s' style='margin-right: 7px;'", targetField->fieldValue) { B("Ok 🌚"); }
                    A("href='/directory?id=%s' style='margin-left: 7px;'", targetField->fieldValue) { B("Thank you ! 💖"); }
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