cmake_minimum_required(VERSION 3.23)
project(wxUI_examples VERSION 0.1)

add_executable(wxUI_HelloWidgets
  ExtendedExample.cpp
  ExtendedExample.hpp
  HelloWidgets.cpp
)

# Special stuff for Mac
if(APPLE)
set_target_properties(
  wxUI_HelloWidgets PROPERTIES
  MACOSX_BUNDLE TRUE
)
endif()

if(MSVC)
set_target_properties(
  wxUI_HelloWidgets PROPERTIES
  WIN32_EXECUTABLE TRUE
)
endif()

wxUI_SetupCompilerForTarget(wxUI_HelloWidgets)

target_link_libraries(wxUI_HelloWidgets 
  wx::core
  wxUI::wxUI
)


