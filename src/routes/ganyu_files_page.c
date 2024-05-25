#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* files_page(CHTTPConnection* con, CHTTPRequest* request) {
    char* start = "0";
    char* end = "100";
    char* name = "%";

    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get != NULL) {
        CHTTPGetField* startField = chttp_get_request_parsed_find_field(get, "start");
        CHTTPGetField* endField = chttp_get_request_parsed_find_field(get, "end");

        if((startField != NULL) && (endField != NULL)) {
            start = startField->fieldValue;
            end = endField->fieldValue;
        }

        CHTTPGetField* nameField = chttp_get_request_parsed_find_field(get, "name");

        if(nameField != NULL)
            name = nameField->fieldValue;
    }
    
    char* params[3] = { start, end, name };

    PGresult *filesRes = ganyu_make_sql_request(con, 
        "SELECT VF.ID, VF.FileName, VF.FileExtension, VF.FileSize, S.ID, S.sourceName \
        FROM maja8801.VirtualFile AS VF \
        JOIN maja8801.Source AS S ON VF.SourceID = S.ID \
        WHERE (VF.ID > $1) AND (VF.ID < $2) AND (VF.FileName LIKE $3);", (const char**) params, 3);
    
    if(filesRes == NULL) {
        GANYU_LOG(CHTTP_ERROR, "Failed to execute sql request");
        return not_found_page(con, request);    
    }

    int rows = PQntuples(filesRes);
    int cols = PQnfields(filesRes);

    char* count = "0";
    PGresult *fileCount = ganyu_make_sql_request(con, "SELECT COUNT(*) FROM maja8801.VirtualFile;", NULL, 0);
    if(fileCount != NULL) {
        count = PQgetvalue(fileCount, 0, 0);
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

                FORM("action='/files' method='get'") {
                    B("Filter by file ID range");
                    INPUT("type='number' style='width:50px;margin-left:10px;' name='start' value='%s'", start);
                    INPUT("type='number' style='width:50px;' name='end' value='%s'", end);
                    INPUT("type='submit' value='Filter' value='%s' style='margin-right: 10px;'", name);

                    I("Database contains total %s files, showing [%s: %s] range", count, start, end);
                }

                FORM("action='/files' method='get'") {
                    B("Filter by file name");
                    INPUT("type='text' name='name' style='margin-left: 10px;'");
                    INPUT("type='submit' value='Filter' style='margin-right: 10px;'");

                    I("File extension not including");
                }

                TABLE("style='width:100%'") {
                    TR("") {
                        TD("style='width:5%'") { B("ID"); }
                        TD("style='width:50%'") { B("File name"); }
                        TD("style='width:10%'") { B("File size"); }
                        TD("style='width:20%'") { B("Source name"); }
                        TD("style='width:15%'") { B("Action"); }
                    }
                }

                HR("style='border-top: 3px solid #bbb; border-radius: 5px;'");
            }

            // Search result
            DIV("style='overflow: scroll;height:60%;width:100%'") {
                if(rows <= 0)
                    H3("Not found any files with provided parameters 😿");

                for(int i = 0; i < rows; ++i) {
                    char* id = PQgetvalue(filesRes, i, 0);
                    char* fileName = PQgetvalue(filesRes, i, 1);
                    char* fileExtension = PQgetvalue(filesRes, i, 2);
                    char* fileSize = PQgetvalue(filesRes, i, 3);
                    char* sourceId = PQgetvalue(filesRes, i, 4);
                    char* sourceName = PQgetvalue(filesRes, i, 5);

                    DIV("") {
                        TABLE("style='width:100%'") {
                            TR("") {
                                TD("style='width:5%'") { STRING("%s", id); }

                                // File name
                                TD("style='width:50%'") { 
                                    A("href='/file?id=%s' style='margin-right: inherit;'", id) { 
                                        STRING("%s %s%s", ganyu_file_extension_to_icon(fileExtension), fileName, fileExtension); 
                                    } 
                                }
                                TD("style='width:10%'") { STRING("%s bytes", fileSize); }
                                TD("style='width:20%'") { 
                                    A("href='/source?id=%s'", sourceId) { STRING("%s", sourceName); }
                                }
                                TD("style='width:15%'") {
                                    A("href='/file?id=%s' style='float: right; margin-right: 5px;'", id) { STRING("edit ✏️"); }
                                    A("href='/delfile?id=%s' style='float: right; margin-right: 5px;'", id) { STRING("delete 🗑️"); }
                                }
                            }
                        }

                        HR("style='border-top: 2px dashed #bbb;'");
                    }
                }
            }
        }
    } 

    PQclear(filesRes);
    PQclear(fileCount);
    chttp_free_get_request_parsed(get);

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}