#include "ganyu_properties.h"

void ganyu_load_properties(GanyuProperties* properties, const char* filePath) {
    FILE * file = fopen(filePath, "r");

    fscanf(file, "%s %s %d %s %s", 
        properties->database, 
        properties->host, 
        &properties->port, 
        properties->user, 
        properties->password);
}