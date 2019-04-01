#ifndef _AT_H
#define _AT_H

#include <bc_atci.h>
#include <bc_cmwx1zzabz.h>
#include <bc_led.h>

void at_init(bc_led_t *led);
bool at_blink(void);
bool at_led_set(bc_atci_param_t *param);
bool at_led_help(void);
bool at_stop(void);
bool at_forward_set(bc_atci_param_t *param);
bool at_backward_set(bc_atci_param_t *param);
bool at_left_set(bc_atci_param_t *param);
bool at_right_set(bc_atci_param_t *param);

#endif // _AT_H
