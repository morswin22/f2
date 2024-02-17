cmake_minimum_required(VERSION 3.28)
project(f2)

include(FetchContent)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        076bfd55be45e7ba5c887d4b32aa03d26881a1fb
)

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG 883e35a3d493d93fa27da5abb6225654c360f9d0
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 2759ceca04db5c36d2806a2280f51b83a6749c12
)

FetchContent_MakeAvailable(glfw glew glm)

file(GLOB_RECURSE f2-src "src/**/*.cpp")
add_library(f2 STATIC ${f2-src})
target_include_directories(f2
  PUBLIC include
)
target_link_libraries(f2 glfw libglew_static glm)

add_executable(main "test/main.cpp")
target_link_libraries(main f2)
