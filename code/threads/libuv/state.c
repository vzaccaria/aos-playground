#include "state.h"
#include <stdio.h>

#define STATES 4

/* Next state function:
 *
 *   action:
 *           a   b   c   d
 *
 * state a   a   b   c   d
 *       b   a   a   b   d
 *       c   a   a   c   d
 *       d   d   d   d   d (exit state)
 */

state_t fsm[STATES][STATES + 1] = {
    "abcd",                                                /* state = a */
    "aabd" /* state = b */, "aacd" /* state = c */, "dddd" /* state = d */
};

state_t next(state_t cur, action_t action) {
  if (action >= 'a' && action <= ('a' + STATES - 1)) {
    return fsm[cur - 'a'][action - 'a'];
  } else
    return cur;
}

int isValidAction(action_t action) {
  return (action >= 'a' && action <= ('a' + STATES - 1));
}

int isExitState(state_t cur) { return cur == ('a' + STATES - 1); }

state_t init() { return 'a'; }

state_t compute_state(int client, state_t s, message_t m) {
  for (int i = 0; i < m.len; i++) {
    state_t prev = s;
    s = next(s, m.actions[i]);
    if (isValidAction(m.actions[i])) {
      printf("Client %d: current %c, action %c, next %c\n", client, prev,
             m.actions[i], s);
    }
  }
  return s;
}
