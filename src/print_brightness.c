// vim:ts=4:sw=4:expandtab

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yajl/yajl_gen.h>
#include <yajl/yajl_version.h>
#include <sys/types.h>

#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "i3status.h"

#define STRING_SIZE 10

void print_brightness(brightness_ctx_t *ctx) {
    const char *walk = ctx->format;
    char *outwalk = ctx->buf;
    char *actual_brightness_buf = scalloc(ctx->max_chars * sizeof(char) + 1);
    char *max_brightness_buf = scalloc(ctx->max_chars * sizeof(char) + 1);

    int actual_fd = open(ctx->actual_brightness_path, O_RDONLY);
    int max_fd = open(ctx->max_brightness_path, O_RDONLY);

    int n = -1;
    if (actual_fd > -1 && max_fd > -1) {
        n = read(actual_fd, actual_brightness_buf, ctx->max_chars);
        if (n != -1) {
            actual_brightness_buf[n] = '\0';
        }
        (void)close(actual_fd);
        n = read(max_fd, max_brightness_buf, ctx->max_chars);
        if (n != -1) {
            max_brightness_buf[n] = '\0';
        }
        (void)close(max_fd);
        START_COLOR("color_good");
    } else {
        walk = ctx->format_bad;
        START_COLOR("color_bad");
    }

    // remove newline chars
    char *src, *dst;
    for (src = dst = actual_brightness_buf; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != '\n') {
            dst++;
        }
    }
    *dst = '\0';
    for (src = dst = max_brightness_buf; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != '\n') {
            dst++;
        }
    }
    *dst = '\0';

    char string_errno[STRING_SIZE];

    sprintf(string_errno, "%d", errno);

    placeholder_t placeholders[] = {
        {.name = "%actual_brighntess", .value = actual_brightness_buf},
        {.name = "%max_brightness", .value = max_brightness_buf},
        {.name = "%errno", .value = string_errno},
        {.name = "%error", .value = strerror(errno)}};

    const size_t num = sizeof(placeholders) / sizeof(placeholder_t);
    char *formatted = format_placeholders(walk, &placeholders[0], num);
    OUTPUT_FORMATTED;
    free(formatted);
    free(actual_brightness_buf);
    free(max_brightness_buf);

    END_COLOR;

    *outwalk = '\0';
    OUTPUT_FULL_TEXT(ctx->buf);
}
