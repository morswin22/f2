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

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) \
  CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) \
  CONCATENATE(str, __LINE__)
#endif

#define F2_SCOPE_BIND \
  f2::scoped_bind ANONYMOUS_VARIABLE(F2_SCOPE_BIND_STATE)

