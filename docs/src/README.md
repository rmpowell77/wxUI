# wxUI
C++ header-only library to make declarative UIs for wxWidgets.

## Quick Start

```cpp
{{{ examples/HelloWorld/HelloWorld.cpp Example "" }}}
```
<img src="docs/images/ExampleDialog.png"/>



## Motivation
This library grew out of work to create a C++ library that allowed a simple declarative style for [`wxWidgets`](https://www.wxwidgets.org) to create the UI for the [CalChart](https://github.com/calband/calchart) project.  

[`wxWidgets`](https://www.wxwidgets.org) is a C++ library that lets developers create applications for Windows, macOS, Linux and other platforms with a single code base.  The programmer creates "UI Elements" such as [`wxControl`](https://docs.wxwidgets.org/3.2/classwx_control.html) elements and inserts them in [`wxSizer`](https://docs.wxwidgets.org/3.2/classwx_sizer.html) objects to describe the layout of the UI in code.

For instance, a usual programming pattern for [`wxWidgets`](https://www.wxwidgets.org) is to create the UI objects first, and then put them in a layout: 

```
{{{ examples/HelloWorld/HelloWorld.cpp withwxWidgets "    // ..." }}}
```

This separates the "what" from the "where", and often makes it challenging to identify what the structure of the UI will be from code inspection.

`wxUI` is a library that allows the user to declare a number of elements in a "struct-like" declaration that creates a "factory" that manufactures at runtime the ownership and sizer heirarchy.


```
{{{ examples/HelloWorld/HelloWorld.cpp withwxUI "    // ..." }}}
```

`wxUI` is not intended to be a replacement for `wxWidgets`, but instead is a library to simplify usage.  It is written such that it should be easy to "step outside" of `wxUI` and access `wxWidgets` directly.

Please consult the [Programmer's Guide](docs/ProgrammersGuide.md) for more information on Library details.
