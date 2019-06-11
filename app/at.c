#include <at.h>
#include <bc_atci.h>
#include "application.h"

void at_init(void)
{
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
