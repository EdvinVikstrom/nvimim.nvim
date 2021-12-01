#include "nvim/Nvim.hpp"
#include "mpack/mpack.h"

#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

nvim::Nvim::Nvim(int output, int input)
  : m_output(output), m_input(input), m_message(1)
{
}

static nvim::Type read_object(::mpack_node_t node)
{
  size_t value_len;
  nvim::String value_str;
  nvim::Array value_arr;
  switch (::mpack_node_type(node))
  {
    case mpack_type_nil:
      return nvim::Type();
    case mpack_type_bool:
      return nvim::Type(::mpack_node_bool(node));
    case mpack_type_int:
      return nvim::Type(::mpack_node_i64(node));
    case mpack_type_double:
      return nvim::Type(::mpack_node_double(node));
    case mpack_type_str:
      value_len = ::mpack_node_strlen(node);
      value_str = nvim::String(::mpack_node_str(node), value_len);
      return nvim::Type(value_str);
    case mpack_type_array:
      value_len = ::mpack_node_array_length(node);
      value_arr = nvim::Array();
      value_arr.reserve(value_len);
      for (size_t i = 0; i != value_len; i++)
	value_arr.push_back(read_object(::mpack_node_array_at(node, i)));
      return value_arr;

    case mpack_type_uint:
      return nvim::Type(::mpack_node_uint(node));

    case mpack_type_missing:
      throw std::runtime_error("type missing");

    default:
      throw std::runtime_error("unknown type");
  }
}

void nvim::Nvim::register_event(const std::string_view &name, EventFunction event, void* ptr)
{
  m_events[name] = {event, ptr};
}

namespace nvimim { extern FILE* log_file; }

void nvim::Nvim::poll()
{
  const Message msg = receive();
  if (m_events.contains(msg.get_method()))
  {
    auto event = m_events.at(msg.get_method());
    event.first(*this, msg, event.second);
  }else
    respond(msg, nullptr, nullptr);
}

void nvim::Nvim::send(const char* data, size_t len)
{
  if (::write(m_output, data, len) == -1)
    throw std::runtime_error("failed to write");
}

const nvim::Message nvim::Nvim::receive()
{
  size_t buffer_len = 1024;
  char buffer[buffer_len];

  ssize_t r;
  while ((r = ::read(m_input, buffer, buffer_len)) <= 0)
  {
    if (r == -1)
      throw std::runtime_error("failed to read");
  }

  ::mpack_tree_t tree;
  ::mpack_tree_init_data(&tree, buffer, r);
  ::mpack_tree_parse(&tree);
  ReturnType val = read_object(::mpack_tree_root(&tree));

  if (::mpack_tree_destroy(&tree) != mpack_ok)
    throw std::runtime_error("failed to destroy mpack reader");

  uint8_t msg_type = val.as_array().at(0).as_integer();

  if (msg_type == 0)
  {
    Integer msg_id = val.as_array().at(1).as_integer();
    String method = val.as_array().at(2).as_string();
    Array params = val.as_array().at(3).as_array();
    return Message(msg_id, method, params);
  }else if (msg_type == 1)
  {
    Integer msg_id = val.as_array().at(1).as_integer();
    Type error = val.as_array().at(2);
    Type result = val.as_array().at(3);
    return Message(msg_id, error, result);
  }else if (msg_type == 2)
  {
    String method = val.as_array().at(1).as_string();
    Array params = val.as_array().at(2).as_array();
    return Message(method, params);
  }
  return Message();
}

nvim::ReturnType nvim::Nvim::call_atomic(ArgType calls)
{
  const Message msg = request("nvim_call_atomic", calls);
  return msg.get_result();
}

void nvim::Nvim::chan_send(ArgType chan, ArgType data)
{
  const Message msg = request("nvim_chan_send", chan, data);
}

nvim::ReturnType nvim::Nvim::create_buf(ArgType listed, ArgType scratch)
{
  const Message msg = request("nvim_create_buf", listed, scratch);
  return msg.get_result();
}

void nvim::Nvim::del_current_line()
{
  const Message msg = request("nvim_del_current_line");
}

