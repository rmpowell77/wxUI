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
#include <wx/bmpbuttn.h>

#include "HelperMacros.h"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_bitmap_button.html
struct BitmapButton : public details::WidgetDetails<BitmapButton, wxBitmapButton> {
    using super = details::WidgetDetails<BitmapButton, wxBitmapButton>;

    explicit BitmapButton(wxBitmap const& bitmap)
        : BitmapButton(wxID_ANY, bitmap)
    {
    }

    BitmapButton(wxWindowID identity, wxBitmap const& bitmap)
        : super(identity)
        , bitmap_(bitmap)
    {
    }

    BitmapButton(wxSizerFlags const& flags, wxBitmap const& bitmap)
        : BitmapButton(flags, wxID_ANY, bitmap)
    {
    }

    BitmapButton(wxSizerFlags const& flags, wxWindowID identity, wxBitmap const& bitmap)
        : super(flags, identity)
        , bitmap_(bitmap)
    {
    }

    auto setDefault() -> BitmapButton&
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
    RULE_OF_SIX_BOILERPLATE(BitmapButton);

private:
    wxBitmap bitmap_;
    bool isDefault_ = false;

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = setProxy(new underlying_t(parent, getIdentity(), bitmap_, getPos(), getSize(), getStyle()));
        if (isDefault_) {
            widget->SetDefault();
        }
        return widget;
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(BitmapButton);
}

#include "ZapMacros.h"
