#include "fs.h"

#include <shlwapi.h>
#include <string.h>



int fs_AbsolutePath(const char *path, char *buf, unsigned int buf_size) {
    char tmp_buf[MAX_PATH];
    if (PathIsRelative(path)) {
        if (GetFullPathName(path, MAX_PATH, tmp_buf, NULL) > MAX_PATH) { return 1; }
    } else {
        if (strlen(path) >= MAX_PATH) { return 1; }
        strcpy(tmp_buf, path);
    }
    if (strlen(tmp_buf) >= buf_size) { return 1; }
    if (!PathCanonicalize(buf, tmp_buf)) { return 1; }
    return 0;
}


int fs_EnginePath(char *buf, unsigned int buf_size) {
    if (GetModuleFileName(NULL, buf, buf_size) == buf_size) { return 1; }
    char *last_sep = strrchr(buf, '\\');
    if (last_sep) { *last_sep = '\0'; }
    return 0;
}


int fs_Append(char *buf, unsigned int buf_size, const char *str) {
    unsigned int buf_len = strlen(buf);
    unsigned int str_len = strlen(str);
    if (buf_len + 1 + str_len + 1 >= buf_size) { return 1; }
    strcat(strcat(buf, "\\"), str);
    return 0;
}


int fs_ChDir(const char *path) {
    return SetCurrentDirectory(path) ? 0 : 1;
}
