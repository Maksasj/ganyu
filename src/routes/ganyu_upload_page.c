#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* upload_page(CHTTPConnection* con, CHTTPRequest* request) {
    PGresult *sourceRes = ganyu_make_sql_request(con, "SELECT * FROM ganyu.Source;", NULL, 0);

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
            DIV("style='height:15%'") {
                H1("Ganyu viewer");
                navigation_element(HTML_STREAM);
            }

            DIV("style='width:100%; display:flex;'") {
                DIV("style='width:33%; margin-right:7px; border-bottom: 1px solid lightgray;'") {
                    H3("Upload file 📑");

                    FORM("action='/upfile' method='get'") {
                        LABEL("") { B("File name: "); }
                        INPUT("type='text' name='name' style='margin-left: 10px;' value='file'");
                        BR();

                        LABEL("") { B("File extension: "); }
                        INPUT("type='text' name='ext' style='margin-left: 10px;' value='.txt'");
                        BR();

                        LABEL("") { B("File size (bytes): "); }
                        INPUT("type='number' name='size' style='margin-left: 10px;' value='1'");
                        BR();

                        LABEL("") { B("File hash: "); }
                        INPUT("type='text' name='hash' style='margin-left: 10px;' value='ffffffffffffffffffffffffffffffff'");
                        BR();

                        LABEL("") { B("Source: "); }
                        SELECT("name='src'") {
                            for(int i = 0; i < rows; ++i) {
                                char* id = PQgetvalue(sourceRes, i, 0);
                                char* sourceName = PQgetvalue(sourceRes, i, 1);
                                char* sourceType = PQgetvalue(sourceRes, i, 3);

                                char atr[25] = { '\0' };
                                sprintf(atr, "value='%s'", id);
                                OPTION(atr) { STRING("%s %s", ganyu_source_type_to_icon(sourceType), sourceName); }
                            }
                        }
                        BR();

                        LABEL("") { B("Source access: "); }
                        INPUT("type='text' name='access' style='margin-left: 10px;' value='./'");
                        BR();

                        INPUT("type='submit' value='Create file' style='margin-right: 10px;'");
                    }
                }
                DIV("style='width:33%; margin-right:7px; border-bottom: 1px solid lightgray;'") {
                    H3("Upload directory 📂");

                    FORM("action='/updir' method='get'") {
                        LABEL("") { B("Directory name: "); }
                        INPUT("type='text' name='name' style='margin-left: 10px;' value='New Directory'");
                        BR();

                        LABEL("") { B("Directory description: "); }
                        BR();
                        TEXTAREA("name='description' rows='10' cols='30'") {
                            STRING("No Description");
                        }
                        BR();

                        INPUT("type='submit' value='Create directory' style='margin-right: 10px;'");
                    }
                }
                DIV("style='width:33%; margin-right:7px; border-bottom: 1px solid lightgray;'") {
                    H3("Upload source 🗄️");

                    FORM("action='/upsrc' method='get'") {
                        LABEL("") { B("Source name: "); }
                        INPUT("type='text' name='name' style='margin-left: 10px;' value='New Source'");
                        BR();

                        LABEL("") { B("Source description: "); }
                        BR();
                        TEXTAREA("name='description' rows='10' cols='30'") {
                            STRING("No Description");
                        }
                        BR();

                        LABEL("") { B("Source type:"); }
                        SELECT("name='type'") {
                            OPTION("value='WEB'") { STRING("Web storage"); }
                            OPTION("value='PHYSICAL'") { STRING("Physical storage"); }
                            OPTION("value='REMOTE'") { STRING("Remote storage"); }
                        }
                        BR();

                        LABEL("") { B("Source root destination:"); }
                        INPUT("type='text' name='root' style='margin-left: 10px;' value='./'");
                        BR();

                        INPUT("type='submit' value='Create source' style='margin-right: 10px;'");
                    }
                }
            }
        }
    } 

    PQclear(sourceRes);
    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}
