#ifndef AERIALFLAPPERS_H
#define	AERIALFLAPPERS_H

#include "common.h"

// Game functions
static void init_game();
static void init_player();
static void render_game();
static void render_header();
static void render_gui();

static void update_player();
static void render_player();

static void update_pillar(pillar_t *pillar);
static void render_pillar(pillar_t *pillar);

#endif

