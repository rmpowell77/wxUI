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

#include <wx/statline.h>
#include <wxUI/Widget.hpp>

#include <wxUI/detail/HelperMacros.hpp>

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_static_line.html
struct Line {
    using underlying_t = wxStaticLine;

    explicit Line(wxWindowID identity = wxID_ANY)
        : details_(identity)
    {
    }

    struct Proxy : details::Proxy<underlying_t> {
    };

private:
    details::WidgetDetails<Line, wxStaticLine> details_;

    template <typename Parent>
    auto createImpl()
    {
        return [](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            return customizations::ParentCreate<underlying_t>(parent, id, pos, size, style);
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(Line)
};

inline auto HLine() -> Line
{
    return Line().withFlags(wxSizerFlags {}.Border(wxALL, 2).Proportion(1).Expand()).withStyle(wxLI_HORIZONTAL);
}

inline auto VLine() -> Line
{
    return Line().withFlags(wxSizerFlags {}.Border(wxALL, 2).Proportion(1).Expand()).withStyle(wxLI_VERTICAL);
}

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(Line);
}

#include <wxUI/detail/ZapMacros.hpp>
