#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* upfile_page(CHTTPConnection* con, CHTTPRequest* request) {
    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Get request body is empty, returning not found page");
        return not_found_page(con, request);
    }

    CHTTPGetField* nameField = chttp_get_request_parsed_find_field(get, "name");
    CHTTPGetField* extField = chttp_get_request_parsed_find_field(get, "ext");
    CHTTPGetField* sizeField = chttp_get_request_parsed_find_field(get, "size");
    CHTTPGetField* hashField = chttp_get_request_parsed_find_field(get, "hash");
    CHTTPGetField* srcField = chttp_get_request_parsed_find_field(get, "src");
    CHTTPGetField* accessField = chttp_get_request_parsed_find_field(get, "access");

    if(nameField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Name field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }

    if(extField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Ext field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }
    
    if(sizeField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Size field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }

    if(hashField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Hash field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }

    if(srcField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Source field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }

    if(accessField == NULL) {
        GANYU_LOG(CHTTP_WARNING, "Source access field is not present in get request, returning not found page");
        return not_found_page(con, request);
    }

    char* params[6] = { 
        nameField->fieldValue,
        extField->fieldValue,
        sizeField->fieldValue,
        hashField->fieldValue,
        srcField->fieldValue,
        accessField->fieldValue
    };
    
    PGresult *res = ganyu_make_sql_request(con, 
    "INSERT INTO maja8801.VirtualFile(FileName, FileExtension, FileSize, ContentHash, SourceID, SourceAccess) \
    VALUES \
        ($1, $2, $3, $4, $5, $6);", (const char**) params, 6);

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
                        H1("🎉 Successfully created new file 🎉");
                        A("href='/upload' style='margin-right: 7px;'") { B("Ok 🌚"); }
                        A("href='/upload' style='margin-left: 7px;'") { B("Thank you ! 💖"); }
                    }
                } else {
                    DIV("style='text-align: center;'") {
                        H1("☠️ Failed to create new file 😵‍💫");
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