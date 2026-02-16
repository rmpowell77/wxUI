# version.cmake
# Automatically determine project version from git tags
# 
# This script extracts the version from git tags instead of hardcoding it in CMakeLists.txt
# Expected tag format: vMAJOR.MINOR.PATCH (e.g., v1.2.3)
#
# Usage:
#   1. Create a git tag: git tag v1.2.3
#   2. Push the tag: git push origin v1.2.3
#   3. CMake will automatically use 1.2.3 as the project version
#
# The version will be:
#   - Extracted from the most recent tag matching v[0-9]*
#   - Used to set PROJECT_VERSION in CMakeLists.txt
#   - Populated into version.hpp.in template to create version.hpp
#
# Fallback: If git is not available or no tags exist, uses FALLBACK_VERSION

# Default fallback version if git is not available or no tags exist
set(FALLBACK_VERSION "0.1.0")

# Try to get version from git describe
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --match "v[0-9]*" --abbrev=0
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    if(GIT_TAG)
        # Remove 'v' prefix if present
        string(REGEX REPLACE "^v" "" GIT_TAG "${GIT_TAG}")
        
        # Parse version components
        string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" VERSION_MATCH "${GIT_TAG}")
        
        if(VERSION_MATCH)
            set(Found_MAJOR ${CMAKE_MATCH_1})
            set(Found_MINOR ${CMAKE_MATCH_2})
            set(Found_PATCH ${CMAKE_MATCH_3})
            set(Found_VERSION "${Found_MAJOR}.${Found_MINOR}.${Found_PATCH}")
            
            message(STATUS "Version from git tag: ${Found_VERSION}")
        else()
            message(WARNING "Git tag '${GIT_TAG}' doesn't match expected format vMAJOR.MINOR.PATCH, using fallback")
            set(Found_VERSION ${FALLBACK_VERSION})
            set(Found_MAJOR 0)
            set(Found_MINOR 1)
            set(Found_PATCH 0)
        endif()
    else()
        message(STATUS "No git tags found, using fallback version: ${FALLBACK_VERSION}")
        set(Found_VERSION ${FALLBACK_VERSION})
        set(Found_MAJOR 0)
        set(Found_MINOR 1)
        set(Found_PATCH 0)
    endif()
else()
    message(STATUS "Git not found, using fallback version: ${FALLBACK_VERSION}")
    set(Found_VERSION ${FALLBACK_VERSION})
    set(Found_MAJOR 0)
    set(Found_MINOR 1)
    set(Found_PATCH 0)
endif()

# Set the project version variables
set(wxUI_VERSION ${Found_VERSION})
set(wxUI_VERSION_MAJOR ${Found_MAJOR})
set(wxUI_VERSION_MINOR ${Found_MINOR})
set(wxUI_VERSION_PATCH ${Found_PATCH})
