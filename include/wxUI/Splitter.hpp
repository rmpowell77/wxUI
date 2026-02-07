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
struct HSplitter {
    using underlying_t = wxSplitterWindow;

    HSplitter(wxWindowID identity, std::pair<W1, W2> widgets)
        : details_(identity)
        , widgets_(std::move(widgets))
    {
        details_.setStyle(wxSP_3D | wxSP_LIVE_UPDATE);
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

    struct Proxy : details::Proxy<underlying_t> {
    };

private:
    details::WidgetDetails<HSplitter<W1, W2>, wxSplitterWindow> details_;
    std::pair<W1, W2> widgets_;
    std::optional<double> stashGravity_ {};

    template <typename Parent>
    auto createImpl()
    {
        return [&widgets = widgets_, &stashGravity = stashGravity_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, pos, size, style);
            widget->SplitHorizontally(std::get<0>(widgets).create(widget), std::get<1>(widgets).create(widget));
            if (stashGravity) {
                widget->SetSashGravity(*stashGravity);
            }
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(HSplitter)
};

template <details::Createable W1, details::Createable W2>
struct VSplitter {
    using underlying_t = wxSplitterWindow;

    VSplitter(wxWindowID identity, std::pair<W1, W2> widgets)
        : details_(identity)
        , widgets_(std::move(widgets))
    {
        details_.setStyle(wxSP_3D | wxSP_LIVE_UPDATE);
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

    struct Proxy : details::Proxy<underlying_t> {
    };

private:
    details::WidgetDetails<VSplitter<W1, W2>, wxSplitterWindow> details_;
    std::pair<W1, W2> widgets_;
    std::optional<double> stashGravity_ {};

    template <typename Parent>
    auto createImpl()
    {
        return [&widgets = widgets_, &stashGravity = stashGravity_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, pos, size, style);
            widget->SplitVertically(std::get<0>(widgets).create(widget), std::get<1>(widgets).create(widget));
            if (stashGravity) {
                widget->SetSashGravity(*stashGravity);
            }
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(VSplitter)
};

struct SplitterProxy : details::Proxy<wxSplitterWindow> {
};

}
#include "ZapMacros.hpp"
