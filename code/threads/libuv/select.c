
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "state.h"
#include <stdint.h>

#define MAXCLIENTS 3

typedef struct {
  state_t state;
  int curfd;
  int invalid;
  int num;
} client_state_t;

client_state_t clients[MAXCLIENTS];

void initClients() {
  for (int i = 0; i < MAXCLIENTS; i++) {
    clients[i].state = init();
    clients[i].curfd = 0;
    clients[i].invalid = 1;
    clients[i].num = i;
  }
}

void addClient(curfd) {
  for (int i = 0; i < MAXCLIENTS; i++) {
    if (clients[i].invalid) {
      clients[i].state = init();
      clients[i].curfd = curfd;
      clients[i].invalid = 0;
      return;
    }
  }
  perror_die("Sorry, maximum number of clients reached");
}

client_state_t *findClient(int fd) {
  for (int i = 0; i < MAXCLIENTS; i++) {
    if (!clients[i].invalid && clients[i].curfd == fd)
      return &clients[i];
  }
  return NULL;
}

int prepareListeningFDs(int list_sock, fd_set *fdset) {
  FD_ZERO(fdset);
  FD_SET(list_sock, fdset);
  int max = list_sock;
  for (int i = 0; i < MAXCLIENTS; i++) {
    if (!clients[i].invalid) {
      FD_SET(clients[i].curfd, fdset);
      if (clients[i].curfd > max)
        max = clients[i].curfd;
    }
  }
  return max + 1;
}

int getReadyFDs(fd_set *listening, int max, int *arrayOfFds) {
  select(max, listening, NULL, NULL, NULL);
  int n = 0;
  for (int i = 0; i < max; i++) {
    if (FD_ISSET(i, listening)) {
      arrayOfFds[n++] = i;
    }
  }
  return n;
}

int main() {
  int list_sock = srv_init();
  initClients();
  while (1) {
    fd_set listening;
    int readyFDs[MAXCLIENTS + 1];

    int max = prepareListeningFDs(list_sock, &listening);
    int nready = getReadyFDs(&listening, max, readyFDs);

    for (int i = 0; i < nready; i++) {
      if (readyFDs[i] == list_sock) {
        int connected_sock = srv_accept_new_connection(list_sock);
        addClient(connected_sock);
      } else {
        client_state_t *c = findClient(readyFDs[i]);
        message_t m = receive(c->curfd);
        c->state = compute_state(c->num, c->state, m);
        if (isExitState(c->state)) {
          printf("peer done");
          sendDone(c->curfd);
          c->invalid = 1;
          break;
        }
      }
    }
  }
}
