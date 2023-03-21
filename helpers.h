#ifndef HELPERS_H
#define HELPERS_H

#include "common.h"

#define	MRE_STRING_MAX_SIZE   (100)
#define	MRE_FILE_CREATE_FAILED  	(-5)
#define	MRE_FILE_OPEN_FAILED        (-3)
#define	MRE_FILE_NAME_SIZE			(50)
#define	MRE_FILE_NOT_WRITTEN        (-4)

#define gfx_rect(x, y, w, h) vm_graphic_fill_rect_ex(layer_hdl[0], x, y, w, h)
#define gfx_color(new_color)     \
    color.vm_color_565 = new_color; \
    vm_graphic_setcolor(&color);

#define gfx_text(x, y, str) vm_graphic_textout_to_layer(layer_hdl[0], x, y, str, vm_graphic_get_string_width(str))
#define get_str(res) (VMWSTR) vm_res_get_string(res)

#define SCREEN_WIDTH vm_graphic_get_screen_width()
#define SCREEN_HEIGHT vm_graphic_get_screen_height()
#define CHAR_HEIGHT vm_graphic_get_character_height()
#define STR_WIDTH(str) vm_graphic_get_string_width(str)

#define to_wstr(src, dst) vm_ascii_to_ucs2(dst, MRE_STRING_MAX_SIZE, src)

#endif
