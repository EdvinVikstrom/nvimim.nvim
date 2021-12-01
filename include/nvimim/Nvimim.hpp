#pragma once

#include "nvimim/Type.hpp"
#include "nvim/types/Buffer.hpp"
#include "nvim/types/Window.hpp"

namespace nvimim {

  class Nvimim {

  private:

    Window m_main_window;

    Window m_filer_window;
    Buffer m_filer_buffer;

  public:

    inline void set_main_window(const Window &win) { m_main_window = win; }
    inline Window& get_main_window() { return m_main_window; }

    inline void set_filer_window(const Window &win) { m_filer_window = win; }
    inline Window& get_filer_window() { return m_filer_window; }

    inline void set_filer_buffer(const Buffer &buf) { m_filer_buffer = buf; }
    inline Buffer& get_filer_buffer() { return m_filer_buffer; }

  };

}
