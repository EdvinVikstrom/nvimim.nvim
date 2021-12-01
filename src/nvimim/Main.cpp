#include "nvimim/Main.hpp"
#include "nvimim/Nvimim.hpp"
#include "nvimim/Listener.hpp"
#include "nvim/Nvim.hpp"

#include <stdexcept>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace nvimim {
  FILE* log_file;
}

int nvimim::main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  Nvimim nvimim;
  Nvim nvim(STDOUT_FILENO, STDIN_FILENO);
  nvim.register_event("Init", Listener::init, &nvimim);
  nvim.register_event("TextChanged", Listener::text_changed, &nvimim);
  nvim.register_event("InitFiler", Listener::init_filer, &nvimim);
  nvim.register_event("SptfyCallback", Listener::sptfy_callback, &nvimim);

  while (true)
  {
    nvim.poll();
  }
  return 0;
}

int main(int argc, char** argv)
{
  nvimim::log_file = fopen("/dev/pts/2", "wb");
  fprintf(nvimim::log_file, "\e[3J\e[1;1H");
  try {
    nvimim::main(argc, argv);
  }catch (std::exception &e)
  {
    fprintf(nvimim::log_file, "error[%s]\n", e.what());
  }
  fclose(nvimim::log_file);
  return 0;
}
