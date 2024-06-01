#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* sources_page(CHTTPConnection* con, CHTTPRequest* request) {
    char* start = "0";
    char* end = "100";
    char* name = "%";

    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get != NULL) {
        CHTTPGetField* startField = chttp_get_request_parsed_find_field(get, "start");
        CHTTPGetField* endField = chttp_get_request_parsed_find_field(get, "end");

        if((startField != NULL) || endField != NULL) {
            start = startField->fieldValue;
            end = endField->fieldValue;
        }

        CHTTPGetField* nameField = chttp_get_request_parsed_find_field(get, "name");

        if(nameField != NULL)
            name = nameField->fieldValue;
    }
    
    char* params[3] = { start, end, name };

    PGresult *sourceRes = ganyu_make_sql_request(con, 
        "SELECT * \
        FROM ganyu.Source AS S \
        WHERE (S.ID > $1) AND (S.ID < $2) AND (S.sourceName LIKE $3);", (const char**) params, 3);
    
    if(sourceRes == NULL) {
        GANYU_LOG(CHTTP_ERROR, "Failed to execute sql request");
        return not_found_page(con, request);
    }

    char* count = "0";
    PGresult *sourceCount = ganyu_make_sql_request(con, "SELECT COUNT(*) FROM ganyu.Source;", NULL, 0);
    if(sourceCount != NULL) {
        count = PQgetvalue(sourceCount, 0, 0);
    }

    int rows = PQntuples(sourceRes);
    int cols = PQnfields(sourceRes);

    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Ganyu viewer");
        }
        BODY("") {
            DIV("style='height:40%'") {
                H1("Ganyu viewer");
                navigation_element(HTML_STREAM);

                H2("Sources");

                FORM("action='/sources' method='get'") {
                    B("Filter by source ID range");
                    INPUT("type='number' style='width:50px;margin-left:10px;' name='start' value='%s'", start);
                    INPUT("type='number' style='width:50px;' name='end' value='%s'", end);
                    INPUT("type='submit' value='Filter' value='%s' style='margin-right: 10px;'", name);

                    I("Database contains total %s sources, showing [%s: %s] range", count, start, end);
                }

                FORM("action='/sources' method='get'") {
                    B("Filter by source name");
                    INPUT("type='text' name='name' style='margin-left: 10px;'");
                    INPUT("type='submit' value='Filter' style='margin-right: 10px;'");
                }

                TABLE("style='width:100%'") {
                    TR("") {
                        TD("style='width:5%'") { B("ID"); }
                        TD("style='width:20%'") { B("Source name"); }
                        TD("style='width:60%'") { B("Source description"); }
                        TD("style='width:15%'") { B("Action"); }
                    }
                }

                HR("style='border-top: 3px solid #bbb; border-radius: 5px;'");
            }

            // Search result
            DIV("style='overflow: scroll;height:60%;width:100%'") {

                for(int i = 0; i < rows; ++i) {
                    char* id = PQgetvalue(sourceRes, i, 0);
                    char* sourceName = PQgetvalue(sourceRes, i, 1);
                    char* sourceDescription = PQgetvalue(sourceRes, i, 2);
                    char* sourceType = PQgetvalue(sourceRes, i, 3);
                    char* sourceRootDestination = PQgetvalue(sourceRes, i, 4);

                    DIV("") {
                        TABLE("style='width:100%'") {
                            TR("") {
                                TD("style='width:5%'") { STRING("%s", id); }

                                // File name
                                TD("style='width:20%'") { 
                                    A("href='/source?id=%s' style='margin-right: inherit;'", id) { 
                                        STRING("%s %s", ganyu_source_type_to_icon(sourceType), sourceName); 
                                    } 
                                }
                                TD("style='width:60%'") { STRING("%s", sourceDescription); }
                                TD("style='width:15%'") {
                                    A("href='/source?id=%s' style='float: right; margin-right: 5px;'", id) { STRING("edit ✏️"); }
                                    A("href='/delsrc?id=%s' style='float: right; margin-right: 5px;'", id) { STRING("delete 🗑️"); }
                                }
                            }
                        }

                        HR("style='border-top: 2px dashed #bbb;'");
                    }
                }
            }
        }
    } 

    PQclear(sourceCount);
    PQclear(sourceRes);
    chttp_free_get_request_parsed(get);

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}