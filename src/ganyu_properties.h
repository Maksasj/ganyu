#ifndef GANYU_PROPERTIES_H
#define GANYU_PROPERTIES_H

#include <stdio.h>
#include <stdlib.h>

typedef struct GanyuProperties {
    char database[64];
    char host[64];
    int port;
    char user[64];
    char password[64];
} GanyuProperties;

void ganyu_load_properties(GanyuProperties* properties, const char* filePath);

#endif