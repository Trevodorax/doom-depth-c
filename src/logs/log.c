#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>

#ifdef _WIN32

#include <direct.h>

#define mkdir(path, mode) _mkdir(path)
#endif

#include <time.h>
#include "log.h"

logger_t *global_logger = NULL;

void create_log_directory() {
    struct stat st = {0};
    if (stat("../logs", &st) == -1) {
        mkdir("../logs", 0700);
    }
}

logger_t *new_logger() {
    global_logger = (logger_t *) malloc(sizeof(logger_t));
    if (global_logger == NULL) {
        return NULL;
    }
    global_logger->error_stream = NULL;
    global_logger->warn_stream = NULL;
    global_logger->info_stream = NULL;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    char date_buffer[25];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d_%H-%M", tm_info);

    char log_filename[40];
    snprintf(log_filename, sizeof(log_filename), "%s.log", date_buffer);

    // By default, use the same file for all logs
    logger_set_outputs(log_filename, log_filename, log_filename);

    global_logger->error = logger_error;
    global_logger->warn = logger_warn;
    global_logger->info = logger_info;

    return global_logger;
}

void init_global_logger() {
    if (global_logger == NULL) {
        global_logger = new_logger();
    }
}

void close_outputs() {
    if (global_logger->error_stream != NULL && global_logger->error_stream != stderr && global_logger->error_stream != stdout) {
        fclose(global_logger->error_stream);
        global_logger->error_stream = NULL;
    }
    if (global_logger->warn_stream != NULL && global_logger->warn_stream != stderr && global_logger->warn_stream != stdout) {
        fclose(global_logger->warn_stream);
        global_logger->warn_stream = NULL;
    }
    if (global_logger->info_stream != NULL && global_logger->info_stream != stderr && global_logger->info_stream != stdout) {
        fclose(global_logger->info_stream);
        global_logger->info_stream = NULL;
    }
}

void logger_free() {
    if (global_logger != NULL) {
        close_outputs();
        free(global_logger);
    }
}

void logger_set_outputs(char *error_stream_path, char *warning_stream_path, char *info_stream_path) {
    close_outputs();
    create_log_directory();

    char error_full_path[512];
    char warning_full_path[512];
    char info_full_path[512];

    sprintf(error_full_path, "../logs/%s", error_stream_path);
    sprintf(warning_full_path, "../logs/%s", warning_stream_path);
    sprintf(info_full_path, "../logs/%s", info_stream_path);

    if (global_logger != NULL) {
        global_logger->error_stream = fopen(error_full_path, "a");
        if (global_logger->error_stream == NULL) {
            perror("Couldn't open error log file");
        }

        global_logger->warn_stream = fopen(warning_full_path, "a");
        if (global_logger->warn_stream == NULL) {
            perror("Couldn't open warn log file");
        }

        global_logger->info_stream = fopen(info_full_path, "a");
        if (global_logger->info_stream == NULL) {
            perror("Couldn't open info log file");
        }
    }
}

void logger_log(FILE *stream, const char *log_level, const char *format, va_list args) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_buffer[9]; // hh:mm:ss\0

    strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", tm_info);
    fprintf(stream, "[%s][DoomDepths] %s - ", time_buffer, log_level);
    vfprintf(stream, format, args);
    fprintf(stream, "\n");
    fflush(stream);
}

void logger_error(const char *format, ...) {
    va_list args;
            va_start(args, format);
    logger_log(global_logger->error_stream, "ERROR", format, args);
            va_end(args);
}

void logger_warn(const char *format, ...) {
    va_list args;
            va_start(args, format);
    logger_log(global_logger->warn_stream, "WARNING", format, args);
            va_end(args);
}

void logger_info(const char *format, ...) {
    va_list args;
            va_start(args, format);
    logger_log(global_logger->info_stream, "INFO", format, args);
            va_end(args);
}