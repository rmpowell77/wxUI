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

#include "Generic.h"
#include "Widget.h"
#include <wx/button.h>
#include <wx/splitter.h>

#include "HelperMacros.h"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_splitter_window.html

// unfortunately due to template types here, it is not easily possible to create a hierarchy of
// a generic Splitter.  This may be able to be fixed with Deducing this...
template <details::Createable W1, details::Createable W2>
struct HSplitter : public details::WidgetDetails<HSplitter<W1, W2>, wxSplitterWindow> {
    using super = details::WidgetDetails<HSplitter<W1, W2>, wxSplitterWindow>;
    using underlying_t = typename details::WidgetDetails<HSplitter<W1, W2>, wxSplitterWindow>::underlying_t;
    using WithStyle = typename super::WithStyle;

    HSplitter(wxWindowID identity, std::pair<W1, W2> widgets)
        : super(identity, WithStyle { wxSP_3D | wxSP_LIVE_UPDATE })
        , widgets(std::move(widgets))
    {
    }

    HSplitter(wxWindowID identity, W1 widget1, W2 widget2)
        : HSplitter(identity, std::make_pair(std::move(widget1), std::move(widget2)))
    {
    }

    explicit HSplitter(std::pair<W1, W2> widgets)
        : HSplitter(wxID_ANY, std::move(widgets))
    {
    }

    HSplitter(W1 widget1, W2 widget2)
        : HSplitter(wxID_ANY, std::make_pair(std::move(widget1), std::move(widget2)))
    {
    }

    HSplitter(wxSizerFlags flags, wxWindowID identity, std::pair<W1, W2> widgets)
        : super(flags, identity, WithStyle { wxSP_3D | wxSP_LIVE_UPDATE })
        , widgets(std::move(widgets))
    {
    }

    HSplitter(wxSizerFlags flags, wxWindowID identity, W1 widget1, W2 widget2)
        : HSplitter(flags, identity, std::make_pair(std::move(widget1), std::move(widget2)))
    {
    }

    HSplitter(wxSizerFlags flags, std::pair<W1, W2> widgets)
        : HSplitter(flags, wxID_ANY, std::move(widgets))
    {
    }

    HSplitter(wxSizerFlags flags, W1 widget1, W2 widget2)
        : HSplitter(flags, wxID_ANY, std::make_pair(std::move(widget1), std::move(widget2)))
    {
    }

    auto withStashGravity(double gravity) -> HSplitter<W1, W2>&
    {
        stashGravity = gravity;
        return *this;
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
        PROXY_BOILERPLATE();
    };

    RULE_OF_SIX_BOILERPLATE(HSplitter);

private:
    std::pair<W1, W2> widgets;
    std::optional<double> stashGravity {};

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = super::setProxy(new underlying_t(parent, super::getIdentity(), super::getPos(), super::getSize(), super::getStyle()));
        widget->SplitHorizontally(std::get<0>(widgets).create(widget), std::get<1>(widgets).create(widget));
        if (stashGravity) {
            widget->SetSashGravity(*stashGravity);
        }
        return widget;
    }
};

template <details::Createable W1, details::Createable W2>
struct VSplitter : public details::WidgetDetails<VSplitter<W1, W2>, wxSplitterWindow> {
    using super = details::WidgetDetails<VSplitter<W1, W2>, wxSplitterWindow>;
    using underlying_t = typename details::WidgetDetails<VSplitter<W1, W2>, wxSplitterWindow>::underlying_t;
    using WithStyle = typename super::WithStyle;

    VSplitter(wxWindowID identity, std::pair<W1, W2> widgets)
        : super(identity, WithStyle { wxSP_3D | wxSP_LIVE_UPDATE })
        , widgets(std::move(widgets))
    {
    }

    VSplitter(wxWindowID identity, W1 widget1, W2 widget2)
        : VSplitter(identity, std::make_pair(std::move(widget1), std::move(widget2)))
    {
    }

    explicit VSplitter(std::pair<W1, W2> widgets)
        : VSplitter(wxID_ANY, std::move(widgets))
    {
    }

    VSplitter(W1 widget1, W2 widget2)
        : VSplitter(wxID_ANY, std::make_pair(std::move(widget1), std::move(widget2)))
    {
    }

    VSplitter(wxSizerFlags flags, wxWindowID identity, std::pair<W1, W2> widgets)
        : super(flags, identity, WithStyle { wxSP_3D | wxSP_LIVE_UPDATE })
        , widgets(std::move(widgets))
    {
    }

    VSplitter(wxSizerFlags flags, wxWindowID identity, W1 widget1, W2 widget2)
        : VSplitter(flags, identity, std::make_pair(std::move(widget1), std::move(widget2)))
    {
    }

    VSplitter(wxSizerFlags flags, std::pair<W1, W2> widgets)
        : VSplitter(flags, wxID_ANY, std::move(widgets))
    {
    }

    VSplitter(wxSizerFlags flags, W1 widget1, W2 widget2)
        : VSplitter(flags, wxID_ANY, std::make_pair(std::move(widget1), std::move(widget2)))
    {
    }

    auto withStashGravity(double gravity) -> VSplitter<W1, W2>&
    {
        stashGravity = gravity;
        return *this;
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
        PROXY_BOILERPLATE();
    };
    RULE_OF_SIX_BOILERPLATE(VSplitter);

private:
    std::pair<W1, W2> widgets;
    std::optional<double> stashGravity {};

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = super::setProxy(new underlying_t(parent, super::getIdentity(), super::getPos(), super::getSize(), super::getStyle()));
        widget->SplitVertically(std::get<0>(widgets).create(widget), std::get<1>(widgets).create(widget));
        if (stashGravity) {
            widget->SetSashGravity(*stashGravity);
        }
        return widget;
    }
};

struct SplitterProxy : details::WidgetProxy<wxSplitterWindow> {
    PROXY_BOILERPLATE();
};

}
#include "ZapMacros.h"
