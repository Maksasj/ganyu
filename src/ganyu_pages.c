#include "ganyu_pages.h"
#include "ganyu_app.h"

void navigation_element(FILE* HTML_STREAM) {
    A("href='/' style='margin-right: 7px;'") { STRING("Main 🌍"); }
    A("href='/files' style='margin-right: 7px;'") { STRING("Files 📑"); }
    A("href='/directories' style='margin-right: 7px;'") { STRING("Directories 📂"); }
    A("href='/sources' style='margin-right: 7px;'") { STRING("Sources 🗄️"); }
    A("href='/catalog' style='margin-right: 7px;'") { STRING("Catalog 📦"); }
    A("href='/upload' style='margin-right: 7px;'") { STRING("Upload 📤"); }
}

char* ganyu_file_extension_to_icon(char* ext) {
    if(
        (strcmp(ext, ".png") == 0) || 
        (strcmp(ext, ".jpg") == 0) || 
        (strcmp(ext, ".gif") == 0) || 
        (strcmp(ext, ".psd") == 0) || 
        (strcmp(ext, ".jpeg") == 0)
    ) {
        return "🖼️";
    }

    if(
        (strcmp(ext, ".mp4") == 0) ||
        (strcmp(ext, ".avi") == 0) ||
        (strcmp(ext, ".mov") == 0)
    ) {
        return "🎞️";
    }

    if(
        (strcmp(ext, ".c") == 0)    ||
        (strcmp(ext, ".cpp") == 0)  ||
        (strcmp(ext, ".h") == 0)    ||
        (strcmp(ext, ".tpp") == 0)  ||
        (strcmp(ext, ".php") == 0)  ||
        (strcmp(ext, ".html") == 0) ||
        (strcmp(ext, ".js") == 0)   ||
        (strcmp(ext, ".py") == 0)
    ) {
        return "⚙️";
    }

    if(
        (strcmp(ext, ".txt") == 0)  ||
        (strcmp(ext, ".xlsx") == 0) ||
        (strcmp(ext, ".docx") == 0) ||
        (strcmp(ext, ".pptx") == 0) ||
        (strcmp(ext, ".md") == 0)
    ) {
        return "📝";
    }

    return "📃";
}

char* ganyu_source_type_to_icon(char* ext) {
    if(strcmp(ext, "WEB             ") == 0)
        return "🌐";

    if(strcmp(ext, "PHYSICAL        ") == 0)
        return "💿";

    if(strcmp(ext, "REMOTE          ") == 0)
        return "☁️";

    return "🪬";
}

PGresult* ganyu_make_sql_request(CHTTPConnection* con, const char* query, const char** params, unsigned int count) {
    GanyuApp* app = (GanyuApp*) con->server->userPtr;
    PGconn* conn = app->pgConnection;

    PGresult* res = PQexecParams(conn, query, count, NULL, params, NULL, NULL, 0);   
    ExecStatusType status = PQresultStatus(res);

    if((status != PGRES_TUPLES_OK) && (status != PGRES_COMMAND_OK)) {
        GANYU_LOG(CHTTP_ERROR, "Failed to execte SQL request: %s", PQerrorMessage(conn));
        PQclear(res);
        return NULL;
    }

    return res;
}

int ganyu_make_sql_request_line(CHTTPConnection* con, const char* command) {
    GanyuApp* app = (GanyuApp*) con->server->userPtr;
    PGconn* conn = app->pgConnection;

    PGresult* lres = PQexec(conn, command);

    if(PQresultStatus(lres) != PGRES_COMMAND_OK) {
        GANYU_LOG(CHTTP_ERROR, "%s command failed: %s", command, PQerrorMessage(conn));
        return 1;
    }

    PQclear(lres);
    
    return 0;
}