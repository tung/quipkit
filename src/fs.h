#ifndef QK_FS_H
#define QK_FS_H


/**
 * Turn the given path into an absolute path to be stored in buf.
 */
int fs_AbsolutePath(const char *path, char *buf, unsigned int buf_size);

/**
 * Store the path to the engine in buf, without the trailing separator.
 */
int fs_EnginePath(char *buf, unsigned int buf_size);

/**
 * Add a string to the end of the path in buf, with a separator if needed.
 */
int fs_Append(char *buf, unsigned int buf_size, const char *str);

/**
 * Change the working directory.
 */
int fs_ChDir(const char *path);


#endif
