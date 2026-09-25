#define _POSIX_C_SOURCE 200809L

#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* `gpioinfo` で確かめる: "GPIO17" という名前のラインを持つチップ。 */
#define CHIP_PATH "/dev/gpiochip0"
/* BCM 番号の GPIO17 = 物理ピンの 11 番。 */
#define LED_OFFSET 17u
#define BLINK_COUNT 10
#define HALF_PERIOD_MS 500L

static void sleep_ms(long ms)
{
    struct timespec ts = {.tv_sec = ms / 1000L, .tv_nsec = (ms % 1000L) * 1000000L};
    nanosleep(&ts, NULL);
}

/* LED を BLINK_COUNT 回点滅させる（点灯と消灯で 1 回）。それぞれ HALF_PERIOD_MS 待つ。
 * 成功したら 0、値の設定に失敗したら -1 を返す。 */
static int blink(struct gpiod_line_request *request, unsigned int offset)
{
    /* TODO: gpiod_line_request_set_value() と sleep_ms() で実装する */
    (void)request;
    (void)offset;
    return 0;
}

int main(void)
{
    int ret = EXIT_FAILURE;
    unsigned int offset = LED_OFFSET;
    struct gpiod_chip *chip = NULL;
    struct gpiod_line_settings *settings = NULL;
    struct gpiod_line_config *line_cfg = NULL;
    struct gpiod_request_config *req_cfg = NULL;
    struct gpiod_line_request *request = NULL;

    chip = gpiod_chip_open(CHIP_PATH);
    if (chip == NULL) {
        perror("gpiod_chip_open");
        goto out;
    }

    settings = gpiod_line_settings_new();
    line_cfg = gpiod_line_config_new();
    req_cfg = gpiod_request_config_new();
    if (settings == NULL || line_cfg == NULL || req_cfg == NULL) {
        perror("gpiod_*_new");
        goto out;
    }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);
    if (gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings) != 0) {
        perror("gpiod_line_config_add_line_settings");
        goto out;
    }
    gpiod_request_config_set_consumer(req_cfg, "blink");

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (request == NULL) {
        perror("gpiod_chip_request_lines");
        goto out;
    }

    if (blink(request, offset) == 0) {
        ret = EXIT_SUCCESS;
    }

out:
    if (request != NULL) {
        gpiod_line_request_release(request);
    }
    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);
    if (chip != NULL) {
        gpiod_chip_close(chip);
    }
    return ret;
}
