#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* directory_page(CHTTPConnection* con, CHTTPRequest* request) {
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

    char* params[1] = { idField->fieldValue };

    // Dir info request
    PGresult *dirRes = ganyu_make_sql_request(con, 
    "SELECT * \
    FROM ganyu.VirtualDirectory AS VD \
    WHERE VD.ID = $1;", (const char**) params, 1);

    if(dirRes == NULL) {
        PQclear(dirRes);
        GANYU_LOG(CHTTP_ERROR, "Failed to execute SQL request");
        return not_found_page(con, request);
    }

    int directoryRows = PQntuples(dirRes);
    int directoryCols = PQnfields(dirRes);

    if((directoryRows <= 0) || (directoryCols <= 0)) {
        GANYU_LOG(CHTTP_ERROR, "Directory not found");
        return not_found_page(con, request);
    }

    char* directoryId = PQgetvalue(dirRes, 0, 0);
    char* directoryName = PQgetvalue(dirRes, 0, 1);
    char* directoryDescription = PQgetvalue(dirRes, 0, 2);

    // Directories stored in this directory request
    PGresult *dirStoredRes = ganyu_make_sql_request(con, 
    "SELECT VD.ID, VD.directoryName \
    FROM ganyu.VirtualDirectoryStored AS VDS \
    JOIN ganyu.VirtualDirectory AS VD ON VD.ID = VDS.VirtualDirectoryID \
    WHERE VDS.TargetVirtualDirectoryID = $1;", (const char**) params, 1);

    if(dirStoredRes == NULL) {
        PQclear(dirRes);
        GANYU_LOG(CHTTP_ERROR, "Failed to execute SQL request");
        return not_found_page(con, request);
    }

    int directoryStoredRows = PQntuples(dirStoredRes);
    int directoryStoredCols = PQnfields(dirStoredRes);

    // Directories stored in this directory request
    PGresult *fileStoredRes = ganyu_make_sql_request(con, 
    "SELECT VF.ID, VF.FileName, VF.FileExtension \
    FROM ganyu.VirtualFileStored AS VFS \
    JOIN ganyu.VirtualFile AS VF ON VF.ID = VFS.VirtualFileID \
    WHERE VFS.VirtualDirectoryID = $1;", (const char**) params, 1);

    if(fileStoredRes == NULL) {
        PQclear(dirRes);
        PQclear(dirStoredRes);
        GANYU_LOG(CHTTP_ERROR, "Failed to execute SQL request");
        return not_found_page(con, request);
    }

    int fileStoredRows = PQntuples(fileStoredRes);
    int fileStoredCols = PQnfields(fileStoredRes);

    // Where this directory could be stored
    PGresult *dirToStoredRes = ganyu_make_sql_request(con, 
    "SELECT \
        VD.ID, \
        VD.directoryName \
    FROM ganyu.VirtualDirectory AS VD \
    WHERE VD.ID NOT IN (\
        SELECT \
            VFD.VirtualDirectoryID \
        FROM VirtualFileStored AS VFD \
        WHERE VFD.VirtualFileID = $1 \
    );", (const char**) params, 1);

    if(dirToStoredRes == NULL) {
        PQclear(dirRes);
        PQclear(dirStoredRes);
        PQclear(fileStoredRes);
        GANYU_LOG(CHTTP_ERROR, "Failed to execute SQL request");
        return not_found_page(con, request);
    }

    int dirToStoredRows = PQntuples(dirToStoredRes);
    int dirToStoredCols = PQnfields(dirToStoredRes);

    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Ganyu viewer");
        }
        BODY("") {
            DIV("style='height:25%'") {
                H1("Ganyu viewer");
                navigation_element(HTML_STREAM);

                H2("Directory📂 '%s' overview", directoryName);
                HR("style='border-top: 2px solid #bbb;'");
            }

            DIV("style='display:flex;height:65%'") {
                DIV("style='width:33%;'") {
                    DIV("style='height:25%;'") {
                        H3("Directory meta data");

                        DIV("style='margin-right:7px; '") {
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
    
                        HR("style='border-top: 1px solid #bbb;'");
                    }

                    DIV("style='width:100%;height:30%;'") {
                        H3("Edit Directory");
            
                        FORM("action='/moddir' method='get'") {
                            INPUT("type='hidden' name='id' value='%s'", directoryId);

                            LABEL("") { B("Directory name: "); }
                            BR();
                            INPUT("type='text' name='name' style='margin-right: 10px;' value='%s'", directoryName);
                            INPUT("type='submit' value='Rename directory' style='margin-right: 10px;'");
                        }

                        FORM("action='/moddir' method='get'") {
                            INPUT("type='hidden' name='id' value='%s'", directoryId);

                            LABEL("") { B("Directory description: "); }
                            BR();
                            TEXTAREA("name='desc' rows='10' cols='30'") {
                                STRING("%s", directoryDescription);
                            }
                            BR();
                            INPUT("type='submit' value='Change description' style='margin-right: 10px;'");
                        }   
                    }
                }

                DIV("style='width:33%;'") {
                    H3("Directory contains");
                    DIV("style='margin-left:7px; overflow:scroll; overflow-x:hidden; height:100%;'") {
                        DIV("") {
                            for(int i = 0; i < directoryStoredRows; ++i) {
                                char* directoryStoredId = PQgetvalue(dirStoredRes, i, 0);
                                char* directoryStoredName = PQgetvalue(dirStoredRes, i, 1);

                                DIV("style='width:100%;'") {
                                    A("href='/directory?id=%s' style='margin-right: 5px;'", directoryStoredId) { 
                                        STRING("📂 %s", directoryStoredName); 
                                    }

                                    A("href='/rmdirstore?id=%s&target=%s' style='margin-right: 5px; float: right;'", directoryStoredId, directoryId) { 
                                        STRING("remove 📌"); 
                                    }

                                    HR("style='border-top: 1px solid #bbb;'");
                                }
                            }

                            for(int i = 0; i < fileStoredRows; ++i) {
                                char* fileId = PQgetvalue(fileStoredRes, i, 0);
                                char* fileName = PQgetvalue(fileStoredRes, i, 1);
                                char* fileExtension = PQgetvalue(fileStoredRes, i, 2);

                                DIV("style='width:100%;'") {
                                    A("href='/file?id=%s' style='margin-right: 5px;'", fileId) { 
                                        STRING("%s %s%s", ganyu_file_extension_to_icon(fileExtension), fileName, fileExtension); 
                                    }

                                    A("href='/rmfilestore?id=%s&target=%s' style='margin-right: 5px; float: right;'", fileId, directoryId) { 
                                        STRING("remove 📌"); 
                                    }

                                    HR("style='border-top: 1px solid #bbb;'");
                                }
                            }
                        }
                    }
                }
                    
                DIV("style='width:33%;'") {
                    H3("Other directories");
                    DIV("style='margin-left:7px; overflow:scroll; overflow-x:hidden; height:100%;'") {
                        for(int i = 0; i < dirToStoredRows; ++i) {
                            char* directoryStoredId = PQgetvalue(dirToStoredRes, i, 0);
                            char* directoryStoredName = PQgetvalue(dirToStoredRes, i, 1);

                            DIV("style='width:100%;'") {
                                A("href='/directory?id=%s' style='margin-right: 5px;'", directoryStoredId) { 
                                    STRING("📂 %s", directoryStoredName); 
                                }

                                A("href='/putdir?id=%s&dir=%s' style='margin-right: 5px; float: right;'", directoryStoredId, directoryId) { 
                                    STRING("put ❇️"); 
                                }

                                HR("style='border-top: 1px solid #bbb;'");
                            }
                        }
                    }
                }
            }
        }
    } 

    PQclear(dirRes);
    PQclear(dirStoredRes);
    PQclear(fileStoredRes);
    PQclear(dirToStoredRes);

    chttp_free_get_request_parsed(get);

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}