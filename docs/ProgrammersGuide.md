# wxUI
C++ header-only library to make declarative UIs for wxWidgets.

## Overview

This overview provides an overview of `wxUI`, but is not intented to be a tutorial on `wxWidgets`.  We assume the reader has working knowledge of `wxWidgets`.  Great documentation and guides are available at [https://www.wxwidgets.org](https://www.wxwidgets.org).

### Stack

The basics of `wxUI` layout is the `Stack` function.   The `Stack` wraps the objects that manage ownership and layout, and pass that as a `Context` object to the items to be layed out in the stack.  This is accomplished by a lambda that takes as an argument the `Context` that should be passed to each of the `wxUI` Control objects.

```cpp
    VStack(this, Sizer::ExpandBorder(), [this](auto const& context) {
        HStack(context, [this](auto const& context) {
            ...
        });
    });
```

In the above example we have constructed a vertical layout stack that will use a `wxSizer` with the `wxSizerFlags` set to expand with a default border.  Then the first item in the stack is a second layer stack with a horizontal layout.  The `wxSizerFlags` are propogated to each layer by the context so the horizontal layout in this example would also be set to expand with a default border.

`wxUI` supports 3 flavors of Stacks: `VStack` (Vertical Stacks), `HStack` (Horizontal Stacks), and `FlexGridStack` (Flexible Grid Stacks).  Both `VStack` and `HStack` can be created with a string to create a "named" box.

### Context

The `Context` object is simple holder of the `wxWindow`, `wxSizer`, and `wxSizerFlags` that is propogaged though the "Stack" layers.  `wxUI` Window objects as well as generic `wxWindow` can be added to the `Context`, which is effectively the same as adding them to the sizer.  For example:

```cpp
    VStack(this, [](auto const& context) {
        Button{ context, "Button" };
        context.add(CreateStdDialogButtonSizer(wxOK));
    });
```

Here we see that we are adding a `wxUI::Button` Control object to the context, which basically adds it to the vertical sizer.  Then we are adding the [`wxStdDialogButtonSizer`](https://docs.wxwidgets.org/3.2/classwx_std_dialog_button_sizer.html) that results from the call to [`CreateStdDialogButtonSizer`](https://docs.wxwidgets.org/3.0/classwx_dialog.html#a8f2f565975f843035391ce62b0b9f8e8) to the vertical sizer.

You can easily create new `Context` with a different wxSizerFlag by using the `with()` member function.

```cpp
struct Context {

...

    Context with(wxSizerFlags flags) const
    {
        auto copy = *this;
        copy.flags = flags;
        return copy;
    }

...

}
```

This enables the ability to add Control objects within the same stack with different a different `wxSizerFlag`, such as in this example:

```cpp
    HStack(context, "Details", [](auto const& context) {
        CheckBox{ context, "Show" };
        TextCtrl<wxALIGN_LEFT>{ context.with(Sizer::ExpandBorder<1>()), "Fill in the blank" };
        Choice{ context, {"Less", "More" } };
    });
```

In this case we have both the `wxUI::CheckBox` using the `wxSizerFlags` of the propogated `Context`.  But the `wxUI::TextCtrl` uses wxSizerFlags that cause that item to grow at max proporational size to the other items.  Because this is a copy of the original Context, the last `wxUI::Choice` object uses the same Context as the `wxUI::CheckBox`.

This hierarchy of `Context` propogation allows an easy way to change the wxSizerFlags of child Stack objects:

```cpp
    VStack(this, Sizer::Border(1), [](auto const& context) {
        HStack(context, Sizer::Border(10), [](auto const& context) {
            CheckBox{ context, "Show" };
            Choice{ context.with(Sizer::Border(2)), {"Less", "More" } };
        });
   });
```

In this example the first element of the Horizontal Stack are added with a border of size 10, but the second element is added with a border size of 20.  Note that the Horizontal wxSizer itself is added to the parent Vertical Stack with a border of 1.  This composition principle of Context and wxSizerFlags allows flexibility to control the layout.


### Controllers

### Menu

### Sizers

## Samples



