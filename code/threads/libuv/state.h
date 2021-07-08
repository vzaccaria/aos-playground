#include "utils.h"
#include <stdio.h>

typedef char state_t;
typedef char action_t;

state_t init();
state_t next(state_t cur, action_t action);
int isExitState(state_t cur);
int isValidAction(action_t cur);
state_t compute_state(int client, state_t s, message_t m);
