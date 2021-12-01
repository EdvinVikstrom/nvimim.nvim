#include "rpc.h"
#include "../mpack/mpack.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

extern FILE* log_file;

int rpc_client_new(struct rpc_client* client, const char* addr)
{
  struct sockaddr_un name = {};
  name.sun_family = AF_LOCAL;
  strcpy(name.sun_path, addr);
  name.sun_path[strlen(addr)] = '\0';

  client->sd = socket(AF_LOCAL, SOCK_STREAM, 0);
  size_t len = strlen(name.sun_path) + sizeof(name.sun_family) + 1;

  if (connect(client->sd, (struct sockaddr*) &name, len) == -1)
  {
    fprintf(log_file, "failed to connect socket [%s]\n", strerror(errno));
    return 1;
  }
  return 0;
}

int rpc_client_delete(struct rpc_client* client)
{
  //if (shutdown(client->sd, SHUT_RDWR) == -1)
  //{
  //  fprintf(stderr, "failed to shutdown socket [%s]\n", strerror(errno));
  //  return 1;
  //}
  close(client->sd);
  return 0;
}

int rpc_client_call(struct rpc_client* client, enum rpc_client_call_types type, const char* args)
{
  char* data;
  size_t len;
  mpack_writer_t writer;
  mpack_writer_init_growable(&writer, &data, &len);

  mpack_build_array(&writer);
  mpack_write_u64(&writer, 0);
  mpack_write_u64(&writer, 1);
  mpack_write_cstr(&writer, "nvim_command");

  mpack_build_array(&writer);
  mpack_write_cstr(&writer, args);
  mpack_complete_array(&writer);

  mpack_complete_array(&writer);

  if (mpack_writer_destroy(&writer) != mpack_ok)
  {
    fprintf(log_file, "something is wrong!\n");
    return 1;
  }

  ssize_t res = send(client->sd, data, len, 0x00100);
  if (res == -1)
  {
    fprintf(log_file, "filed to send [%s][%i]\n", strerror(errno), errno);
    return 1;
  }
  //write(client->sd, data, len);
  free(data);

  size_t buffer_len = 1024;
  char buffer[buffer_len];
  ssize_t r;
  while ((r = read(client->sd, buffer, buffer_len)) > 0)
  {
    buffer[r] = '\0';
    fprintf(log_file, "got[%li][%s]\n", r, buffer);
    break;
  }
  fprintf(log_file, "done read\n");

  return 0;
}
