# The dependencies are managed by FetchContent, which means that
# cmake does all the heavy lifting to find and download the dependencies.
# When we need another package, add it to here.
# We use the convention of specifying the git tag completely, as that can
# avoid issues when a tag changes.

cmake_minimum_required(VERSION 3.23)


include(FetchContent)

if (WXUI_WITH_TESTS OR WXUI_WITH_EXAMPLES)
  find_package(wxWidgets CONFIG)
  if(NOT ${wxWidgets_FOUND})
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
      GIT_TAG 8aef5f40b93958719771331ca03866b7b6fff6bf # v3.2.8
    )
    FetchContent_MakeAvailable(wxWidgets)
  endif()
endif()


