set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
set(VCPKG_OSX_ARCHITECTURES arm64)

# Keep sanitizer instrumentation aligned between the app and vcpkg dependencies.
set(VCPKG_C_FLAGS_DEBUG "${VCPKG_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
set(VCPKG_CXX_FLAGS_DEBUG "${VCPKG_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
set(VCPKG_LINKER_FLAGS_DEBUG "${VCPKG_LINKER_FLAGS_DEBUG} -fsanitize=address")
