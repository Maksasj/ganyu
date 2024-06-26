#include "ganyu_app.h"
#include "ganyu_pages.h"

void ganyu_init_pg(GanyuApp* app, const char* propertiesFile) {
    GanyuProperties properties;
    ganyu_load_properties(&properties, propertiesFile);

    char conInfo[4096] = { '\0' };
    sprintf(conInfo, "dbname='%s' host='%s' port='%d' user='%s' password='%s'", properties.database, properties.host, properties.port, properties.user, properties.password);

    app->pgConnection = PQconnectdb(conInfo);

    if (PQstatus(app->pgConnection) != CONNECTION_OK) {
        GANYU_LOG(CHTTP_ERROR, "Connection to database failed: %s", PQerrorMessage(app->pgConnection));
        PQfinish(app->pgConnection);
        exit(1);
    }

    GANYU_LOG(CHTTP_INFO, "Connected to the database successfully");
}

void ganyu_init_web(GanyuApp* app) {
    app->server = chttp_new_server(80);

    chttp_str_route(app->server, "/", index_page);

    chttp_glob_route(app->server, "/files*", files_page);
    chttp_glob_route(app->server, "/directories*", directories_page);
    chttp_glob_route(app->server, "/sources*", sources_page);
    chttp_glob_route(app->server, "/catalog*", catalog_page);
    chttp_str_route(app->server, "/upload", upload_page);

    chttp_glob_route(app->server, "/file?*", file_page);
    chttp_glob_route(app->server, "/directory?*", directory_page);
    chttp_glob_route(app->server, "/source?*", source_page);
    
    chttp_glob_route(app->server, "/delfile?*", delfile_page);
    chttp_glob_route(app->server, "/deldir?*", deldir_page);
    chttp_glob_route(app->server, "/delsrc?*", delsrc_page);

    chttp_glob_route(app->server, "/upfile?*", upfile_page);
    chttp_glob_route(app->server, "/updir?*", updir_page);
    chttp_glob_route(app->server, "/upsrc?*", upsrc_page);

    chttp_glob_route(app->server, "/modfile?*", modfile_page);
    chttp_glob_route(app->server, "/moddir?*", moddir_page);
    chttp_glob_route(app->server, "/modsrc?*", modsrc_page);
    
    chttp_glob_route(app->server, "/putfile?*", putfile_page);
    chttp_glob_route(app->server, "/putdir?*", putdir_page);

    chttp_glob_route(app->server, "/rmfilestore?*", rmfilestore_page);
    chttp_glob_route(app->server, "/rmdirstore?*", rmdirstore_page);

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
