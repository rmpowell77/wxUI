# The dependencies are managed by vcpkg.
# When we need another package, ensure it's listed in vcpkg.json.

cmake_minimum_required(VERSION 3.23)

# Option to use FetchContent for wxWidgets (for CI compatibility testing)
option(WXUI_FETCH_WXWIDGETS "Use FetchContent to fetch wxWidgets instead of find_package" OFF)

if (WXUI_WITH_TESTS OR WXUI_WITH_EXAMPLES)
  if (WXUI_FETCH_WXWIDGETS)
    include(FetchContent)
    
    # Allow override of wxWidgets version/tag for CI testing
    if(NOT DEFINED WXUI_WXWIDGETS_GIT_TAG)
      set(WXUI_WXWIDGETS_GIT_TAG "v3.3.2" CACHE STRING "wxWidgets git tag or commit hash to fetch")
    endif()
    
    message(STATUS "Fetching wxWidgets from GitHub (tag: ${WXUI_WXWIDGETS_GIT_TAG})")
    
    # Configure wxWidgets build options
    set(wxBUILD_SHARED OFF CACHE BOOL "Build wxWidgets as static library")
    set(wxBUILD_TESTS OFF CACHE BOOL "Don't build wxWidgets tests")
    set(wxBUILD_SAMPLES OFF CACHE BOOL "Don't build wxWidgets samples")
    set(wxBUILD_DEMOS OFF CACHE BOOL "Don't build wxWidgets demos")
    set(wxBUILD_BENCHMARKS OFF CACHE BOOL "Don't build wxWidgets benchmarks")
    
    FetchContent_Declare(
      wxWidgets
      GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
      GIT_TAG ${WXUI_WXWIDGETS_GIT_TAG}
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    
    FetchContent_MakeAvailable(wxWidgets)
    
    # Set wxWidgets variables for compatibility with find_package
    set(wxWidgets_FOUND TRUE)
    set(wxWidgets_LIBRARIES wx::net wx::core wx::base wx::gl wx::aui wx::html)
    
    # Also fetch Catch2 when using FetchContent for wxWidgets
    if(WXUI_WITH_TESTS)
      FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0
        GIT_SHALLOW TRUE
      )
      FetchContent_MakeAvailable(Catch2)
    endif()
    
  else()
    # Use vcpkg/find_package for normal development
    find_package(wxWidgets CONFIG REQUIRED COMPONENTS net core base gl aui html)
    if(WXUI_WITH_TESTS)
      find_package(Catch2 CONFIG REQUIRED)
    endif()
  endif()
endif()


