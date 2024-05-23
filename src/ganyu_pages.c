#include "ganyu_pages.h"

void navigation_element(FILE* HTML_STREAM) {
    A("href='/' style='margin-right: 7px;'") { STRING("Main 🌍"); }
    A("href='/files?start=0&end=100' style='margin-right: 7px;'") { STRING("Files 📑"); }
    A("href='/directories?start=0&end=100' style='margin-right: 7px;'") { STRING("Directories 📂"); }
    A("href='/sources' style='margin-right: 7px;'") { STRING("Sources 🗄️"); }
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