#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* file_page(CHTTPConnection* con, CHTTPRequest* request) {
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

    char* params[1] = {
        idField->fieldValue
    };

    PGresult *fileRes = ganyu_make_sql_request(con, 
    "SELECT * \
    FROM maja8801.VirtualFile as VF \
    JOIN maja8801.Source AS S ON VF.SourceID = S.ID \
    WHERE VF.ID = $1;", (const char**) params, 1);

    if(fileRes == NULL) {
        GANYU_LOG(CHTTP_ERROR, "Failed to execute SQL request");
        return not_found_page(con, request);
    }

    int fileMetaRows = PQntuples(fileRes);
    int fileMetaCols = PQnfields(fileRes);

    if((fileMetaRows <= 0) || (fileMetaCols <= 0)) {
        GANYU_LOG(CHTTP_ERROR, "File not found");
        return not_found_page(con, request);
    }

    char* fileId = PQgetvalue(fileRes, 0, 0);
    char* fileName = PQgetvalue(fileRes, 0, 1);
    char* fileExtension = PQgetvalue(fileRes, 0, 2);
    char* fileSize = PQgetvalue(fileRes, 0, 3);
    char* fileContentHash = PQgetvalue(fileRes, 0, 4);
    char* fileDateCreated = PQgetvalue(fileRes, 0, 5);
    char* fileDateModified = PQgetvalue(fileRes, 0, 6);
    char* sourceId = PQgetvalue(fileRes, 0, 7);
    char* sourceAccess = PQgetvalue(fileRes, 0, 8);
    char* sourceName = PQgetvalue(fileRes, 0, 10);
    char* sourceDesciption = PQgetvalue(fileRes, 0, 11);
    char* sourceType = PQgetvalue(fileRes, 0, 12);
    char* sourceRootDestination = PQgetvalue(fileRes, 0, 13);

    PGresult *dirRes = ganyu_make_sql_request(con, 
    "WITH VirtualDirectories(ID) AS \
        (SELECT VFS.VirtualDirectoryID \
        FROM maja8801.VirtualFileStored as VFS \
        WHERE VFS.VirtualFileID = $1) \
    SELECT VD.ID, VD.directoryName, VD.directoryDescription \
    FROM maja8801.VirtualDirectory AS VD \
    JOIN VirtualDirectories ON VD.ID = VirtualDirectories.ID;", (const char**) params, 1);

    if(dirRes == NULL) {
        GANYU_LOG(CHTTP_ERROR, "Failed to execute SQL request");
        return not_found_page(con, request);
    }

    int dirMetaRows = PQntuples(dirRes);
    int dirMetaCols = PQnfields(dirRes);

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

            H2("File %s '%s%s' overview", ganyu_file_extension_to_icon(fileExtension), fileName, fileExtension);
            HR("style='border-top: 2px solid #bbb;'");

            // Search result
            DIV("style='width:100%; display:flex;'") {
                DIV("style='width:33%; margin-right:7px; border-right: 1px solid lightgray;'") {
                    H3("File meta data");

                    TABLE("") {
                        TR("") {
                            TD("") { B("File ID"); }
                            TD("") { STRING("%s", fileId); }
                        }

                        TR("") {
                            TD("") { B("File name"); }
                            TD("") { STRING("%s", fileName); }
                        }
                        
                        TR("") {
                            TD("") { B("File extension"); }
                            TD("") { STRING("%s", fileExtension); }
                        }

                        TR("") {
                            TD("") { B("File size"); }
                            TD("") { STRING("%s bytes", fileSize); }
                        }

                        TR("") {
                            TD("") { B("Content hash"); }
                            TD("") { STRING("%s", fileContentHash); }
                        }

                        TR("") {
                            TD("") { B("File creation date"); }
                            TD("") { STRING("%s", fileDateCreated); }
                        }

                        TR("") {
                            TD("") { B("File modification date"); }
                            TD("") { STRING("%s", fileDateModified); }
                        }

                        TR("") {
                            TD("") { B("Source access"); }
                            TD("") { STRING("%s", sourceAccess); }
                        }
                    }
                }

                DIV("style='width:33%; margin-right:7px;'") {
                    H3("Virtual directories");

                    if(dirMetaRows <= 0) {
                        DIV("style='width:100%;'") {
                            STRING("This file is not contained in any Virtual Directory 👹");
                        }
                    } else {
                        for(int i = 0; i < dirMetaRows; ++i) {
                            char* dirId = PQgetvalue(dirRes, i, 0);
                            char* dirName = PQgetvalue(dirRes, i, 1);
                            char* dirDesciption = PQgetvalue(dirRes, i, 2);

                            DIV("style='width:100%;'") {
                                A("href='/directory?id=%s' style='margin-right: 5px;'", dirId) { 
                                    STRING("%s", dirName); 
                                }

                                A("href='/rmfilestore?id=%s&target=%s' style='margin-right: 5px; float: right;'", fileId, dirId) { 
                                    STRING("remove 📌"); 
                                }

                                if(strcmp(dirDesciption, "") != 0) {
                                    BR();

                                    SPAN("style='color:#888888;'") {
                                        STRING("• %s", dirDesciption);
                                    }
                                }
                            }
                        }
                    }
                }

                DIV("style='width:33%; padding-left:7px; border-left: 1px solid lightgray;'") {
                    H3("Source %s %s", ganyu_source_type_to_icon(sourceType), sourceName);

                    TABLE("") {
                        TR("") {
                            TD("") { B("Source name"); }
                            TD("") { STRING("%s", sourceName); }
                        }

                        TR("") {
                            TD("") { B("Source Description"); }
                            TD("") { STRING("%s", sourceDesciption); }
                        }
                        
                        TR("") {
                            TD("") { B("Source Type"); }
                            TD("") { STRING("%s", sourceType); }
                        }

                        TR("") {
                            TD("") { B("Source Root Destination"); }
                            TD("") { STRING("%s", sourceRootDestination); }
                        }
                    }

                    A("href='/source?id=%s' style='float: left; margin-right: 5px;'", sourceId) { STRING("view 👀"); }
                }
            }
        }
    } 

    PQclear(fileRes);
    PQclear(dirRes);

    chttp_free_get_request_parsed(get);

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}