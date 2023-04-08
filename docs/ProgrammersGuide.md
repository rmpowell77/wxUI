# wxUI
C++ header-only library to make declarative UIs for wxWidgets.

## Overview

This overview provides an overview of `wxUI`, but is not intented to be a tutorial on `wxWidgets`.  We assume the reader has working knowledge of `wxWidgets`.  Great documentation and guides are available at [https://www.wxwidgets.org](https://www.wxwidgets.org).

In `wxUI`, you use `wxUI::Menu` to declare the layout of your menu items and the "actions" they call.  Similarly, you use `wxUI::Sizer` to declare the layout of *Controllers* for your application.

### Menu

`wxUI::Menu` is a way to lay out menus in a declarative, visual way.

The general concept is you declare a set of structures and then `attachTo` a frame.

```cpp
HelloWorldFrame::HelloWorldFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxUI::MenuBar {
        wxUI::Menu {
            "&File",
    // ...
            wxUI::Item { "&Example with wxUI...\tCtrl-F", [this] {
                            ExampleDialog dialog(this);
                            dialog.ShowModal();
                        } },
            wxUI::Separator {}, wxUI::Item { wxID_EXIT, [this] {
                                                Close(true);
                                            } },
    // ...
        }
    }.attachTo(this);
```

In `wxWidgets` the general paradigm is to create an enumeration of identity ints that you associate with a member, then you would bind, either statically or dynamically, to a function.  With `wxUI::Menu` the construction of the identify and assocation with a function is handled automatically.  By default `wxUI::Menu` starts the enumeration with `wxID_AUTO_LOWEST` and increments for each item.  Take caution if you use these enumerations as it may collide with other ids assocated with the frame.

The top level `wxUI::MenuBar` holds a collection of `wxUI::Menu` objects.  The `wxUI::Menu` object consists of a name of the menu, and a collection of "Items", which can be one of `wxUI::Item` (normal), `wxUI::Separator`, `wxUI::CheckItem`, and `wxUI::RadioItem`.

Menu Items are generally a name with a handler lambda, or name and id with a lambda.  Menu Items can also be assocated with `wxStandardID`.  Many of these like `wxID_EXIT` and `wxID_HELP` have predefined name, help, and handlers, so declaration with just an ID is allowed.

Handlers are callable items that handle events.  The handler can be declared with both no arguments or the `wxCommandEvent` argument for deeper inspection of the event.

```cpp
            wxUI::Separator {}, wxUI::Item { "&ExtendedExample...", [this] {
                                                ExtendedExample dialog(this);
                                                dialog.ShowModal();
                                            } },
            wxUI::Item { "&Example Item...", [] {
                            wxLogMessage("Hello World!");
                        } },
            wxUI::CheckItem { "&Example Checked Item...", [](wxCommandEvent& event) {
                                 wxLogMessage(event.IsChecked() ? "is checked" : "is not checked");
                             } },
```

Menu items (except `Separator`) follow the general pattern:

```
Items { ID }  // for primatives that have a system handler
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
        wxUI::Menu {
            "&Extra", wxUI::Menu {
                          "Pets",
                          wxUI::CheckItem { "Cats", [](wxCommandEvent& event) {
                                               wxLogMessage("Cats %s checked", event.IsChecked() ? "are" : "are not");
                                           } },
                          wxUI::CheckItem { "Dogs", [](wxCommandEvent& event) {
                                               wxLogMessage("Dogs %s checked", event.IsChecked() ? "are" : "are not");
                                           } },
                      },
    // ...
            wxUI::Separator {}, wxUI::Item { "&ExtendedExample...", [this] {
                                                ExtendedExample dialog(this);
                                                dialog.ShowModal();
                                            } },
            wxUI::Item { "&Example Item...", [] {
                            wxLogMessage("Hello World!");
                        } },
            wxUI::CheckItem { "&Example Checked Item...", [](wxCommandEvent& event) {
                                 wxLogMessage(event.IsChecked() ? "is checked" : "is not checked");
                             } },
        },
```

The `wxUI::MenuBar` and related objects are generally "lazy" objects.  They hold the details of the menu layout, but do not call any wxWidget primatives on construction.  When `attachTo` a frame is invoked does the underlying logic construct the menu structure.


### Layout

The basics of `wxUI` layout is the *Layout*.  You use a specific type of *Layout*, with the `wxUI::VSizer` (Vertical Sizer or "row") and `wxUI::HSizer` (Horizontal Sizer or "column") being the most common. When a *Layout* is set as the top level, it uses the layout as a sort of "blueprint" for stamping out the UI by constructing the ownership hierarchy and layout.

```cpp
    VSizer {
        wxSizerFlags().Expand().Border(),
        VSizer {
            "Text examples",
    // ...
    }
        .attachToAndFit(this);
```

In the above example we have constructed a vertical layout sizer that will use a `wxSizer` with the `wxSizerFlags` set to expand with a default border.  Then the first item in the sizer is a second layer sizer with horizontal layout.  The `wxSizerFlags` are propogated to each layer so the horizontal layout in this example would also be set to expand with a default border.  The second sizer would be created as a "named" box horizonal sizer.

A *Layout* takes a collection of objects, which can be either additional *Layout* (to create a tree of *Layouts*) or *Controllers*.  Here is the general form of constructions for *Sizers*:

```
Sizer { Items... }
Sizer { SizerFlags, Items... }
Sizer { "Name", Items... }
Sizer { "Name", SizerFlags, Items... }
```

`wxUI` supports 3 flavors of Sizers: `wxUI::VSizer` (Vertical Sizers), `wxUI::HSizer` (Horizontal Sizers), and `wxUI::FlexGridSizer` (Flexible Grid Sizers).  Both `wxUI::VSizer` and `wxUI::HSizer` can be created with a string to create a "named" box.

Note: Because Sizers are intented to be "recursive" data structures, it is possible for a `wxUI::VSizer` to contain a `wxUI::VSizer`.  However, be aware that if an empty `wxUI::VSizer` is created with *just* a `wxUI::VSizer` as the argument, we collapse that to be a single `wxUI::VSizer`.  ie, this:

```
wxUI::VSizer { wxUI::VSizer { "Current Frame" } }.attachTo(this);
```

is equivalant to:

```
wxUI::VSizer { "Current Frame" }.attachTo(this);
```


#### Generic

One special type of *Layout* is `Generic`.  There are cases where you may have an existing layout as a `wxSizer` (such as a common dialog) or `wxWindow` (such as a custom window) that you wish to use with `wxUI`.  This is a case to use `Generic`:

```cpp
    VSizer {
        wxSizerFlags().Expand().Border(),
    // ...
        Generic { CreateStdDialogButtonSizer(wxOK) },
    }
        .attachToAndFit(this);
```

### Controllers

*Controllers* are the general term to refer to items that behave like a [`wxContol`](https://docs.wxwidgets.org/3.0/classwx_control.html).  In `wxUI` we attempt to conform a consistent style that favors the common things you do with a specific `wxControl`.

```cpp
        HSizer {
            "Details",
            CheckBox { "Show" },
            Choice { { "Less", "More" } },
            TextCtrl { wxSizerFlags(1).Expand().Border(), "Fill in the blank" }
                .withStyle(wxALIGN_LEFT),
        },
```

By default *Controllers* are constructed using the default arguments for position, style, size, etc.  *Controllers* are designed to use [Method Chaining](https://en.wikipedia.org/wiki/Method_chaining) to specialize the way the controller is constructed.  In the example above we see that `wxUI::TextCtrl` is being augmented with the style `wxALIGN_LEFT`.

The list of Methods supported by all controllers:

 * `withPosition(wxPoint pos)` : Specifies the `pos` of the `wxControl`.
 * `withSize(wxSize size)` : Specifies the `size` of the `wxControl`.
 * `withStyle(long style)` : Specifies the `style` of the `wxControl`.

#### Bind

Some *Controllers* support "binding" a function call to their event handlers.  When the event for that controller is emitted, the function-like object supplied will be called.

```cpp
            Button { wxSizerFlags().Border(wxRIGHT), "Left" }
                .bind([] { wxLogMessage("Pressed Left"); }),
            Button { wxSizerFlags().Border(wxLEFT), "Right" }
                .bind([](wxCommandEvent&) { wxLogMessage("Pressed Right"); }),
```

For convenience the event parameter of the function can be omitted in cases where it is unused.

#### Proxy

Often the value of a *Controller* in a layout needs to be referenced, or sometimes the backing `wxWindow` itself needs to be used directly.  This could be for reading a currently typed in value in a `TextCtrl`, or to change the selection of a `Choice`.  *Controllers* support `Proxy` objects, a way to get the handle to the underlying `wxWindow` that is created for the *Controller*.

Some *Controllers* do not support values that are intended to change, such as a `Line`, and others can have several values of interest, such as a `ComboBox`.  `Proxy` objects can have several accessors that allow access to these, most commonly called `value()` and `selection()` (see Supported Controllers for details of each supported *Controller*).  These accessors are proxy objects support `get()` and `set()` functions, as well as a set of appropriate overloads for the underlying type, allowing more ergonomic interaction with the code.  `Proxy` also supplies `operator*` and `operator->` which reference the most common accessor.

`Proxy` supply `control()`, which is intended to allow access to the underlying controller.

As `Proxy` objects need to be a named variable that exist outside of a *Controller*, and require being "attached".  This is done with the `operator=`, allowing for an ergonomic way to attach `Proxy` objects to controls.  Accessing a proxy object that has not been attached to a controller will cause an exception to be raised.


```cpp
auto getHandle(UnderlyingType** handlePtr);
```

```cpp
class ExtendedExample : public wxDialog {
public:
    explicit ExtendedExample(wxWindow* parent);
private:
    wxTextCtrl* mText;
};

ExtendedExample::ExtendedExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExtendedExample")
{
    using namespace wxUI;
    VSizer {
        proxy = TextCtrl { "Hello" }
    }
        .attachTo(this);
}
```

#### Supported Controllers

The "Controllers" currently supported by `wxUI`:

| wxUI                 | wxWidget               | Proxy                     | Proxy accessors value |
| :------------------- | :--------------------- | :------------------------ | :-------------- |
| `Bitmap`             | `wxStaticBitmap`       | `BitmapProxy`             | n/a             |
| `BitmapButton`       | `wxBitmapButton`       | `BitmapButtonProxy`       | n/a             |
| `BitmapComboBox`     | `wxBitmapComboBox`     | `BitmapComboBoxProxy`     | `selection` -> `int`<BR>`value` -> `std::string`<BR>*default*: `value` |
| `BitmapToggleButton` | `wxBitmapToggleButton` | `BitmapToggleButtonProxy` | `value` -> `bool`<BR>*default*: `value` |
| `Button`             | `wxButton`             | `ButtonProxy`             | n/a             |
| `CheckBox`           | `wxCheckBox`           | `CheckBoxProxy`           | `value` -> `bool`<BR>*default*: `value` |
| `Choice`             | `wxChoice`             | `ChoiceProxy`             | `selection` -> `int`<BR>*default*: `selection` |
| `ComboBox`           | `wxComboBox`           | `ComboBoxProxy`           | `selection` -> `int`<BR>`value` -> `std::string`<BR>*default*: `value` |
| `Hypertext`          | `wxHypertextCtrl`      | `HypertextProxy`          | n/a             |
| `Line`               | `wxStaticLine`         | `LineProxy`               | n/a             |
| `ListBox`            | `wxListBox`            | `ListBoxProxy`            | `selection` -> `int`<BR>*default*: `selection` |
| `RadioBox`           | `wxRadioBox`           | `RadioBoxProxy`           | `selection` -> `int`<BR>*default*: `selection` |
| `Slider`             | `wxSlider`             | `SliderProxy`             | `value` -> `int`<BR>*default*: `value` |
| `SpinCtrl`           | `wxSpinCtrl`           | `SpinCtrlProxy`           | `value` -> `int`<BR>*default*: `value` |
| `Text`               | `wxStaticText`         | `TextProxy`               | `label` -> `std::string`<BR>*default*: `label` |
| `TextCtrl`           | `wxTextCtrl`           | `TextCtrlProxy`           | `label` -> `std::string`<BR>*default*: `label` |

Additional "Contollers" should be easy to add in future updates.

#### Custom

From time to time you may need to do some complicated custom wxWidget "controller" construction.  Use `Custom` "controller" to hook into the construction of the widget tree.  A `Custom` object is created supplying a function that conforms to the `CreateAndAdd` function.

```cpp
template <typename T>
concept CreateAndAddFunction = requires(T function, wxWindow* window, wxSizer* sizer)
{
    function(window, sizer, wxSizerFlags {});
};
```

You would then create the controller to confomr 

```cpp
        HSizer {
            Custom {
                [](wxWindow* window, wxSizer* sizer, wxSizerFlags flags) {
                    for (auto&& title : { "1", "2", "3" }) {
                        Button { title }.createAndAdd(window, sizer, flags);
                    }
                },
            },
        },
        Generic { CreateStdDialogButtonSizer(wxOK) },
```

#### Misc notes.

`wxRadioBox` requires a list of strings to operate correctly, so `RadioBox` requires a `std::vector` of strings.  Note, you *can* provide an empty `std::vector`, but a crash may occur if you do so.

`Button` and `BitmapButton` support the `setDefault` function which allows you to set them as the default button.
