#include "events.h"
#include "globals.h"
#include "helpers.h"
#include "logger.h"

#include "AerialFlappers.h"

#include "ResID.h"

#include <math.h>

void vm_main(void) {
    layer_hdl[0] = -1;

    vm_reg_sysevt_callback(on_sys_event);
    vm_reg_keyboard_callback(on_key_event);
    vm_reg_pen_callback(on_pen_event);

    vm_res_init();

    init_game();
}

void on_sys_event(VMINT message, VMINT param) {
    switch (message) {
    case VM_MSG_CREATE:
    case VM_MSG_ACTIVE:
        layer_hdl[0] = vm_graphic_create_layer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -1);

        vm_graphic_set_clip(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        break;

    case VM_MSG_PAINT:
        vm_create_timer(1, on_timer_tick);
        break;

    case VM_MSG_INACTIVE:
        if (layer_hdl[0] != -1)
            vm_graphic_delete_layer(layer_hdl[0]);

        break;
    case VM_MSG_QUIT:
        if (layer_hdl[0] != -1)
            vm_graphic_delete_layer(layer_hdl[0]);

        vm_res_deinit();
        break;
    }
}

void on_key_event(VMINT event, VMINT keycode) {
    if (event == VM_KEY_EVENT_DOWN) {
        switch (keycode) {
        case VM_KEY_OK:
        case VM_KEY_NUM5:
        case VM_KEY_LEFT_SOFTKEY:
            if (player.dead) {
                init_player();
                break;
            }

            player.velocity = 10;
            player.jump_velocity = JUMP_VELOCITY;
            player.y -= 10;

            break;
        case VM_KEY_RIGHT_SOFTKEY:
            vm_exit_app();
            break;
        default:
            break;
        }
    }
}

void on_pen_event(VMINT event, VMINT x, VMINT y) {
    if (layer_hdl[0] != -1) {
        vm_graphic_delete_layer(layer_hdl[0]);
        layer_hdl[0] = -1;
    }
}

static void init_game() {
    vm_log_debug("Initializing game...");

    s_appName = get_str(APP_NAME);
    s_gameOver = get_str(GAME_OVER);
    s_actionLeft = get_str(ACTION_LEFT);
    s_actionRight = get_str(ACTION_RIGHT);

    navBarSize = 24;
    headerSize = 18;

    init_player();

    pillarTop1.pillarHeight = 50;
    pillarTop1.id = 40;
    pillarTop1.position = 0;
    pillarTop1.x = 0;

    pillarBot1.pillarHeight = 30;
    pillarBot1.id = 40;
    pillarBot1.position = 1;
    pillarBot1.x = 0;

    pillarTop2.pillarHeight = 50;
    pillarTop2.id = 210;
    pillarTop2.position = 0;
    pillarTop2.x = 0;

    pillarBot2.pillarHeight = 30;
    pillarBot2.id = 210;
    pillarBot2.position = 1;
    pillarBot1.x = 0;

    time = vm_get_tick_count();
}

static void init_player() {
    player.dead = 0;
    player.score = 0;
    player.x = SCREEN_WIDTH / 2 - PLAYER_SIZE / 2;
    player.y = SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2;
}

void on_timer_tick(VMINT timerId) {
    render_game();
    render_gui();

    vm_graphic_flush_layer(layer_hdl, 1);
}

static VMINT player_collides_pillar(player_t player, pillar_t pillar) {
    return ((player.x > pillar.x && player.x < pillar.x + PILLAR_WIDTH) &&
            (player.y > pillar.y && player.y < pillar.y + pillar.pillarHeight));
}

static void render_header() {
    VMWSTR str;
    sprintf(d_scoreStr, "Score: %d", player.score);
    vm_ascii_to_ucs2((VMWSTR)str, MRE_STRING_MAX_SIZE, d_scoreStr);

    gfx_color(VM_COLOR_WHITE);
    vm_graphic_set_font(VM_SMALL_FONT);
    gfx_text(10, 10, (VMWSTR)str);
}

