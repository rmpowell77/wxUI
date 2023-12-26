/*
MIT License

Copyright (c) 2022 Richard Powell

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

#include "GetterSetter.h"
#include "Widget.h"
#include <wx/stattext.h>

#include "HelperMacros.h"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_text.html
struct Text : public details::WidgetDetails<Text, wxStaticText> {
    using super = details::WidgetDetails<Text, wxStaticText>;

    explicit Text(std::string text = "")
        : Text(wxID_ANY, std::move(text))
    {
    }

    explicit Text(wxWindowID identity, std::string text = "")
        : super(identity)
        , text(std::move(text))
    {
    }

    explicit Text(wxSizerFlags const& flags, std::string text = "")
        : Text(flags, wxID_ANY, std::move(text))
    {
    }

    Text(wxSizerFlags const& flags, wxWindowID identity, std::string text = "")
        : super(flags, identity)
        , text(std::move(text))
    {
    }

    auto withWrap(int wrapLength) -> Text&
    {
        wrap = wrapLength;
        return *this;
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
        PROXY_BOILERPLATE();

        [[nodiscard]] auto label() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return static_cast<std::string>(controller->GetLabel()); },
                [controller](std::string label) { controller->SetLabel(label); }
            };
        }

        auto operator*() const { return label(); }
    };

    RULE_OF_SIX_BOILERPLATE(Text);

private:
    std::string text;
    std::optional<int> wrap;

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = new underlying_t(parent, getIdentity(), text, getPos(), getSize(), getStyle());
        if (wrap) {
            widget->Wrap(*wrap);
        }
        return setProxy(widget);
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(Text);
}

#include "ZapMacros.h"
