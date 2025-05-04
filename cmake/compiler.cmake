# compiler.cmake
# any compiler specific details do them here

macro(wxUI_SetupCompilerForTarget arg)
  if(NOT MSVC)
  target_compile_options(${arg} PRIVATE -Wall -Wextra)
  endif()
  set_target_properties(${arg} PROPERTIES CXX_STANDARD 20)
  set_property(TARGET ${arg} PROPERTY COMPILE_WARNING_AS_ERROR ON)
endmacro()

# Adding address sanitizer (see https://stackoverflow.com/questions/44320465/whats-the-proper-way-to-enable-addresssanitizer-in-cmake-that-works-in-xcode)
option(ENABLE_ASAN "Enable AddressSanitizer in Debug" ON)
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND ENABLE_ASAN AND NOT MSVC)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()
