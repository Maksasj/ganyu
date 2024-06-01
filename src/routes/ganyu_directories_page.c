#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* directories_page(CHTTPConnection* con, CHTTPRequest* request) {
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
    PGresult *dirRes = ganyu_make_sql_request(con, 
        "WITH  \
        STEP0(ID, directoryName, directoryDescription) AS ( \
            SELECT  \
                VD.ID,  \
                VD.directoryName,  \
                VD.directoryDescription \
            FROM ganyu.VirtualDirectory AS VD \
            WHERE (VD.ID > $1) AND (VD.ID < $2) AND(VD.directoryName LIKE $3) \
        ), \
        STEP1(ID, directoryName, directoryDescription, storedDirs) AS ( \
            SELECT  \
                VD.ID,  \
                VD.directoryName,  \
                VD.directoryDescription,  \
                COUNT(VDS.TargetVirtualDirectoryID) \
            FROM STEP0 AS VD \
            LEFT JOIN VirtualDirectoryStored AS VDS  \
                ON VD.ID = VDS.TargetVirtualDirectoryID \
            GROUP BY  \
                VD.ID,  \
                VD.directoryName,  \
                VD.directoryDescription \
        ) \
        SELECT  \
            VD.ID,  \
            VD.directoryName,  \
            VD.directoryDescription,  \
            VD.storedDirs,  \
            COUNT(VFS.VirtualDirectoryID) \
        FROM STEP1 AS VD \
        LEFT JOIN VirtualFileStored AS VFS  \
            ON VD.ID = VFS.VirtualDirectoryID \
        GROUP BY  \
            VD.ID,  \
            VD.directoryName,  \
            VD.directoryDescription, \
            VD.storedDirs \
        ORDER BY VD.ID;", (const char**) params, 3);
    
    if(dirRes == NULL) {
        GANYU_LOG(CHTTP_ERROR, "Failed to execute sql request");
        return not_found_page(con, request);
    }

    char* count = "0";
    PGresult *directoryCount = ganyu_make_sql_request(con, "SELECT COUNT(*) FROM ganyu.VirtualDirectory;", NULL, 0);
    if(directoryCount != NULL) {
        count = PQgetvalue(directoryCount, 0, 0);
    }

    int rows = PQntuples(dirRes);
    int cols = PQnfields(dirRes);

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

                H2("Directories");

                FORM("action='/directories' method='get'") {
                    B("Filter by directory ID range");
                    INPUT("type='number' style='width:50px;margin-left:10px;' name='start' value='%s'", start);
                    INPUT("type='number' style='width:50px;' name='end' value='%s'", end);
                    INPUT("type='submit' value='Filter' value='%s' style='margin-right: 10px;'", name);

                    I("Database contains total %s directories, showing [%s: %s] range", count, start, end);
                }

                FORM("action='/directories' method='get'") {
                    B("Filter by directory name");
                    INPUT("type='text' name='name' style='margin-left: 10px;'");
                    INPUT("type='submit' value='Filter' style='margin-right: 10px;'");
                }

                TABLE("style='width:100%'") {
                    TR("") {
                        TD("style='width:5%'") { B("ID"); }
                        TD("style='width:20%'") { B("Directory name"); }
                        TD("style='width:30%'") { B("Directory stores"); }
                        TD("style='width:30%'") { B("Directory description"); }
                        TD("style='width:15%'") { B("Action"); }
                    }
                }

                HR("style='border-top: 3px solid #bbb; border-radius: 5px;'");
            }

            // Search result
            DIV("style='overflow: scroll;height:60%;width:100%'") {

                for(int i = 0; i < rows; ++i) {
                    char* id = PQgetvalue(dirRes, i, 0);
                    char* dirName = PQgetvalue(dirRes, i, 1);
                    char* dirDescription = PQgetvalue(dirRes, i, 2);
                    char* dirStored = PQgetvalue(dirRes, i, 3);
                    char* filesStored = PQgetvalue(dirRes, i, 4);

                    DIV("") {
                        TABLE("style='width:100%'") {
                            TR("") {
                                TD("style='width:5%'") { STRING("%s", id); }

                                TD("style='width:20%'") { 
                                    A("href='/directory?id=%s' style='margin-right: inherit;'", id) { 
                                        STRING("📂 %s", dirName); 
                                    }
                                }
                                TD("style='width:30%'") { STRING("Stores %s directories, %s files", dirStored, filesStored); }
                                TD("style='width:30%'") { STRING("%s", dirDescription); }
                                TD("style='width:15%'") {
                                    A("href='/directory?id=%s' style='float: right; margin-right: 5px;'", id) { STRING("edit ✏️"); }
                                    A("href='/deldir?id=%s' style='float: right; margin-right: 5px;'", id) { STRING("delete 🗑️"); }
                                }
                            }
                        }

                        HR("style='border-top: 2px dashed #bbb;'");
                    }
                }
            }
        }
    } 

    PQclear(directoryCount);
    PQclear(dirRes);
    chttp_free_get_request_parsed(get);

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}