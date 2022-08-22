#pragma once

#include <wxUI/Widget.h>

namespace wxUI {

template <long Style = 0>
struct Choice : public details::WidgetDetails<Choice<Style>> {
    using super = details::WidgetDetails<Choice<Style>>;
    using underlying_t = wxChoice;

    std::vector<wxString> choices;
    int majorDim{};
    int selection{};

    Choice(wxWindowID identity, std::vector<wxString> const& choices = {})
    : super(identity)
    , choices(choices)
    {
    }

    Choice(std::vector<wxString> const& choices = {})
        : Choice(wxID_ANY, choices)
    {
    }

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags) {
        auto widget = new underlying_t(parent, this->identity, this->pos, this->size, choices.size(), choices.data(), Style);
        super::add(widget, sizer, flags);
        return widget;
    }

};

}
