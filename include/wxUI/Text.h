#pragma once

#include <wx/hyperlink.h>
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

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
    {
        auto widget = new underlying_t(parent, identity, text, wxDefaultPosition, wxDefaultSize);
        super::add(widget, sizer, flags);
        return widget;
    }
};

struct TextCtrl : public details::WidgetDetails<TextCtrl> {
    using super = details::WidgetDetails<TextCtrl>;
    using underlying_t = wxTextCtrl;

    std::string text = "";

    TextCtrl(wxSizerFlags const& flags, wxWindowID identity, std::string const& text = "")
        : super(flags, identity)
        , text(text)
    {
    }

    TextCtrl(wxSizerFlags const& flags, std::string const& text = "")
        : TextCtrl(flags, wxID_ANY, text)
    {
    }

    TextCtrl(wxWindowID identity, std::string const& text = "")
        : super(identity)
        , text(text)
    {
    }

    TextCtrl(std::string const& text = "")
        : TextCtrl(wxID_ANY, text)
    {
    }

    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& flags)
    {
        auto widget = new underlying_t(parent, this->identity, text, this->pos, this->size, this->usingStyle);
        super::add(widget, sizer, flags);
        return widget;
    }
};

}
