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

#include <wx/tglbtn.h>
#include <wxUI/GetterSetter.hpp>
#include <wxUI/Widget.hpp>

#include <wxUI/detail/HelperMacros.hpp>

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_bitmap_toggle_button.html
struct BitmapToggleButton {
    using underlying_t = wxBitmapToggleButton;

    explicit BitmapToggleButton(wxBitmap const& bitmap, std::optional<wxBitmap> bitmapPressed = std::nullopt)
        : BitmapToggleButton(wxID_ANY, bitmap, std::move(bitmapPressed))
    {
    }

    BitmapToggleButton(wxWindowID identity, wxBitmap const& bitmap, std::optional<wxBitmap> bitmapPressed = std::nullopt)
        : details_(identity)
        , bitmap_(bitmap)
        , bitmapPressed_(std::move(bitmapPressed))
    {
    }

    template <typename Function>
    auto bind(Function func) & -> BitmapToggleButton&
    {
        details_.bind(wxEVT_TOGGLEBUTTON, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> BitmapToggleButton&&
    {
        details_.bind(wxEVT_TOGGLEBUTTON, func);
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> {
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

private:
    details::WidgetDetails<BitmapToggleButton, wxBitmapToggleButton> details_;
    wxBitmap bitmap_;
    std::optional<wxBitmap> bitmapPressed_;

    template <typename Parent>
    auto createImpl()
    {
        return [&bitmap = bitmap_, &bitmapPressed = bitmapPressed_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, bitmap, pos, size, style);
            if (bitmapPressed) {
                widget->SetBitmapPressed(*bitmapPressed);
            }
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(BitmapToggleButton)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(BitmapToggleButton);
}

#include <wxUI/detail/ZapMacros.hpp>
