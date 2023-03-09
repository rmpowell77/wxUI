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
#include <wx/statbmp.h>

namespace wxUI {

struct Bitmap : public details::WidgetDetails<Bitmap, wxStaticBitmap> {
    using super = details::WidgetDetails<Bitmap, wxStaticBitmap>;

    Bitmap(wxWindowID identity, wxBitmap const& bitmap)
        : super(identity)
        , bitmap(bitmap)
    {
    }

    explicit Bitmap(wxBitmap const& bitmap)
        : Bitmap(wxID_ANY, bitmap)
    {
    }

    explicit Bitmap(wxSizerFlags const& flags, wxWindowID identity, wxBitmap const& bitmap)
        : super(flags, identity)
        , bitmap(bitmap)
    {
    }

    explicit Bitmap(wxSizerFlags const& flags, wxBitmap const& bitmap)
        : Bitmap(flags, wxID_ANY, bitmap)
    {
    }

    auto create(wxWindow* parent) -> wxWindow* override
    {
        return new underlying_t(parent, getIdentity(), bitmap, getPos(), getSize(), getStyle());
    }

    virtual ~Bitmap() = default;
    Bitmap(Bitmap const&) = default;
    Bitmap(Bitmap&&) = default;
    auto operator=(Bitmap const&) -> Bitmap& = default;
    auto operator=(Bitmap&&) -> Bitmap& = default;

private:
    wxBitmap bitmap;
};

static_assert(details::Widget<Bitmap>);
}
