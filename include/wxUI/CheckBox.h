#pragma once

#include <wx/checkbox.h>
#include <wxUI/Widget.h>

namespace wxUI {

template <long Style = 0>
struct CheckBox : public details::WidgetDetails<CheckBox<Style>> {
    using super = details::WidgetDetails<CheckBox<Style>>;
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

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
    {
        auto widget = new underlying_t(parent, this->identity, text, this->pos, this->size, Style);
        super::add(widget, sizer, flags);
        return widget;
    }
};

}
