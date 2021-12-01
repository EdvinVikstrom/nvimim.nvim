#ifndef NVIMXX_OBJECT_HPP
  #define NVIMXX_OBJECT_HPP

#include "nvim/types/Basic.hpp"
#include "nvim/types/Special.hpp"

#include <string>
#include <vector>

namespace nvim {

  enum class Types {
    NIL,
    STRING,
    BOOLEAN,
    INTEGER,
    FLOAT,
    ARRAY,
    BUFFER,
    WINDOW,
    TABPAGE
  };

  struct Buffer;
  struct Window;
  struct Tabpage;

  class Type {

  private:

    Types m_type;
    union {
      Boolean m_boolean;
      Integer m_integer;
      Float m_float;
      SpecialType m_special;
    };
    String m_string;
    Array m_array;

  public:

    Type();
    Type(const String &str);
    Type(Boolean b);
    Type(Integer i);
    Type(uint32_t i);
    Type(Float f);
    Type(const Array &arr);
    Type(const Buffer &buf);
    Type(const Window &win);
    Type(const Tabpage &tab);

    Types get_type() const;
    Boolean as_boolean() const;
    Integer as_integer() const;
    Float as_float() const;
    const String& as_string() const;
    const Array& as_array() const;
    Buffer as_buffer() const;
    Window as_window() const;
    Tabpage as_tabpage() const;

  };

  typedef const Type& ArgType;
  typedef const Type ReturnType;

}

#endif
