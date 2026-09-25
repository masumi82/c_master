#ifndef CMD_H
#define CMD_H

#include <stddef.h>

#define CMD_MAX_ARGS 8

typedef enum {
    CMD_OK = 0,
    CMD_ERR_EMPTY = -1,         /* 行にトークンが1つもない */
    CMD_ERR_UNKNOWN = -2,       /* argv[0] が表にない */
    CMD_ERR_TOO_MANY_ARGS = -3, /* トークンが CMD_MAX_ARGS 個を超えた */
} cmd_status_t;

/* コマンドのハンドラ。argv[argc] は NULL。
 * out/out_len は、ハンドラが応答を書き込んでよいバッファ。 */
typedef int (*cmd_handler_t)(int argc, char *argv[], char *out, size_t out_len);

typedef struct {
    const char *name;
    cmd_handler_t handler;
} cmd_entry_t;

/* `line` をその場で ' '（連続してもよい）で区切り、名前が argv[0] と完全に
 * 一致するエントリを探して、そのハンドラを out/out_len 付きで呼ぶ。
 * ハンドラの戻り値を返す。エラーのときは負の cmd_status_t を返し、
 * その場合ハンドラは呼ばない。動的確保はしない。 */
int cmd_dispatch(const cmd_entry_t *table, size_t table_len, char *line, char *out, size_t out_len);

#endif
