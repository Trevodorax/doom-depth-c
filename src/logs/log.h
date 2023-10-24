#ifndef DOOM_DEPTH_C_LOG_H
#define DOOM_DEPTH_C_LOG_H

#include <stdio.h>

typedef struct logger_t logger_t;

typedef void (*log_function)(logger_t *logger, const char *format, ...);

struct logger_t {
    FILE *error_stream;
    FILE *warn_stream;
    FILE *info_stream;
    log_function error;
    log_function warn;
    log_function info;
};

logger_t *new_logger();
void logger_free(logger_t *logger);
void logger_set_outputs(logger_t *logger, char *error_stream_path, char *warning_stream_path, char *info_stream_path);

void logger_error(logger_t *logger, const char *format, ...);
void logger_warn(logger_t *logger, const char *format, ...);
void logger_info(logger_t *logger, const char *format, ...);

#endif