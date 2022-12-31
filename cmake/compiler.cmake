# compiler.cmake
# any compiler specific details do them here

macro(SetupCompilerForTarget arg)
  if(NOT MSVC)
  target_compile_options(${arg} PRIVATE -Wall -Wextra)
  endif()
  set_target_properties(${arg} PROPERTIES CXX_STANDARD 20)
  set_target_properties(${arg} PROPERTIES CMAKE_COMPILE_WARNING_AS_ERROR ON)
  set_property(TARGET ${arg} PROPERTY COMPILE_WARNING_AS_ERROR ON)
endmacro()

