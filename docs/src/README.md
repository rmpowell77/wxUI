# wxUI
C++ header-only library to make declarative UIs for wxWidgets.

```cpp
{{{ examples/HelloWidgets/HelloWidgets.cpp Example "" }}}
```
<img src="docs/images/ExampleDialog.png"/>

## Quick Start

### Installing

**Option 1: FetchContent (recommended for most projects)**

```cmake
include(FetchContent)

FetchContent_Declare(
	wxUI
	GIT_REPOSITORY https://github.com/rmpowell77/wxUI.git
	GIT_TAG        <tag-or-commit>
)
FetchContent_MakeAvailable(wxUI)
```

**Option 2: Build/install locally, then `find_package`**

```bash
cmake -S . -B build
cmake --build build
cmake --install build --prefix <install-prefix>
```

Then in your project:

```cmake
find_package(wxUI REQUIRED)
```

### Including

**CMake**

```cmake
target_link_libraries(your_target
	PRIVATE
	wx::core
	wxUI::wxUI
)
```

**C++**

```cpp
#include <wxUI/wxUI.hpp>
```


## Motivation
This library grew out of work to create a C++ library that allowed a simple declarative style for [`wxWidgets`](https://www.wxwidgets.org) to create the UI for the [CalChart](https://github.com/calband/calchart) project.  

[`wxWidgets`](https://www.wxwidgets.org) is a C++ library that lets developers create applications for Windows, macOS, Linux and other platforms with a single code base.  The programmer creates "UI Elements" such as [`wxControl`](https://docs.wxwidgets.org/3.2/classwx_control.html) elements and inserts them in [`wxSizer`](https://docs.wxwidgets.org/3.2/classwx_sizer.html) objects to describe the layout of the UI in code.

For instance, a usual programming pattern for [`wxWidgets`](https://www.wxwidgets.org) is to create the UI objects first, and then put them in a layout: 

```
{{{ examples/HelloWidgets/HelloWidgets.cpp withwxWidgets "    // ..." }}}
```

This separates the "what" from the "where", and often makes it challenging to identify what the structure of the UI will be from code inspection.

`wxUI` is a library that allows the user to declare a number of elements in a "struct-like" declaration that creates a "factory" that manufactures at runtime the ownership and sizer heirarchy.


```
{{{ examples/HelloWidgets/HelloWidgets.cpp withwxUI "    // ..." }}}
```

`wxUI` is not intended to be a replacement for `wxWidgets`, but instead is a library to simplify usage.  It is written such that it should be easy to "step outside" of `wxUI` and access `wxWidgets` directly.

Please consult the [Programmer's Guide](docs/ProgrammersGuide.md) for more information on Library details.

## Contributing

Contributions are welcome! If you’d like to contribute to `wxUI`, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Submit a pull request with a clear description of your changes.
