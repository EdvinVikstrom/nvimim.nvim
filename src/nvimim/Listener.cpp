#include "nvimim/Listener.hpp"
#include "nvimim/Nvimim.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void nvimim::Listener::init(Nvim &nvim, const Message &msg, void* ptr)
{
  Nvimim &instance = *reinterpret_cast<Nvimim*>(ptr);
  instance.set_main_window(msg.get_params().at(0).as_window());

  ::fprintf(log_file, "init!\n");
  nvim.respond(msg, nullptr, nullptr);
}

void nvimim::Listener::text_changed(Nvim &nvim, const Message &msg, void* ptr)
{
  ::fprintf(log_file, "text_changed!\n");
  nvim.respond(msg, nullptr, nullptr);
}

void nvimim::Listener::init_filer(Nvim &nvim, const Message &msg, void* ptr)
{
  ::fprintf(log_file, "init_filer!\n");

  Nvimim &instance = *reinterpret_cast<Nvimim*>(ptr);
  instance.set_filer_window(msg.get_params().at(0).as_window());
  instance.set_filer_buffer(msg.get_params().at(1).as_buffer());

  nvim.respond(msg, nullptr, nullptr);
}

void nvimim::Listener::sptfy_callback(Nvim &nvim, const Message &msg, void* ptr)
{
  Nvimim &instance = *reinterpret_cast<Nvimim*>(ptr);

  struct sockaddr_in addr;
  int addr_len = sizeof(addr);

  int fd = ::socket(PF_INET, SOCK_STREAM, 0);
  if (fd <= 0)
  {
    ::fprintf(log_file, "failed to create socket");
    return;
  }

  // ::setsockopt(fd, SOL_SOCKET, )

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = ::htons(8888);

  if (::bind(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1)
  {
    ::fprintf(log_file, "failed to bind socket");
    return;
  }

  if (::listen(fd, 3) == -1)
  {
    ::fprintf(log_file, "failed to listen socket");
    return;
  }

  ::fprintf(log_file, "going to accept\n");
    
  int sock = ::accept(fd, (struct sockaddr*) &addr, (socklen_t*) &addr_len);
  if (sock == -1)
  {
    ::fprintf(log_file, "failed to accept socket");
    return;
  }
  ::fprintf(log_file, "accepted\n");

  size_t buffer_len = 1024;
  char buffer[buffer_len];
  size_t r = ::read(sock, buffer, buffer_len);

  ::close(sock);
  ::close(fd);

  nvim.respond(msg, nullptr, String(buffer, r));
}
