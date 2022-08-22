cmake_minimum_required(VERSION 3.23)

add_executable(wxUI_Tests
  ${CMAKE_CURRENT_SOURCE_DIR}/wxUI_Tests.cpp
)

target_link_libraries(
  wxUI_Tests
  PRIVATE
  Catch2::Catch2WithMain
  wx::core
)

