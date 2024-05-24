#ifndef GANYU_UTILS_H
#define GANYU_UTILS_H

#include <stdio.h>
#include <stdarg.h>

#include "chttp/chttp_utils.h"

void ganyu_log(CHTTPLogLevel logLevel, const char *format, ...);

#define GANYU_LOG(logLevel, ...) ganyu_log(logLevel, __VA_ARGS__)

#endif