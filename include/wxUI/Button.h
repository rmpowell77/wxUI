#pragma once

#include <wx/button.h>
#include <wxUI/Widget.h>

namespace wxUI {

struct Button : public details::WidgetDetails<Button> {
    using super = details::WidgetDetails<Button>;
    using underlying_t = wxButton;

    std::string text = "";

    Button(wxSizerFlags const& flags, wxWindowID identity, std::string const& text = "")
    : super(flags, identity)
    , text(text)
    {
    }

    Button(wxSizerFlags const& flags, std::string const& text = "")
        : Button(flags, wxID_ANY, text)
    {
    }

    Button(wxWindowID identity, std::string const& text = "")
    : super(identity)
    , text(text)
    {
    }

    Button(std::string const& text = "")
        : Button(wxID_ANY, text)
    {
    }

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags) {
        auto widget = new underlying_t(parent, identity, text, wxDefaultPosition, wxDefaultSize);
        super::add(widget, sizer, flags);
        return widget;
    }

    template<typename Function>
    auto bind(Function func)
    {
        return details::BindWidgetToEvent{ *this, wxEVT_BUTTON, func };
    }
};

}
