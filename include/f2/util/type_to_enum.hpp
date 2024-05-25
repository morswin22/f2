#pragma once

#include <GL/glew.h>
#include <type_traits>

namespace f2 {

  namespace details {
    template<typename ...>
    inline constexpr bool always_false = false;
  }

  template<typename T>
  constexpr GLenum type_to_enum() {
    if constexpr (std::is_same_v<T, float>) 
      return GL_FLOAT;
    else if constexpr (std::is_same_v<T, double>)
      return GL_DOUBLE;
    else if constexpr (std::is_same_v<T, int>)
      return GL_INT;
    else if constexpr (std::is_same_v<T, unsigned int>)
      return GL_UNSIGNED_INT;
    else if constexpr (std::is_same_v<T, short>)
      return GL_SHORT;
    else if constexpr (std::is_same_v<T, unsigned short>)
      return GL_UNSIGNED_SHORT;
    else if constexpr (std::is_same_v<T, char>)
      return GL_BYTE;
    else if constexpr (std::is_same_v<T, unsigned char>)
      return GL_UNSIGNED_BYTE;
    else
      static_assert(details::always_false<T>, "Unsupported type");
  }

}
