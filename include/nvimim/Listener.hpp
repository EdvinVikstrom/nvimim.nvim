#pragma once

#include "nvimim/Type.hpp"
#include "nvim/Nvim.hpp"

namespace nvimim {

  class Listener {

  public:

    static void init(Nvim &nvim, const Message &msg, void* ptr);
    static void text_changed(Nvim &nvim, const Message &msg, void* ptr);
    static void init_filer(Nvim &nvim, const Message &msg, void* ptr);
    static void sptfy_callback(Nvim &nvim, const Message &msg, void* ptr);

  };


}
