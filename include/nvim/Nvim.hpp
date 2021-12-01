#ifndef NVIM_HPP
  #define NVIM_HPP

#include "nvim/Message.hpp"
#include "nvim/types/Buffer.hpp"
#include "nvim/types/Window.hpp"
#include "nvim/types/Tabpage.hpp"
#include "mpack/mpack.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <map>

namespace nvim {

  typedef void (*EventFunction) (class Nvim &nvim, const Message &msg, void* ptr);

  class Nvim {

  private:

    int m_output;
    int m_input;
    uint32_t m_message;
    std::map<std::string_view, std::pair<EventFunction, void*>> m_events;

  public:

    Nvim(int output, int input);

    void register_event(const std::string_view &name, EventFunction event, void* ptr);
    void poll();

    template<typename... Args>
    const Message request(const std::string_view &method, const Args&... args);

    template<typename... Args>
    const Message notification(const std::string_view &method, const Args&... args);

    template<typename Error, typename Result>
    void respond(const Message &msg, const Error &error, const Result &result);

    ReturnType call_atomic(ArgType calls);
    void chan_send(ArgType chan, ArgType data);
    ReturnType create_buf(ArgType listed, ArgType scratch);
    void del_current_line();
    void del_keymap(ArgType mode, ArgType lhs);
    ReturnType del_mark(ArgType name);
    void del_var();
    void echo(ArgType chunks, ArgType history);
    void err_write(ArgType str);
    void err_writeln(ArgType str);
    ReturnType eval_status_line(ArgType str, ArgType opts);
    ReturnType exec_lua(ArgType code, ArgType args);
    void feedkeys(ArgType keys, ArgType mode, ArgType escape_csi);
    ReturnType get_all_options_info();
    ReturnType get_api_info();
    ReturnType get_chan_info(ArgType chan);
    ReturnType get_color_by_name(ArgType name);
    ReturnType get_color_map();
    ReturnType get_commands(ArgType opts);
    ReturnType get_context(ArgType opts);
    ReturnType get_current_buf();
    ReturnType get_current_line();
    ReturnType get_current_tabpage();
    ReturnType get_current_win();
    ReturnType get_hl_by_id(ArgType hl_id, ArgType rgb);
    ReturnType get_hl_by_name(ArgType name, ArgType rgb);
    ReturnType get_hl_id_by_name(ArgType name);
    ReturnType get_keymap(ArgType mode);
    ReturnType get_mark(ArgType name, ArgType opts);
    ReturnType get_mode();
    ReturnType get_option(ArgType name);
    ReturnType get_option_info(ArgType name);
    ReturnType get_proc(ArgType pid);
    ReturnType get_proc_children(ArgType pid);
    ReturnType get_runtime_file(ArgType name, ArgType all);
    ReturnType get_var(ArgType name);
    ReturnType get_vvar(ArgType name);
    ReturnType input(ArgType keys);
    void input_mouse(ArgType button, ArgType action, ArgType modifier, ArgType grid, ArgType row, ArgType col);
    ReturnType list_bufs();
    ReturnType list_chans();
    ReturnType list_runtime_paths();
    ReturnType list_tabpages();
    ReturnType list_uis();
    ReturnType list_wins();
    void load_context(ArgType dict);
    void notify(ArgType msg, ArgType log_level, ArgType opts);
    ReturnType open_term(ArgType buffer, ArgType opts);
    void out_write(ArgType str);
    ReturnType paste(ArgType data, ArgType crlf, ArgType phase);
    void put(ArgType lines, ArgType type, ArgType after, ArgType follow);
    void replace_termcodes(ArgType str, ArgType from_part, ArgType do_lt, ArgType special);
    void select_popupmenu_item(ArgType item, ArgType insert, ArgType finish, ArgType opts);
    void set_client_info(ArgType name, ArgType version, ArgType type, ArgType methods, ArgType attributes);
    void set_current_buf(ArgType buffer);
    void set_current_dir(ArgType dir);
    void set_current_line(ArgType line);
    void set_current_tabpage(ArgType tabpage);
    void set_current_win(ArgType win);
    void set_hl(ArgType ns_id, ArgType name, ArgType val);
    void set_keymap(ArgType mode, ArgType lhs, ArgType rhs, ArgType opts);
    void set_option(ArgType name, ArgType value);
    void set_var(ArgType name, ArgType value);
    void set_vvar(ArgType name, ArgType value);
    ReturnType strwidth(ArgType text);
    void subscribe(ArgType event);
    void unsubscribe(ArgType event);

  private:

    void send(const char* data, size_t len);
    const Message receive();

  };

}

template<typename Type, typename... Args> static void iterate_args(::mpack_writer_t* writer, const Type &arg, const Args&... rest);
static void write_param(::mpack_writer_t* writer, nvim::ArgType arg);
static void write_param(::mpack_writer_t* writer, std::nullptr_t);
static void write_param(::mpack_writer_t* writer, nvim::Boolean arg);
static void write_param(::mpack_writer_t* writer, nvim::Integer arg);
static void write_param(::mpack_writer_t* writer, nvim::Float arg);
static void write_param(::mpack_writer_t* writer, const nvim::String &arg);
static void write_param(::mpack_writer_t* writer, const nvim::Buffer &arg);
static void write_param(::mpack_writer_t* writer, const nvim::Window &arg);
static void write_param(::mpack_writer_t* writer, const nvim::Tabpage &arg);
template<typename Type> static void write_param(::mpack_writer_t* writer, const std::vector<Type> &arg);

