cmake_minimum_required(VERSION 3.23)

# Initial project declaration (version will be updated from git tags)
project(wxUI)

# Determine version from git tags and set PROJECT_VERSION
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cmake)
set(PROJECT_VERSION ${wxUI_VERSION})
set(PROJECT_VERSION_MAJOR ${wxUI_VERSION_MAJOR})
set(PROJECT_VERSION_MINOR ${wxUI_VERSION_MINOR})
set(PROJECT_VERSION_PATCH ${wxUI_VERSION_PATCH})

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
# Generate version header from template
#============================================================================
configure_file(
  ${PROJECT_SOURCE_DIR}/include/wxUI/version.hpp.in
  ${PROJECT_BINARY_DIR}/generated-includes/wxUI/version.hpp
  @ONLY
)

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
  ${PROJECT_BINARY_DIR}/generated-includes/wxUI/version.hpp
  include/wxUI/detail/BindInfo.hpp
  include/wxUI/detail/fwd.hpp
  include/wxUI/detail/LayoutDetails.hpp
  include/wxUI/detail/HelperMacros.hpp
  include/wxUI/detail/ZapMacros.hpp
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
  include/wxUI/Factory.hpp
  include/wxUI/Gauge.hpp
  include/wxUI/GetterSetter.hpp
  include/wxUI/Hyperlink.hpp
  include/wxUI/Layout.hpp
  include/wxUI/LayoutIf.hpp
  include/wxUI/Line.hpp
  include/wxUI/Menu.hpp
  include/wxUI/RadioBox.hpp
  include/wxUI/SpinCtrl.hpp
  include/wxUI/Splitter.hpp
  include/wxUI/Slider.hpp
  include/wxUI/Text.hpp
  include/wxUI/TextCtrl.hpp
  include/wxUI/Widget.hpp
  include/wxUI/Wrapper.hpp
)

add_library(wxUI::wxUI ALIAS wxUI)

target_include_directories(
  wxUI
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated-includes>
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
  PATTERN "*.in" EXCLUDE
)

# Install generated version.hpp
install(
  FILES ${PROJECT_BINARY_DIR}/generated-includes/wxUI/version.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wxUI
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

