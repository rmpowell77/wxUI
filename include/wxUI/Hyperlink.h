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
#include <wx/hyperlink.h>

#include "HelperMacros.h"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_hyperlink_ctrl.html
struct Hyperlink : public details::WidgetDetails<Hyperlink, wxHyperlinkCtrl> {
    using super = details::WidgetDetails<Hyperlink, wxHyperlinkCtrl>;

    Hyperlink(wxWindowID identity, std::string text, std::string url)
        : super(identity, super::WithStyle { wxHL_DEFAULT_STYLE })
        , text(std::move(text))
        , url(std::move(url))
    {
    }

    Hyperlink(std::string text, std::string url)
        : Hyperlink(wxID_ANY, std::move(text), std::move(url))
    {
    }

    Hyperlink(wxSizerFlags const& flags, wxWindowID identity, std::string text, std::string url)
        : super(flags, identity, super::WithStyle { wxHL_DEFAULT_STYLE })
        , text(std::move(text))
        , url(std::move(url))
    {
    }

    Hyperlink(wxSizerFlags const& flags, std::string text, std::string url)
        : Hyperlink(flags, wxID_ANY, std::move(text), std::move(url))
    {
    }

    struct Proxy : super::WidgetProxy {
        PROXY_BOILERPLATE();
    };
    RULE_OF_SIX_BOILERPLATE(Hyperlink);

private:
    std::string text;
    std::string url;

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        return setProxy(new underlying_t(parent, getIdentity(), text, url, getPos(), getSize(), getStyle()));
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(Hyperlink);
}

#include "ZapMacros.h"
