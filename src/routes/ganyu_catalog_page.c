#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* catalog_page(CHTTPConnection* con, CHTTPRequest* request) {
    char* pstart = "0";
    char* pend = "100000";

    char* rstart = "0";
    char* rend = "100000";

    char* wstart = "0";
    char* wend = "100000";
    
    char* name = "%";

    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get != NULL) {
        CHTTPGetField* physicalStartField = chttp_get_request_parsed_find_field(get, "pstart");
        CHTTPGetField* physicalEndField = chttp_get_request_parsed_find_field(get, "pend");

        if((physicalStartField != NULL) && physicalEndField != NULL) {
            pend = physicalEndField->fieldValue;
            pstart = physicalStartField->fieldValue;
        }

        CHTTPGetField* remoteStartField = chttp_get_request_parsed_find_field(get, "rstart");
        CHTTPGetField* remoteEndField = chttp_get_request_parsed_find_field(get, "rend");

        if((remoteStartField != NULL) && remoteEndField != NULL) {
            rend = remoteEndField->fieldValue;
            rstart = remoteStartField->fieldValue;
        }

        CHTTPGetField* webStartField = chttp_get_request_parsed_find_field(get, "wstart");
        CHTTPGetField* webEndField = chttp_get_request_parsed_find_field(get, "wend");

        if((webStartField != NULL) && webEndField != NULL) {
            wend = webEndField->fieldValue;
            wstart = webStartField->fieldValue;
        }

        CHTTPGetField* nameField = chttp_get_request_parsed_find_field(get, "name");

        if(nameField != NULL)
            name = nameField->fieldValue;
    } 

    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Ganyu viewer");
        }
        BODY("") {
            DIV("style='height:35%'") {
                H1("Ganyu viewer");
                navigation_element(HTML_STREAM);

                H2("Catalog");

                FORM("action='/catalog' method='get'") {
                    B("Filter by file name");
                    INPUT("type='text' name='name' style='margin-left: 10px;'");
                    INPUT("type='submit' value='Filter' style='margin-right: 10px;'");

                    I("File extension not including");
                }

                TABLE("style='width:100%'") {
                    TR("") {
                        TD("style='width:33%'") { B("Physical files"); }
                        TD("style='width:33%'") { B("Remote files"); }
                        TD("style='width:33%'") { B("Web files"); }
                    }
                }

                HR("style='border-top: 3px solid #bbb; border-radius: 5px;'");
            }

            // Search result
            DIV("style='height:65%; width:100%; display:flex;'") {
                DIV("style='width:33%; overflow: scroll;'") {
                    char* params[3] = { pstart, pend, name };

                    PGresult *filesRes = ganyu_make_sql_request(con, 
                        "SELECT Files.ID, VF.FileName, VF.FileExtension \
                        FROM maja8801.PhysicalVirtualFile AS Files \
                        JOIN maja8801.VirtualFile AS VF ON Files.ID = VF.ID \
                        WHERE (Files.ID > $1) AND (Files.ID < $2) AND (VF.FileName LIKE $3);", (const char**) params, 3);
                    
                    if(filesRes == NULL) {
                        GANYU_LOG(CHTTP_ERROR, "Failed to execute sql request");
                        H3("🪅 Sadly there is no files in this catalog 🪅");
                        continue;
                    }

                    int rows = PQntuples(filesRes);
                    int cols = PQnfields(filesRes);

                    for(int i = 0; i < rows; ++i) {
                        char* id = PQgetvalue(filesRes, i, 0);
                        char* fileName = PQgetvalue(filesRes, i, 1);
                        char* fileExtension = PQgetvalue(filesRes, i, 2);

                        DIV("") {
                            TABLE("style='width:100%'") {
                                TR("") {
                                    TD("style='width:10%'") { STRING("%s", id); }

                                    // File name
                                    TD("style='width:90%'") { 
                                        A("href='/file?id=%s' style='margin-right: inherit;'", id) { 
                                            STRING("%s %s%s", ganyu_file_extension_to_icon(fileExtension), fileName, fileExtension); 
                                        } 
                                    }
                                }
                            }

                            HR("style='border-top: 2px dashed #bbb;'");
                        }
                    }

                    PQclear(filesRes);
                }
                DIV("style='width:33%; overflow: scroll;'") {
                    char* params[3] = { rstart, rend, name };

                    PGresult *filesRes = ganyu_make_sql_request(con, 
                        "SELECT Files.ID, VF.FileName, VF.FileExtension \
                        FROM maja8801.RemoteVirtualFile AS Files \
                        JOIN maja8801.VirtualFile AS VF ON Files.ID = VF.ID \
                        WHERE (Files.ID > $1) AND (Files.ID < $2) AND (VF.FileName LIKE $3);", (const char**) params, 3);
                    
                    if(filesRes == NULL) {
                        GANYU_LOG(CHTTP_ERROR, "Failed to execute sql request");
                        H3("🪅 Sadly there is no files in this catalog 🪅");
                        continue;
                    }

                    int rows = PQntuples(filesRes);
                    int cols = PQnfields(filesRes);

                    for(int i = 0; i < rows; ++i) {
                        char* id = PQgetvalue(filesRes, i, 0);
                        char* fileName = PQgetvalue(filesRes, i, 1);
                        char* fileExtension = PQgetvalue(filesRes, i, 2);

                        DIV("") {
                            TABLE("style='width:100%'") {
                                TR("") {
                                    TD("style='width:10%'") { STRING("%s", id); }

                                    // File name
                                    TD("style='width:90%'") { 
                                        A("href='/file?id=%s' style='margin-right: inherit;'", id) { 
                                            STRING("%s %s%s", ganyu_file_extension_to_icon(fileExtension), fileName, fileExtension); 
                                        } 
                                    }
                                }
                            }

                            HR("style='border-top: 2px dashed #bbb;'");
                        }
                    }

                    PQclear(filesRes);
                }
                DIV("style='width:33%; overflow: scroll;'") {
                    char* params[3] = { wstart, wend, name };

                    PGresult *filesRes = ganyu_make_sql_request(con, 
                        "SELECT Files.ID, VF.FileName, VF.FileExtension \
                        FROM maja8801.WebVirtualFile AS Files \
                        JOIN maja8801.VirtualFile AS VF ON Files.ID = VF.ID \
                        WHERE (Files.ID > $1) AND (Files.ID < $2) AND (VF.FileName LIKE $3);", (const char**) params, 3);
                    
                    if(filesRes == NULL) {
                        GANYU_LOG(CHTTP_ERROR, "Failed to execute sql request");
                        H3("🪅 Sadly there is no files in this catalog 🪅");
                        continue;
                    }

                    int rows = PQntuples(filesRes);
                    int cols = PQnfields(filesRes);

                    for(int i = 0; i < rows; ++i) {
                        char* id = PQgetvalue(filesRes, i, 0);
                        char* fileName = PQgetvalue(filesRes, i, 1);
                        char* fileExtension = PQgetvalue(filesRes, i, 2);

                        DIV("") {
                            TABLE("style='width:100%'") {
                                TR("") {
                                    TD("style='width:10%'") { STRING("%s", id); }

                                    // File name
                                    TD("style='width:90%'") { 
                                        A("href='/file?id=%s' style='margin-right: inherit;'", id) { 
                                            STRING("%s %s%s", ganyu_file_extension_to_icon(fileExtension), fileName, fileExtension); 
                                        } 
                                    }
                                }
                            }

                            HR("style='border-top: 2px dashed #bbb;'");
                        }
                    }

                    PQclear(filesRes);
                }
            }
        }
    } 

    chttp_free_get_request_parsed(get);

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}