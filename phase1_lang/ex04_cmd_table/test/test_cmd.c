#include <stdbool.h>
#include <stdio.h>

#include "cmd.h"
#include "unity.h"

/* スパイ: どう呼ばれたかを記録する。 */
static int spy_calls;
static int spy_argc;
static char spy_args[CMD_MAX_ARGS][32];
static bool spy_argv_null_terminated;
static char *spy_out;
static size_t spy_out_len;

static int spy_handler(int argc, char *argv[], char *out, size_t out_len)
{
    spy_calls++;
    spy_argc = argc;
    for (int i = 0; i < argc && i < CMD_MAX_ARGS; i++) {
        snprintf(spy_args[i], sizeof spy_args[i], "%s", argv[i]);
    }
    spy_argv_null_terminated = (argv[argc] == NULL);
    spy_out = out;
    spy_out_len = out_len;
    return 42;
}

static int other_handler(int argc, char *argv[], char *out, size_t out_len)
{
    (void)argc;
    (void)argv;
    (void)out;
    (void)out_len;
    return 7;
}

static const cmd_entry_t table[] = {
    {"led", spy_handler},
    {"reset", other_handler},
};
#define TABLE_LEN (sizeof table / sizeof table[0])

void setUp(void)
{
    spy_calls = 0;
    spy_argc = -1;
    spy_argv_null_terminated = false;
    spy_out = NULL;
    spy_out_len = 0;
}

void tearDown(void) {}

static void test_dispatch_calls_handler_with_args(void)
{
    char line[] = "led on 3";
    char out[16];

    TEST_ASSERT_EQUAL_INT(42, cmd_dispatch(table, TABLE_LEN, line, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(1, spy_calls);
    TEST_ASSERT_EQUAL_INT(3, spy_argc);
    TEST_ASSERT_EQUAL_STRING("led", spy_args[0]);
    TEST_ASSERT_EQUAL_STRING("on", spy_args[1]);
    TEST_ASSERT_EQUAL_STRING("3", spy_args[2]);
    TEST_ASSERT_TRUE(spy_argv_null_terminated);
    TEST_ASSERT_EQUAL_PTR(out, spy_out);
    TEST_ASSERT_EQUAL_size_t(sizeof out, spy_out_len);
}

static void test_dispatch_selects_matching_entry(void)
{
    char line[] = "reset";
    char out[16];

    TEST_ASSERT_EQUAL_INT(7, cmd_dispatch(table, TABLE_LEN, line, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(0, spy_calls);
}

static void test_dispatch_collapses_repeated_spaces(void)
{
    char line[] = "  led   on  ";
    char out[16];

    TEST_ASSERT_EQUAL_INT(42, cmd_dispatch(table, TABLE_LEN, line, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(2, spy_argc);
    TEST_ASSERT_EQUAL_STRING("led", spy_args[0]);
    TEST_ASSERT_EQUAL_STRING("on", spy_args[1]);
}

static void test_dispatch_empty_line(void)
{
    char empty[] = "";
    char spaces[] = "   ";
    char out[16];

    TEST_ASSERT_EQUAL_INT(CMD_ERR_EMPTY, cmd_dispatch(table, TABLE_LEN, empty, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(CMD_ERR_EMPTY, cmd_dispatch(table, TABLE_LEN, spaces, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(0, spy_calls);
}

static void test_dispatch_unknown_command(void)
{
    char unknown[] = "blink 1";
    char prefix[] = "le";
    char longer[] = "ledx";
    char out[16];

    TEST_ASSERT_EQUAL_INT(CMD_ERR_UNKNOWN, cmd_dispatch(table, TABLE_LEN, unknown, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(CMD_ERR_UNKNOWN, cmd_dispatch(table, TABLE_LEN, prefix, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(CMD_ERR_UNKNOWN, cmd_dispatch(table, TABLE_LEN, longer, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(0, spy_calls);
}

static void test_dispatch_empty_table(void)
{
    char line[] = "led";
    char out[16];

    TEST_ASSERT_EQUAL_INT(CMD_ERR_UNKNOWN, cmd_dispatch(table, 0, line, out, sizeof out));
}

static void test_dispatch_max_args(void)
{
    char ok[] = "led 1 2 3 4 5 6 7";
    char too_many[] = "led 1 2 3 4 5 6 7 8";
    char out[16];

    TEST_ASSERT_EQUAL_INT(42, cmd_dispatch(table, TABLE_LEN, ok, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(CMD_MAX_ARGS, spy_argc);
    TEST_ASSERT_EQUAL_STRING("7", spy_args[CMD_MAX_ARGS - 1]);
    TEST_ASSERT_TRUE(spy_argv_null_terminated);

    spy_calls = 0;
    TEST_ASSERT_EQUAL_INT(CMD_ERR_TOO_MANY_ARGS, cmd_dispatch(table, TABLE_LEN, too_many, out, sizeof out));
    TEST_ASSERT_EQUAL_INT(0, spy_calls);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_dispatch_calls_handler_with_args);
    RUN_TEST(test_dispatch_selects_matching_entry);
    RUN_TEST(test_dispatch_collapses_repeated_spaces);
    RUN_TEST(test_dispatch_empty_line);
    RUN_TEST(test_dispatch_unknown_command);
    RUN_TEST(test_dispatch_empty_table);
    RUN_TEST(test_dispatch_max_args);
    return UNITY_END();
}
