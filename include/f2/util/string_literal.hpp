#pragma once
#include <algorithm>

// TODO: move implementation to a source file
// TODO: maybe place into a f2 namespace

template <auto N>
struct string_literal {
  constexpr string_literal(const char (&str)[N]) { 
    std::copy_n(str, N, value);
  }

  template <auto M>
  constexpr bool ends_with(const char (&str)[M]) const {
    if (N < M)
      return false;
    for (int i = 0; i < M; ++i)
      if (value[N - M + i] != str[i])
        return false;
    return true;
  }

  char value[N];
};

