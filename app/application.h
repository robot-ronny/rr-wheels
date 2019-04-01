#ifndef _APPLICATION_H
#define _APPLICATION_H

#ifndef VERSION
#define VERSION "vdev"
#endif

#include <bcl.h>

typedef enum
{
    ACTION_GO_FORWARD = 0,
    ACTION_GO_BACKWARD = 1,
    ACTION_GO_LEFT = 2,
    ACTION_GO_RIGHT = 3

} action_go_t;

typedef struct
{
    bool moving;
    action_go_t action;
    bc_scheduler_task_id_t stop_task_id;

} application_t;

extern application_t application;

bool action_stop(void);
bool action_go(action_go_t action, int interval);

#endif
