cmake_minimum_required(VERSION 3.23)
project(wxUI VERSION 0.1)

# prevent In-source builds:
# https://towardsdatascience.com/7-tips-for-clean-cmake-scripts-c8d276587389
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
message(FATAL_ERROR "In-source build detected!")
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)


#============================================================================
# Settable options
#============================================================================
option(WXUI_WITH_TESTS "Build tests." OFF)
option(WXUI_WITH_EXAMPLE "Build example." OFF)

include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)

#============================================================================
# Internal compiler options
#============================================================================
# C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
  wxUI
  INTERFACE
  include/wxUI/wxUI.hpp
  include/wxUI/wxUITypes.hpp
  include/wxUI/BindInfo.hpp
  include/wxUI/Bitmap.hpp
  include/wxUI/BitmapToggleButton.hpp
  include/wxUI/BitmapComboBox.hpp
  include/wxUI/BitmapButton.hpp
  include/wxUI/Button.hpp
  include/wxUI/CheckBox.hpp
  include/wxUI/Choice.hpp
  include/wxUI/ComboBox.hpp
  include/wxUI/Custom.hpp
  include/wxUI/ForEach.hpp
  include/wxUI/Gauge.hpp
  include/wxUI/Generic.hpp
  include/wxUI/GetterSetter.hpp
  include/wxUI/HelperMacros.hpp
  include/wxUI/Hyperlink.hpp
  include/wxUI/Layout.hpp
  include/wxUI/Line.hpp
  include/wxUI/Menu.hpp
  include/wxUI/RadioBox.hpp
  include/wxUI/SpinCtrl.hpp
  include/wxUI/Splitter.hpp
  include/wxUI/Slider.hpp
  include/wxUI/Text.hpp
  include/wxUI/TextCtrl.hpp
  include/wxUI/Widget.hpp
  include/wxUI/ZapMacros.hpp
)

add_library(wxUI::wxUI ALIAS wxUI)

target_include_directories(
  wxUI
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#============================================================================
# Install + package config
#============================================================================
install(
  TARGETS wxUI
  EXPORT wxUITargets
)

install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT wxUITargets
  FILE wxUITargets.cmake
  NAMESPACE wxUI::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wxUI
)

configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/wxUIConfig.cmake.in
  ${PROJECT_BINARY_DIR}/wxUIConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wxUI
)

write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/wxUIConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    ${PROJECT_BINARY_DIR}/wxUIConfig.cmake
    ${PROJECT_BINARY_DIR}/wxUIConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wxUI
)

#============================================================================
# Examples
#============================================================================
if(WXUI_WITH_EXAMPLE)
  add_subdirectory(examples)
endif()

#============================================================================
# Tests
#============================================================================
if(WXUI_WITH_TESTS)
  add_subdirectory(tests)
  include(CTest)
  add_test(NAME UnitTest COMMAND wxUI_Tests)
endif()

