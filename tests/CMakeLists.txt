cmake_minimum_required(VERSION 3.23)

add_executable(wxUI_Tests
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_BitmapTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_BitmapButtonTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_BitmapComboBox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_BitmapToggleButtonTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_ButtonTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_CheckBoxTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_ChoiceTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_ComboBox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_LayoutTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_LineTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_ListBoxTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_MenuTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_RadioBoxTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_SpinCtrlTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_SliderTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_Tests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_Text.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_TextCtrl.cpp
)

wxUI_SetupCompilerForTarget(wxUI_Tests)

target_link_libraries(
  wxUI_Tests
  PRIVATE
  Catch2::Catch2WithMain
  ${wxWidgets_LIBRARIES}
  wxUI
)

