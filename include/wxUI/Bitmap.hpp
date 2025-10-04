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

#include "Widget.hpp"
#include <wx/statbmp.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_static_bitmap.html
struct Bitmap {
    using underlying_t = wxStaticBitmap;

    explicit Bitmap(wxBitmap const& bitmap)
        : Bitmap(wxID_ANY, bitmap)
    {
    }

    Bitmap(wxWindowID identity, wxBitmap const& bitmap)
        : details_(identity)
        , bitmap_(bitmap)
    {
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
    };

private:
    details::WidgetDetails<Bitmap, wxStaticBitmap> details_;
    wxBitmap bitmap_;

    auto createImpl()
    {
        return [&bitmap = bitmap_](wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) -> underlying_t* {
            return new underlying_t(parent, id, bitmap, pos, size, style);
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(Bitmap)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(Bitmap);

}

#include "ZapMacros.hpp"
