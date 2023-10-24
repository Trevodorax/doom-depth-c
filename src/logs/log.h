#ifndef DOOM_DEPTH_C_LOG_H
#define DOOM_DEPTH_C_LOG_H

#include <stdio.h>

typedef struct logger_t logger_t;

typedef void (*log_function)(const char *format, ...);

struct logger_t {
    FILE *error_stream;
    FILE *warn_stream;
    FILE *info_stream;
    log_function error;
    log_function warn;
    log_function info;
};

extern logger_t *global_logger;

void init_global_logger();
void logger_free();
void logger_set_outputs(char *error_stream_path, char *warning_stream_path, char *info_stream_path);

void logger_error(const char *format, ...);
void logger_warn(const char *format, ...);
void logger_info(const char *format, ...);

#endif