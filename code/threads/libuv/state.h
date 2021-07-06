#include <stdio.h>

#define STATES 4

typedef char state_t;
typedef char action_t;

state_t next(state_t cur, action_t action);
