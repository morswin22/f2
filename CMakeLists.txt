cmake_minimum_required(VERSION 3.28)
project(f2)

# option(f2-enable-imgui "Compile with Dear ImGui" TRUE)
set(f2-enable-imgui TRUE)

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

if(f2-enable-imgui)
  FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
  )
  FetchContent_MakeAvailable(imgui)

  project(imgui_cmake)
  file(GLOB imgui_sources "${imgui_SOURCE_DIR}/*.cpp")
  set(imgui_backend_sources "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp;${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")
  add_library(imgui_cmake ${imgui_sources} ${imgui_backend_sources})
  target_include_directories(imgui_cmake PUBLIC ${imgui_SOURCE_DIR} PUBLIC "${imgui_SOURCE_DIR}/backends" PUBLIC "${glfw_SOURCE_DIR}/include")
endif()

file(GLOB_RECURSE f2-src "src/**/*.cpp")
add_library(f2 STATIC ${f2-src})
target_include_directories(f2
  PUBLIC include
)
target_link_libraries(f2 glfw libglew_static glm)
if (f2-enable-imgui)
  target_compile_definitions(f2 PUBLIC -Df2_enable_imgui)
  target_link_libraries(f2 imgui_cmake)
endif()

add_executable(main "test/main.cpp")
target_link_libraries(main f2)
