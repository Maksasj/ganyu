#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* sources_page(CHTTPConnection* con, CHTTPRequest* request) {
    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get == NULL)
        return not_found_page(con, request);

    CHTTPGetField* startField = chttp_get_request_parsed_find_field(get, "start");
    CHTTPGetField* endField = chttp_get_request_parsed_find_field(get, "end");

    if(startField == NULL)
        return not_found_page(con, request);

    if(endField == NULL)
        return not_found_page(con, request);

    int startRange = atoi(startField->fieldValue);
    int endRange = atoi(endField->fieldValue);

    if(startRange > endRange) {
        int saved = endRange;
        endRange = startRange;
        startRange = saved;
    }

    char* params[2] = {
        startField->fieldValue,
        endField->fieldValue
    };

    PGresult *sourceRes = ganyu_make_sql_request(con, 
        "SELECT * \
        FROM maja8801.Source AS S \
        WHERE (S.ID > $1) AND (S.ID < $2);", (const char**) params, 2);
    
    if(sourceRes == NULL) {
        GANYU_LOG(CHTTP_ERROR, "Failed to execute sql request");
        return not_found_page(con, request);
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

                H2("Files");
                SPAN("") {
                    STRING("Database contains total 12752 files, showing [%d: %d] range", startRange, endRange);
                }

                FORM("action='/files' method='get'") {
                    INPUT("type='number' name='start' value='%d'", startRange);
                    INPUT("type='number' name='end' value='%d'", endRange);
                    INPUT("type='submit' value='Show'");\
                }

                TABLE("style='width:100%'") {
                    TR("") {
                        TD("style='width:10%'") {
                            B("Source ID");
                        }
                        TD("style='width:20%'") {
                            B("Source name");
                        }
                        TD("style='width:50%'") {
                            B("Source description");
                        }
                        TD("style='width:20%'") {
                            B("Action");
                        }
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
                                TD("style='width:10%'") { STRING("%s", id); }

                                // File name
                                TD("style='width:20%'") { 
                                    A("href='/source?id=%s' style='margin-right: inherit;'", id) { 
                                        STRING("%s", sourceName); 
                                    } 
                                }
                                TD("style='width:50%'") { STRING("%s", sourceDescription); }
                                TD("style='width:20%'") {
                                    A("href='/source?id=%s' style='float: right; margin-right: 5px;'", id) { STRING("edit ✏️"); }
                                    A("href='/source?id=%s' style='float: right; margin-right: 5px;'", id) { STRING("delete 🗑️"); }
                                }
                            }
                        }

                        HR("style='border-top: 2px dashed #bbb;'");
                    }
                }
            }
        }
    } 

    PQclear(sourceRes);
    chttp_free_get_request_parsed(get);

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}