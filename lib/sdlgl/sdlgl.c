#include <GL/gl.h>
#include <GL/glu.h>
#include <lua.h>
#include <lauxlib.h>
#include <math.h>
#include <SDL.h>



#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#   define RMASK 0xff000000
#   define GMASK 0x00ff0000
#   define BMASK 0x0000ff00
#   define AMASK 0x000000ff
#else
#   define RMASK 0x000000ff
#   define GMASK 0x0000ff00
#   define BMASK 0x00ff0000
#   define AMASK 0xff000000
#endif

typedef struct {
    int w, h;
    GLuint texId;
    int texW, texH;
} sdlgl_Texture;



/**
 * sdlgl Helper Functions
 */

/* Draws all of src onto dest, alpha and all. */
/* Returns 0 on success, -1 on failure. */
static int RawBlit(SDL_Surface *src, SDL_Surface *dest) {
    if (SDL_MUSTLOCK(src) && SDL_LockSurface(src) == -1) return -1;
    if (SDL_MUSTLOCK(dest) && SDL_LockSurface(dest) == -1) {
        if (SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
        return -1;
    }

    int min_w = (src->w < dest->w) ? src->w : dest->w;
    int min_h = (src->h < dest->h) ? src->h : dest->h;

    Uint8 r, g, b, a;

    int use_key = src->flags & SDL_SRCCOLORKEY;
    Uint32 key = src->format->colorkey;

    /* No bounds checking, so this may segfault. */
    for (int y = 0; y < min_h; ++y) {
        for (int x = 0; x < min_w; ++x) {
            /* Get pixel from src. */
            Uint32 src_pixel;
            int src_bpp = src->format->BytesPerPixel;
            Uint8 *src_p = (Uint8 *)src->pixels + y * src->pitch + x * src_bpp;
            switch (src_bpp) {
                case 1: src_pixel = *src_p; break;
                case 2: src_pixel = *(Uint16 *)src_p; break;
                case 3:
#               if SDL_BYTEORDER == SDL_BIG_ENDIAN
                    src_pixel = src_p[0] << 16 | src_p[1] << 8 | src_p[2];
#               else
                    src_pixel = src_p[0] | src_p[1] << 8 | src_p[2] << 16;
#               endif
                    break;
                case 4: src_pixel = *(Uint32 *)src_p; break;
                default: src_pixel = 0;
            }
            if (use_key && src_pixel == key) {
                continue;
            }
            SDL_GetRGBA(src_pixel, src->format, &r, &g, &b, &a);

            /* Put pixel to dest. */
            Uint32 dest_pixel = SDL_MapRGBA(dest->format, r, g, b, a);
            int dest_bpp = dest->format->BytesPerPixel;
            Uint8 *dest_p = (Uint8 *)dest->pixels + y * dest->pitch + x * dest_bpp;
            switch (dest_bpp) {
                case 1: *dest_p = dest_pixel; break;
                case 2: *(Uint16 *)dest_p = dest_pixel; break;
                case 3:
#               if SDL_BYTEORDER == SDL_BIG_ENDIAN
                    dest_p[0] = (dest_pixel >> 16) & 0xff;
                    dest_p[1] = (dest_pixel >> 8) & 0xff;
                    dest_p[2] = dest_pixel & 0xff;
#               else
                    dest_p[0] = dest_pixel & 0xff;
                    dest_p[1] = (dest_pixel >> 8) & 0xff;
                    dest_p[2] = (dest_pixel >> 16) & 0xff;
#               endif
                    break;
                case 4: *(Uint32 *)dest_p = dest_pixel; break;
                default: break;
            }
        }
    }

    if (SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
    if (SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
    return 0;
}



/**
 * Exported sdlgl functions
 */

/* __gc finaliser metamethod of sdlgl.texture userdata. */
static int TextureGc(lua_State *L) {
    sdlgl_Texture *tex = luaL_checkudata(L, 1, "sdlgl.texture");
    GLuint textures[1];
    textures[0] = tex->texId;
    glDeleteTextures(1, textures);
    return 0;
}


/* __index metamethod of sdlgl.texture userdata. */
static int TextureIndex(lua_State *L) {
    sdlgl_Texture *tex = luaL_checkudata(L, 1, "sdlgl.texture");
    const char *key = luaL_checkstring(L, 2);
    if (strncmp(key, "w", 1) == 0) {
        lua_pushinteger(L, tex->w);
    } else if (strncmp(key, "h", 1) == 0) {
        lua_pushinteger(L, tex->h);
    } else if (strncmp(key, "texId", 5) == 0) {
        lua_pushinteger(L, tex->texId);
    } else if (strncmp(key, "texW", 4) == 0) {
        lua_pushinteger(L, tex->texW);
    } else if (strncmp(key, "texH", 4) == 0) {
        lua_pushinteger(L, tex->texH);
    } else {
        lua_pushnil(L);
    }
    return 1;
}


/* Constructor for sdlgl.texture userdatum. Call like sdlgl.texture:new(my_sdl_surface). */
/* userdatum<SDL_Surface **> -> userdatum<sdlgl.texture> */
static int TextureNew(lua_State *L) {
    if (!lua_isuserdata(L, 2)) {
        return luaL_error(L, "expected userdatum at arg 2, got %s", lua_typename(L, 2));
    }
    void *ud = lua_touserdata(L, 2);
    if (ud == NULL) {
        return luaL_error(L, "cannot convert null SDL_Surface at arg 2");
    }
    SDL_Surface *surface = *((SDL_Surface **)ud);

    /* Import the pixels with the right dimensions and format. */
    SDL_Surface *proxy;
    int full_w = (int)pow(2, ceil(log(surface->w) / log(2)));
    int full_h = (int)pow(2, ceil(log(surface->h) / log(2)));
    proxy = SDL_CreateRGBSurface(SDL_SWSURFACE, full_w, full_h, 32,
                                 RMASK, GMASK, BMASK, AMASK);
    if (proxy == NULL) {
        return luaL_error(L, "SDL_CreateRGBSurface failed: %s", SDL_GetError());
    }
    if (RawBlit(surface, proxy) == -1) {
        SDL_FreeSurface(proxy);
        return luaL_error(L, "cannot convert SDL_Surface to needed format");
    }

    /* Load the pixels into an OpenGL texture. */
    GLuint textures[1];
    glGenTextures(1, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    if (SDL_MUSTLOCK(proxy) && SDL_LockSurface(proxy) == -1) {
        glDeleteTextures(1, textures);
        SDL_FreeSurface(proxy);
        return luaL_error(L, "cannot lock proxy SDL_Surface for conversion");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, full_w, full_h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, proxy->pixels);
    SDL_FreeSurface(proxy);

    /* Check for OpenGL errors. */
    GLenum gl_error = glGetError();
    if (gl_error != GL_NO_ERROR) {
        glDeleteTextures(1, textures);
        return luaL_error(L, "OpenGL error: %s", gluErrorString(gl_error));
    }

    /* Create the sdlgl.texture userdatum. */
    sdlgl_Texture *tex;
    tex = (sdlgl_Texture *)lua_newuserdata(L, sizeof(sdlgl_Texture));
    if (tex == NULL) {
        glDeleteTextures(1, textures);
        return luaL_error(L, "cannot allocate new sdlgl.texture userdatum");
    }
    tex->w = surface->w;
    tex->h = surface->h;
    tex->texId = textures[0];
    tex->texW = full_w;
    tex->texH = full_h;
    luaL_getmetatable(L, "sdlgl.texture");
    lua_setmetatable(L, -2);

    return 1;
}


static int CheckGlError(lua_State *L) {
    GLenum gl_error = glGetError();
    if (gl_error != GL_NO_ERROR)
        return luaL_error(L, "OpenGL error: %s", gluErrorString(gl_error));
    return 0;
}



/**
 * Public API
 */

static const struct luaL_reg sdlgl_TextureMethods[] = {
    {"__gc", TextureGc},
    {"__index", TextureIndex},
    {"new", TextureNew},
    {NULL, NULL}
};

int luaopen_sdlgl(lua_State *L) {
    lua_createtable(L, 0, 1);

    lua_pushliteral(L, "texture");
    luaL_newmetatable(L, "sdlgl.texture");
    luaL_register(L, NULL, sdlgl_TextureMethods);
    lua_rawset(L, -3);

    lua_pushliteral(L, "checkGlError");
    lua_pushcfunction(L, CheckGlError);
    lua_rawset(L, -3);

    lua_setglobal(L, "sdlgl");
    lua_getglobal(L, "sdlgl");
    return 1;
}
