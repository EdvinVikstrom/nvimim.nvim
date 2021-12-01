#include "nvim/Type.hpp"
#include "nvim/types/Buffer.hpp"
#include "nvim/types/Window.hpp"
#include "nvim/types/Tabpage.hpp"

nvim::Type::Type() : m_type(Types::NIL) { }
nvim::Type::Type(const String &str) : m_type(Types::STRING), m_string(str) { }
nvim::Type::Type(Boolean b) : m_type(Types::BOOLEAN), m_boolean(b) { }
nvim::Type::Type(Integer i) : m_type(Types::INTEGER), m_integer(i) { }
nvim::Type::Type(uint32_t i) : m_type(Types::INTEGER), m_integer(i) { }
nvim::Type::Type(Float f) : m_type(Types::FLOAT), m_float(f) { }
nvim::Type::Type(const Array &arr) : m_type(Types::ARRAY), m_array(arr) { }
nvim::Type::Type(const Buffer &buf) : m_type(Types::BUFFER), m_special{buf} { }
nvim::Type::Type(const Window &win) : m_type(Types::WINDOW), m_special{win} { }
nvim::Type::Type(const Tabpage &tab) : m_type(Types::TABPAGE), m_special{tab} { }

nvim::Types nvim::Type::get_type() const { return m_type; }
nvim::Boolean nvim::Type::as_boolean() const { return m_boolean; }
nvim::Integer nvim::Type::as_integer() const { return m_integer; }
nvim::Float nvim::Type::as_float() const { return m_float; }
const nvim::String& nvim::Type::as_string() const { return m_string; }
const nvim::Array& nvim::Type::as_array() const { return m_array; }
nvim::Buffer nvim::Type::as_buffer() const { return Buffer{m_special}; }
nvim::Window nvim::Type::as_window() const { return Window{m_special}; }
nvim::Tabpage nvim::Type::as_tabpage() const { return Tabpage{m_special}; }
