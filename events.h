#ifndef EVENTS_H
#define EVENTS_H

#include "common.h"

// System Event Handlers
void on_sys_event(VMINT message, VMINT param);
void on_key_event(VMINT event, VMINT keycode);
void on_pen_event(VMINT event, VMINT x, VMINT y);

void on_timer_tick(VMINT timerId);

#endif