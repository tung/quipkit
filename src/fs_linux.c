#include "fs.h"

/* _GNU_SOURCE defined for realpath from stdlib.h */
#define _GNU_SOURCE
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int fs_AbsolutePath(const char *path, char *buf, int buf_size) {
    char *rp = realpath(path, NULL);
    if (rp == NULL) { return -1; }
    int rp_len = strlen(rp);
    if (rp_len >= buf_size) {
        free(rp);
        return -1;
    }
    strncpy(buf, rp, rp_len);
    buf[rp_len] = '\0';
    free(rp);
    return 0;
}


int fs_EnginePath(char *buf, int buf_size) {
    /* Use a temp buffer because dirname might reuse its input string. */
    char tmp_buf[buf_size];
    ssize_t len = readlink("/proc/self/exe", tmp_buf, buf_size - 1);
    if (len != -1) {
        tmp_buf[len] = '\0';
    } else {
        return 1;
    }
    /* dirname should be no longer than its input. */
    strcpy(buf, dirname(tmp_buf));
    return 0;
}


int fs_Append(char *buf, int buf_size, const char *str) {
    size_t buf_len = strlen(buf);
    size_t str_len = strlen(str);
    if (buf_len + 1 + str_len + 1 >= (size_t)buf_size) {
        return 1;
    }
    strcat(strcat(buf, "/"), str);
    return 0;
}
