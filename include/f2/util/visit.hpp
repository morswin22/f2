#pragma once
#include <type_traits>
#include <utility>

// Thanks to CppCon 2015: Paul Fultz II “Reflection Techniques in C++” (https://github.com/cppcon/cppcon2015)

// Basic definitions
#define REM(...) __VA_ARGS__
#define EAT(...)

#define STRIP(x) EAT x
#define PAIR(x) REM x

#define NARGS_SEQ(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)

#define EACH(macro, ...) CAT(EACH_, NARGS(__VA_ARGS__))(macro, __VA_ARGS__)
#define EACH_1(m, x0) m(x0)
#define EACH_2(m, x0, x1) m(x0) m(x1)
#define EACH_3(m, x0, x1, x2) m(x0) m(x1) m(x2)
#define EACH_4(m, x0, x1, x2, x3) m(x0) m(x1) m(x2) m(x3)
#define EACH_5(m, x0, x1, x2, x3, x4) m(x0) m(x1) m(x2) m(x3) m(x4)
#define EACH_6(m, x0, x1, x2, x3, x4, x5) m(x0) m(x1) m(x2) m(x3) m(x4) m(x5)
#define EACH_7(m, x0, x1, x2, x3, x4, x5, x6) m(x0) m(x1) m(x2) m(x3) m(x4) m(x5) m(x6)
#define EACH_8(m, x0, x1, x2, x3, x4, x5, x6, x7) m(x0) m(x1) m(x2) m(x3) m(x4) m(x5) m(x6) m(x7)
#define EACH_9(m, x0, x1, x2, x3, x4, x5, x6, x7, x8) m(x0) m(x1) m(x2) m(x3) m(x4) m(x5) m(x6) m(x7) m(x8)
#define EACH_10(m, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) m(x0) m(x1) m(x2) m(x3) m(x4) m(x5) m(x6) m(x7) m(x8) m(x9)

#define EACH_ARG(macro, arg, ...) CAT(EACH_ARG_, NARGS(__VA_ARGS__))(macro, arg, __VA_ARGS__)
#define EACH_ARG_1(m, a, x0) m(x0, a)
#define EACH_ARG_2(m, a, x0, x1) m(x0, a) m(x1, a)
#define EACH_ARG_3(m, a, x0, x1, x2) m(x0, a) m(x1, a) m(x2, a)
#define EACH_ARG_4(m, a, x0, x1, x2, x3) m(x0, a) m(x1, a) m(x2, a) m(x3, a)
#define EACH_ARG_5(m, a, x0, x1, x2, x3, x4) m(x0, a) m(x1, a) m(x2, a) m(x3, a) m(x4, a)
#define EACH_ARG_6(m, a, x0, x1, x2, x3, x4, x5) m(x0, a) m(x1, a) m(x2, a) m(x3, a) m(x4, a) m(x5, a)
#define EACH_ARG_7(m, a, x0, x1, x2, x3, x4, x5, x6) m(x0, a) m(x1, a) m(x2, a) m(x3, a) m(x4, a) m(x5, a) m(x6, a)
#define EACH_ARG_8(m, a, x0, x1, x2, x3, x4, x5, x6, x7) m(x0, a) m(x1, a) m(x2, a) m(x3, a) m(x4, a) m(x5, a) m(x6, a) m(x7, a)
#define EACH_ARG_9(m, a, x0, x1, x2, x3, x4, x5, x6, x7, x8) m(x0, a) m(x1, a) m(x2, a) m(x3, a) m(x4, a) m(x5, a) m(x6, a) m(x7, a) m(x8, a)
#define EACH_ARG_10(m, a, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) m(x0, a) m(x1, a) m(x2, a) m(x3, a) m(x4, a) m(x5, a) m(x6, a) m(x7, a) m(x8, a) m(x9, a)

#define PRIMITIVE_COMMA(m) , m
#define COMMA(m) PRIMITIVE_COMMA EAT() (m)

#define PRIMITIVE_TAIL(x, ...) __VA_ARGS__
#define TAIL(...) PRIMITIVE_TAIL(__VA_ARGS__)

#define ENUM(macro, ...) TAIL(EACH(COMMA(macro), __VA_ARGS__))
#define ENUM_ARG(macro, arg, ...) TAIL(EACH_ARG(COMMA(macro), arg, __VA_ARGS__))


#define STRINGIZE(x) PRIMITIVE_STRINGIZE(x)
#define PRIMITIVE_STRINGIZE(x) #x

// Visitor pattern
#define MEMBER_EACH(x) PAIR(x);

#define UNPACK_EACH(x) \
  std::pair<std::string_view, decltype((self.STRIP(x)))>(STRINGIZE(STRIP(x)), self.STRIP(x))

#define UNPACK_MEMBER_EACH(x, name) \
  std::pair<std::string_view, decltype((&name::STRIP(x)))>(STRINGIZE(STRIP(x)), &name::STRIP(x))

#define F2_VISITABLE_STRUCT(name, ...) \
struct name { \
  EACH(MEMBER_EACH, __VA_ARGS__) \
  struct unpack { \
    template<class T, class F> \
    static void apply(T&& self, F&& f){ \
      f(ENUM(UNPACK_EACH, __VA_ARGS__)); \
    } \
    template<class F> \
    static void apply(F f) { \
      f(ENUM_ARG(UNPACK_MEMBER_EACH, name, __VA_ARGS__)); \
    } \
  }; \
}

namespace f2 {
  template<class C, class F>
  void visit_each(F f) {
    using unpack = typename std::remove_cv_t<std::remove_reference_t<C>>::unpack;
    unpack::apply([&](auto&&... xs) {
      (void)std::initializer_list<int>{
        (f(std::forward<decltype(xs)>(xs)), 0)...
      };
    });
  }

  template<class C, class F>
  void visit_each(C&& c, F f) {
    using unpack = typename std::remove_cv_t<std::remove_reference_t<C>>::unpack;
    unpack::apply(c, [&](auto&&... xs) {
      (void)std::initializer_list<int>{
        (f(std::forward<decltype(xs)>(xs)), 0)...
      };
    });
  }
}