template<typename Type, typename... Args>
static void iterate_args(::mpack_writer_t* writer, const Type &arg, const Args&... rest)
{
  write_param(writer, arg);
  if constexpr (sizeof...(rest) != 0)
    return iterate_args(writer, rest...);
}

static inline void write_param(::mpack_writer_t* writer, nvim::ArgType arg)
{
  if (arg.get_type() == nvim::Types::NIL) write_param(writer, nullptr);
  if (arg.get_type() == nvim::Types::BOOLEAN) write_param(writer, arg.as_boolean());
  if (arg.get_type() == nvim::Types::INTEGER) write_param(writer, arg.as_integer());
  if (arg.get_type() == nvim::Types::FLOAT) write_param(writer, arg.as_float());
  if (arg.get_type() == nvim::Types::STRING) write_param(writer, arg.as_string());
  if (arg.get_type() == nvim::Types::ARRAY) write_param(writer, arg.as_array());
  if (arg.get_type() == nvim::Types::BUFFER) write_param(writer, arg.as_buffer());
  if (arg.get_type() == nvim::Types::WINDOW) write_param(writer, arg.as_window());
  if (arg.get_type() == nvim::Types::TABPAGE) write_param(writer, arg.as_tabpage());
}

static inline void write_param(::mpack_writer_t* writer, std::nullptr_t)
{
  ::mpack_write_nil(writer);
}

static inline void write_param(::mpack_writer_t* writer, nvim::Boolean arg)
{
  ::mpack_write_bool(writer, arg);
}

static inline void write_param(::mpack_writer_t* writer, nvim::Integer arg)
{
  ::mpack_write_i64(writer, arg);
}

static inline void write_param(::mpack_writer_t* writer, nvim::Float arg)
{
  ::mpack_write_double(writer, arg);
}

static inline void write_param(::mpack_writer_t* writer, const nvim::String &arg)
{
  ::mpack_write_str(writer, arg.data(), arg.size());
}

template<typename Type>
static void write_param(::mpack_writer_t* writer, const std::vector<Type> &arg)
{
  ::mpack_build_array(writer);
  for (const Type &v : arg)
    write_param(writer, v);
  ::mpack_complete_array(writer);
}

static inline void write_param(::mpack_writer_t* writer, const nvim::Buffer &arg)
{
  ::mpack_write_i64(writer, arg.id);
}

static inline void write_param(::mpack_writer_t* writer, const nvim::Window &arg)
{
  ::mpack_write_i64(writer, arg.id);
}

static inline void write_param(::mpack_writer_t* writer, const nvim::Tabpage &arg)
{
  ::mpack_write_i64(writer, arg.id);
}

template<typename... Args>
const nvim::Message nvim::Nvim::request(const std::string_view &method, const Args&... args)
{
  char* data;
  size_t len;

  ::mpack_writer_t writer;
  ::mpack_writer_init_growable(&writer, &data, &len);

  ::mpack_build_array(&writer);
  ::mpack_write_u64(&writer, 0);
  ::mpack_write_u64(&writer, m_message++);
  ::mpack_write_str(&writer, method.data(), method.size());

  ::mpack_build_array(&writer);
  if constexpr (sizeof...(args) != 0)
    iterate_args(&writer, args...);
  ::mpack_complete_array(&writer);

  ::mpack_complete_array(&writer);

  if (::mpack_writer_destroy(&writer) != mpack_ok)
    throw std::runtime_error("failed to destroy mpack writer");

  send(data, len);
  ::free(data);
  return receive();
}

template<typename... Args>
const nvim::Message nvim::Nvim::notification(const std::string_view &method, const Args&... args)
{
  char* data;
  size_t len;

  ::mpack_writer_t writer;
  ::mpack_writer_init_growable(&writer, &data, &len);

  ::mpack_build_array(&writer);
  ::mpack_write_u64(&writer, 2);
  ::mpack_write_str(&writer, method.data(), method.size());

  ::mpack_build_array(&writer);
  if constexpr (sizeof...(args) != 0)
    iterate_args(&writer, args...);
  ::mpack_complete_array(&writer);

  ::mpack_complete_array(&writer);

  if (::mpack_writer_destroy(&writer) != mpack_ok)
    throw std::runtime_error("failed to destroy mpack writer");

  send(data, len);
  ::free(data);
  return receive();
}

template<typename Error, typename Result>
void nvim::Nvim::respond(const Message &msg, const Error &error, const Result &result)
{
  char* data;
  size_t len;

  ::mpack_writer_t writer;
  ::mpack_writer_init_growable(&writer, &data, &len);

  ::mpack_build_array(&writer);
  ::mpack_write_u64(&writer, 1);
  ::mpack_write_u64(&writer, msg.get_id());

  //::mpack_write_u8(&writer, 0);
  //::mpack_write_u8(&writer, 0);
  ::mpack_write_nil(&writer);
  ::mpack_write_nil(&writer);
  //write_param(&writer, error);
  //write_param(&writer, result);

  ::mpack_complete_array(&writer);

  if (::mpack_writer_destroy(&writer) != mpack_ok)
    throw std::runtime_error("failed to destroy mpack writer");

  send(data, len);
  ::free(data);
}

#endif
