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
#include <wx/spinctrl.h>

namespace wxUI {

struct SpinCtrl : public details::WidgetDetails<SpinCtrl, wxSpinCtrl> {
    using super = details::WidgetDetails<SpinCtrl, wxSpinCtrl>;

    std::optional<std::pair<int, int>> range;
    std::optional<int> initial;

    explicit SpinCtrl(wxWindowID identity, std::optional<std::pair<int, int>> range = {}, std::optional<int> initial = {})
        : super(identity)
        , range(range)
        , initial(initial)
    {
    }

    explicit SpinCtrl(std::optional<std::pair<int, int>> range = {}, std::optional<int> initial = {})
        : SpinCtrl(wxID_ANY, range, initial)
    {
    }

    explicit SpinCtrl(wxSizerFlags const& flags, wxWindowID identity, std::optional<std::pair<int, int>> range = {}, std::optional<int> initial = {})
        : super(flags, identity)
        , range(range)
        , initial(initial)
    {
    }

    explicit SpinCtrl(wxSizerFlags const& flags, std::optional<std::pair<int, int>> range = {}, std::optional<int> initial = {})
        : SpinCtrl(flags, wxID_ANY, range, initial)
    {
    }

    auto create(wxWindow* parent) -> wxWindow* override
    {
        auto min = range ? range->first : 0;
        auto max = range ? range->second : 100;
        auto initvalue = initial ? *initial : min;
        auto* widget = new underlying_t(parent, getIdentity(), wxEmptyString, getPos(), getSize(), getStyle(), min, max, initvalue);
        return widget;
    }

    template <typename Function>
    auto bind(Function func)
    {
        return details::BindWidgetToEvent { *this, wxEVT_SPINCTRL, func };
    }

    virtual ~SpinCtrl() = default;
    SpinCtrl(SpinCtrl const&) = default;
    SpinCtrl(SpinCtrl&&) = default;
    auto operator=(SpinCtrl const&) -> SpinCtrl& = default;
    auto operator=(SpinCtrl&&) -> SpinCtrl& = default;
};

static_assert(details::Widget<SpinCtrl>);
}
