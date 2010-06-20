#include <png.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>

#ifdef __MINGW32__
#   include "png.h"
#endif



/**
 * Declarations
 */

typedef struct {
    const char *filename;
    lua_State *L;
} luapng_Error;

typedef struct {
    const char *name;
    int value;
} luapng_ConstExport;

typedef enum {
    PF_UNKNOWN,
    PF_RGB,
    PF_RGBA
} luapng_PixelFormat;



/**
 * Lua PNG Helper Functions
 */

/* Handler for internal libpng errors. Pushes a string that the original function can then throw as a Lua error. */
static void ErrorHandler(png_structp png_ptr, png_const_charp message) {
    luapng_Error *error_ptr = (luapng_Error *)png_get_error_ptr(png_ptr);
    lua_pushfstring(error_ptr->L, "Internal libpng error while loading %s: %s", error_ptr->filename, message);

    jmp_buf jmpbuf;
    memcpy(jmpbuf, png_jmpbuf(png_ptr), sizeof(jmp_buf));
    longjmp(jmpbuf, 1);
}



/**
 * Exported PNG Functions
 */

/* Open a PNG file. Supports 24-bit RGB and 32-bit RGBA formats. */
/* string filename -> table image{ number width, number height, number format, userdata data } */
static int Open(lua_State *L) {
    luapng_Error error_data;
    error_data.L = L;

    const char *filename = luaL_checkstring(L, 1);
    error_data.filename = filename;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return luaL_error(L, "Could not open %s", filename);
    }

    /* Check PNG header. */
    unsigned char header[8];
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8) != 0) {
        fclose(fp);
        return luaL_error(L, "Invalid PNG header in %s", filename);
    }

    /* Create the PNG read struct, passing in the Lua state so we can push errors onto the stack. */
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)&error_data, (png_error_ptr)ErrorHandler, NULL);
    if (!png_ptr) {
        fclose(fp);
        return luaL_error(L, "Could not create PNG read struct");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return luaL_error(L, "Could not create PNG info struct");
    }

    /* libpng exception-like error handling. */
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        if (fp) {
            fclose(fp);
        }
        return lua_error(L);
    }

    png_init_io(png_ptr, fp);

    /* Skip PNG header bytes read earlier. */
    png_set_sig_bytes(png_ptr, 8);

    /* Read PNG; always get back 8-bit samples. */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_EXPAND, NULL);
    fclose(fp);
    fp = NULL;

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);

    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    if (bit_depth != 8) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return luaL_error(L, "Bit depth %d not supported", bit_depth);
    }

    int channels = png_get_channels(png_ptr, info_ptr);
    if (channels < 3) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return luaL_error(L, "Paletted image formats not supported");
    } else if (channels > 4) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return luaL_error(L, "%d-channel PNG images not supported", channels);
    }

    /* PNG table to be returned. */
    lua_createtable(L, 0, 4);

    lua_pushliteral(L, "width");
    lua_pushinteger(L, width);
    lua_rawset(L, -3);

    lua_pushliteral(L, "height");
    lua_pushinteger(L, height);
    lua_rawset(L, -3);

    lua_pushliteral(L, "format");
    lua_pushinteger(L, channels == 3 ? PF_RGB : PF_RGBA);
    lua_rawset(L, -3);

    lua_pushliteral(L, "data");
    unsigned char *image_data = (unsigned char *)lua_newuserdata(L, width * height * channels);
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    if (!row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return luaL_error(L, "Could not read PNG image data from %s", filename);
    }
    for (int y = 0; y < height; ++y) {
        memcpy(image_data + y * width * channels, row_pointers[y], width * channels);
    }
    lua_rawset(L, -3);

    lua_pushliteral(L, "data_ptr");
    lua_pushlightuserdata(L, (void *)image_data);
    lua_rawset(L, -3);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 1;
}



/**
 * Public API
 */

static const luapng_ConstExport m_png_constants[] = {
    /* Pixel formats. */
    {"RGB", PF_RGB},
    {"RGBA", PF_RGBA},
    {NULL, 0}
};

static void AddPngConstants(lua_State *L, int index) {
    const luapng_ConstExport *p;
    for (p = m_png_constants; p->name; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->value);
        lua_settable(L, index < 0 ? index - 2 : index);
    }
}


static const struct luaL_reg png_functions[] = {
    {"Open", Open},
    {NULL, NULL}
};

int
#ifdef __MINGW32__
DLL_EXPORT
#endif
luaopen_luapng(lua_State *L) {
    luaL_register(L, "PNG", png_functions);
    AddPngConstants(L, -1);
    return 1;
}

#ifdef __MINGW32__
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)hinstDLL;
    (void)fdwReason;
    (void)lpvReserved;
    return TRUE;
}
#endif
