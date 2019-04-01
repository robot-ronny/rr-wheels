#include <at.h>
#include <bc_atci.h>
#include "application.h"

static struct
{
    bc_led_t *led;

} _at;

void at_init(bc_led_t *led)
{
    _at.led = led;
}

bool at_stop(void)
{
    return action_stop();
}

bool at_forward_set(bc_atci_param_t *param)
{
    int interval = atoi(param->txt);
    int pwm = 100;

    char *comma = strstr(param->txt, ",");
    if(comma != NULL)
    {
        pwm = atoi((comma+1));
    }

    return action_go(ACTION_GO_FORWARD, interval, pwm);
}

bool at_backward_set(bc_atci_param_t *param)
{
    int interval = atoi(param->txt);
    int pwm = 100;

    char *comma = strstr(param->txt, ",");
    if(comma != NULL)
    {
        pwm = atoi((comma+1));
    }

    return action_go(ACTION_GO_BACKWARD, interval, pwm);
}

bool at_left_set(bc_atci_param_t *param)
{
    int interval = atoi(param->txt);
    int pwm = 100;

    char *comma = strstr(param->txt, ",");
    if(comma != NULL)
    {
        pwm = atoi((comma+1));
    }

    return action_go(ACTION_GO_LEFT, interval, pwm);
}

bool at_right_set(bc_atci_param_t *param)
{
    int interval = atoi(param->txt);
    int pwm = 100;

    char *comma = strstr(param->txt, ",");
    if(comma != NULL)
    {
        pwm = atoi((comma+1));
    }

    return action_go(ACTION_GO_RIGHT, interval, pwm);
}

bool at_blink(void)
{
    bc_led_blink(_at.led, 3);

    return true;
}

bool at_led_set(bc_atci_param_t *param)
{
    if (param->length != 1)
    {
        return false;
    }

    if (param->txt[0] == '1')
    {
        bc_led_set_mode(_at.led, BC_LED_MODE_ON);

        return true;
    }

    if (param->txt[0] == '0')
    {
        bc_led_set_mode(_at.led, BC_LED_MODE_OFF);

        return true;
    }

    return false;
}

bool at_led_help(void)
{
    bc_atci_printf("$LED: (0,1)");

    return true;
}
