/*
MIT License

Copyright (c) 2022 Richard Powell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

#include <optional>
#include <wx/sizer.h>

namespace wxUI::details {

// A widget is anything that supports the createAndAdd function.

// clang-format off
template <typename T>
concept Widget = requires(T widget, wxWindow* window, wxSizer* sizer)
{
    widget.createAndAdd(window, sizer, wxSizerFlags {});
};

template <typename T>
concept CreateAndAddFunction = requires(T function, wxWindow* window, wxSizer* sizer)
{
    function(window, sizer, wxSizerFlags {});
};

// clang-format on

// https://stackoverflow.com/questions/27866909/get-function-arity-from-template-parameter
template <typename T>
constexpr bool noarg_callable_impl(
    typename std::enable_if<bool(sizeof((std::declval<T>()(), 0)))>::type*)
{
    return true;
}

template <typename T>
constexpr bool noarg_callable_impl(...)
{
    return false;
}

template <typename T>
constexpr bool is_noarg_callable()
{
    return noarg_callable_impl<T>(nullptr);
}

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <Widget W, typename Event, typename Function>
struct BindWidgetToEvent {
    W widget;
    Event event;
    Function function;

    BindWidgetToEvent(W const& widget, Event const& event, Function const& function)
        : widget(widget)
        , event(event)
        , function(function)
    {
    }

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
    {
        wxWindow* window = widget.createAndAdd(parent, sizer, flags);
        if constexpr (is_noarg_callable<Function>()) {
            window->Bind(event, [function = function](auto) {
                function();
            });
        } else {
            window->Bind(event, function);
        }
        return window;
    }
};

template <Widget W, typename Event, typename Function>
BindWidgetToEvent(W, Event, Function) -> BindWidgetToEvent<W, Event, Function>;

// This is to allow disambiguation of construction with a style
struct withStyle {
};

// The WidgetDetails are the base class of the Controllers.  The common details
// across many controllers are stored in the base class.
// The "recipe" for constructing a controller is pretty straight forward:
// 1. give a name.
// 2. inherit from WidgetDetails.
// 3. implement the create function for constructing the concrete widget.
template <typename ConcreteWidget, typename Underlying>
struct WidgetDetails {
    using underlying_t = Underlying;
    explicit WidgetDetails(wxWindowID identity = wxID_ANY)
        : identity(identity)
    {
    }

    WidgetDetails(wxSizerFlags const& flags, wxWindowID identity)
        : flags(flags)
        , identity(identity)
    {
    }

    explicit WidgetDetails(wxWindowID identity, withStyle, int64_t style)
        : identity(identity)
        , style(style)
    {
    }

    WidgetDetails(wxSizerFlags const& flags, wxWindowID identity, withStyle, int64_t style)
        : flags(flags)
        , identity(identity)
        , style(style)
    {
    }

    auto withPosition(wxPoint pos_) -> ConcreteWidget&
    {
        pos = pos_;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withSize(wxSize size_) -> ConcreteWidget&
    {
        size = size_;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto withStyle(int64_t style_) -> ConcreteWidget&
    {
        style = style_;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto getHandle(Underlying** handle) -> ConcreteWidget&
    {
        windowHandle = handle;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& parentFlags)
    {
        auto widget = dynamic_cast<Underlying*>(create(parent));
        sizer->Add(widget, flags ? *flags : parentFlags);
        if (windowHandle) {
            *windowHandle = widget;
        }
        return widget;
    }

    auto getIdentity() const { return identity; }
    auto getPos() const { return pos; }
    auto getSize() const { return size; }
    auto getStyle() const { return style; }

private:
    // these should be implemented in the derived classes.
    // aka the Template Pattern
    virtual auto create(wxWindow* parent) -> wxWindow* = 0;

    std::optional<wxSizerFlags> flags;
    // these are common across the controls
    wxWindowID identity = wxID_ANY;
    wxPoint pos = wxDefaultPosition;
    wxSize size = wxDefaultSize;
    int64_t style {};
    Underlying** windowHandle {};
};

}
