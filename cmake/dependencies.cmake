# The dependencies are managed by vcpkg.
# When we need another package, ensure it's listed in vcpkg.json.

cmake_minimum_required(VERSION 3.23)

include(FetchContent)

if (WXUI_WITH_TESTS OR WXUI_WITH_EXAMPLES)
  # Have wxWidgets build as static libraries
  # FetchContent is used to download the wxWidgets library
  # Have wxWidgets build as static libraries
  set(wxBUILD_SHARED OFF)
  set(wxUSE_STL ON)
  set(wxUSE_STC OFF)
  set(wxUSE_STD_CONTAINERS ON)
  FetchContent_Declare(
    wxWidgets
    GIT_REPOSITORY "https://github.com/wxWidgets/wxWidgets.git"
    GIT_TAG 49c6810948f40c457e3d0848b9111627b5b61de5 # v3.2.8
  )
  FetchContent_MakeAvailable(wxWidgets)
endif()
# if (WXUI_WITH_TESTS OR WXUI_WITH_EXAMPLES)
#   find_package(wxWidgets CONFIG REQUIRED COMPONENTS net core base gl aui html)
# endif()


