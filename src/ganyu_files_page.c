#include "ganyu_app.h"
#include "ganyu_pages.h"

PGresult* ganyu_make_sql_request(CHTTPConnection* con, const char* query, const char** params, unsigned int count) {
    GanyuApp* app = (GanyuApp*) con->server->userPtr;
    PGconn* conn = app->pgConnection;

    PGresult* res = PQexecParams(conn, query, count, NULL, params, NULL, NULL, 0);   
    ExecStatusType resStatus = PQresultStatus(res);

    if (resStatus != PGRES_TUPLES_OK) {
        PQclear(res);
        return NULL;
    }

    return res;
}

CHTTPResponse* files_page(CHTTPConnection* con, CHTTPRequest* request) {

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
                            B("File ID");
                        }
                        TD("style='width:40%'") {
                            B("File name");
                        }
                        TD("style='width:10%'") {
                            B("File size");
                        }
                        TD("style='width:20%'") {
                            B("Source name");
                        }
                        TD("style='width:20%'") {
                            B("Action");
                        }
                    }
                }

                HR("style='border-top: 3px solid #bbb; border-radius: 5px;'");
            }

            char* params[2] = {
                startField->fieldValue,
                endField->fieldValue
            };

            PGresult *res = ganyu_make_sql_request(con, 
                "SELECT VF.ID, VF.FileName, VF.FileExtension, VF.FileSize, S.ID, S.sourceName \
                FROM maja8801.VirtualFile AS VF \
                JOIN maja8801.Source AS S ON VF.SourceID = S.ID \
                WHERE (VF.ID > $1) AND (VF.ID < $2);", (const char**) params, 2);
            
            if(res == NULL) {
                CHTTP_LOG(CHTTP_ERROR, "Failed to execute sql request");
                continue;
            }

            int rows = PQntuples(res);
            int cols = PQnfields(res);

            // Search result
            DIV("style='overflow: scroll;height:60%;width:100%'") {
                for(int i = 0; i < rows; ++i) {
                    char* id = PQgetvalue(res, i, 0);
                    char* fileName = PQgetvalue(res, i, 1);
                    char* fileExtension = PQgetvalue(res, i, 2);
                    char* fileSize = PQgetvalue(res, i, 3);
                    char* sourceId = PQgetvalue(res, i, 4);
                    char* sourceName = PQgetvalue(res, i, 5);

                    DIV("") {
                        TABLE("style='width:100%'") {
                            TR("") {
                                TD("style='width:10%'") { STRING("%s", id); }

                                // File name
                                TD("style='width:40%'") { 
                                    A("href='/file?id=%s' style='margin-right: inherit;'", id) { 
                                        STRING("%s %s%s", ganyu_file_extension_to_icon(fileExtension), fileName, fileExtension); 
                                    } 
                                }
                                TD("style='width:10%'") { STRING("%s bytes", fileSize); }
                                TD("style='width:20%'") { 
                                    A("href='/source?id=%s'", sourceId) { STRING("%s", sourceName); }
                                }
                                TD("style='width:20%'") {
                                    A("href='/file' style='float: right; margin-right: 5px;'") { STRING("edit ✏️"); }
                                    A("href='/file' style='float: right; margin-right: 5px;'") { STRING("delete 🗑️"); }
                                }
                            }
                        }

                        HR("style='border-top: 2px dashed #bbb;'");
                    }
                }
            }

            PQclear(res);
        }
    } 

    chttp_free_get_request_parsed(get);

    char* string = HTML_COMPILE();
    return chttp_ok_response_flag(HTTP_1_1, string, CHTTP_FREE_MESSAGE);
}