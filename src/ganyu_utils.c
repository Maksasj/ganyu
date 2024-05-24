#include "ganyu_utils.h"

void ganyu_log(CHTTPLogLevel logLevel, const char *format, ...) {
    va_list args;
    va_start(args, format);

    printf("[GANYU][%s] ", chttp_stringify_log_level(logLevel));
    vprintf(format, args);
    printf("\n");

    va_end(args);
}
