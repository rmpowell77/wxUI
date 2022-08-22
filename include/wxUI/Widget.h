#pragma once

#include <wx/sizer.h>

namespace wxUI::details {

// A widget is anything that supports the createAndAdd function.

template <typename T>
concept Widget = requires(T widget, wxWindow* w, wxSizer* s) {
    widget.createAndAdd(w, s, wxSizerFlags{});
};


template <Widget W, typename Event, typename Function>
struct BindWidgetToEvent {
    W widget;
    Event event;
    Function function;

    BindWidgetToEvent(W const& w, Event const& e, Function const& f)
    : widget(w)
    , event(e)
    , function(f) {}

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags) {
        wxWindow* w = widget.createAndAdd(parent, sizer, flags);
        w->Bind(event, function);
        return w;
    }
};

template <Widget W, typename Event, typename Function>
BindWidgetToEvent(W, Event, Function) -> BindWidgetToEvent<W, Event, Function>;

// a place to put the common details of a Widget.
template <typename ConcreteWidget>
struct WidgetDetails {
    WidgetDetails(wxWindowID identity = wxID_ANY) : identity(identity) {}

    WidgetDetails(wxSizerFlags const& flags, wxWindowID identity)
    : flags(flags)
    , identity(identity) {}

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

protected:
    void add(wxWindow* widget, wxSizer* sizer, wxSizerFlags const& parentFlags) const {
          sizer->Add(widget, flags ? *flags : parentFlags);
    }

    std::optional<wxSizerFlags> flags;
    // these are common across the controls
    wxWindowID identity = wxID_ANY;
    wxPoint pos = wxDefaultPosition;
    wxSize size = wxDefaultSize;
    long usingStyle{};
};

}

namespace wxUI {
struct Generic {
    std::optional<wxSizerFlags> flags;
    wxSizer* sizer;

    Generic(wxSizerFlags const& flags, wxSizer* sizer)
    : flags(flags)
    , sizer(sizer) {}

    Generic(wxSizer* sizer)
    : sizer(sizer) {}

    void createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags) const {
        // the item has already been created, we're mearly holding on to it.
        parentSizer->Add(sizer, flags ? *flags : parentFlags);
    }

};

}
