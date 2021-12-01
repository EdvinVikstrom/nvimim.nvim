#include "nvim/types/Buffer.hpp"
#include "nvim/Nvim.hpp"

nvim::ReturnType nvim::Buffer::attach(Nvim &nvim, ArgType send_buffer, ArgType opts)
{
  const Message msg = nvim.request("nvim_buf_attach", *this, send_buffer, opts);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::call(Nvim &nvim, ArgType fun)
{
  const Message msg = nvim.request("nvim_buf_call", *this, fun);
  return msg.get_result();
}

void nvim::Buffer::del_keymap(Nvim &nvim, ArgType mode, ArgType lhs)
{
  const Message msg = nvim.request("nvim_buf_del_keymap", *this, mode, lhs);
  (void) msg;
}

nvim::ReturnType nvim::Buffer::del_mark(Nvim &nvim, ArgType name)
{
  const Message msg = nvim.request("nvim_buf_del_mark", *this, name);
  return msg.get_result();
}

void nvim::Buffer::del_var(Nvim &nvim, ArgType name)
{
  const Message msg = nvim.request("nvim_buf_del_var", *this, name);
  (void) msg;
}

void nvim::Buffer::destroy(Nvim &nvim, ArgType opts)
{
  const Message msg = nvim.request("nvim_buf_destroy", *this, opts);
  (void) msg;
}

nvim::ReturnType nvim::Buffer::detach(Nvim &nvim)
{
  const Message msg = nvim.request("nvim_buf_detach", *this);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::get_changedtick(Nvim &nvim)
{
  const Message msg = nvim.request("nvim_buf_get_changedtick", *this);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::get_commands(Nvim &nvim, ArgType opts)
{
  const Message msg = nvim.request("nvim_buf_get_commands", *this, opts);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::get_keymap(Nvim &nvim, ArgType mode)
{
  const Message msg = nvim.request("nvim_buf_get_keymap", *this, mode);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::get_lines(Nvim &nvim, ArgType start, ArgType end, ArgType strict_indexing)
{
  const Message msg = nvim.request("nvim_buf_get_lines", *this, start, end, strict_indexing);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::get_mark(Nvim &nvim, ArgType name)
{
  const Message msg = nvim.request("nvim_buf_get_mark", *this, name);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::get_name(Nvim &nvim)
{
  const Message msg = nvim.request("nvim_buf_get_name", *this);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::get_offset(Nvim &nvim, ArgType index)
{
  const Message msg = nvim.request("nvim_buf_get_offset", *this, index);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::get_option(Nvim &nvim, ArgType name)
{
  const Message msg = nvim.request("nvim_buf_get_option", *this, name);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::get_var(Nvim &nvim, ArgType name)
{
  const Message msg = nvim.request("nvim_buf_get_var", *this, name);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::is_loaded(Nvim &nvim)
{
  const Message msg = nvim.request("nvim_buf_is_loaded", *this);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::is_valid(Nvim &nvim)
{
  const Message msg = nvim.request("nvim_buf_is_valid", *this);
  return msg.get_result();
}

nvim::ReturnType nvim::Buffer::line_count(Nvim &nvim)
{
  const Message msg = nvim.request("nvim_buf_line_count", *this);
  return msg.get_result();
}

void nvim::Buffer::set_keymap(Nvim &nvim, ArgType mode, ArgType lhs, ArgType rhs, ArgType opts)
{
  const Message msg = nvim.request("nvim_buf_set_keymap", *this, mode, lhs, rhs, opts);
  (void) msg;
}

void nvim::Buffer::set_lines(Nvim &nvim, ArgType start, ArgType end, ArgType strict_indexing, ArgType replacement)
{
  const Message msg = nvim.request("nvim_buf_set_lines", *this, start, end, strict_indexing, replacement);
  (void) msg;
}

nvim::ReturnType nvim::Buffer::set_mark(Nvim &nvim, ArgType name, ArgType line, ArgType col, ArgType opts)
{
  const Message msg = nvim.request("nvim_buf_set_mark", *this, name, line, col, opts);
  return msg.get_result();
}

namespace nvimim {
  extern FILE* log_file;
}
void nvim::Buffer::set_name(Nvim &nvim, ArgType name)
{
  const Message msg = nvim.request("nvim_buf_set_name", *this, name);
  (void) msg;
}

void nvim::Buffer::set_option(Nvim &nvim, ArgType name, ArgType value)
{
  const Message msg = nvim.request("nvim_buf_set_option", *this, name, value);
  (void) msg;
}

void nvim::Buffer::set_text(Nvim &nvim, ArgType start_row, ArgType start_col, ArgType end_row, ArgType end_col)
{
  const Message msg = nvim.request("nvim_buf_set_text", *this, start_row, start_col, end_row, end_col);
  (void) msg;
}

void nvim::Buffer::set_var(Nvim &nvim, ArgType name, ArgType value)
{
  const Message msg = nvim.request("nvim_buf_set_var", *this, name, value);
  (void) msg;
}
