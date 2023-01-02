cmake_minimum_required(VERSION 3.23)
project(wxUI_examples VERSION 0.1)

add_executable(wxUI_HelloWorld
  MACOSX_BUNDLE
  ExtendedExample.cpp
  ExtendedExample.h
  HelloWorld.cpp
)

wxUI_SetupCompilerForTarget(wxUI_HelloWorld)

target_link_libraries(wxUI_HelloWorld 
  ${wxWidgets_LIBRARIES}
  wxUI
)


