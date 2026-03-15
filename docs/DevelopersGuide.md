# wxUI Developer's Guide

Welcome to wxUI! This guide will help you make successful contributions to the project. Our goal is to help you craft code that passes CI on the first try.

## Table of Contents
- [Philosophy](#philosophy)
- [Quick Start for Contributors](#quick-start-for-contributors)
- [Development Environment Setup](#development-environment-setup)
- [Git Hooks](#git-hooks)
- [Code Formatting](#code-formatting)
- [Documentation](#documentation)
- [Testing](#testing)
- [Adding New Controllers](#adding-new-controllers)
- [Release Notes](#release-notes)
- [Using ExtendedExample for Smoke Testing](#using-extendedexample-for-smoke-testing)
- [CI Pipeline](#ci-pipeline)
- [Common Pitfalls](#common-pitfalls)

## Philosophy

wxUI is a **cross-platform** C++ header-only library that provides a declarative layer over wxWidgets. Because this library is intended to be used in environments and platforms we can't predict or test directly, we emphasize:

- **Testing**: Comprehensive unit tests using customization points
- **Clarity**: Code should be readable and intentions should be clear
- **Documentation**: Code is the source of truth; documentation is generated from it
- **Automation**: Guard rails and checklists catch common mistakes before CI

Our approach is to be inviting to contributors while maintaining high standards through automation rather than gatekeeping.

## Quick Start for Contributors

1. **Fork and clone** the repository
2. **Install git hooks** (see [Git Hooks](#git-hooks))
3. **Build the project** (see [Development Environment Setup](#development-environment-setup))
4. **Make your changes**
5. **Run the checklists** before committing:
   - `./checklists/do_copyright` - Ensure copyright headers are present
   - `./checklists/do_md` - Regenerate documentation
6. **Test your changes** (see [Testing](#testing))
7. **Submit a PR**

## Development Environment Setup

### Requirements

- **CMake** 3.23 or later
- **C++20** capable compiler (Clang, GCC, or MSVC)
- **Git**

### Building with Tests and Examples

wxUI uses **CMake's `find_package`** to locate dependencies on your system:
- **wxWidgets** - The underlying GUI framework
- **Catch2** - Unit testing framework

Dependencies are managed via **vcpkg** (declared in `vcpkg.json`), which handles downloading and building the correct versions.

#### Option 1: Using CMake Presets (Recommended)

The project includes `CMakePresets.json` with platform-specific configurations:

```bash
# List available presets
cmake --list-presets

# Configure using a preset (macOS Debug example)
cmake --preset mac-debug

# Build
cmake --build --preset mac-debug

# Run tests
ctest --preset mac-debug --output-on-failure
```

Available presets:
- **mac-debug** / **mac-release** - macOS builds (with Ninja)
- **windows-debug** / **windows-release** - Windows builds (Visual Studio)

**Prerequisites for presets**: Set the `VCPKG_ROOT` environment variable to point to your vcpkg installation.

#### Option 2: Manual Configuration

If you're not using presets or need custom configuration:

```bash
# Configure (out-of-source build required)
cmake -B build -S . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DWXUI_WITH_TESTS=ON \
  -DWXUI_WITH_EXAMPLE=ON

# Build
cmake --build build

# Run tests
ctest --test-dir build --output-on-failure
```

**Note**: In-source builds are explicitly rejected. Always use an out-of-source build directory.

### CMake Build Options

- `WXUI_WITH_TESTS=ON` - Build unit tests (target: `wxUI_Tests`)
- `WXUI_WITH_EXAMPLE=ON` - Build examples (includes HelloWidgets and ExtendedExample)
- `ENABLE_ASAN=ON` - Enable AddressSanitizer in Debug builds (default: ON)

These options are automatically enabled when wxUI is the top-level project (`PROJECT_IS_TOP_LEVEL`).

### Dependency Management with vcpkg

wxUI's dependencies are declared in `vcpkg.json`. The vcpkg toolchain integration ensures these dependencies are automatically built and made available to CMake's `find_package`.

**Setting up vcpkg** (if not already installed):

```bash
# Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg

# Bootstrap vcpkg
./bootstrap-vcpkg.sh  # macOS/Linux
# or
.\bootstrap-vcpkg.bat  # Windows

# Set environment variable (add to your shell profile)
export VCPKG_ROOT=~/vcpkg  # macOS/Linux
# or
set VCPKG_ROOT=C:\vcpkg  # Windows
```

Once vcpkg is set up, the CMake presets and manual builds will automatically use it to provide wxWidgets and Catch2.

### Platform-Specific Notes

**macOS**: Use the default Clang toolchain
**Linux**: May require installing GTK development libraries
**Windows**: Use MSVC; static runtime is forced for consistency

## Git Hooks

We provide git hooks to automate common tasks and prevent mistakes.

### Installing Hooks

Run the installation script from the repository root:

```bash
./scripts/install-hooks.sh
```

This will install:
- **pre-commit**: Detects version.hpp changes and prepares for tagging
- **post-commit**: Creates git tags when version is updated

### What the Hooks Do

The pre-commit hook provides two important automations:

1. **Documentation Generation**: Automatically regenerates documentation when:
   - Documentation source files in `docs/src/` are modified
   - Example code in `examples/` is changed
   
   The hook runs `./checklists/do_md` and auto-stages the regenerated `README.md` and `docs/ProgrammersGuide.md` files, preventing the common mistake of forgetting to update docs.

2. **Version Management**: Detects when `version.hpp` is updated and prepares for automatic git tag creation in the post-commit hook.

## Code Formatting

wxUI uses **clang-format** to ensure consistent code style across the project. The formatting check runs automatically in CI on all pull requests.

### Formatting Configuration

The project uses a `.clang-format` configuration based on **WebKit style** with customizations:
- 4-space indentation (no tabs)
- Pointer alignment to the left (`int* ptr`)
- Function braces on new lines
- No column limit (lines can be as long as needed)
- Sorting of includes

### Running clang-format Locally

Before submitting a PR, format your code using clang-format version 21:

```bash
# Format a single file
clang-format -i path/to/your/file.cpp

# Format all C++ files in a directory
find include/ -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
find examples/ -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
find tests/ -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
```

### CI Formatting Check

The `formatting-check` job in CI:
- Runs on Ubuntu with clang-format version 21
- Uses the `jidicula/clang-format-action@v4.17.0` GitHub Action
- Currently runs but **does not block CI** from passing (not in the summary dependencies)
- Checks files in the repository for formatting compliance

**Note**: While the formatting check is advisory and won't fail your PR, please follow the formatting standards to maintain code consistency

## Documentation

wxUI documentation is **generated from source code**. This ensures that examples in the documentation are actual working code.

### How Documentation Works

1. **Source files** live in `docs/src/`
   - `docs/src/README.md` → generates `README.md`
   - `docs/src/docs/ProgrammersGuide.md` → generates `docs/ProgrammersGuide.md`

2. **Source files** contain special markers that pull in code from examples:
   ```markdown
   <!-- include:examples/HelloWidgets/HelloWidgets.cpp:marker_name -->
   ```

3. **The generator** (`docs/src/make_md.py`) processes these markers and creates final documentation

### Regenerating Documentation

**Always run this before committing if you changed example code or documentation source:**

```bash
./checklists/do_md
```

This script regenerates both README.md and docs/ProgrammersGuide.md.

### Verifying Documentation

The CI runs `./checklists/check_md` which fails if documentation is out of sync:

```bash
./checklists/check_md
```

If this fails locally, run `./checklists/do_md` to fix it.

## Testing

wxUI has a comprehensive testing strategy designed to work **without requiring a GUI**.

### Testing Philosophy

Since wxUI creates wxWidgets UI elements, but we want to test on CI servers without displays, we use **customization points** to intercept widget creation and inspect what would be created.

### Customization Points

The customization point system is defined in `include/wxUI/Customizations.hpp` and implemented for testing in `tests/TestCustomizations.hpp`.

#### How It Works

1. **Normal mode**: wxUI calls `createControl<T>()` to create actual `wxWindow*` objects
2. **Test mode**: We override customization points to return string descriptions instead of real widgets

Example from `tests/TestCustomizations.hpp`:

```cpp
// Instead of creating a real wxButton, return a string description
template <typename... Args>
auto createControl(wxUI::ControlType<wxButton>, Args&&... args)
{
    return std::format("(wxButton:label=\"{}\",id={},pos={},size={},style={})", 
                       /* extract arguments */);
}
```

These string descriptions are then tested to verify correct construction.

### Writing Tests

Tests use Catch2 and follow these patterns:

#### Basic Controller Test Pattern

```cpp
TEST_CASE("Button basic test", "[Button]")
{
    auto result = Button{"Click Me"}
        .withProxy(proxy)
        .fitTo(nullptr);
    
    // Verify the string description
    CHECK(result.description() == expected_description);
}
```

See `tests/wxUI_TestControlCommon.hpp` for helper functions like:
- `addWithFlags()` - Test flag setting
- `addWithStyle()` - Test style combinations
- `addWithSize()` - Test size specifications
- `checkWithStyle()` - Verify styles in output

#### Testing a New Controller

When adding a new controller:

1. Create `tests/wxUI_YourControlTests.cpp`
2. Include `TestCustomizations.hpp` for test infrastructure
3. Write test cases covering:
   - Basic construction
   - Style flags
   - Proxy access
   - Bind callbacks
   - Custom parameters specific to your control

Example test file structure:

```cpp
#include "TestCustomizations.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("YourControl basic construction", "[YourControl]")
{
    YourControl::Proxy proxy;
    auto result = YourControl{"initial value"}
        .withProxy(proxy)
        .fitTo(nullptr);
    
    CHECK(result.description().find("initial value") != std::string::npos);
}

TEST_CASE("YourControl with style", "[YourControl]")
{
    auto result = YourControl{"test"}
        .withStyle(wxYOUR_STYLE_FLAG)
        .fitTo(nullptr);
    
    checkWithStyle(wxYOUR_STYLE_FLAG, result.description());
}
```

### Running Tests

```bash
# Build with tests enabled using CMake presets
cmake --preset mac-debug  # or windows-debug
cmake --build --preset mac-debug

# Run all tests with preset
ctest --preset mac-debug --output-on-failure

# Or build manually
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -DWXUI_WITH_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure

# Run the test binary directly
./build/tests/wxUI_Tests

# Run specific tests by tag
./build/tests/wxUI_Tests "[Button]"
```

## Adding New Controllers

When adding a new wxWidgets control wrapper to wxUI, follow this pattern:

### 1. Create the Header File

Create `include/wxUI/YourControl.hpp`:

```cpp
#pragma once

#include <wxUI/Widget.hpp>
#include <wx/yourcontrol.h>

namespace wxUI {

class YourControl : public details::Widget<YourControl> {
public:
    using Proxy = details::Proxy<wxYourControl>;
    
    // Constructor taking initial state
    explicit YourControl(std::string label = "")
        : mLabel(std::move(label))
    {}
    
    // Method chaining style setters
    auto withCustomOption(int value) -> YourControl& {
        mCustomOption = value;
        return *this;
    }
    
    // Required: implement fit() to create the actual widget
    auto fit(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags) const
    {
        auto control = createControl(
            ControlType<wxYourControl>{},
            parent,
            wxID_ANY,
            mLabel,
            wxDefaultPosition,
            getSize(),
            getStyle()
        );
        
        fitCommon(control, parent, sizer, flags);
        return control;
    }

private:
    std::string mLabel;
    int mCustomOption = 0;
};

} // namespace wxUI
```

### 2. Add Test Customization

Add to `tests/TestCustomizations.hpp`:

```cpp
// Add createControl overload for testing
template <typename... Args>
auto createControl(wxUI::ControlType<wxYourControl>, Args&&... args)
{
    return formatControl("wxYourControl", std::forward<Args>(args)...);
}
```

### 3. Create Tests

Create `tests/wxUI_YourControlTests.cpp` as shown in the [Testing](#testing) section.

### 4. Add to Examples

Add usage to `examples/HelloWidgets/ExtendedExample.cpp` to demonstrate and smoke test.

### 5. Update Documentation

If this is a commonly-used control, add an example to `docs/src/docs/ProgrammersGuide.md`.

### 6. Integration Checklist

- [ ] Header file in `include/wxUI/`
- [ ] Test customization in `tests/TestCustomizations.hpp`
- [ ] Unit tests in `tests/wxUI_YourControlTests.cpp`
- [ ] Example usage in `examples/HelloWidgets/ExtendedExample.cpp`
- [ ] Copyright header on all new files (`./checklists/do_copyright`)
- [ ] Documentation regenerated (`./checklists/do_md`)
- [ ] **Release notes updated** in `LATEST_RELEASE_NOTES.md`
- [ ] All tests pass locally
- [ ] Built on your platform successfully

## Release Notes

wxUI maintains a running changelog in `LATEST_RELEASE_NOTES.md` that gets published with each release.

### When to Update Release Notes

**Best Practice**: When your PR addresses a bug or adds a feature, add an entry to `LATEST_RELEASE_NOTES.md`.

This is not strictly enforced, but it helps maintainers during the release process and provides users with clear documentation of what changed.

### Format

The release notes are organized into two sections:

1. **Bugs addressed in this release**: Fixed issues, regressions, crashes
2. **Other changes**: New features, enhancements, refactorings, documentation

Each entry links to its GitHub issue:

```markdown
# wxUI release notes for v0.4.0

Bugs addressed in this release:

* [#274](../../issues/274) ASAN failure when using ComboBox { { hello, goodbye } }
* [#297](../../issues/297) Make sure that mutable lambdas are usable

Other changes:

* [#23](../../issues/23) Add a cmake alias
* [#81](../../issues/81) need wxNotebook
* [#273](../../issues/273) Create a contributor's guide
```

### Adding Your Entry

1. **Create a GitHub issue** if one doesn't exist for your work
2. **Add a bullet point** in the appropriate section of `LATEST_RELEASE_NOTES.md`:
   - Bugs go under "Bugs addressed in this release:"
   - Features/enhancements go under "Other changes:"
3. **Use the format**: `* [#NNN](../../issues/NNN) Brief description`
4. **Keep it concise**: One line that describes what users/contributors need to know

### Why This Matters

- **Automated releases**: The CI pipeline uses `LATEST_RELEASE_NOTES.md` to generate GitHub release notes
- **User visibility**: Users can see what changed between versions
- **Maintainer efficiency**: Reduces work during release preparation
- **Project history**: Creates a clear record of project evolution

See [RELEASE_INSTRUCTIONS.md](RELEASE_INSTRUCTIONS.md) for details on how release notes are used during the release process.

## Using ExtendedExample for Smoke Testing

The `examples/HelloWidgets/ExtendedExample.cpp` serves as a **runnable showcase and smoke test** for wxUI functionality.

### Purpose

- **Demonstrate** new features in a real UI context
- **Manually test** complex interactions that are hard to unit test
- **Provide examples** for users learning the library

### Adding to ExtendedExample

When you add a new controller or feature:

1. **Add a section** to one of the existing dialogs (or create a new one)
2. **Demonstrate typical usage** patterns
3. **Test interactivity** where applicable

Example pattern:

```cpp
// In ExtendedExample.cpp
VSizer {
    "Your New Feature",
    YourControl { "Demo value" }
        .withProxy(proxy)
        .withCustomOption(42)
        .bind([this]() {
            wxLogMessage("This demonstrates the feature works!");
        }),
    Text { "Explanation of what this demonstrates" }
}
```

### Running ExtendedExample

```bash
# Build with examples enabled using CMake presets
cmake --preset mac-debug  # or windows-debug
cmake --build --preset mac-debug

# Or build manually
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -DWXUI_WITH_EXAMPLE=ON
cmake --build build

# Run the example (platform-specific path)
# macOS:
./build/mac-debug/examples/HelloWidgets/wxUI_HelloWidgets.app/Contents/MacOS/wxUI_HelloWidgets
# or with default build dir:
./build/examples/HelloWidgets/wxUI_HelloWidgets.app/Contents/MacOS/wxUI_HelloWidgets

# Linux:
./build/examples/HelloWidgets/wxUI_HelloWidgets

# Windows:
.\build\windows-debug\examples\HelloWidgets\Debug\wxUI_HelloWidgets.exe
# or with default build dir:
.\build\examples\HelloWidgets\Debug\wxUI_HelloWidgets.exe
```

Interact with all dialogs and verify:
- Controls appear correctly
- Interactions work as expected
- No crashes or unexpected behavior
- Visual layout is reasonable

## CI Pipeline

Understanding the CI pipeline helps you anticipate what checks your PR will face.

### CI Checks (from `.github/workflows/ci_automation.yml`)

1. **Copyright Check** (`copyright-check`)
   - Verifies all source files have proper MIT license headers
   - Runs: `./checklists/check_copyright`

2. **Documentation Check** (`check_md`)
   - Verifies documentation is up-to-date with source
   - Runs: `./checklists/check_md`

3. **BuildTests Check** (`buildtests-check`)
   - Verifies build test files are up-to-date
   - Runs: `./checklists/check_buildtests`

4. **Build Matrix** (`build`)
   - **Platforms**: macOS, Ubuntu Linux, Windows
   - **Configurations**: Debug, Release
   - Builds with tests and examples enabled
   - Runs all unit tests via CTest
   - Uses vcpkg for dependency management

5. **Summary**
   - Aggregates results from all checks

6. **Release** (on version tags)
   - Creates GitHub release with notes from `LATEST_RELEASE_NOTES.md`

### Passing CI on First Try

Run these commands before pushing:

```bash
# 1. Ensure copyright headers
./checklists/do_copyright

# 2. Regenerate documentation
./checklists/do_md

# 3. Build and test locally
# Option A: Using CMake presets (if vcpkg is set up)
cmake --preset mac-debug  # or windows-debug
cmake --build --preset mac-debug
ctest --preset mac-debug --output-on-failure

# Option B: Manual configuration
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -DWXUI_WITH_TESTS=ON \
  -DWXUI_WITH_EXAMPLE=ON
cmake --build build
ctest --test-dir build --output-on-failure

# 4. Check that everything is as expected
./checklists/check_copyright
./checklists/check_md
./checklists/check_buildtests

# 5. Manual smoke test
# Run the ExtendedExample app and interact with your changes
```

## Common Pitfalls

### 1. Forgetting to Regenerate Documentation

**Problem**: You update example code but forget to run `./checklists/do_md`

**Symptom**: CI fails with "README.md is out of sync"

**Fix**: Run `./checklists/do_md` before committing

**Prevention**: Install git hooks (future enhancement) or add to your commit checklist

### 2. Missing Copyright Headers

**Problem**: New files don't have the MIT license header

**Symptom**: CI fails with "file does not have copyright"

**Fix**: Run `./checklists/do_copyright` which automatically adds headers

**Prevention**: Run this as part of your pre-commit routine

### 3. In-Source Builds

**Problem**: Running `cmake .` in the project root

**Symptom**: CMake errors about in-source builds being disallowed

**Fix**: Always use `cmake -B build` or a separate build directory

**Convention**: Use `build/` (which is in `.gitignore`)

### 4. Test Failures Due to Customization Point Issues

**Problem**: Tests can't compile or fail unexpectedly

**Symptom**: Linker errors or incorrect test results

**Fix**: Ensure your controller's `createControl` overload is in `TestCustomizations.hpp`

**Debugging**: Check that the formatter output matches what your test expects

### 5. Proxy Lifetime Issues

**Problem**: Accessing a Proxy after its parent window is destroyed

**Symptom**: Crashes or undefined behavior in examples

**Fix**: Only store Proxies as members of the dialog/frame that created them

**Reference**: See [docs/ProgrammersGuide.md](docs/ProgrammersGuide.md) Proxy section

### 6. Breaking Header-Only Semantics

**Problem**: Adding implementation details that require linking

**Symptom**: Linker errors when users include wxUI

**Fix**: Keep all implementations in headers, use `inline` for functions, and template-based designs

**Verification**: Build examples with just `#include <wxUI/wxUI.hpp>` and no additional link requirements

### 7. Platform-Specific Code

**Problem**: Using macOS/Windows/Linux-specific APIs without guards

**Symptom**: CI fails on other platforms

**Fix**: Use wxWidgets cross-platform APIs, or add `#ifdef` guards and test on all platforms

**Best Practice**: Test at least on two platforms before submitting

## Getting Help

- **Issues**: Check [GitHub Issues](https://github.com/rmpowell77/wxUI/issues) for known problems
- **Discussions**: Start a discussion for design questions
- **Code Reference**: See `docs/ProgrammersGuide.md` for usage patterns
- **Examples**: Study `examples/HelloWidgets/` for practical patterns

## Contributing Workflow Summary

1. Fork and clone
2. Create a feature branch
3. Install git hooks: `./scripts/install-hooks.sh`
4. Make your changes
   - Add/modify code in `include/wxUI/`
   - Add tests in `tests/`
   - Add example usage in `examples/`
   - **Update `LATEST_RELEASE_NOTES.md`** with your bug fix or feature
5. Run checklists:
   ```bash
   ./checklists/do_copyright
   ./checklists/do_md
   ```
6. Test locally:
   ```bash
   # Using CMake presets (recommended)
   cmake --preset mac-debug  # or windows-debug
   cmake --build --preset mac-debug
   ctest --preset mac-debug --output-on-failure
   
   # Or manually with vcpkg
   cmake -B build \
     -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
     -DWXUI_WITH_TESTS=ON -DWXUI_WITH_EXAMPLE=ON
   cmake --build build
   ctest --test-dir build --output-on-failure
   ```
7. Smoke test with ExtendedExample
8. Commit and push
9. Create pull request

Welcome to the wxUI project, and thank you for contributing! 🎉
