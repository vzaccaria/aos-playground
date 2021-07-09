#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXMSG 1024

typedef struct {
  uint8_t actions[MAXMSG];
  int len;
} message_t;

void perror_die(char *msg);
int srv_init();
int srv_accept_new_connection(int);
message_t receive(int sockfd);
void sendDone(int sockfd);
void make_socket_non_blocking(int sockfd);
