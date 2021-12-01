#ifndef NVIM_TYPES_BUFFER_HPP
  #define NVIM_TYPES_BUFFER_HPP

#include "nvim/Type.hpp"

namespace nvim {

  struct Buffer : public SpecialType {

    ReturnType attach(class Nvim &nvim, ArgType send_buffer, ArgType opts);
    ReturnType call(class Nvim &nvim, ArgType fun);
    void del_keymap(class Nvim &nvim, ArgType mode, ArgType lhs);
    ReturnType del_mark(class Nvim &nvim, ArgType name);
    void del_var(class Nvim &nvim, ArgType name);
    void destroy(class Nvim &nvim, ArgType opts);
    ReturnType detach(class Nvim &nvim);
    ReturnType get_changedtick(class Nvim &nvim);
    ReturnType get_commands(class Nvim &nvim, ArgType opts);
    ReturnType get_keymap(class Nvim &nvim, ArgType mode);
    ReturnType get_lines(class Nvim &nvim, ArgType start, ArgType end, ArgType strict_indexing);
    ReturnType get_mark(class Nvim &nvim, ArgType name);
    ReturnType get_name(class Nvim &nvim);
    ReturnType get_offset(class Nvim &nvim, ArgType index);
    ReturnType get_option(class Nvim &nvim, ArgType name);
    ReturnType get_var(class Nvim &nvim, ArgType name);
    ReturnType is_loaded(class Nvim &nvim);
    ReturnType is_valid(class Nvim &nvim);
    ReturnType line_count(class Nvim &nvim);
    void set_keymap(class Nvim &nvim, ArgType mode, ArgType lhs, ArgType rhs, ArgType opts);
    void set_lines(class Nvim &nvim, ArgType start, ArgType end, ArgType strict_indexing, ArgType replacement);
    ReturnType set_mark(class Nvim &nvim, ArgType name, ArgType line, ArgType col, ArgType opts);
    void set_name(class Nvim &nvim, ArgType name);
    void set_option(class Nvim &nvim, ArgType name, ArgType value);
    void set_text(class Nvim &nvim, ArgType start_row, ArgType start_col, ArgType end_row, ArgType end_col);
    void set_var(class Nvim &nvim, ArgType name, ArgType value);

  };

}

#endif
