cmake_minimum_required(VERSION 3.23)
project(wxUI_examples VERSION 0.1)

add_executable(wxUI_HelloWorld
  ExtendedExample.cpp
  ExtendedExample.hpp
  HelloWorld.cpp
)

# Special stuff for Mac
if(APPLE)
set_target_properties(
  wxUI_HelloWorld PROPERTIES
  MACOSX_BUNDLE TRUE
)
endif()

if(MSVC)
set_target_properties(
  wxUI_HelloWorld PROPERTIES
  WIN32_EXECUTABLE TRUE
)
endif()

wxUI_SetupCompilerForTarget(wxUI_HelloWorld)

target_link_libraries(wxUI_HelloWorld 
  wx::core
  wxUI::wxUI
)


