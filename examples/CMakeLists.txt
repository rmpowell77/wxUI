cmake_minimum_required(VERSION 3.23)
project(wxUI_examples VERSION 0.1)

include(GNUInstallDirs)
include(FetchContent)

#============================================================================
# Internal compiler options
#============================================================================
# C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 20)
	set(CMAKE_CXX_STANDARD 20)
endif()

add_subdirectory(HelloWorld)

