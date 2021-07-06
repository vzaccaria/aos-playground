
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "state.h"
#include "utils.h"
#include <stdint.h>

#define MAXMSG 1024

typedef struct {
  uint8_t actions[MAXMSG];
  int len;
} message_t;

message_t receive(int sockfd) {
  message_t themsg;
  int len = recv(sockfd, themsg.actions, MAXMSG, 0);
  if (len < 0) {
    perror_die("recv");
  };
  return themsg;
}

state_t compute_state(state_t s, message_t m) {
  for (int i = 0; i < m.len; i++) {
    if (m.actions == "$") {
    }
    s = next(s, m.actions[i]);
  }
  return s;
}

int main() {
  int list_sock = srv_init();
  while (1) {
    int connected = srv_accept_new_connection(list_sock);
    while (1)
      ;
  }
}
