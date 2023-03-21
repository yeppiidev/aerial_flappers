#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"

#define PAGE_MAIN 0
#define PAGE_SETTINGS 1
#define PAGE_ABOUT 2

#define NAVBAR_COLOR 0x5ACB
#define PLAYER_COLOR 0xF4E0

#define PLAYER_SIZE 16
#define PLAYER_GRAVITY 5.2
#define PILLAR_WIDTH 32
#define PILLAR_SPEED 4
#define JUMP_VELOCITY -1.5
#define PILLAR_MIN_HEIGHT 64
#define PILLAR_MAX_HEIGHT 128

#define PLAYER_DEFAULT_POS_HORIZ 16 + PLAYER_SIZE
#define PLAYER_DEFAULT_POS_VERT (SCREEN_HEIGHT / 2) - PLAYER_SIZE / 2

typedef struct player_t {
    VMINT x;
    VMINT y;
    double velocity;
    double jump_velocity;
    VMINT score;
    VMINT dead;
} player_t;

typedef struct pillar_t {
    VMINT x;
    VMINT y;
    VMINT pillarHeight;
    VMINT position;
    VMINT id;
} pillar_t;

// Images
VMUINT8 *i_bgBuffer;
VMUINT8 *i_temp;
VMUINT8 *layer_buf;

// Layer handle
VMINT layer_hdl[1];

// Game update timer
VMINT gameUpdateTimer;

// String resources
VMWSTR s_appName;
VMWSTR s_gameOver;
VMWSTR s_actionLeft;
VMWSTR s_actionRight;

// Dynamic strings
VMSTR d_scoreStr;

// Globals
VMINT headerSize;
VMINT navBarSize;
VMINT i_tempSize;

// Deltatime
VMFLOAT delta;
VMINT32 time;

vm_graphic_color color;

player_t player;

pillar_t pillarTop1;
pillar_t pillarBot1;

pillar_t pillarTop2;
pillar_t pillarBot2;

#endif