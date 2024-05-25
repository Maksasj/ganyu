#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* updir_page(CHTTPConnection* con, CHTTPRequest* request) {
    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Get request body is empty, returning not found page");
        return not_found_page(con, request);
    }

    CHTTPGetField* nameField = chttp_get_request_parsed_find_field(get, "name");
    CHTTPGetField* descriptionField = chttp_get_request_parsed_find_field(get, "description");

    if(nameField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Name field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }

    if(descriptionField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Description field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }

    char* params[2] = { 
        nameField->fieldValue,
        descriptionField->fieldValue
    };
    
    PGresult *res = ganyu_make_sql_request(con, 
    "INSERT INTO maja8801.VirtualDirectory(directoryName, directoryDescription) \
    VALUES \
        ($1, $2);", (const char**) params, 2);

    if(res == NULL) {
        GANYU_LOG(CHTTP_ERROR, "Failed to upload file");
        return not_found_page(con, request);
    }

    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Ganyu viewer");
        }
        BODY("") {
            DIV("style='width:100%; height:100%; display:grid; place-items: center'") {
                if(res != NULL) {
                    DIV("style='text-align: center;'") {
                        H1("🎉 Successfully created new directory 🎉");
                        A("href='/upload' style='margin-right: 7px;'") { B("Ok 🌚"); }
                        A("href='/upload' style='margin-left: 7px;'") { B("Thank you ! 💖"); }
                    }
                } else {
                    DIV("style='text-align: center;'") {
                        H1("☠️ Failed to create new directory 😵‍💫");
                        A("href='/upload' style='margin-right: 7px;'") { B("I will try to do it again ! 👻"); }
                        A("href='/upload' style='margin-left: 7px;'") { B("Ok 👽"); }
                    }
                }
                
            }
        }
    } 

    if(res != NULL)
        PQclear(res);
   
    chttp_free_get_request_parsed(get);

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}