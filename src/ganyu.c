#include <libssh2.h>
#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "ganyu_properties.h"

#include "chttp/chttp.h"

HTTPResponse* index_page(HTTPConnection* con, HTTPRequest* request);
HTTPResponse* file_page(HTTPConnection* con, HTTPRequest* request);
HTTPResponse* directory_page(HTTPConnection* con, HTTPRequest* request);

int main() {
    HTTPServer* server = http_new_server(80);

    http_str_route(server, "/", index_page);
    http_glob_route(server, "/file*", file_page);
    http_str_route(server, "/directory", directory_page);

    while(http_running(server)) {
        http_listen(server);
    }

    http_free_server(server);

    return 0;
}

/*
char* cstring(const char* format, ...) {
    char buffer[4096];

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    unsigned int length = strlen(buffer);

    char* result = calloc(1, length + 1);
    strcpy(result, buffer);

    return result;
}

char* chtml(const char *format, ...) {
    char* openTag = "<html>\n";
    char* closeTag = "</html>\n";
  
    char* child = NULL;

    unsigned int openTagLength = strlen(openTag);
    unsigned int childLength = strlen(child);
    unsigned int closeTagLength = strlen(closeTag);

    char* result = (char*) calloc(1, openTagLength + childLength + closeTagLength + 1);

    strcpy(result, openTag);
    strcat(result, child);
    strcat(result, closeTag);

    free(child);

    return result;
}

char* cp(const char *format, ...) {
    char* openTag = "<p>";
    char* closeTag = "</p>";
  
    unsigned int openTagLength = strlen(openTag);
    unsigned int childLength = strlen(child);
    unsigned int closeTagLength = strlen(closeTag);

    char* result = (char*) calloc(1, openTagLength + childLength + closeTagLength + 1);

    strcpy(result, openTag);
    strcat(result, child);
    strcat(result, closeTag);

    free(child);

    return result;
}
*/

char* celement(const char *format, ...) {
    char* openTag = "<p>";
    char* closeTag = "</p>";
  
    unsigned int childLength = 0;

    va_list args;
    va_start(args, format);
    childLength = vsnprintf(NULL, 0, format, args);
    va_end(args);
    ++childLength;

    char* child = (char*) calloc(1, childLength);
    
    va_start(args, format);
    vsnprintf(child, childLength, format, args);
    va_end(args);

    unsigned int openTagLength = strlen(openTag);
    unsigned int closeTagLength = strlen(closeTag);

    char* result = (char*) calloc(1, openTagLength + (childLength - 1) + closeTagLength + 1);

    strcpy(result, openTag);
    strcat(result, child);
    strcat(result, closeTag);

    free(child);

    return result;
}

// https://www.reddit.com/r/C_Programming/comments/11at6d8/generate_html_in_c/
#define SCOPE(atStart, atEnd) for (int _scope_break = ((atStart), 1); _scope_break; _scope_break = ((atEnd), 0))

#define HTML(attributes) SCOPE(fprintf(HTML_STREAM, "<html %s>", attributes), fprintf(HTML_STREAM, "</html>"))

#define HEAD(attributes) SCOPE(fprintf(HTML_STREAM, "<head %s>", attributes), fprintf(HTML_STREAM, "</head>"))
#define BODY(attributes) SCOPE(fprintf(HTML_STREAM, "<body %s>", attributes), fprintf(HTML_STREAM, "</body>"))

#define H1(format, ...) fprintf(HTML_STREAM, "<h1>" format "</h1>", ##__VA_ARGS__)
#define P(format, ...) fprintf(HTML_STREAM, "<p>" format "</p>", ##__VA_ARGS__)
#define TITLE(format, ...) fprintf(HTML_STREAM, "<title>" format "</title>", ##__VA_ARGS__)
#define META(attributes, ...) fprintf(HTML_STREAM, "<meta " attributes ">", ##__VA_ARGS__)

#define HTML_BEGIN()\
    size_t size;    \
    char *buffer;   \
                    \
    FILE* HTML_STREAM = open_memstream(&buffer, &size); \
    if (HTML_STREAM == NULL) {  \
        perror("Error opening memory stream");  \
    }   \

#define HTML_COMPILE() (fclose(HTML_STREAM), buffer)

HTTPResponse* index_page(HTTPConnection* con, HTTPRequest* request) {
    HTML_BEGIN()

    HTML("lang='en'") {
        HEAD("") {
            META("charset='UTF-8'");
            META("name='viewport' content='width=device-width, initial-scale=1.0'");
            TITLE("Ganyu viewer");
        }
        BODY("") {
            H1("Ganyu viewer 📑");
            P("Hello world !");
            P("This is a test article, number %d", 1);
        }
    } 

    char* string = HTML_COMPILE();
    return http_ok_response_flag(HTTP_1_1, string, CHTTP_FREE_MESSAGE);
}

HTTPResponse* file_page(HTTPConnection* con, HTTPRequest* request) {
    CHTTPGetRequestParsed* get = chttp_parse_get_request(request);

    if(get == NULL)
        return http_not_found_response(HTTP_1_1, "404 Not found");

    for(int i = 0; i < get->fieldCount; ++i) {
        printf("%d: '%s' = '%s'\n", i, get->fields[i]->fieldName, get->fields[i]->fieldValue);
    }

    chttp_free_get_request_parsed(get);
    return http_ok_response_file(HTTP_1_1, "file.html");
}

HTTPResponse* directory_page(HTTPConnection* con, HTTPRequest* request) {
    return http_ok_response_file(HTTP_1_1, "directory.html");
}

/*
int main() {
    GanyuProperties properties;
    ganyu_load_properties(&properties, "properties.ganyu.txt");

    char conInfo[4096] = { '\0' };
    sprintf(conInfo, "dbname='%s' host='%s' port='%d' user='%s' password='%s'", properties.database, properties.host, properties.port, properties.user, properties.password);

    PGconn *conn = PQconnectdb(conInfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        printf("Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        return -1;
    }

    printf("Connected to the database successfully!\n");


 // We have successfully established a connection to the database server
    printf("Connection Established\n");
    printf("Port: %s\n", PQport(conn));
    printf("Host: %s\n", PQhost(conn));
    printf("DBName: %s\n", PQdb(conn));

    char *query = "SELECT * FROM VirtualFile";

    PGresult *res = PQexec(conn, query);

    // Check the status of the query result
    ExecStatusType resStatus = PQresultStatus(res);

    // Convert the status to a string and print it
    printf("Query Status: %s\n", PQresStatus(resStatus));

    // Check if the query execution was successful
    if (resStatus != PGRES_TUPLES_OK) {
        // If not successful, print the error message and finish the connection
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));

        // Clear the result
        PQclear(res);

        // Finish the connection
        PQfinish(conn);

        // Exit the program
        exit(1);
    }

    // We have successfully executed the query
    printf("Query Executed Successfully\n");

    // Get the number of rows and columns in the query result
    int rows = PQntuples(res);
    int cols = PQnfields(res);
    printf("Number of rows: %d\n", rows);
    printf("Number of columns: %d\n", cols);

    // Print the column names
    for (int i = 0; i < cols; i++) {
        printf("%s\t", PQfname(res, i));
    }
    printf("\n");

    // Print all the rows and columns
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Print the column value
            printf("%s\t", PQgetvalue(res, i, j));
        }
        printf("\n");
    }

    // Clear the result
    PQclear(res);

    // Finish the connection
    PQfinish(conn);

    return 0;
}
*/
