cmake_minimum_required(VERSION 3.23)

target_sources(wxUI_Tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/BindInfo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Bitmap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BitmapButton.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BitmapComboBox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BitmapToggleButton.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Button.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CheckBox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Choice.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ComboBox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Custom.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ForEach.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Gauge.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Generic.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/GetterSetter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/HelperMacros.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Hyperlink.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Layout.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Line.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ListBox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Menu.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/RadioBox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Slider.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/SpinCtrl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Splitter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Text.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TextCtrl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Widget.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ZapMacros.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUITypes.cpp
)

