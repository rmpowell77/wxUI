#pragma once

#include <wx/sizer.h>

namespace wxUI::details {

// A widget is anything that supports the createAndAdd function.

template <typename T>
concept Widget = requires(T widget, wxWindow* w, wxSizer* s)
{
    widget.createAndAdd(w, s, wxSizerFlags {});
};

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

namespace wxUI {
struct Generic {
    std::optional<wxSizerFlags> flags;
    wxSizer* sizer;

    Generic(wxSizerFlags const& flags, wxSizer* sizer)
        : flags(flags)
        , sizer(sizer)
    {
    }

    Generic(wxSizer* sizer)
        : sizer(sizer)
    {
    }

    void createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags) const
    {
        // the item has already been created, we're mearly holding on to it.
        parentSizer->Add(sizer, flags ? *flags : parentFlags);
    }
};

}
