#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* directory_page(CHTTPConnection* con, CHTTPRequest* request) {
    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get == NULL)
        return not_found_page(con, request);

    CHTTPGetField* idField = chttp_get_request_parsed_find_field(get, "id");

    if(idField == NULL)
        return not_found_page(con, request);

    char* params[1] = {
        idField->fieldValue
    };

    PGresult *dirRes = ganyu_make_sql_request(con, 
    "SELECT * \
    FROM maja8801.VirtualDirectory AS VD \
    WHERE VD.ID = $1;", (const char**) params, 1);

    if(dirRes == NULL) {
        PQclear(dirRes);
        CHTTP_LOG(CHTTP_ERROR, "Failed to execute SQL request");
        return not_found_page(con, request);
    }

    int directoryRows = PQntuples(dirRes);
    int directoryCols = PQnfields(dirRes);

    if((directoryRows <= 0) || (directoryCols <= 0)) {
        CHTTP_LOG(CHTTP_ERROR, "Directory not found");
        return not_found_page(con, request);
    }

    char* directoryId = PQgetvalue(dirRes, 0, 0);
    char* directoryName = PQgetvalue(dirRes, 0, 1);
    char* directoryDescription = PQgetvalue(dirRes, 0, 2);

    printf("%s %s %s\n", directoryId, directoryName, directoryDescription);

    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Ganyu viewer");
        }
        BODY("") {
            DIV("style='height:15%'") {
                H1("Ganyu viewer");
                navigation_element(HTML_STREAM);
            }

            H2("Directory📂 %s overview", directoryName);
            HR("style='border-top: 2px solid #bbb;'");

            DIV("style='width:100%; display:flex;'") {
                DIV("style='width:33%; margin-right:7px; border-right: 1px solid lightgray;'") {
                    H3("Directory meta data");

                    TABLE("") {
                        TR("") {
                            TD("") { B("Directory ID"); }
                            TD("") { STRING("%s", directoryId); }
                        }

                        TR("") {
                            TD("") { B("Directory name"); }
                            TD("") { STRING("%s", directoryName); }
                        }
                        
                        TR("") {
                            TD("") { B("Directory description"); }
                            TD("") { STRING("%s", directoryDescription); }
                        }
                    }
                }
            }
        }
    } 

    PQclear(dirRes);

    chttp_free_get_request_parsed(get);

    char* string = HTML_COMPILE();
    return chttp_ok_response_flag(HTTP_1_1, string, CHTTP_FREE_MESSAGE);
}