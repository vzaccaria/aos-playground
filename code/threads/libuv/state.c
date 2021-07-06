#include "state.h"
#include <stdio.h>

state_t fsm[STATES][STATES + 1] = {
    "abcd" /* state = a */, "babb" /* state = b */, "cadd" /* state = c */,
    "dded" /* state = d */};

state_t next(state_t cur, action_t action) {
  return fsm[cur - 'a'][action - 'a'];
}
