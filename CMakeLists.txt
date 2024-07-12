cmake_minimum_required(VERSION 3.28)
project(f2)
set(CMAKE_CXX_STANDARD 26)

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

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG ae721c50eaf761660b4f90cc590453cdb0c2acd0 
)

FetchContent_Declare(
  tinyobjloader
  GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
  GIT_TAG cab4ad7254cbf7eaaafdb73d272f99e92f166df8
)

FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG 4e49843d1f70d6bb47727d3bc67720e9abb03df1
)

FetchContent_Declare(
  cmake-scripts
  GIT_REPOSITORY https://github.com/StableCoder/cmake-scripts.git
  GIT_TAG 45f72fdc2d715030c6cb61e3d5977811ea45abda
)

FetchContent_MakeAvailable(glfw glew glm stb tinyobjloader doctest cmake-scripts)

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

add_executable(assets-codegen assets_codegen/assets_codegen.cpp)

function(f2_set_assets_dir target path)
  file(GLOB_RECURSE assets "${path}/*")
  file(GLOB templates "${CMAKE_SOURCE_DIR}/assets_codegen/*_template.*pp")
  
  set(output_path "${CMAKE_BINARY_DIR}/f2_assets/${target}")
  set(output "")
  foreach(asset ${assets})
    get_filename_component(filename ${asset} NAME)
    list(APPEND output "${output_path}/${filename}.cpp")
  endforeach()

  add_custom_command(OUTPUT "${output_path}/f2_assets.hpp" ${output}
    COMMAND assets-codegen "${path}" "${output_path}" "${CMAKE_SOURCE_DIR}/assets_codegen" "${target}"
    DEPENDS ${assets} ${templates}
    VERBATIM
  )

  add_library("f2_assets_for_${target}" STATIC ${output})
  target_link_libraries("f2_assets_for_${target}" tinyobjloader)
  target_include_directories("f2_assets_for_${target}"
    PUBLIC "${CMAKE_BINARY_DIR}/f2_assets"
    PUBLIC "${CMAKE_SOURCE_DIR}/include")

  target_link_libraries(${target} "f2_assets_for_${target}")
endfunction()

function(f2_common_properties target)
  target_include_directories(${target}
    PUBLIC include
    PUBLIC ${stb_SOURCE_DIR}
  )
  target_link_libraries(${target} glfw libglew_static glm tinyobjloader doctest)
  f2_set_assets_dir(${target} "${CMAKE_SOURCE_DIR}/src/render/shaders")
  if (f2-enable-imgui)
    target_compile_definitions(${target} PUBLIC -Df2_enable_imgui)
    target_link_libraries(${target} imgui_cmake)
  endif()
endfunction()

file(GLOB_RECURSE f2-src "src/**/*.cpp")
file(GLOB_RECURSE f2-headers "include/f2/**/*.hpp")

add_library(f2 STATIC ${f2-src})
f2_common_properties(f2)
target_sources(f2
  PUBLIC FILE_SET HEADERS
  BASE_DIRS include
  FILES ${f2-headers}
)
target_compile_definitions(f2 PUBLIC DOCTEST_CONFIG_DISABLE)

# Tests
add_executable(f2-doctest ${f2-src} "test/doctest.cpp")
f2_common_properties(f2-doctest)

# set(CODE_COVERAGE ON)
include("${cmake-scripts_SOURCE_DIR}/code-coverage.cmake")
target_code_coverage(f2-doctest PLAIN EXTERNAL EXCLUDE "/usr/*;*f2/build/*")

# User-space
set(assets-dir "${CMAKE_BINARY_DIR}/assets")
file(DOWNLOAD https://storage3d.com/storage/2019.10/0df7af19ab278d1489bca0d4c5467361.jpg "${assets-dir}/bricks_texture.jpg")
file(DOWNLOAD https://gist.githubusercontent.com/morswin22/afbfb5cb638f0fe2efdceae40bb91f44/raw/ad10e81bc5d0d0a83b08df835c958251fa346e4f/texture.vert "${assets-dir}/texture.vert")
file(DOWNLOAD https://gist.githubusercontent.com/morswin22/afbfb5cb638f0fe2efdceae40bb91f44/raw/ad10e81bc5d0d0a83b08df835c958251fa346e4f/texture.frag "${assets-dir}/texture.frag")
file(DOWNLOAD https://graphics.stanford.edu/~mdfisher/Data/Meshes/bunny.obj "${assets-dir}/bunny.obj")

add_executable(main "test/main.cpp")
target_link_libraries(main f2)
f2_set_assets_dir(main ${assets-dir})

include(GNUInstallDirs)
install(TARGETS f2 main
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/f2
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
