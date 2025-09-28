### Quick orientation (what this repo is)
- wxUI is a C++ header-only library that provides a declarative layer over wxWidgets. The public API is in `include/wxUI/*.hpp` and the INTERFACE target is declared in `CMakeLists.txt`.
- Key docs: `docs/ProgrammersGuide.md` (patterns & examples) and `examples/HelloWorld/HelloWorld.cpp` (runnable example).

### Big-picture architecture
- Header-only INTERFACE library: CMake target `wxUI` (INTERFACE) wraps headers in `include/`.
- Runtime behavior: wxUI builds a tree of "Layouts" (VSizer/HSizer/FlexGrid) composed of Controllers (Button, TextCtrl, Choice, etc.) which then instantiate wxWidgets objects when `.fitTo(...)` is called (see `docs/ProgrammersGuide.md`).
- Important flows: Layout -> Controller -> concrete wxWindow/wxSizer creation at fit time; Proxy objects attach to named Controllers to access underlying controls after creation.

### Project-specific conventions and patterns to follow
- Method-chaining style for controller customization: e.g. `TextCtrl{"x"}.withStyle(...).withProxy(p).bind(...)` (see `examples/HelloWorld/HelloWorld.cpp`).
- Proxy lifetime caution: `Proxy` objects are references to runtime-created controls. Do not access a Proxy outside the lifetime of its parent window — this is undefined behavior (Programmer's Guide sections "Proxy").
- Sizer collapse rule: an empty nested `VSizer{ VSizer{...} }` is collapsed. Be aware when refactoring nested layouts.
- Menu ID enumeration: `wxUI::Menu` auto-assigns ids starting at `wxID_AUTO_LOWEST` — beware of collisions with manually chosen ids. See `docs/ProgrammersGuide.md` Menu section.

### Build, tests and examples (concrete commands and flags)
- CMake minimum: 3.23 (declared in `CMakeLists.txt`). Project requires C++20. In-source builds are explicitly rejected by CMake.
- Build (out-of-source). To enable tests/examples set CMake options:
  - WXUI_WITH_TESTS=ON to build unit tests
  - WXUI_WITH_EXAMPLE=ON to build examples
- The project uses FetchContent for dependencies (see `cmake/dependencies.cmake`): enabling tests/examples will fetch and build wxWidgets and Catch2.
- Debug builds set AddressSanitizer when ENABLE_ASAN=ON (default) via `cmake/compiler.cmake`.
- Test executable: `wxUI_Tests` (built when `WXUI_WITH_TESTS=ON`). Tests are wired to CTest via `add_test(NAME UnitTest COMMAND wxUI_Tests)` in the tests CMake file.

### Typical agent tasks and where to start in code
- Small UI change: edit headers under `include/wxUI/` and update examples in `examples/` for manual smoke tests.
- Add a controller or Proxy accessor: follow patterns in `include/wxUI/TextCtrl.hpp` and `include/wxUI/Widget.hpp` (use method-chaining and implement `.withProxy()` semantics).
- Change build/test flow: edit `CMakeLists.txt`, `cmake/dependencies.cmake`, and `cmake/compiler.cmake` (these centralize options, FetchContent and compiler flags).

### Useful examples to reference in code suggestions
- `examples/HelloWorld/HelloWorld.cpp` — full usage of Menu, VSizer/HSizer, Controllers, Proxy and bind examples.
- `docs/ProgrammersGuide.md` — detailed patterns (Menu, Layout, ForEach, Proxy, Bind) to mirror in code changes or tests.
- `tests/` — unit tests show intended usage and edge cases. Adding tests should follow the Catch2 style used in `tests/CMakeLists.txt`.

### Safety/behavioral notes for generated code
- Preserve header-only semantics: modifications should keep public API stable and include guards / inline semantics intact.
- Avoid introducing runtime Proxy dereferences without lifetime checks. Prefer creating small tests under `tests/` to validate behavior.
- Respect the no in-source-build rule; any generated contributor instructions should run CMake out-of-source.

If anything is unclear or you want this tailored to specific agent roles (refactoring, feature, doc-only), tell me which role and I'll iterate.
