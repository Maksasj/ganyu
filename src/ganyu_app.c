#include "ganyu_app.h"
#include "ganyu_pages.h"

void ganyu_init_pg(GanyuApp* app, const char* propertiesFile) {
    GanyuProperties properties;
    ganyu_load_properties(&properties, propertiesFile);

    char conInfo[4096] = { '\0' };
    sprintf(conInfo, "dbname='%s' host='%s' port='%d' user='%s' password='%s'", properties.database, properties.host, properties.port, properties.user, properties.password);

    app->pgConnection = PQconnectdb(conInfo);

    if (PQstatus(app->pgConnection) != CONNECTION_OK) {
        CHTTP_LOG(CHTTP_ERROR, "Connection to database failed: %s", PQerrorMessage(app->pgConnection));
        PQfinish(app->pgConnection);
        exit(1);
    }

    CHTTP_LOG(CHTTP_INFO, "Connected to the database successfully");
}

void ganyu_init_web(GanyuApp* app) {
    app->server = chttp_new_server(80);

    chttp_str_route(app->server, "/", index_page);

    chttp_glob_route(app->server, "/files?*", files_page);
    chttp_glob_route(app->server, "/directories?*", directories_page);
    chttp_glob_route(app->server, "/sources?*", sources_page);

    chttp_glob_route(app->server, "/file?*", file_page);
    chttp_glob_route(app->server, "/directory?*", directory_page);
    chttp_glob_route(app->server, "/source?*", source_page);

    // Setting user pointer
    chttp_server_set_user_pointer(app->server, app);
}

void ganyu_init(GanyuApp* app, const char* propertiesFile) {
    ganyu_init_pg(app, propertiesFile);
    ganyu_init_web(app);
}

void ganyu_start(GanyuApp* app) {
    while(chttp_running(app->server))
        chttp_listen(app->server);
}

void ganyu_exit(GanyuApp* app) {
    PQfinish(app->pgConnection);
    chttp_free_server(app->server);
}
