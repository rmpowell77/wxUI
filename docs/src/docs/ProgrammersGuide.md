# wxUI
C++ header-only library to make declarative UIs for wxWidgets.

## Table of Contents
- [Overview](#overview)
- [Menu](#menu)
- [Layout](#layout)
  - [Generic](#generic)
  - [Splitter](#splitter)
  - [ForEach](#foreach)
- [Controllers](#controllers)
  - [Bind](#bind)
  - [Proxy](#proxy)
  - [Supported Controllers](#supported-controllers)
  - [Custom](#custom)
- [Miscellaneous Notes](#miscellaneous-notes)

## Overview

This overview provides an overview of `wxUI`, but is not intended to be a tutorial on `wxWidgets`.  We assume the reader has working knowledge of `wxWidgets`.  Great documentation and guides are available at [https://www.wxwidgets.org](https://www.wxwidgets.org).

In `wxUI`, you use `wxUI::Menu` to declare the layout of your menu items and the "actions" they call.  Similarly, you use `wxUI::Sizer` to declare the layout of *Controllers* for your application.

### Menu

`wxUI::Menu` is a way to lay out menus in a declarative, visual way.

The general concept is you declare a set of structures and then `fitTo` a frame.

```cpp
{{{ examples/HelloWorld/HelloWorld.cpp wxUIMenu "    // ..." }}}
```

In `wxWidgets` the general paradigm is to create an enumeration of identity ints that you associate with a member, then you would bind, either statically or dynamically, to a function.  With `wxUI::Menu` the construction of the identify and association with a function is handled automatically.  By default `wxUI::Menu` starts the enumeration with `wxID_AUTO_LOWEST` and increments for each item.  Take caution if you use these enumerations as it may collide with other ids assocated with the frame.

The top level `wxUI::MenuBar` holds a collection of `wxUI::Menu` objects.  The `wxUI::Menu` object consists of a name of the menu, and a collection of "Items", which can be one of `wxUI::Item` (normal), `wxUI::Separator`, `wxUI::CheckItem`, and `wxUI::RadioItem`.

Menu Items are generally a name with a handler closure, such as a lambda, or name and id with a closure.  Menu Items can also be assocated with `wxStandardID`.  Many of these like `wxID_EXIT` and `wxID_HELP` have predefined name, help, and handlers, so declaration with just an ID is allowed.

Handlers are callable items that handle events.  The handler can be declared with both no arguments or the `wxCommandEvent` argument for deeper inspection of the event.

```cpp
{{{ examples/HelloWorld/HelloWorld.cpp wxUIMenuExample1 "    // ..." }}}
```

Menu items (except `Separator`) follow the general pattern:

```
Items { ID }  // for primitives that have a system handler
Items { ID, "Name" }
Items { ID, "Name", "Help" }
Items { ID, Handler }
Items { ID, "Name", Handler }
Items { ID, "Name", "Help", Handler }
Items { "Name", Handler }
Items { "Name", "Help", Handler }
```

`wxUI::Menu` also allows nesting of menus.  This allows complicated menus to be composed easily.

```cpp
{{{ examples/HelloWorld/HelloWorld.cpp wxUIMenuSubMenu "    // ..." }}}
```

The `wxUI::MenuBar` and related objects are generally "lazy" objects.  They hold the details of the menu layout, but do not call any wxWidget primitives on construction.  When `fitTo` a frame is invoked does the underlying logic construct the menu structure.


### Layout

The basics of `wxUI` layout is the *Layout*.  You use a specific type of *Layout*, with the `wxUI::VSizer` (Vertical Sizer or "row") and `wxUI::HSizer` (Horizontal Sizer or "column") being the most common. When a *Layout* is set as the top level, it uses the layout as a sort of "blueprint" for stamping out the UI by constructing the ownership hierarchy and layout.

```cpp
{{{ examples/HelloWorld/HelloWorld.cpp wxUILayoutBasic "    // ..." }}}
```

In the above example we have constructed a vertical layout sizer that will use a `wxSizer` with the `wxSizerFlags` set to expand with a default border.  Then the first item in the sizer is a second layer sizer with horizontal layout.  The `wxSizerFlags` are propagated to each layer so the horizontal layout in this example would also be set to expand with a default border.  The second sizer would be created as a "named" box horizonal sizer.

A *Layout* takes a collection of "Items", which can be either additional *Layout* (to create a tree of *Layouts*), *Controllers*, anything that is convertable `wxSizer*`.  Here is the general form of constructions for *Sizers*:

```
Sizer { Items... }
Sizer { SizerFlags, Items... }
Sizer { "Name", Items... }
Sizer { "Name", SizerFlags, Items... }
```

`wxUI` supports 3 flavors of Sizers: `wxUI::VSizer` (Vertical Sizers), `wxUI::HSizer` (Horizontal Sizers), and `wxUI::FlexGridSizer` (Flexible Grid Sizers).  Both `wxUI::VSizer` and `wxUI::HSizer` can be created with a string to create a "named" box.

Note: Because Sizers are intended to be "recursive" data structures, it is possible for a `wxUI::VSizer` to contain a `wxUI::VSizer`.  However, be aware that if an empty `wxUI::VSizer` is created with *just* a `wxUI::VSizer` as the argument, we collapse that to be a single `wxUI::VSizer`.  ie, this:

```
wxUI::VSizer { wxUI::VSizer { "Current Frame" } }.fitTo(this);
```

is equivalent to:

```
wxUI::VSizer { "Current Frame" }.fitTo(this);
```


#### Generic

One special type of *Layout* is `Generic`.  There are cases where you have to construct a `wxWindow*` with a parent.  This is a case to use `Generic`:

```cpp
{{{ examples/HelloWorld/ExtendedExample.cpp GenericExample "    // ..." }}}
```

Essentially, you supply a object that converts to `wxSizer*` or `wxWindow*`, or a closure or function that returns a `wxWindow*` when supplied with a `wxWindow*`, and it will be inserted into the *Layout*.

#### Splitter

`HSplitter` and `VSplitter` are special types of *Layout* objects that take in two *Controllers*.

```cpp
{{{ examples/HelloWorld/ExtendedExample.cpp SplitterExample "    // ..." }}}
```

#### ForEach

Often times you will need to layout several widgets which only are different in their wxWindowID and Name.  Or perhaps there are cases where the items to be laid out are dynamic.  `ForEach` allows you to specify a range of values or `std::tuples` that are arguments to a closure that will returns a *Controller*.  These will then be added one at a time.

```
{{{ examples/HelloWorld/ExtendedExample.cpp ForEachExample "    // ..." }}}
```

*Ranges* are valid arguments for `ForEach`, which allows you to build up complicated layouts at run time.

```
{{{ examples/HelloWorld/ExtendedExample.cpp ComplicatedForEachExample "    // ..." }}}
```

Often times you would be laying out a set of buttons in a horizontal sizer.  The `HForEach` and `VForEach` functions are provided as convenience functions:

```
{{{ examples/HelloWorld/ExtendedExample.cpp HForEach "    // ..." }}}
```


### Controllers

*Controllers* are the general term to refer to items that behave like a [`wxContol`](https://docs.wxwidgets.org/3.0/classwx_control.html).  In `wxUI` we attempt to conform a consistent style that favors the common things you do with a specific `wxControl`.

```cpp
{{{ examples/HelloWorld/HelloWorld.cpp wxUIController "    // ..." }}}
```

By default *Controllers* are constructed using the default arguments for position, style, size, etc.  *Controllers* are designed to use [Method Chaining](https://en.wikipedia.org/wiki/Method_chaining) to specialize the way the controller is constructed.  In the example above we see that `wxUI::TextCtrl` is being augmented with the style `wxALIGN_LEFT`.

The list of Methods supported by all controllers:

 * `withFlags(wxSizerFlags flags)` : Specifies the sizer flags to use for layout of this controller.
 * `withPosition(wxPoint pos)` : Specifies the `pos` of the `wxControl`.
 * `withSize(wxSize size)` : Specifies the `size` of the `wxControl`.
 * `withWidth(int size)` : Specifies the width of `size` of the `wxControl`.
 * `withHeight(int size)` : Specifies the height of `size` of the `wxControl`.
 * `withStyle(long style)` : Adds the style flags for the `style` of the `wxControl`.
 * `withoutStyle(long style)` : Removes the style flags  for the `style` of the `wxControl`.
 * `setStyle(long style)` : Sets the style flag for the `style` of the `wxControl`.
 * `setFont(wxFontInfo)` : Sets the font of the `wxControl`.
 * `setEnabled(bool)` : Enables or disables the `wxControl`.

#### Bind

*Controllers* support "binding" a function call to their event handlers.  When the event for that *controller* is emitted, the function-like object supplied will be called.  You can bind multiple events on a single *controller*.  *Controllers* `bind` the most common events, and supply additional `bind` extensions (like `ListBox::bindDClick`) for more specialized events.

```cpp
{{{ examples/HelloWorld/HelloWorld.cpp wxUIBind "    // ..." }}}
```

For convenience the event parameter of the function can be omitted in cases where it is unused.

#### Proxy

Often the value of a *Controller* in a layout needs to be referenced, or sometimes the backing `wxWindow` itself needs to be used directly.  This could be for reading a currently typed in value in a `TextCtrl`, or to change the selection of a `Choice`.  *Controllers* support `Proxy` objects, a way to get the handle to the underlying `wxWindow` that is created for the *Controller*.

Some *Controllers* do not support values that are intended to change, such as a `Line`, and others can have several values of interest, such as a `ComboBox`.  `Proxy` objects can have several accessors that allow access to these, most commonly called `value()` and `selection()` (see Supported Controllers for details of each supported *Controller*).  These accessors are proxy objects support `get()` and `set()` functions, as well as a set of appropriate overloads for the underlying type, allowing more ergonomic interaction with the code.  `Proxy` also supplies `operator*` which reference the most common accessor.

`Proxy` supply `control()`, which is intended to allow access to the underlying controller.  `Proxy` overloads `operator->` to allow a "natural" syntax for calling functions on the underlying *Controller*.

As `Proxy` objects need to be a named variable that exist outside of a *Controller*, and require being "attached".  This is done with `withProxy`.  Accessing a proxy object that has not been attached to a controller will cause an exception to be raised.

```cpp
class ExtendedExample : public wxDialog {
public:
    explicit ExtendedExample(wxWindow* parent);
    void Reset();
private:
    wxUI::TextCtrl::Proxy mText;
};

ExtendedExample::ExtendedExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExtendedExample")
{
    using namespace wxUI;
    VSizer {
        TextCtrl { "Hello" }.withProxy(mText)
    }
        .fitTo(this);
}

ExtendedExample::Reset() {
    mText->DiscardEdits();
}
```

Care must be taken to not access `Proxy` objects outside the lifetime of the *Controller* and parent window lifetime.  It is undefined what occurs if a `Proxy` object is used after a the underlying *Controller* is destroyed.

#### Supported Controllers

The "Controllers" currently supported by `wxUI`:

| wxUI                 | wxWidget               | Default Event      | Proxy accessors value |
| :------------------- | :--------------------- | :----------------- | :-------------------- |
| `Bitmap`             | `wxStaticBitmap`       | n/a                | n/a                   |
| `BitmapButton`       | `wxBitmapButton`       | `EVT_BUTTON`       | n/a                   |
| `BitmapComboBox`     | `wxBitmapComboBox`     | `EVT_COMBOBOX`     | `selection` -> `int`<BR>`value` -> `std::string`<BR>*default*: `value` |
| `BitmapToggleButton` | `wxBitmapToggleButton` | `EVT_TOGGLEBUTTON` | `value` -> `bool`<BR>*default*: `value` |
| `Button`             | `wxButton`             | `EVT_BUTTON`       | n/a                   |
| `CheckBox`           | `wxCheckBox`           | `EVT_CHECKBOX`     | `value` -> `bool`<BR>*default*: `value` |
| `Choice`             | `wxChoice`             | `EVT_CHOICE`       | `selection` -> `int`<BR>*default*: `selection` |
| `ComboBox`           | `wxComboBox`           | `EVT_COMBOBOX`     | `selection` -> `int`<BR>`value` -> `std::string`<BR>*default*: `value` |
| `Gauge`              | `wxGauge`              | n/a                | `range` -> `int`<BR>`value` -> `int`<BR>*default*: `value` |
| `Hypertext`          | `wxHypertextCtrl`      | n/a                | n/a                   |
| `Line`               | `wxStaticLine`         | n/a                | n/a                   |
| `ListBox`            | `wxListBox`            | `EVT_LISTBOX`      | `selection` -> `int`<BR>`selections` -> `std::vector<int>`<BR>*default*: `selection` |
| `RadioBox`           | `wxRadioBox`           | `EVT_RADIOBOX`     | `selection` -> `int`<BR>*default*: `selection` |
| `Slider`             | `wxSlider`             | `EVT_SLIDER`       | `value` -> `int`<BR>*default*: `value` |
| `SpinCtrl`           | `wxSpinCtrl`           | `EVT_SPINCTRL`     | `value` -> `int`<BR>*default*: `value` |
| `Text`               | `wxStaticText`         | n/a                | `label` -> `std::string`<BR>*default*: `label` |
| `TextCtrl`           | `wxTextCtrl`           | `EVT_TEXT`         | `label` -> `std::string`<BR>*default*: `label` |

Additional "Controllers" should be easy to add in future updates.

#### Custom

From time to time you may need to do some complicated custom wxWidget "controller" construction.  Use `Custom` "controller" to hook into the construction of the widget tree.  A `Custom` object is created supplying a function that conforms to the `CreateAndAdd` function.

```cpp
{{{ include/wxUI/Custom.hpp requires "    // ..." }}}
```

An example of how to use could be as follows:

```cpp
{{{ examples/HelloWorld/ExtendedExample.cpp CustomExample "    // ..." }}}
```

#### Misc notes.

`wxRadioBox` requires a list of strings to operate correctly, so `RadioBox` requires a `std::vector` of strings.  Note, you *can* provide an empty `std::vector`, but a crash may occur if you do so.  In addition, because `RadioBox` can take in a string as a "caption", a key-value is necessary to prevent `char`-arrays from being interpreted as `initializer_list<std::string>`.

`Button` and `BitmapButton` support the `setDefault` function which allows you to set them as the default button.
