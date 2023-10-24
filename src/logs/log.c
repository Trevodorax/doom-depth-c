#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>

#ifdef _WIN32

#include <direct.h>
#include <time.h>

#define mkdir(path, mode) _mkdir(path)
#endif

#include "log.h"

void create_log_directory() {
    struct stat st = {0};
    if (stat("../logs", &st) == -1) {
        mkdir("../logs", 0700);
    }
}

logger_t *new_logger() {
    logger_t *logger = (logger_t *) malloc(sizeof(logger_t));
    if (logger == NULL) {
        return NULL;
    }
    logger->error_stream = NULL;
    logger->warn_stream = NULL;
    logger->info_stream = NULL;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    char date_buffer[25];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d_%H-%M", tm_info);

    char log_filename[40];
    snprintf(log_filename, sizeof(log_filename), "%s.log", date_buffer);

    // By default, use the same file for all logs
    logger_set_outputs(logger, log_filename, log_filename, log_filename);

    logger->error = logger_error;
    logger->warn = logger_warn;
    logger->info = logger_info;

    return logger;
}

void close_outputs(logger_t *logger) {
    if (logger->error_stream != NULL && logger->error_stream != stderr && logger->error_stream != stdout) {
        fclose(logger->error_stream);
        logger->error_stream = NULL;
    }
    if (logger->warn_stream != NULL && logger->warn_stream != stderr && logger->warn_stream != stdout) {
        fclose(logger->warn_stream);
        logger->warn_stream = NULL;
    }
    if (logger->info_stream != NULL && logger->info_stream != stderr && logger->info_stream != stdout) {
        fclose(logger->info_stream);
        logger->info_stream = NULL;
    }
}

void logger_free(logger_t *logger) {
    if (logger != NULL) {
        close_outputs(logger);
        free(logger);
    }
}

void logger_set_outputs(logger_t *logger, char *error_stream_path, char *warning_stream_path, char *info_stream_path) {
    close_outputs(logger);
    create_log_directory();

    char error_full_path[512];
    char warning_full_path[512];
    char info_full_path[512];

    sprintf(error_full_path, "../logs/%s", error_stream_path);
    sprintf(warning_full_path, "../logs/%s", warning_stream_path);
    sprintf(info_full_path, "../logs/%s", info_stream_path);

    if (logger != NULL) {
        logger->error_stream = fopen(error_full_path, "a");
        if (logger->error_stream == NULL) {
            perror("Couldn't open error log file");
        }

        logger->warn_stream = fopen(warning_full_path, "a");
        if (logger->warn_stream == NULL) {
            perror("Couldn't open warn log file");
        }

        logger->info_stream = fopen(info_full_path, "a");
        if (logger->info_stream == NULL) {
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

void logger_error(logger_t *logger, const char *format, ...) {
    va_list args;
            va_start(args, format);
    logger_log(logger->error_stream, "ERROR", format, args);
            va_end(args);
}

void logger_warn(logger_t *logger, const char *format, ...) {
    va_list args;
            va_start(args, format);
    logger_log(logger->warn_stream, "WARNING", format, args);
            va_end(args);
}

void logger_info(logger_t *logger, const char *format, ...) {
    va_list args;
            va_start(args, format);
    logger_log(logger->info_stream, "INFO", format, args);
            va_end(args);
}