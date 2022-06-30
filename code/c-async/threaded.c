
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "state.h"
#include <stdint.h>

typedef struct {
  int sockfd;
  int clientno;
} thread_config_t;

#define MAXCLIENTS 10

state_t clients[MAXCLIENTS];
int nclients = 0;

void *server_thread(void *arg) {
  thread_config_t *config = (thread_config_t *)arg;
  int connected_sock = config->sockfd;
  int client = config->clientno;
  free(config);

  // This cast will work for Linux, but in general casting pthread_id to an
  // integral type isn't portable.
  unsigned long id = (unsigned long)pthread_self();
  printf("Thread %lu created to handle connection with socket %d\n", id,
         connected_sock);

  clients[client] = init();
  while (1) {
    message_t m = receive(connected_sock);
    clients[client] = compute_state(client, clients[client], m);
    if (isExitState(clients[client])) {
      printf("peer done");
      sendDone(connected_sock);
      break;
    }
  };

  printf("Thread %lu done\n", id);
  return 0;
}

void serveSockWithThread(int connected_sock) {
  nclients++;
  pthread_t the_thread;
  thread_config_t *config = (thread_config_t *)malloc(sizeof(*config));
  config->sockfd = connected_sock;
  config->clientno = nclients - 1;
  pthread_create(&the_thread, NULL, server_thread, config);
  // Detach the thread - when it's done, its resources will be cleaned up.
  // Since the main thread lives forever, it will outlive the serving threads.
  pthread_detach(the_thread);
}

int main() {
  int list_sock = srv_init();
  while (1) {
    int connected_sock = srv_accept_new_connection(list_sock);
    serveSockWithThread(connected_sock);
  }
}
