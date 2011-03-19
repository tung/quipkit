/**
 * See copyright notice in doc/COPYRIGHT.
 */

#include "texture.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <lua.h>
#include <lauxlib.h>
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



/* Draws all of src onto dest, alpha and all. */
/* Returns 0 on success, -1 on failure. */
static int RawBlit(SDL_Surface *src, SDL_Surface *dest, int *has_alpha) {
    if (SDL_MUSTLOCK(src) && SDL_LockSurface(src) == -1) return -1;
    if (SDL_MUSTLOCK(dest) && SDL_LockSurface(dest) == -1) {
        if (SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
        return -1;
    }

    /* Detect with a local variable so we're not deref'ing has_alpha every pixel. */
    int has_alpha_local = 0;

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
                if (!has_alpha_local) has_alpha_local = 1;
                continue;
            }
            SDL_GetRGBA(src_pixel, src->format, &r, &g, &b, &a);
            if (!has_alpha_local && a < 255) has_alpha_local = 1;

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

    *has_alpha = has_alpha_local;

    if (SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
    if (SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
    return 0;
}


/* Bind the texture, enabling blending if needed. */
static int TextureBind(lua_State *L) {
    sdlgl_Texture *tex = luaL_checkudata(L, 1, "sdlgl.texture");
    glBindTexture(GL_TEXTURE_2D, tex->texId);
    if (tex->texHasAlpha) glEnable(GL_BLEND);
    return 0;
}


/* __gc finaliser metamethod of sdlgl.texture userdata. */
static int TextureGc(lua_State *L) {
    sdlgl_Texture *tex = luaL_checkudata(L, 1, "sdlgl.texture");
    GLuint textures[1];
    textures[0] = tex->texId;
    glDeleteTextures(1, textures);
    return 0;
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
    int full_w = surface->w < 64 ? 64 : surface->w;
    int full_h = surface->h < 64 ? 64 : surface->h;
    proxy = SDL_CreateRGBSurface(SDL_SWSURFACE, full_w, full_h, 32,
                                 RMASK, GMASK, BMASK, AMASK);
    if (proxy == NULL) {
        return luaL_error(L, "SDL_CreateRGBSurface failed: %s", SDL_GetError());
    }

    int has_alpha;
    if (RawBlit(surface, proxy, &has_alpha) == -1) {
        SDL_FreeSurface(proxy);
        return luaL_error(L, "cannot convert SDL_Surface to needed format");
    }

    /* Load the pixels into an OpenGL texture. */
    GLuint textures[1];
    glGenTextures(1, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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
    tex->texHasAlpha = has_alpha;
    luaL_getmetatable(L, "sdlgl.texture");
    lua_setmetatable(L, -2);

    return 1;
}


/* Unbind the texture, disabling blending if needed. */
static int TextureUnbind(lua_State *L) {
    sdlgl_Texture *tex = luaL_checkudata(L, 1, "sdlgl.texture");
    if (tex->texHasAlpha) glDisable(GL_BLEND);
    return 0;
}


/* __index metamethod of sdlgl.texture userdata. */
/* Out of alphabetical order because of method declaration dependencies above. */
static int TextureIndex(lua_State *L) {
    sdlgl_Texture *tex = luaL_checkudata(L, 1, "sdlgl.texture");
    const char *key = luaL_checkstring(L, 2);
    if (strncmp(key, "bind", 5) == 0) {
        lua_pushcfunction(L, TextureBind);
    } else if (strncmp(key, "unbind", 7) == 0) {
        lua_pushcfunction(L, TextureUnbind);
    } else if (strncmp(key, "w", 2) == 0) {
        lua_pushinteger(L, tex->w);
    } else if (strncmp(key, "h", 2) == 0) {
        lua_pushinteger(L, tex->h);
    } else if (strncmp(key, "texId", 6) == 0) {
        lua_pushinteger(L, tex->texId);
    } else if (strncmp(key, "texW", 5) == 0) {
        lua_pushinteger(L, tex->texW);
    } else if (strncmp(key, "texH", 5) == 0) {
        lua_pushinteger(L, tex->texH);
    } else {
        lua_pushnil(L);
    }
    return 1;
}


static const struct luaL_reg sdlgl_TextureMethods[] = {
    {"__gc", TextureGc},
    {"__index", TextureIndex},
    {"new", TextureNew},
    {NULL, NULL}
};

/* Adds sdlgl.texture to the sdlgl module table. */
void luaopen_sdlgl_texture(lua_State *L, int module_index) {
    if (module_index < 0) { module_index = lua_gettop(L) + module_index + 1; }
    lua_pushliteral(L, "texture");
    luaL_newmetatable(L, "sdlgl.texture");
    luaL_register(L, NULL, sdlgl_TextureMethods);
    lua_rawset(L, module_index);
}
