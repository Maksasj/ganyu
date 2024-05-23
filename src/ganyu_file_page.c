#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* file_page(CHTTPConnection* con, CHTTPRequest* request) {
    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get == NULL)
        return not_found_page(con, request);

    CHTTPGetField* idField = chttp_get_request_parsed_find_field(get, "id");

    if(idField == NULL)
        return not_found_page(con, request);

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

                H2("File");
            }

            // Search result
            DIV("style='overflow: scroll;height:60%; width:100%'") {
                /*
                for(int i = 0; i < rows; ++i) {
                    char* id = PQgetvalue(res, i, 0);
                    char* fileName = PQgetvalue(res, i, 1);
                    char* fileExtension = PQgetvalue(res, i, 2);
                    char* fileSize = PQgetvalue(res, i, 3);
                    char* sourceName = PQgetvalue(res, i, 5);

                    DIV("") {
                        TABLE("style='width:100%'") {
                            TR("") {
                                TD("style='width:10%'") { STRING("%s", id); }

                                // File name
                                TD("style='width:40%'") { 
                                    A("href='/file' style='margin-right: inherit;'") { 
                                        STRING("%s %s%s", ganyu_file_extension_to_icon(fileExtension), fileName, fileExtension); 
                                    } 
                                }
                                TD("style='width:10%'") { STRING("%s bytes", fileSize); }
                                TD("style='width:20%'") { 
                                    A("href='/file'") { STRING("%s", sourceName); }
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
                */
            }
        }
    } 

    chttp_free_get_request_parsed(get);

    char* string = HTML_COMPILE();
    return chttp_ok_response_flag(HTTP_1_1, string, CHTTP_FREE_MESSAGE);
}