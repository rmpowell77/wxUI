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
concept Widget = requires(T widget, wxWindow* w, wxSizer* s)
{
    widget.createAndAdd(w, s, wxSizerFlags {});
};
// clang-format on

// https://stackoverflow.com/questions/27866909/get-function-arity-from-template-parameter
template <typename T>
struct get_arity : get_arity<decltype(&T::operator())> {
};
template <typename R, typename... Args>
struct get_arity<R (*)(Args...)> : std::integral_constant<unsigned, sizeof...(Args)> {
};
// Possibly add specialization for variadic functions
// Member functions:
template <typename R, typename C, typename... Args>
struct get_arity<R (C::*)(Args...)> : std::integral_constant<unsigned, sizeof...(Args)> {
};
template <typename R, typename C, typename... Args>
struct get_arity<R (C::*)(Args...) const> : std::integral_constant<unsigned, sizeof...(Args)> {
};

template <Widget W, typename Event, typename Function>
struct BindWidgetToEvent {
    W widget;
    Event event;
    Function function;

    BindWidgetToEvent(W const& w, Event const& e, Function const& f)
        : widget(w)
        , event(e)
        , function(f)
    {
    }

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
    {
        wxWindow* w = widget.createAndAdd(parent, sizer, flags);
        if constexpr (get_arity<Function> {} == 0) {
            w->Bind(event, [this](auto) {
                function();
            });
        } else {
            w->Bind(event, function);
        }
        return w;
    }
};

template <Widget W, typename Event, typename Function>
BindWidgetToEvent(W, Event, Function) -> BindWidgetToEvent<W, Event, Function>;

// The WidgetDetails are the base class of the Controllers.  The common details
// across many controllers are stored in the base class.
// The "recipe" for constructing a controller is pretty straight forward:
// 1. give a name.
// 2. inherit from WidgetDetails.
// 3. implement the create function for constructing the concrete widget.
template <typename ConcreteWidget>
struct WidgetDetails {
    WidgetDetails(wxWindowID identity = wxID_ANY)
        : identity(identity)
    {
    }

    WidgetDetails(wxSizerFlags const& flags, wxWindowID identity)
        : flags(flags)
        , identity(identity)
    {
    }

    ConcreteWidget& withPosition(wxPoint pos_)
    {
        pos = pos_;
        return static_cast<ConcreteWidget&>(*this);
    }

    ConcreteWidget& withSize(wxSize size_)
    {
        size = size_;
        return static_cast<ConcreteWidget&>(*this);
    }

    ConcreteWidget& style(long style)
    {
        usingStyle = style;
        return static_cast<ConcreteWidget&>(*this);
    }

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& parentFlags)
    {
        auto widget = create(parent);
        sizer->Add(widget, flags ? *flags : parentFlags);
        return widget;
    }

private:
    // these should be implemented in the derived classes.
    // aka the Template Pattern
    virtual wxWindow* create(wxWindow* parent) = 0;

public:
    std::optional<wxSizerFlags> flags;
    // these are common across the controls
    wxWindowID identity = wxID_ANY;
    wxPoint pos = wxDefaultPosition;
    wxSize size = wxDefaultSize;
    long usingStyle {};
};

}
