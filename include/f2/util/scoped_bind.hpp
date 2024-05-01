#pragma once

#include <tuple>

namespace f2 {

  template<typename T>
  concept Bindable = requires(T t) {
    { t.bind() } -> std::same_as<void>;
    { t.unbind() } -> std::same_as<void>;
  };

  template<Bindable ... Ts>
  class scoped_bind {
  public:
    scoped_bind(Ts& ... objects) : _objects(objects...) {
      std::apply([](auto& ... objects) { (objects.bind(), ...); }, _objects);
    }

    ~scoped_bind() {
      std::apply([](auto& ... objects) { (objects.unbind(), ...); }, _objects);
    }

    scoped_bind(const scoped_bind&) = delete;
    scoped_bind(scoped_bind&&) = delete;

    scoped_bind& operator=(const scoped_bind&) = delete;
    scoped_bind& operator=(scoped_bind &&) = delete;

  private:
    std::tuple<Ts& ...> _objects;
  };

}
