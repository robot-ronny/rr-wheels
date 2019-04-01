#include <application.h>
#include <at.h>

#define PWM_LEFT BC_PWM_P2
#define PWM_RIGHT BC_PWM_P14
#define IN_A_LEFT BC_GPIO_P3
#define IN_B_LEFT BC_GPIO_P4
#define IN_A_RIGHT BC_GPIO_P13
#define IN_B_RIGHT BC_GPIO_P12

application_t application;
bc_led_t led;
bc_button_t button;

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_set_mode(&led, BC_LED_MODE_TOGGLE);
    }
}

void task_stop(void *param)
{
    application.moving = false;

    bc_pwm_disable(PWM_LEFT);
    bc_pwm_disable(PWM_RIGHT);

    bc_gpio_set_output(IN_A_LEFT, 0);
    bc_gpio_set_output(IN_A_RIGHT, 0);
    bc_gpio_set_output(IN_B_LEFT, 0);
    bc_gpio_set_output(IN_B_RIGHT, 0);

    bc_scheduler_unregister(bc_scheduler_get_current_task_id());

    bc_atci_printf("+$STOP");
}

bool action_stop(void)
{
    if (!application.moving)
    {
        return false;
    }

    bc_scheduler_plan_now(application.stop_task_id);

    return true;
}

bool action_go(action_go_t action, int interval)
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

    bc_pwm_set(PWM_LEFT, 100);
    bc_pwm_enable(PWM_LEFT);
    bc_pwm_set(PWM_RIGHT, 100);
    bc_pwm_enable(PWM_RIGHT);

    if (application.moving)
    {
        bc_scheduler_plan_absolute(application.stop_task_id, bc_tick_get() + (uint64_t) interval);
    }
    else
    {
        application.moving = true;
        application.action = action;
        application.stop_task_id = bc_scheduler_register(task_stop, NULL, bc_tick_get() + (uint64_t) interval);
    }

    return true;
}

void application_init(void)
{
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
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    at_init(&led);

    static const bc_atci_command_t commands[] =
    {
        {"$STOP", at_stop, NULL, NULL, NULL, "Stop motors"},
        {"$FORWARD", NULL, at_forward_set, NULL, NULL, "Go forward"},
        {"$BACKWARD", NULL, at_backward_set, NULL, NULL, "Go backward"},
        {"$LEFT", NULL, at_left_set, NULL, NULL, "Go left"},
        {"$RIGHT", NULL, at_right_set, NULL, NULL, "Go right"},
        {"$BLINK", at_blink, NULL, NULL, NULL, "LED blink 3 times"},
        {"$LED", NULL, at_led_set, NULL, at_led_help, "LED on/off"},
        BC_ATCI_COMMAND_CLAC,
        BC_ATCI_COMMAND_HELP
    };

    bc_atci_init(commands, BC_ATCI_COMMANDS_LENGTH(commands));
}
