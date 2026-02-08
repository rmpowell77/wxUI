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

#include "GetterSetter.hpp"
#include "Widget.hpp"
#include <wx/stattext.h>
#include <wx/string.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_text.html
struct Text {
    using underlying_t = wxStaticText;

    explicit Text(std::string_view text = "")
        : Text(wxID_ANY, text)
    {
    }

    explicit Text(wxWindowID identity, std::string_view text = "")
        : Text(identity, wxUI_String {}, wxString::FromUTF8(text.data(), text.size()))
    {
    }

    explicit Text(wxUI_String tag, wxString text)
        : Text(wxID_ANY, tag, std::move(text))
    {
    }

    explicit Text(wxWindowID identity, wxUI_String, wxString text)
        : details_(identity)
        , text_(std::move(text))
    {
    }

    auto withWrap(int wrapLength) & -> Text&
    {
        wrap_ = wrapLength;
        return *this;
    }

    auto withWrap(int wrapLength) && -> Text&&
    {
        wrap_ = wrapLength;
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> {
        [[nodiscard]] auto label() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return controller->GetLabel().utf8_string(); },
                [controller](std::string const& label) { controller->SetLabel(wxString::FromUTF8(label)); }
            };
        }

        auto operator*() const { return label(); }
    };

private:
    details::WidgetDetails<Text, wxStaticText> details_;
    wxString text_;
    std::optional<int> wrap_;

    // Templated createImpl to support test Providers
    template <typename Parent>
    auto createImpl()
    {
        return [&text = text_, &wrap = wrap_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, text, pos, size, style);
            if (wrap) {
                widget->Wrap(*wrap);
            }
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(Text)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(Text);
}

#include "ZapMacros.hpp"
