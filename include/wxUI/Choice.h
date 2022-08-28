#pragma once

#include <wx/choice.h>
#include <wxUI/Widget.h>

namespace wxUI {

struct Choice : public details::WidgetDetails<Choice> {
    using super = details::WidgetDetails<Choice>;
    using underlying_t = wxChoice;

    std::vector<wxString> choices;
    int selection {};

    Choice(wxWindowID identity, std::vector<wxString> const& choices = {})
        : super(identity)
        , choices(choices)
    {
    }

    Choice(std::vector<wxString> const& choices = {})
        : Choice(wxID_ANY, choices)
    {
    }

    Choice(wxSizerFlags const& flags, wxWindowID identity, std::vector<wxString> const& choices = {})
        : super(flags, identity)
        , choices(choices)
    {
    }

    Choice(wxSizerFlags const& flags, std::vector<wxString> const& choices = {})
        : Choice(flags, wxID_ANY, choices)
    {
    }

    auto& withSelection(int which)
    {
        selection = which;
        return *this;
    }

    wxWindow* create(wxWindow* parent) override
    {
        return new underlying_t(parent, this->identity, this->pos, this->size, choices.size(), choices.data(), this->usingStyle);
    }

    using super::createAndAdd;
};

}
