cmake_minimum_required(VERSION 3.23)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG        2b60af89e23d28eefc081bc930831ee9d45ea58b # v3.8.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(wxUI_Tests
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_BitmapTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_BitmapButtonTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_BitmapComboBoxTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_BitmapToggleButtonTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_ButtonTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_CheckBoxTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_ChoiceTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_ComboBoxTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_ForEachTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_GaugeTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_GenericTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_GetterSetterTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_HSplitterTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_HyperlinkTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_LayoutTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_LineTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_ListBoxTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_MenuTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_RadioBoxTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_SpinCtrlTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_SliderTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_Tests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_TextCtrlTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_TextTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_VSplitterTests.cpp
)

add_subdirectory(BuildTests)

wxUI_SetupCompilerForTarget(wxUI_Tests)

target_link_libraries(
  wxUI_Tests
  PRIVATE
  Catch2::Catch2WithMain
  wx::core
  wxUI
)

