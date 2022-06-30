#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define _GNU_SOURCE
#include "state.h"
#include "uv.h"
#include <netdb.h>

#define N_BACKLOG 64

void die(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}

void *xmalloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr) {
    die("malloc failed");
  }
  return ptr;
}

void perror_die(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void report_peer_connected(const struct sockaddr_in *sa, socklen_t salen) {
  char hostbuf[NI_MAXHOST];
  char portbuf[NI_MAXSERV];
  if (getnameinfo((struct sockaddr *)sa, salen, hostbuf, NI_MAXHOST, portbuf,
                  NI_MAXSERV, 0) == 0) {
    printf("peer (%s, %s) connected\n", hostbuf, portbuf);
  } else {
    printf("peer (unknonwn) connected\n");
  }
}

void uv_report_connected(uv_tcp_t *client) {
  struct sockaddr_storage peername;
  int namelen = sizeof(peername);
  uv_tcp_getpeername(client, (struct sockaddr *)&peername, &namelen);
  report_peer_connected((const struct sockaddr_in *)&peername, namelen);
}

int listen_inet_socket(int portnum) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror_die("ERROR opening socket");
  }

  // This helps avoid spurious EADDRINUSE when the previous instance of this
  // server died.
  int opt = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror_die("setsockopt");
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portnum);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror_die("ERROR on binding");
  }

  if (listen(sockfd, N_BACKLOG) < 0) {
    perror_die("ERROR on listen");
  }

  return sockfd;
}

void make_socket_non_blocking(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
    perror_die("fcntl F_GETFL");
  }

  if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
    perror_die("fcntl F_SETFL O_NONBLOCK");
  }
}

int srv_accept_new_connection(int sockfd) {
  struct sockaddr_in peer_addr;
  socklen_t peer_addr_len = sizeof(peer_addr);

  int newsockfd = accept(sockfd, (struct sockaddr *)&peer_addr, &peer_addr_len);

  if (newsockfd < 0) {
    perror_die("ERROR on accept");
  }
  report_peer_connected(&peer_addr, peer_addr_len);
  return newsockfd;
}

int srv_init() {
  setvbuf(stdout, NULL, _IONBF, 0);
  int portnum = 9090;
  printf("Serving on port %d\n", portnum);
  return listen_inet_socket(portnum);
}

message_t receive(int sockfd) {
  message_t themsg;
  themsg.len = recv(sockfd, themsg.actions, MAXMSG, 0);
  if (themsg.len < 0) {
    perror_die("recv");
  };
  return themsg;
}

uv_tcp_t srv_uv_init() {
  uv_tcp_t server_stream;
  setvbuf(stdout, NULL, _IONBF, 0);

  int portnum = 9090;
  printf("Serving on port %d\n", portnum);

  int rc;
  if ((rc = uv_tcp_init(uv_default_loop(), &server_stream)) < 0) {
    die("uv_tcp_init failed: %s", uv_strerror(rc));
  }

  struct sockaddr_in server_address;
  if ((rc = uv_ip4_addr("0.0.0.0", portnum, &server_address)) < 0) {
    die("uv_ip4_addr failed: %s", uv_strerror(rc));
  }

  if ((rc = uv_tcp_bind(&server_stream,
                        (const struct sockaddr *)&server_address, 0)) < 0) {
    die("uv_tcp_bind failed: %s", uv_strerror(rc));
  }
  return server_stream;
}

void sendDone(int sockfd) {
  char buf[2] = "$";
  send(sockfd, &buf, 2, 0);
}