void nvim::Nvim::del_keymap(ArgType mode, ArgType lhs)
{
  const Message msg = request("nvim_del_current_line", mode, lhs);
}

nvim::ReturnType nvim::Nvim::del_mark(ArgType name)
{
  const Message msg = request("nvim_del_current_line", name);
  return msg.get_result();
}

void nvim::Nvim::del_var()
{
  const Message msg = request("nvim_del_var");
}

void nvim::Nvim::echo(ArgType chunks, ArgType history)
{
  const Message msg = request("nvim_del_current_line", chunks, history);
}

void nvim::Nvim::err_write(ArgType str)
{
  const Message msg = request("nvim_err_write", str);
}

void nvim::Nvim::err_writeln(ArgType str)
{
  const Message msg = request("nvim_err_writeln", str);
}

nvim::ReturnType nvim::Nvim::eval_status_line(ArgType str, ArgType opts)
{
  const Message msg = request("nvim_status_line", str, opts);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::exec_lua(ArgType code, ArgType args)
{
  const Message msg = request("nvim_exec_lua", code, args);
  return msg.get_result();
}

void nvim::Nvim::feedkeys(ArgType keys, ArgType mode, ArgType escape_csi)
{
  const Message msg = request("nvim_feedkeys", keys, mode, escape_csi);
}

nvim::ReturnType nvim::Nvim::get_all_options_info()
{
  const Message msg = request("nvim_get_all_options_info");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_api_info()
{
  const Message msg = request("nvim_get_api_info");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_chan_info(ArgType chan)
{
  const Message msg = request("nvim_get_chan_info", chan);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_color_by_name(ArgType name)
{
  const Message msg = request("nvim_get_color_by_name", name);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_color_map()
{
  const Message msg = request("nvim_get_color_map");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_commands(ArgType opts)
{
  const Message msg = request("nvim_get_commands", opts);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_context(ArgType opts)
{
  const Message msg = request("nvim_get_context", opts);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_current_buf()
{
  const Message msg = request("nvim_get_current_buf");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_current_line()
{
  const Message msg = request("nvim_get_current_line");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_current_tabpage()
{
  const Message msg = request("nvim_get_current_tabpage");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_current_win()
{
  const Message msg = request("nvim_get_current_win");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_hl_by_id(ArgType hl_id, ArgType rgb)
{
  const Message msg = request("nvim_get_hl_by_id", hl_id, rgb);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_hl_by_name(ArgType name, ArgType rgb)
{
  const Message msg = request("nvim_get_hl_by_name", name, rgb);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_hl_id_by_name(ArgType name)
{
  const Message msg = request("nvim_get_hl_id_by_name", name);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_keymap(ArgType mode)
{
  const Message msg = request("nvim_get_keymap", mode);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_mark(ArgType name, ArgType opts)
{
  const Message msg = request("nvim_get_mark", name, opts);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_mode()
{
  const Message msg = request("nvim_get_mode");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_option(ArgType name)
{
  const Message msg = request("nvim_get_option", name);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_option_info(ArgType name)
{
  const Message msg = request("nvim_get_option_info", name);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_proc(ArgType pid)
{
  const Message msg = request("nvim_get_proc", pid);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_proc_children(ArgType pid)
{
  const Message msg = request("nvim_get_proc_children", pid);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_runtime_file(ArgType name, ArgType all)
{
  const Message msg = request("nvim_get_runtime_file", name, all);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_var(ArgType name)
{
  const Message msg = request("nvim_get_var", name);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::get_vvar(ArgType name)
{
  const Message msg = request("nvim_get_vvar", name);
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::input(ArgType keys)
{
  const Message msg = request("nvim_input", keys);
  return msg.get_result();
}

void nvim::Nvim::input_mouse(ArgType button, ArgType action, ArgType modifier, ArgType grid, ArgType row, ArgType col)
{
  const Message msg = request("nvim_input_mouse", button, action, modifier, grid, row, col);
}

nvim::ReturnType nvim::Nvim::list_bufs()
{
  const Message msg = request("nvim_list_bufs");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::list_chans()
{
  const Message msg = request("nvim_list_chans");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::list_runtime_paths()
{
  const Message msg = request("nvim_list_runtime_paths");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::list_tabpages()
{
  const Message msg = request("nvim_list_tabpages");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::list_uis()
{
  const Message msg = request("nvim_list_uis");
  return msg.get_result();
}

nvim::ReturnType nvim::Nvim::list_wins()
{
  const Message msg = request("nvim_list_wins");
  return msg.get_result();
}

void nvim::Nvim::load_context(ArgType dict)
{
  const Message msg = request("nvim_load_context", dict);
}

void nvim::Nvim::notify(ArgType msg, ArgType log_level, ArgType opts)
{
  const Message msg2 = request("nvim_notify", msg, log_level, opts);
}

nvim::ReturnType nvim::Nvim::open_term(ArgType buffer, ArgType opts)
{
  const Message msg = request("nvim_open_term", buffer, opts);
  return msg.get_result();
}

void nvim::Nvim::out_write(ArgType str)
{
  const Message msg = request("nvim_out_write", str);
}

nvim::ReturnType nvim::Nvim::paste(ArgType data, ArgType crlf, ArgType phase)
{
  const Message msg = request("nvim_paste", data, crlf, phase);
  return msg.get_result();
}

void nvim::Nvim::put(ArgType lines, ArgType type, ArgType after, ArgType follow)
{
  const Message msg = request("nvim_put", lines, type, after, follow);
}

void nvim::Nvim::replace_termcodes(ArgType str, ArgType from_part, ArgType do_lt, ArgType special)
{
  const Message msg = request("nvim_replace_termcodes", str, from_part, do_lt, special);
}

void nvim::Nvim::select_popupmenu_item(ArgType item, ArgType insert, ArgType finish, ArgType opts)
{
  const Message msg = request("nvim_select_popupmenu_item", item, insert, finish, opts);
}

void nvim::Nvim::set_client_info(ArgType name, ArgType version, ArgType type, ArgType methods, ArgType attributes)
{
  const Message msg = request("nvim_set_client_info", name, version, type, methods, attributes);
}

void nvim::Nvim::set_current_buf(ArgType buffer)
{
  const Message msg = request("nvim_set_current_buf", buffer);
}

void nvim::Nvim::set_current_dir(ArgType dir)
{
  const Message msg = request("nvim_set_current_dir", dir);
}

void nvim::Nvim::set_current_line(ArgType line)
{
  const Message msg = request("nvim_set_current_line", line);
}

void nvim::Nvim::set_current_tabpage(ArgType tabpage)
{
  const Message msg = request("nvim_set_current_tabpage", tabpage);
}

void nvim::Nvim::set_current_win(ArgType win)
{
  const Message msg = request("nvim_set_current_win", win);
}

void nvim::Nvim::set_hl(ArgType ns_id, ArgType name, ArgType val)
{
  const Message msg = request("nvim_set_hl", ns_id, name, val);
}

void nvim::Nvim::set_keymap(ArgType mode, ArgType lhs, ArgType rhs, ArgType opts)
{
  const Message msg = request("nvim_set_keymap", mode, lhs, rhs, opts);
}

void nvim::Nvim::set_option(ArgType name, ArgType value)
{
  const Message msg = request("nvim_set_option", name, value);
}

void nvim::Nvim::set_var(ArgType name, ArgType value)
{
  const Message msg = request("nvim_set_var", name, value);
}

void nvim::Nvim::set_vvar(ArgType name, ArgType value)
{
  const Message msg = request("nvim_set_vvar", name, value);
}

nvim::ReturnType nvim::Nvim::strwidth(ArgType text)
{
  const Message msg = request("nvim_strwidth", text);
  return msg.get_result();
}

void nvim::Nvim::subscribe(ArgType event)
{
  const Message msg = request("nvim_subscribe", event);
}

void nvim::Nvim::unsubscribe(ArgType event)
{
  const Message msg = request("nvim_unsubscribe", event);
}
