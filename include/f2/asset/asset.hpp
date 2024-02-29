#pragma once
#include <f2/util/string_literal.hpp>
#include <f2/asset/image.hpp>

namespace f2 {
  template <string_literal S>
  struct asset {
    operator image() const {
      return image(S.value); // may not be an image
    }
    // other operators to turn into any asset (image, shader, etc)
  };
}
