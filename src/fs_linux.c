#include "fs.h"

#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>


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
