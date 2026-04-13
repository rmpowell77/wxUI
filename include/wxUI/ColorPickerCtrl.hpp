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

#include <wx/clrpicker.h>
#include <wxUI/Widget.hpp>

#include <wxUI/detail/HelperMacros.hpp>

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_button.html
struct ColorPickerCtrl {
    using underlying_t = wxColourPickerCtrl;

    explicit ColorPickerCtrl(wxColour const& color = *wxBLACK)
        : ColorPickerCtrl(wxID_ANY, color)
    {
    }

    explicit ColorPickerCtrl(wxWindowID identity, wxColour const& color = *wxBLACK)
        : details_(identity)
        , color_(color)
    {
    }

    template <typename Function>
    auto bind(Function func) & -> ColorPickerCtrl&
    {
        details_.bind(wxEVT_COLOURPICKER_CHANGED, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> ColorPickerCtrl&&
    {
        details_.bind(wxEVT_COLOURPICKER_CHANGED, func);
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> {
    };

private:
    details::WidgetDetails<ColorPickerCtrl, wxColourPickerCtrl> details_;
    wxColour color_;

    template <typename Parent>
    auto createImpl()
    {
        return [&color = color_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            return customizations::ParentCreate<underlying_t>(parent, id, color, pos, size, style);
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(ColorPickerCtrl)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(ColorPickerCtrl);
}

#include <wxUI/detail/ZapMacros.hpp>
