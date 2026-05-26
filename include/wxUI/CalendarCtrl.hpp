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

#include <wx/calctrl.h>
#include <wxUI/Widget.hpp>

#include <wxUI/detail/HelperMacros.hpp>

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_button.html
struct CalendarCtrl {
    using underlying_t = wxCalendarCtrl;

    explicit CalendarCtrl(wxDateTime const& dateTime = wxDefaultDateTime)
        : CalendarCtrl(wxID_ANY, dateTime)
    {
    }

    explicit CalendarCtrl(wxWindowID identity, wxDateTime const& dateTime = wxDefaultDateTime)
        : details_(identity)
        , dateTime_(dateTime)
    {
    }

    template <typename Function>
    auto bind(Function func) & -> CalendarCtrl&
    {
        details_.bind(wxEVT_CALENDAR_SEL_CHANGED, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> CalendarCtrl&&
    {
        details_.bind(wxEVT_CALENDAR_SEL_CHANGED, func);
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> {
    };

private:
    details::WidgetDetails<CalendarCtrl, wxCalendarCtrl> details_;
    wxDateTime dateTime_;

    template <typename Parent>
    auto createImpl()
    {
        return [&dateTime = dateTime_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            return customizations::ParentCreate<underlying_t>(parent, id, dateTime, pos, size, style);
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(CalendarCtrl)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(CalendarCtrl);
}

#include <wxUI/detail/ZapMacros.hpp>
