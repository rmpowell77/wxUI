#pragma once

#include <wx/textctrl.h>
#include <wxUI/Widget.h>

namespace wxUI {

struct TextCtrl : public details::WidgetDetails<TextCtrl> {
    using super = details::WidgetDetails<TextCtrl>;
    using underlying_t = wxTextCtrl;

    std::string text = "";

    TextCtrl(wxWindowID identity, std::string const& text = "")
        : super(identity)
        , text(text)
    {
    }

    TextCtrl(std::string const& text = "")
        : TextCtrl(wxID_ANY, text)
    {
    }

    TextCtrl(wxSizerFlags const& flags, wxWindowID identity, std::string const& text = "")
        : super(flags, identity)
        , text(text)
    {
    }

    TextCtrl(wxSizerFlags const& flags, std::string const& text = "")
        : TextCtrl(flags, wxID_ANY, text)
    {
    }

    wxWindow* create(wxWindow* parent) override
    {
        return new underlying_t(parent, this->identity, text, this->pos, this->size, this->usingStyle);
    }

    using super::createAndAdd;
};

}
