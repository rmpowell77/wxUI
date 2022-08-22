#pragma once

#include <wx/radiobox.h>

namespace wxUI {

struct RadioBox : details::WidgetDetails<RadioBox> {
    using super = details::WidgetDetails<RadioBox>;
    using underlying_t = wxRadioBox;

    std::string text = "";
    std::vector<wxString> choices;
    int majorDim_{};
    int selection{};

    RadioBox(wxWindowID identity, std::string const& text = "", std::vector<wxString> const& choices = {})
    : super(identity)
    , text(text)
    , choices(choices)
    {
        usingStyle = wxRA_SPECIFY_COLS;
    }

    RadioBox(std::string const& text = "", std::vector<wxString> const& choices = {})
        : RadioBox(wxID_ANY, text, choices)
    {
    }

    wxWindow* createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags) {
        auto widget = new underlying_t(parent, this->identity, text, this->pos, this->size, choices.size(), choices.data(), majorDim_, this->usingStyle);
        widget->SetSelection(selection);
        sizer->Add(widget, flags);
        return widget;
    }

    // if a style changes, this must be recalled
    auto& withSelection(int which)
    {
        selection = which;
        return *this;
    }
    // if a style changes, this must be recalled
    auto& majorDim(int majorDim)
    {
        majorDim_ = majorDim;
        return *this;
    }
};

}

