#ifndef NVIMER_RPC_H
  #define NVIMER_RPC_H

#include <stdint.h>

enum rpc_client_call_types {
  RPC_CLIENT_CALL_TYPE_COMMAND
};

struct rpc_client {
  int sd;
};

int rpc_client_new(struct rpc_client* client, const char* addr);
int rpc_client_delete(struct rpc_client* client);
int rpc_client_call(struct rpc_client* client, enum rpc_client_call_types type, const char* args);

#endif
