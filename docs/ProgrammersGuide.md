# wxUI
C++ header-only library to make declarative UIs for wxWidgets.

## Overview

This overview provides an overview of `wxUI`, but is not intented to be a tutorial on `wxWidgets`.  We assume the reader has working knowledge of `wxWidgets`.  Great documentation and guides are available at [https://www.wxwidgets.org](https://www.wxwidgets.org).

In `wxUI`, you use "Menu" to declary the layout of your menu items and the "actions" they call.  Similarly, you use "Sizers" to declare the layout of "Controllers" for your application.

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

The top level `MenuBar` holds a collection of `Menu` objects.  The `Menu` object consists of a name of the menu, and a collection of "Items", which can be one of `Item` (normal), `Separator`, `CheckItem`, and `RadioItem`.

Menu Items are generally a name with a handler lambda, or name and id with a lambda.  Menu Items can also be assocated with `wxStandardID`.  Many of these like `wxID_EXIT` and `wxID_HELP` have predefined name, help, and handlers, so declaration with just an ID is allowed.

Handlers are callable items that handle events.  The handler can be declared with both no arguments or the `wxCommandEvent` argument for deeper inspection of the event.

```cpp
            wxUI::Item { "&Example1...\tCtrl-D", [] {
                            wxLogMessage("Hello World!");
                        } },
            wxUI::CheckItem { "&Example2...\tCtrl-D", [](wxCommandEvent& event) {
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

wxUI::Menu also allows nesting of menus.  This allows complicated menus to be composed easily.

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
        },
```

The `wxUI::MenuBar` and related objects are generally "lazy" objects.  They hold the details of the menu layout, but do not call any wxWidget primatives on construction.  When `attachTo` a frame is invoked does the underlying logic construct the menu structure.


### Layout

The basics of `wxUI` layout is the "Layout".  You use a specific type of "Layout", with the `VSizer` (Vertical Sizer or "row") and `HSizer` (Horizontal Sizer or "column") being the most common. When a "Layout" is set as the top level, it uses the layout as a sort of "blueprint" for stamping out the UI by constructing the ownership hierarchy and layout.

```cpp
    VSizer {
        wxSizerFlags().Expand().Border(),
        VSizer {
            "Text examples",
    // ...
    }
        .attachTo(this);
```

In the above example we have constructed a vertical layout sizer that will use a `wxSizer` with the `wxSizerFlags` set to expand with a default border.  Then the first item in the sizer is a second layer sizer with horizontal layout.  The `wxSizerFlags` are propogated to each layer so the horizontal layout in this example would also be set to expand with a default border.  The second sizer would be created as a "named" box horizonal sizer.

A "Layout" takes a collection of Items, which can be either additional "Layout" (to create a tree of "Layouts") or "Controllers".  Here is the general form of constructions for Sizers:

```
Sizer { Items... }
Sizer { SizerFlags, Items... }
Sizer { "Name", Items... }
Sizer { "Name", SizerFlags, Items... }
```

`wxUI` supports 3 flavors of Sizers: `VSizer` (Vertical Sizers), `HSizer` (Horizontal Sizers), and `FlexGridSizer` (Flexible Grid Sizers).  Both `VSizer` and `HSizer` can be created with a string to create a "named" box.

Note: Because Sizers are intented to be "recursive" data structures, it is possible for a `VSizer` to contain a `VSizer`.  However, be aware that if an empty `VSizer` is created with *just* a `VSizer` as the argument, we collapse that to be a single `VSizer`.  ie, this:

```
wxUI::VSizer { wxUI::VSizer { "Current Frame" } }.attachTo(this);
```

is equivalant to:

```
wxUI::VSizer { "Current Frame" }.attachTo(this);
```


#### Generic

One special type of "Layout" is `Generic`.  There are cases where you may have an existing layout as a `wxSizer` (such as a common dialog) or `wxWindow` (such as a custom window) that you wish to use with `wxUI`.  This is a case to use `Generic`:

```cpp
    VSizer {
        wxSizerFlags().Expand().Border(),
    // ...
        Generic { CreateStdDialogButtonSizer(wxOK) },
    }
        .attachTo(this);
```

### Controllers

"Controllers" are the general term to refer to items that behave like a [`wxContol`](https://docs.wxwidgets.org/3.0/classwx_control.html).  In `wxUI` we attempt to conform a consistent style that favors the common things you do with a specific `wxControl`.

```cpp
        HSizer {
            "Details",
            CheckBox { "Show" },
            Choice { { "Less", "More" } },
            TextCtrl { wxSizerFlags(1).Expand().Border(), "Fill in the blank" }
                .withStyle(wxALIGN_LEFT),
        },
```

By default "Controllers" are constructed using the default arguments for position, style, size, etc.  "Controllers" are designed to use Method Chaining to specialize the way the controller is constructed.  In the example above we see that `TextCtrl` is being augmented with the style `wxALIGN_LEFT`.

The list of Methods supported by all controllers:

 * `withPosition(wxPoint pos)` : Specifies the `pos` of the `wxControl`.
 * `withSize(wxSize size)` : Specifies the `size` of the `wxControl`.
 * `withStyle(long style)` : Specifies the `style` of the `wxControl`.

The "Controllers" currently supported by `wxUI`:

 * `BitmapButton` for `wxBitmapButton`
 * `BitmapComboBox` for `wxBitmapComboBox`
 * `BitmapToggleButton` for `wxBitmapToggleButton`
 * `Button` for `wxButton`
 * `CheckBox` for `wxCheckBox`
 * `Choice` for `wxChoice`
 * `ComboBox` for `ComboBox`
 * `ListBox` for `wxListBox`
 * `Line` for `wxStaticLine`
 * `RadioBox` for `wxRadioBox`
 * `Slider` for `wxSlider`
 * `SpinCtrl` for `wxSpinCtrl`
 * `Text` for `wxStaticText`
 * `TextCtrl` for `wxTextCtrl`

Additional "Contollers" should be easy to add in future updates.

#### Bind

Some "Controllers" support "Binding" a function call to their event handlers.  When the event for that controller is emitted, the function-like object supplied will be called.

```cpp
            Button { wxSizerFlags().Border(wxRIGHT), "Left" }
                .bind([] { wxLogMessage("Pressed Left"); }),
            Button { wxSizerFlags().Border(wxLEFT), "Right" }
                .bind([](wxCommandEvent&) { wxLogMessage("Pressed Right"); }),
```

For convenience the event parameter of the function can be omitted in cases where it is unused.

#### `getHandle`

Often a `wxWindow` object needs to be referenced.  For example, perhaps for reading a currently selected value.  "Controllers" support `getHandle`, a way to get the handle to the underlying `wxWindow` that is created for the "Controller".  You provide the address of the handle that you would like to be populated when the "Controller" is created

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
        TextCtrl { "Hello" }
            .getHandle(&mText),
        },
    }
        .attachTo(this);
}
```

Note that this handle is non-owning, so it is vital to not have the ptr participate in any lifecycle management of the object.

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
```

#### Misc notes.

`wxRadioBox` requires a list of strings to operate correctly, so `RadioBox` requires a `std::vector` of strings.  Note, you *can* provide an empty `std::vector`, but a crash may occur if you do so.

`Button` and `BitmapButton` support the `setDefault` function which allows you to set them as the default button.
