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
#include <wx/hyperlink.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_hyperlink_ctrl.html
struct Hyperlink {
    using underlying_t = wxHyperlinkCtrl;

    Hyperlink(std::string text, std::string url)
        : Hyperlink(wxID_ANY, std::move(text), std::move(url))
    {
    }

    Hyperlink(wxWindowID identity, std::string text, std::string url)
        : details_(identity)
        , text_(std::move(text))
        , url_(std::move(url))
    {
        details_.setStyle(wxHL_DEFAULT_STYLE);
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
    };

private:
    details::WidgetDetails<Hyperlink, wxHyperlinkCtrl> details_;
    std::string text_;
    std::string url_;

    template <typename Parent>
    auto createImpl()
    {
        return [&text = text_, &url = url_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            return customizations::ParentCreate<underlying_t>(parent, id, text, url, pos, size, style);
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(Hyperlink)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(Hyperlink);
}

#include "ZapMacros.hpp"
