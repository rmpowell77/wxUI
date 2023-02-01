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
#include <wx/statline.h>

namespace wxUI {

struct Line : public details::WidgetDetails<Line, wxStaticLine> {
    using super = details::WidgetDetails<Line, wxStaticLine>;

    explicit Line(wxWindowID identity = wxID_ANY)
        : super(identity)
    {
    }

    explicit Line(wxSizerFlags const& flags, wxWindowID identity = wxID_ANY)
        : super(flags, identity)
    {
    }

    auto create(wxWindow* parent) -> wxWindow* override
    {
        return new underlying_t(parent, getIdentity(), getPos(), getSize(), getStyle());
    }

    virtual ~Line() = default;
    Line(Line const&) = default;
    Line(Line&&) = default;
    auto operator=(Line const&) -> Line& = default;
    auto operator=(Line&&) -> Line& = default;
};

inline auto HLine() -> Line
{
    return Line(wxSizerFlags {}.Border(wxALL, 2).Proportion(1).Expand()).withStyle(wxLI_HORIZONTAL);
}

inline auto VLine() -> Line
{
    return Line(wxSizerFlags {}.Border(wxALL, 2).Proportion(1).Expand()).withStyle(wxLI_VERTICAL);
}

static_assert(details::Widget<Line>);
}
