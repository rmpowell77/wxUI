#pragma once

#include <wx/stattext.h>
#include <wxUI/Widget.h>

namespace wxUI {

struct Text : private details::WidgetDetails<Text> {
    using super = details::WidgetDetails<Text>;
    using underlying_t = wxStaticText;

    std::string text = "";

    Text(wxWindowID identity, std::string const& text = "")
        : super(identity)
        , text(text)
    {
    }

    Text(wxSizerFlags const& flags, wxWindowID identity, std::string const& text = "")
        : super(flags, identity)
        , text(text)
    {
    }

    Text(std::string const& text = "")
        : super(wxID_ANY)
        , text(text)
    {
    }

    Text(wxSizerFlags const& flags, std::string const& text = "")
        : super(flags, wxID_ANY)
        , text(text)
    {
    }

    wxWindow* create(wxWindow* parent) override
    {
        return new underlying_t(parent, identity, text, wxDefaultPosition, wxDefaultSize);
    }

    using super::createAndAdd;
};

}
