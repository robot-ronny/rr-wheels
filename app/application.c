#include <application.h>
#include <at.h>

#define PWM_RAMP_TIME ((bc_tick_t) 300)

#define PWM_LEFT BC_PWM_P2
#define PWM_RIGHT BC_PWM_P14
#define IN_A_LEFT BC_GPIO_P3
#define IN_B_LEFT BC_GPIO_P4
#define IN_A_RIGHT BC_GPIO_P13
#define IN_B_RIGHT BC_GPIO_P12

application_t application;
bc_led_t led;

bool action_stop(void)
{
    if (!application.moving)
    {
        return false;
    }

    application.pwm_change = true;
    application.pwm_target = 0;
    application.pwm_timeout = 0;

    return true;
}

bool action_go(action_go_t action, int interval, int pwm)
{
    if (application.moving && application.action != action)
    {
        return false;
    }

    switch (action)
    {
        case ACTION_GO_FORWARD:
        {
            bc_gpio_set_output(IN_A_LEFT, 0);
            bc_gpio_set_output(IN_A_RIGHT, 1);
            bc_gpio_set_output(IN_B_LEFT, 1);
            bc_gpio_set_output(IN_B_RIGHT, 0);

            break;
        }
        case ACTION_GO_BACKWARD:
        {
            bc_gpio_set_output(IN_A_LEFT, 1);
            bc_gpio_set_output(IN_A_RIGHT, 0);
            bc_gpio_set_output(IN_B_LEFT, 0);
            bc_gpio_set_output(IN_B_RIGHT, 1);

            break;
        }
        case ACTION_GO_LEFT:
        {
            bc_gpio_set_output(IN_A_LEFT, 1);
            bc_gpio_set_output(IN_A_RIGHT, 1);
            bc_gpio_set_output(IN_B_LEFT, 0);
            bc_gpio_set_output(IN_B_RIGHT, 0);

            break;
        }
        case ACTION_GO_RIGHT:
        {
            bc_gpio_set_output(IN_A_LEFT, 0);
            bc_gpio_set_output(IN_A_RIGHT, 0);
            bc_gpio_set_output(IN_B_LEFT, 1);
            bc_gpio_set_output(IN_B_RIGHT, 1);

            break;
        }
        default:
        {
            return false;
        }
    }

    application.pwm_change = true;
    application.pwm_target = pwm;
    application.pwm_timeout = bc_tick_get() + (bc_tick_t) interval + PWM_RAMP_TIME;
    application.action = action;
    application.moving = true;

    return true;
}

void application_init(void)
{
    memset(&application, 0, sizeof(application));

    bc_gpio_init(IN_A_LEFT);
    bc_gpio_init(IN_A_RIGHT);
    bc_gpio_init(IN_B_LEFT);
    bc_gpio_init(IN_B_RIGHT);

    bc_gpio_set_output(IN_A_LEFT, 0);
    bc_gpio_set_output(IN_A_RIGHT, 0);
    bc_gpio_set_output(IN_B_LEFT, 0);
    bc_gpio_set_output(IN_B_RIGHT, 0);

    bc_gpio_set_mode(IN_A_LEFT, BC_GPIO_MODE_OUTPUT);
    bc_gpio_set_mode(IN_B_LEFT, BC_GPIO_MODE_OUTPUT);
    bc_gpio_set_mode(IN_A_RIGHT, BC_GPIO_MODE_OUTPUT);
    bc_gpio_set_mode(IN_B_RIGHT, BC_GPIO_MODE_OUTPUT);

    bc_pwm_init(PWM_LEFT);
    bc_pwm_init(PWM_RIGHT);

    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_BLINK_FAST);

    at_init();

    static const bc_atci_command_t commands[] =
    {
        {"$STOP", at_stop, NULL, NULL, NULL, "Stop motors"},
        {"$FORWARD", NULL, at_forward_set, NULL, NULL, "Go forward"},
        {"$BACKWARD", NULL, at_backward_set, NULL, NULL, "Go backward"},
        {"$LEFT", NULL, at_left_set, NULL, NULL, "Go left"},
        {"$RIGHT", NULL, at_right_set, NULL, NULL, "Go right"},
        BC_ATCI_COMMAND_CLAC,
        BC_ATCI_COMMAND_HELP
    };

    bc_atci_init(commands, BC_ATCI_COMMANDS_LENGTH(commands));
}

void application_task(void *param)
{
    if (application.moving)
    {
        if (application.pwm_change)
        {
            bc_ramp_init(&application.pwm_ramp, application.pwm_now, application.pwm_target, PWM_RAMP_TIME);
            bc_ramp_start(&application.pwm_ramp);

            application.pwm_change = false;
        }

#if 0
        int pwm_old = application.pwm_now;

        application.pwm_now = bc_ramp_get(&application.pwm_ramp);

        if (pwm_old != application.pwm_now)
        {
            bc_atci_printf("+$PWM: %d", application.pwm_now);
        }
#else
        application.pwm_now = bc_ramp_get(&application.pwm_ramp);
#endif

        bc_pwm_set(PWM_LEFT, application.pwm_now);
        bc_pwm_enable(PWM_LEFT);
        bc_pwm_set(PWM_RIGHT, application.pwm_now);
        bc_pwm_enable(PWM_RIGHT);

        if (application.pwm_timeout == 0 && application.pwm_now == 0)
        {
            application.pwm_now = 0;
            application.moving = false;

            bc_pwm_disable(PWM_LEFT);
            bc_pwm_disable(PWM_RIGHT);

            bc_gpio_set_output(IN_A_LEFT, 0);
            bc_gpio_set_output(IN_A_RIGHT, 0);
            bc_gpio_set_output(IN_B_LEFT, 0);
            bc_gpio_set_output(IN_B_RIGHT, 0);

            bc_atci_printf("+$STOP");
        }
        else if (application.pwm_timeout != 0 && bc_tick_get() >= application.pwm_timeout)
        {
            action_stop();
        }
    }

    bc_scheduler_plan_current_now();
}
