# The dependencies are managed by FetchContent, which means that
# cmake does all the heavy lifting to find and download the dependencies.
# When we need another package, add it to here.
# We use the convention of specifying the git tag completely, as that can
# avoid issues when a tag changes.

cmake_minimum_required(VERSION 3.23)


include(FetchContent)

if (WITH_TESTS OR WITH_EXAMPLES)
  # Have wxWidgets build as static libraries
  set(wxUSE_STL ON)
  set(wxUSE_STD_CONTAINERS ON)
  FetchContent_Declare(
    wxWidgets
    GIT_REPOSITORY "https://github.com/wxWidgets/wxWidgets"
    GIT_TAG 5b5ec3804a31a3765d60ff351bbdf79eddb1e1f9 # v3.2
  )
  FetchContent_MakeAvailable(wxWidgets)
endif()

if (WITH_TESTS)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG        605a34765aa5d5ecbf476b4598a862ada971b0cc # v3.0.1
  )
  FetchContent_MakeAvailable(Catch2)
endif()


