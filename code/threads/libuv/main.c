#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

static uv_loop_t *loop;

void on_close(uv_handle_t *handle) { printf("closed."); }

static void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  *buf = uv_buf_init(malloc(size), size);
}

void on_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf) {
  printf("on_read. %p\n", tcp);
  if (nread >= 0) {
    printf("read: %s\n", buf->base);
  } else {
    uv_close((uv_handle_t *)tcp, on_close);
  }

  // cargo-culted
  free(buf->base);
}

void on_connect(uv_connect_t *connection, int status) {
  if (status < 0) {
    printf("failed to connect\n");
    return;
  }
  printf("connected. %p %d\n", connection, status);

  uv_stream_t *stream = connection->handle;
  free(connection);
  uv_read_start(stream, alloc_cb, on_read);
}

void startConn(const char *host, int port) {
  uv_tcp_t *pSock = malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, pSock);
  uv_tcp_keepalive(pSock, 1, 60);

  struct sockaddr_in dest;
  uv_ip4_addr(host, port, &dest);

  uv_connect_t *pConn = malloc(sizeof(uv_connect_t));
  printf("allocated %p\n", pConn);
  uv_tcp_connect(pConn, pSock, (const struct sockaddr *)&dest, on_connect);
}

int main() {
  const char *urls[1] = {"142.250.180.68"};
  loop = uv_default_loop();
  startConn(urls[0], 80);

  uv_run(loop, UV_RUN_DEFAULT);

  uv_loop_close(loop);
  free(loop);
  return 0;
}
