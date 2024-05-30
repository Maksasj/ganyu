#include "ganyu_app.h"
#include "ganyu_pages.h"

CHTTPResponse* modsrc_page(CHTTPConnection* con, CHTTPRequest* request) {
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

    CHTTPGetField* nameField = chttp_get_request_parsed_find_field(get, "name");
    CHTTPGetField* descField = chttp_get_request_parsed_find_field(get, "desc");

    int res = 0;

    {
        PGresult* lres = PQexec(conn, "BEGIN");

        if(PQresultStatus(lres) != PGRES_COMMAND_OK) {
            GANYU_LOG(CHTTP_ERROR, "BEGIN command failed: %s", PQerrorMessage(conn));
            res = 1;
        }

        PQclear(lres);
    }

    if(nameField != NULL) {
        char* params[2] = { idField->fieldValue, nameField->fieldValue };

        PGresult *pgres = ganyu_make_sql_request(con, 
        "UPDATE maja8801.Source \
            SET sourceName = $2 \
        WHERE ID = $1;", (const char**) params, 2);

        if(pgres != NULL) {
            res = 1;
            PQclear(pgres);
        }
    }

    if(descField != NULL) {
        char* params[2] = { idField->fieldValue, descField->fieldValue };

        PGresult *pgres = ganyu_make_sql_request(con, 
        "UPDATE maja8801.Source \
            SET sourceDescription = $2 \
        WHERE ID = $1;", (const char**) params, 2);

        if(pgres != NULL) {
            res = 1;
            PQclear(pgres);
        }
    }
    
    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Ganyu viewer");
        }
        BODY("") {
            DIV("style='width:100%; height:100%; display:grid; place-items: center'") {
                if(res) {
                    DIV("style='text-align: center;'") {
                        H1("🎉 Successfully modified source 🎉");
                        A("href='/source?id=%s' style='margin-right: 7px;'", idField->fieldValue) { B("Ok 🌚"); }
                        A("href='/source?id=%s' style='margin-left: 7px;'", idField->fieldValue) { B("Thank you ! 💖"); }
                    }
                } else {
                    DIV("style='text-align: center;'") {
                        H1("☠️ Failed to modify source 😵‍💫");
                        A("href='/source?id=%s' style='margin-right: 7px;'", idField->fieldValue) { B("I will try to do it again ! 👻"); }
                        A("href='/source?id=%s' style='margin-left: 7px;'", idField->fieldValue) { B("Ok 👽"); }
                    }
                }

            }
        }
    } 
   
    {
        PGresult* lres = PQexec(conn, "END");

        if(PQresultStatus(lres) != PGRES_COMMAND_OK) {
            GANYU_LOG(CHTTP_ERROR, "END command failed: %s", PQerrorMessage(conn));
            res = 1;
        }

        PQclear(lres);
    }

    chttp_free_get_request_parsed(get);

    GANYU_LOG(CHTTP_INFO, "Started HTML compilation");

    char* string = HTML_COMPILE();

    CHTTPResponse* response = chttp_ok_response(HTTP_1_1, string); 
    free(string);

    return response; 
}