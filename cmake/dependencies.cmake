# The dependencies are managed by vcpkg.
# When we need another package, ensure it's listed in vcpkg.json.

cmake_minimum_required(VERSION 3.23)

if (WXUI_WITH_TESTS OR WXUI_WITH_EXAMPLES)
  find_package(wxWidgets CONFIG REQUIRED COMPONENTS net core base gl aui html)
endif()


