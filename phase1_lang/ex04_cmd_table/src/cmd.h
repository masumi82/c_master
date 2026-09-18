#ifndef CMD_H
#define CMD_H

#include <stddef.h>

#define CMD_MAX_ARGS 8

typedef enum {
    CMD_OK = 0,
    CMD_ERR_EMPTY = -1,         /* line contains no tokens */
    CMD_ERR_UNKNOWN = -2,       /* argv[0] is not in the table */
    CMD_ERR_TOO_MANY_ARGS = -3, /* more than CMD_MAX_ARGS tokens */
} cmd_status_t;

/* A command handler. argv[argc] is NULL.
 * out/out_len is a buffer the handler may write a response into. */
typedef int (*cmd_handler_t)(int argc, char *argv[], char *out, size_t out_len);

typedef struct {
    const char *name;
    cmd_handler_t handler;
} cmd_entry_t;

/* Splits `line` in place on runs of ' ' characters, finds the entry whose
 * name exactly equals argv[0], and calls its handler with out/out_len.
 * Returns the handler's return value, or a negative cmd_status_t on error
 * (the handler is not called in that case). No dynamic allocation. */
int cmd_dispatch(const cmd_entry_t *table, size_t table_len, char *line, char *out, size_t out_len);

#endif
