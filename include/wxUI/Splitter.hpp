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

#include "Generic.hpp"
#include "Widget.hpp"
#include <wx/button.h>
#include <wx/splitter.h>

#include "HelperMacros.hpp"

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
        , widgets_(std::move(widgets))
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

    auto withStashGravity(double gravity) -> HSplitter<W1, W2>&
    {
        stashGravity_ = gravity;
        return *this;
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
    };

    RULE_OF_SIX_BOILERPLATE(HSplitter);

private:
    std::pair<W1, W2> widgets_;
    std::optional<double> stashGravity_ {};

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = super::bindProxy(new underlying_t(parent, super::getIdentity(), super::getPos(), super::getSize(), super::getStyle()));
        widget->SplitHorizontally(std::get<0>(widgets_).create(widget), std::get<1>(widgets_).create(widget));
        if (stashGravity_) {
            widget->SetSashGravity(*stashGravity_);
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
        , widgets_(std::move(widgets))
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

    auto withStashGravity(double gravity) -> VSplitter<W1, W2>&
    {
        stashGravity_ = gravity;
        return *this;
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
    };
    RULE_OF_SIX_BOILERPLATE(VSplitter);

private:
    std::pair<W1, W2> widgets_;
    std::optional<double> stashGravity_ {};

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = super::bindProxy(new underlying_t(parent, super::getIdentity(), super::getPos(), super::getSize(), super::getStyle()));
        widget->SplitVertically(std::get<0>(widgets_).create(widget), std::get<1>(widgets_).create(widget));
        if (stashGravity_) {
            widget->SetSashGravity(*stashGravity_);
        }
        return widget;
    }
};

struct SplitterProxy : details::WidgetProxy<wxSplitterWindow> {
};

}
#include "ZapMacros.hpp"
