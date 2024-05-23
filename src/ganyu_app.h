#ifndef GANYU_APP
#define GANYU_APP

#include <libpq-fe.h>

#include "ganyu_properties.h"

#include "chtml.h"
#include "chttp/chttp.h"

typedef struct GanyuApp {
    PGconn *pgConnection;
    CHTTPServer* server;
} GanyuApp;

void ganyu_init(GanyuApp* app, const char* propertiesFile);
void ganyu_start(GanyuApp* app);
void ganyu_exit(GanyuApp* app);

#endif