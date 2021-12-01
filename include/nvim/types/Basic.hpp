#ifndef NVIM_TYPES_BASIC_HPP
  #define NVIM_TYPES_BASIC_HPP

#include "nvim/Defs.hpp"

#include <string>
#include <vector>

namespace nvim {

  typedef std::string String;
  typedef bool Boolean;
  typedef int64_t Integer;
  typedef double Float;
  typedef std::vector<class Type> Array;

}

#endif
