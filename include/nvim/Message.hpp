#ifndef NVIM_MESSAGE_HPP
  #define NVIM_MESSAGE_HPP

#include "nvim/Type.hpp"

namespace nvim {

  enum class MessageTypes {
    REQUEST = 0, RESPONSE = 1, NOTIFICATION = 2, INVALID = 3
  };

  class Message {

  private:

    const MessageTypes m_type;
    const Integer m_id;
    const std::string m_method;
    const std::vector<Type> m_params;

    /* Request */
    Message(Integer id, const std::string_view &method, const std::vector<Type> &params)
      : m_type(MessageTypes::REQUEST), m_id(id), m_method(method), m_params(params)
    {
    }

    /* Response */
    Message(Integer id, ArgType error, ArgType result)
      : m_type(MessageTypes::RESPONSE), m_id(id), m_method(""), m_params({error, result})
    {
    }

    /* Notification */
    Message(const std::string_view &method, const std::vector<Type> &params)
      : m_type(MessageTypes::NOTIFICATION), m_id(0), m_method(method), m_params(params)
    {
    }

    /* Invalid */
    Message()
      : m_type(MessageTypes::INVALID), m_id(0), m_method("")
    {
    }

    friend class Nvim;

  public:

    MessageTypes get_type() const { return m_type; }
    Integer get_id() const { return m_id; }
    const std::string& get_method() const { return m_method; }
    const std::vector<Type>& get_params() const { return m_params; }
    ReturnType get_error() const { return m_params.at(0); }
    ReturnType get_result() const { return m_params.at(1); }

  };

}

#endif
