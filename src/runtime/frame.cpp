#include <f2/runtime/frame.hpp>

f2::frame::operator bool() const {
  return !should_close;
}

f2::frame::frame(bool _should_close, f2::window& _window)
  : should_close(_should_close), window(_window) {
  // TODO: skip for now 
}

f2::frame::~frame() {
  window.end_frame(); 
}
