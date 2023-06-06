/* 
 * minimal pico-sdk lua example
 *
 * Copyright 2023 Jeremy Grosser <jeremy@synack.me>
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>

#include "hardware/watchdog.h"
#include "pico/stdlib.h"
#include "pico/bootrom.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define PROMPT "lua> "

// reset()
static int l_reset(lua_State *L) {
    watchdog_reboot(0, 0, 0);
    return 0;
}

// bootsel()
static int l_bootsel(lua_State *L) {
    reset_usb_boot(0, 0);
    return 0;
}

// set_output(pin, bool)
static int l_set_output(lua_State *L) {
    int pin = lua_tointeger(L, 1);
    int output = lua_toboolean(L, 2);
    lua_pop(L, 2);
    gpio_init(pin);
    gpio_set_dir(pin, output);
    return 0;
}

// set_pin(pin, bool)
static int l_set_pin(lua_State *L) {
    int pin = lua_tointeger(L, 1);
    int state = lua_toboolean(L, 2);
    lua_pop(L, 2);
    gpio_put(pin, state == 1);
    return 0;
}

// bool get_pin(pin)
static int l_get_pin(lua_State *L) {
    int pin = lua_tointeger(L, 1);
    int state = gpio_get(pin);
    lua_pop(L, 1);
    lua_pushboolean(L, state);
    return 1;
}

int main() {
    lua_State *L;
    luaL_Buffer buf;
    int status;
    size_t len;
    char ch;

    stdio_init_all();

    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_buffinit(L, &buf);

    lua_register(L, "reset", l_reset);
    lua_register(L, "bootsel", l_bootsel);
    lua_register(L, "set_output", l_set_output);
    lua_register(L, "set_pin", l_set_pin);
    lua_register(L, "get_pin", l_get_pin);

    printf("\n*** picolua\n Ctrl-C  Clear buffer\n Ctrl-D  Execute buffer\n Ctrl-L  Clear screen\n\n" PROMPT);

    while(1) {
        ch = (char)getchar();
        if(ch == '\r') {
            ch = '\n';
        }

        if(ch == 0x7F || ch == 0x08) { // DEL or BS
            if(luaL_bufflen(&buf) > 0) {
                luaL_buffsub(&buf, 1);
                printf("\b \b");
            }
        }else if(ch == 0x0C) { // Ctrl-L (ANSI clear screen)
            printf("\x1b[2J\x1b[1;1H" PROMPT);
        }else if(ch == 0x03) { // Ctrl-C (clear buffer without executing)
            luaL_buffinit(L, &buf);
            printf("\n" PROMPT);
        }else if(ch == 0x04) { // Ctrl-D
            luaL_pushresult(&buf);
            const char *s = lua_tolstring(L, -1, &len);
            status = luaL_loadbuffer(L, s, len, "picolua");
            if(status != LUA_OK) {
                const char *msg = lua_tostring(L, -1);
                lua_writestringerror("parse error: %s\n", msg);
            }else{
                status = lua_pcall(L, 0, 0, 0);
                if(status != LUA_OK) {
                    const char *msg = lua_tostring(L, -1);
                    lua_writestringerror("execute error: %s\n", msg);
                }
            }

            lua_pop(L, 1);
            luaL_buffinit(L, &buf);
            printf(PROMPT);
        }else if((ch >= 0x20 && ch < 0x7F) || ch == '\t' || ch == '\n') { // [ \t\na-zA-z]
            putchar(ch);
            luaL_addchar(&buf, ch);
        }
    }

    lua_close(L);
    return 0;
}
