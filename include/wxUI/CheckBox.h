#pragma once

#include <wx/checkbox.h>
#include <wxUI/Widget.h>

namespace wxUI {

struct CheckBox : public details::WidgetDetails<CheckBox> {
    using super = details::WidgetDetails<CheckBox>;
    using underlying_t = wxCheckBox;

    std::string text = "";
    std::vector<wxString> choices;
    int majorDim {};
    int selection {};

    CheckBox(wxWindowID identity, std::string const& text = "")
        : super(identity)
        , text(text)
    {
    }

    CheckBox(std::string const& text = "")
        : CheckBox(wxID_ANY, text)
    {
    }

    CheckBox(wxSizerFlags const& flags, wxWindowID identity, std::string const& text = "")
        : super(flags, identity)
        , text(text)
    {
    }

    CheckBox(wxSizerFlags const& flags, std::string const& text = "")
        : CheckBox(flags, wxID_ANY, text)
    {
    }

    wxWindow* create(wxWindow* parent) override
    {
        return new underlying_t(parent, this->identity, text, this->pos, this->size, this->usingStyle);
    }

    using super::createAndAdd;
};

}
