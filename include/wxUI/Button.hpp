/*
MIT License

Copyright (c) 2022-2026 Richard Powell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

#include <wx/button.h>
#include <wxUI/Widget.hpp>

#include <wxUI/detail/HelperMacros.hpp>

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_button.html
struct Button {
    using underlying_t = wxButton;

    explicit Button(std::string_view text = "")
        : Button(wxID_ANY, text)
    {
    }

    explicit Button(wxWindowID identity, std::string_view text = "")
        : Button(identity, wxUI_String {}, wxString::FromUTF8(text.data(), text.size()))
    {
    }

    explicit Button(wxUI_String tag, wxString text)
        : Button(wxID_ANY, tag, std::move(text))
    {
    }

    explicit Button(wxWindowID identity, wxUI_String, wxString text)
        : details_(identity)
        , text_(std::move(text))
    {
    }

    auto setDefault() & -> Button&
    {
        isDefault_ = true;
        return *this;
    }

    auto setDefault() && -> Button&&
    {
        isDefault_ = true;
        return std::move(*this);
    }

    template <typename Function>
    auto bind(Function func) & -> Button&
    {
        details_.bind(wxEVT_BUTTON, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> Button&&
    {
        details_.bind(wxEVT_BUTTON, func);
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> {
    };

private:
    details::WidgetDetails<Button, wxButton> details_;
    wxString text_;
    bool isDefault_ = false;

    template <typename Parent>
    auto createImpl()
    {
        return [&text = text_, isDefault = isDefault_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, text, pos, size, style);
            if (isDefault) {
                widget->SetDefault();
            }
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(Button)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(Button);
}

#include <wxUI/detail/ZapMacros.hpp>
