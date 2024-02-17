#pragma once
#include <f2/runtime/window.hpp>

namespace f2 {

  struct frame {
    const bool should_close{ false };
    int width;
    int height;
    double elapsed_time;
    f2::window& window;

    // frame() = default;
    frame(bool _should_close, f2::window& _window);
    ~frame();
    

    operator bool() const;
  };

}
