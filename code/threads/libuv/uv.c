
#include "uv.h"
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
#define N_BACKLOG 64

int numclients = 0;

typedef struct {
  state_t state;
  uv_tcp_t cur_client_stream;
  int num;
  message_t curmsg;
} client_state_t;

void newClient(client_state_t **c, uv_stream_t **s) {
  *c = xmalloc(sizeof(client_state_t));
  (*c)->state = init();
  (*c)->num = numclients++;
  uv_tcp_init(uv_default_loop(), &(*c)->cur_client_stream);
  *s = (uv_stream_t *)&(*c)->cur_client_stream;
  (*s)->data = (*c);
}

#define attach_client_state(stream, state) (stream->data = state)
#define get_client_state(stream) ((client_state_t *)stream->data)

void on_buffer_alloc_request(uv_handle_t *stream, size_t suggested_size,
                             uv_buf_t *buf) {
  buf->base = (char *)get_client_state(stream)->curmsg.actions;
  buf->len = MAXMSG;
}

void on_client_closed(uv_handle_t *client_stream) {
  free(get_client_state(client_stream));
}

void closeStream(uv_write_t *write_req, int status) {
  uv_handle_t *client_stream =
      (uv_handle_t *)&get_client_state(write_req)->cur_client_stream;
  free(write_req);
  uv_close(client_stream, on_client_closed);
}

void send_done(uv_stream_t *client_stream) {
  client_state_t *c = get_client_state(client_stream);
  message_t m = c->curmsg;
  strcpy((char *)m.actions, "$");
  uv_buf_t writebuf = uv_buf_init((char *)m.actions, 2);
  uv_write_t *writereq = (uv_write_t *)xmalloc(sizeof(*writereq));
  attach_client_state(writereq, c);
  uv_write(writereq, client_stream, &writebuf, 1 /*just one writebuf*/,
           closeStream);
}

void on_buffer_received(uv_stream_t *client_stream, ssize_t nread,
                        const uv_buf_t *buf) {
  client_state_t *c = get_client_state(client_stream);
  message_t m = c->curmsg;
  m.len = nread;
  c->state = compute_state(c->num, c->state, m);
  if (isExitState(c->state)) {
    printf("peer done\n");
    send_done(client_stream);
  } else {
    uv_read_start(client_stream, on_buffer_alloc_request, on_buffer_received);
  }
}

void client_connected(uv_stream_t *listening, int status) {
  client_state_t *client_state;
  uv_stream_t *client_stream;

  newClient(&client_state, &client_stream);

  uv_accept(listening, client_stream);
  uv_report_connected((uv_tcp_t *)client_stream);
  uv_read_start(client_stream, on_buffer_alloc_request, on_buffer_received);
}

int main() {
  uv_tcp_t stream = srv_uv_init();
  uv_listen((uv_stream_t *)&stream, N_BACKLOG, client_connected);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return uv_loop_close(uv_default_loop());
}
