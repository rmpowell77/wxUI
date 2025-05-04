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

#include "Widget.h"
#include <wx/button.h>

#include "HelperMacros.h"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_button.html
struct Button : public details::WidgetDetails<Button, wxButton> {
    using super = details::WidgetDetails<Button, wxButton>;
    using details::WidgetDetails<Button, wxButton>::underlying_t;

    explicit Button(std::string text = "")
        : Button(wxID_ANY, std::move(text))
    {
    }

    explicit Button(wxWindowID identity, std::string text = "")
        : super(identity)
        , text_(std::move(text))
    {
    }

    explicit Button(wxSizerFlags const& flags, std::string const& text = "")
        : Button(flags, wxID_ANY, text)
    {
    }

    Button(wxSizerFlags const& flags, wxWindowID identity, std::string text = "")
        : super(flags, identity)
        , text_(std::move(text))
    {
    }

    auto setDefault() -> Button&
    {
        isDefault_ = true;
        return *this;
    }

    using super::bind;
    template <typename Function>
    auto bind(Function func)
    {
        return super::bind(wxEVT_BUTTON, func);
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
        PROXY_BOILERPLATE();
    };
    RULE_OF_SIX_BOILERPLATE(Button);

private:
    std::string text_;
    bool isDefault_ = false;

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = setProxy(new underlying_t(parent, getIdentity(), text_, getPos(), getSize(), getStyle()));
        if (isDefault_) {
            widget->SetDefault();
        }
        return widget;
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(Button);
}

#include "ZapMacros.h"
