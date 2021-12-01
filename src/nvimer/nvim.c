#include "rpc.h"

#include <stdlib.h>
#include <stdio.h>

FILE* log_file;

int main(int argc, char** argv)
{
  //log_file = fopen("nvimer.log", "wb");
  log_file = stdout;

  const char* addr = NULL;

  if (argc == 2)
    addr = argv[1];
  else
    addr = getenv("NVIM_LISTEN_ADDRESS");
  fprintf(log_file, "addr[%s]\n", addr);

  struct rpc_client client;
  rpc_client_new(&client, addr);
  rpc_client_call(&client, RPC_CLIENT_CALL_TYPE_COMMAND, "echo \"hello!!!\"");
  rpc_client_delete(&client);

  //fclose(log_file);
  return 0;
}