static void render_gui() {
    VMINT navPad = 2;

    // Navigation bar
    gfx_color(NAVBAR_COLOR);
    gfx_rect(0, SCREEN_HEIGHT - navBarSize, SCREEN_WIDTH, navBarSize);

    gfx_color(VM_COLOR_WHITE);
    vm_graphic_set_font(VM_SMALL_FONT);

    // action left
    gfx_text(navPad, (SCREEN_HEIGHT - CHAR_HEIGHT) - navPad, s_actionLeft);

    // action right
    gfx_text((SCREEN_WIDTH - STR_WIDTH(s_actionRight) - navPad), (SCREEN_HEIGHT - CHAR_HEIGHT) - navPad, s_actionRight);

    render_header();
}

static void render_game() {
    VMUINT32 currentTime = vm_get_tick_count();
    delta = (currentTime - time) * 0.001f;

    gfx_color(VM_COLOR_BLACK);
    gfx_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - navBarSize);

    if (player.dead) {
        gfx_color(VM_COLOR_RED);
        vm_graphic_set_font(VM_LARGE_FONT);
        gfx_text(SCREEN_WIDTH / 2 - STR_WIDTH(s_gameOver) / 2, (SCREEN_HEIGHT / 2 - CHAR_HEIGHT / 2), s_gameOver);

        return;
    }

    update_pillar(&pillarBot1);
    update_pillar(&pillarBot2);

    update_pillar(&pillarTop1);
    update_pillar(&pillarTop2);

    render_pillar(&pillarBot1);
    render_pillar(&pillarBot2);

    render_pillar(&pillarTop1);
    render_pillar(&pillarTop2);

    update_player();
    render_player();

    if (player_collides_pillar(player, pillarTop1) ||
        player_collides_pillar(player, pillarBot1) ||
        player_collides_pillar(player, pillarTop2) ||
        player_collides_pillar(player, pillarBot2)) {
        player.dead = 1;
    }

    time = currentTime;
}

static void update_player() {
    player.jump_velocity += player.jump_velocity == 5 ? -0.1 : 0.1;
    player.velocity -= player.velocity == 0 ? 0 : 0.1;
    player.y += -player.velocity + PLAYER_GRAVITY + player.jump_velocity * delta;

    if (player.x > SCREEN_WIDTH - PLAYER_SIZE) {
        player.x = SCREEN_WIDTH - PLAYER_SIZE;
    }

    if (player.y > SCREEN_HEIGHT - PLAYER_SIZE - navBarSize) {
        player.y = SCREEN_HEIGHT - PLAYER_SIZE - navBarSize;
        player.velocity = 0;
    }

    if (player.x < PLAYER_SIZE)
        player.x = PLAYER_SIZE;

    if (player.y < PLAYER_SIZE)
        player.y = PLAYER_SIZE;
}

static void render_player() {
    gfx_color(PLAYER_COLOR);
    gfx_rect(player.x, player.y, PLAYER_SIZE, PLAYER_SIZE);
}

static void render_pillar(pillar_t *pillar) {
    gfx_color(VM_COLOR_WHITE);
    gfx_rect(pillar->x, pillar->y, PILLAR_WIDTH, pillar->pillarHeight);
}

static void update_pillar(pillar_t *pillar) {
    pillar->x -= PILLAR_SPEED;
    pillar->y = pillar->position == 1 ? SCREEN_HEIGHT - pillar->pillarHeight : 0;

    if (pillar->x < -PILLAR_WIDTH - pillar->id) {
        pillar->x = SCREEN_WIDTH + pillar->id;
        pillar->pillarHeight = (rand() % (PILLAR_MAX_HEIGHT - PILLAR_MIN_HEIGHT + 1)) + PILLAR_MIN_HEIGHT;
    }

    if (pillar->x == player.x) {
        player.score++;
    }
}