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
#include <wx/tglbtn.h>

namespace wxUI {

struct BitmapToggleButton : public details::WidgetDetails<BitmapToggleButton, wxBitmapToggleButton> {
    using super = details::WidgetDetails<BitmapToggleButton, wxBitmapToggleButton>;

    wxBitmap bitmap;
    std::optional<wxBitmap> bitmapPressed;

    BitmapToggleButton(wxWindowID identity, wxBitmap const& bitmap, std::optional<wxBitmap> bitmapPressed = {})
        : super(identity)
        , bitmap(bitmap)
        , bitmapPressed(std::move(bitmapPressed))
    {
    }

    explicit BitmapToggleButton(wxBitmap const& bitmap, std::optional<wxBitmap> bitmapPressed = {})
        : BitmapToggleButton(wxID_ANY, bitmap, std::move(bitmapPressed))
    {
    }

    BitmapToggleButton(wxSizerFlags const& flags, wxWindowID identity, wxBitmap const& bitmap, std::optional<wxBitmap> bitmapPressed = {})
        : super(flags, identity)
        , bitmap(bitmap)
        , bitmapPressed(std::move(bitmapPressed))
    {
    }

    BitmapToggleButton(wxSizerFlags const& flags, wxBitmap const& bitmap, std::optional<wxBitmap> bitmapPressed = {})
        : BitmapToggleButton(flags, wxID_ANY, bitmap, std::move(bitmapPressed))
    {
    }

    auto create(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = new underlying_t(parent, identity, bitmap, wxDefaultPosition, wxDefaultSize);
        if (bitmapPressed) {
            widget->SetBitmapPressed(*bitmapPressed);
        }
        return widget;
    }

    template <typename Function>
    auto bind(Function func)
    {
        return details::BindWidgetToEvent { *this, wxEVT_TOGGLEBUTTON, func };
    }

    virtual ~BitmapToggleButton() = default;
    BitmapToggleButton(BitmapToggleButton const&) = default;
    BitmapToggleButton(BitmapToggleButton&&) = default;
    auto operator=(BitmapToggleButton const&) -> BitmapToggleButton& = default;
    auto operator=(BitmapToggleButton&&) -> BitmapToggleButton& = default;
};

static_assert(details::Widget<BitmapToggleButton>);
}
