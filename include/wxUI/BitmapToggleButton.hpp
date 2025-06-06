/*
MIT License

Copyright (c) 2022-2025 Richard Powell

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
#include <wx/tglbtn.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_bitmap_toggle_button.html
struct BitmapToggleButton : public details::WidgetDetails<BitmapToggleButton, wxBitmapToggleButton> {
    using super = details::WidgetDetails<BitmapToggleButton, wxBitmapToggleButton>;

    explicit BitmapToggleButton(wxBitmap const& bitmap, std::optional<wxBitmap> bitmapPressed = std::nullopt)
        : BitmapToggleButton(wxID_ANY, bitmap, std::move(bitmapPressed))
    {
    }

    BitmapToggleButton(wxWindowID identity, wxBitmap const& bitmap, std::optional<wxBitmap> bitmapPressed = std::nullopt)
        : super(identity)
        , bitmap_(bitmap)
        , bitmapPressed_(std::move(bitmapPressed))
    {
    }

    using super::bind;
    template <typename Function>
    auto bind(Function func) & -> BitmapToggleButton&
    {
        return super::bind(wxEVT_TOGGLEBUTTON, func);
    }

    template <typename Function>
    auto bind(Function func) && -> BitmapToggleButton&&
    {
        return std::move(*this).super::bind(wxEVT_TOGGLEBUTTON, func);
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
        [[nodiscard]] auto value() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return controller->GetValue(); },
                [controller](bool value) { controller->SetValue(value); }
            };
        }

        auto operator*() const { return value(); }
    };
    RULE_OF_SIX_BOILERPLATE(BitmapToggleButton);

private:
    wxBitmap bitmap_;
    std::optional<wxBitmap> bitmapPressed_;

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = bindProxy(new underlying_t(parent, getIdentity(), bitmap_, getPos(), getSize(), getStyle()));
        if (bitmapPressed_) {
            widget->SetBitmapPressed(*bitmapPressed_);
        }
        return widget;
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(BitmapToggleButton);
}

#include "ZapMacros.hpp"
