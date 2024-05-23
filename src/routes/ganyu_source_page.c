#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* source_page(CHTTPConnection* con, CHTTPRequest* request) {
    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get == NULL)
        return not_found_page(con, request);

    CHTTPGetField* idField = chttp_get_request_parsed_find_field(get, "id");

    if(idField == NULL)
        return not_found_page(con, request);

    char* params[1] = {
        idField->fieldValue
    };

    PGresult *sourceRes = ganyu_make_sql_request(con, 
    "SELECT * \
    FROM maja8801.Source AS S \
    WHERE S.ID = $1;", (const char**) params, 1);

    if(sourceRes == NULL) {
        CHTTP_LOG(CHTTP_ERROR, "Failed to execute SQL request");
        return not_found_page(con, request);
    }

    int sourceRows = PQntuples(sourceRes);
    int sourceCols = PQnfields(sourceRes);

    if((sourceRows <= 0) || (sourceCols <= 0)) {
        PQclear(sourceRes);
        CHTTP_LOG(CHTTP_ERROR, "Source not found");
        return not_found_page(con, request);
    }

    char* sourceId = PQgetvalue(sourceRes, 0, 0);
    char* sourceName = PQgetvalue(sourceRes, 0, 1);
    char* sourceDescription = PQgetvalue(sourceRes, 0, 2);
    char* sourceType = PQgetvalue(sourceRes, 0, 3);
    char* sourceRoodDestination = PQgetvalue(sourceRes, 0, 4);

    printf("%s %s %s %s %s\n", sourceId, sourceName, sourceDescription, sourceType, sourceRoodDestination);

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

            H2("Source %s %s overview", ganyu_source_type_to_icon(sourceType), sourceName);
            HR("style='border-top: 2px solid #bbb;'");

            DIV("style='width:100%; display:flex;'") {
                DIV("style='width:33%; margin-right:7px; border-right: 1px solid lightgray;'") {
                    H3("Source meta data");

                    TABLE("") {
                        TR("") {
                            TD("") { B("Source ID"); }
                            TD("") { STRING("%s", sourceId); }
                        }

                        TR("") {
                            TD("") { B("Source name"); }
                            TD("") { STRING("%s", sourceName); }
                        }
                        
                        TR("") {
                            TD("") { B("Source description"); }
                            TD("") { STRING("%s", sourceDescription); }
                        }

                        TR("") {
                            TD("") { B("Source type"); }
                            TD("") { STRING("%s %s", ganyu_source_type_to_icon(sourceType), sourceType); }
                        }

                        TR("") {
                            TD("") { B("Source root destination"); }
                            TD("") { STRING("%s", sourceRoodDestination); }
                        }
                    }
                }
            }
        }
    } 

    PQclear(sourceRes);

    chttp_free_get_request_parsed(get);

    char* string = HTML_COMPILE();
    return chttp_ok_response_flag(HTTP_1_1, string, CHTTP_FREE_MESSAGE);
}