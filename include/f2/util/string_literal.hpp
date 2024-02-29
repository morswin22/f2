#pragma once
#include <algorithm>

// TODO: move implementation to a source file
// TODO: maybe place into a f2 namespace

template <auto N>
struct string_literal {
  constexpr string_literal(const char (&str)[N]) { 
    std::copy_n(str, N, value);
  }

  char value[N];
};

