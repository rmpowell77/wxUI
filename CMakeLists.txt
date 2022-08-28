cmake_minimum_required(VERSION 3.23)
project(wxUI VERSION 0.1)

# prevent In-source builds:
# https://towardsdatascience.com/7-tips-for-clean-cmake-scripts-c8d276587389
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
message(FATAL_ERROR "In-source build detected!")
endif()

include(GNUInstallDirs)

#============================================================================
# Settable options
#============================================================================
option(WITH_TESTS "Build tests." OFF)
option(WITH_EXAMPLE "Build example." OFF)

include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)

#============================================================================
# Internal compiler options
#============================================================================
# C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 20)
	set(CMAKE_CXX_STANDARD 20)
endif()

add_library(
  wxUI
  INTERFACE
  include/wxUI/wxUI.h
  include/wxUI/Button.h
  include/wxUI/CheckBox.h
  include/wxUI/Choice.h
  include/wxUI/Menu.h
  include/wxUI/RadioBox.h
  include/wxUI/Sizer.h
  include/wxUI/Text.h
  include/wxUI/Widget.h
)

target_include_directories(
  wxUI
  INTERFACE
  include
)

#============================================================================
# Examples
#============================================================================
if(WITH_EXAMPLE)
  add_subdirectory(examples)
endif()

#============================================================================
# Tests
#============================================================================
if(WITH_TESTS)
  add_subdirectory(tests)
endif()
