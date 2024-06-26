#pragma once
#include <f2/runtime/window.hpp>

namespace f2 {

  struct frame {
    const bool should_close;
    const int width;
    const int height;
    const double elapsed_time;
    const size_t frame_count;
    f2::window& window;

    frame(bool _should_close, int _width, int _height, double _elapsed_time, size_t _frame_count, f2::window& _window);
    ~frame();

    operator bool() const { return !should_close; }
  };

}
