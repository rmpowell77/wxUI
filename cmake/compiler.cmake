# compiler.cmake
# any compiler specific details do them here

macro(wxUI_SetupCompilerForTarget arg)
  if(NOT MSVC)
  target_compile_options(${arg} PRIVATE -Wall -Wextra)
  endif()
  set_target_properties(${arg} PROPERTIES CXX_STANDARD 20)
  set_property(TARGET ${arg} PROPERTY COMPILE_WARNING_AS_ERROR ON)
endmacro()

