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
#include <wx/slider.h>

namespace wxUI {

struct Slider : public details::WidgetDetails<Slider, wxSlider> {
    using super = details::WidgetDetails<Slider, wxSlider>;

    std::optional<std::pair<int, int>> range;
    std::optional<int> initial;

    explicit Slider(wxWindowID identity, std::optional<std::pair<int, int>> range = {}, std::optional<int> initial = {})
        : super(identity)
        , range(range)
        , initial(initial)
    {
    }

    explicit Slider(std::optional<std::pair<int, int>> range = {}, std::optional<int> initial = {})
        : Slider(wxID_ANY, range, initial)
    {
    }

    explicit Slider(wxSizerFlags const& flags, wxWindowID identity, std::optional<std::pair<int, int>> range = {}, std::optional<int> initial = {})
        : super(flags, identity)
        , range(range)
        , initial(initial)
    {
    }

    explicit Slider(wxSizerFlags const& flags, std::optional<std::pair<int, int>> range = {}, std::optional<int> initial = {})
        : Slider(flags, wxID_ANY, range, initial)
    {
    }

    auto create(wxWindow* parent) -> wxWindow* override
    {
        auto min = range ? range->first : 0;
        auto max = range ? range->second : 100;
        auto initvalue = initial ? *initial : min;
        auto* widget = new underlying_t(parent, getIdentity(), initvalue, min, max, getPos(), getSize(), getStyle());
        return widget;
    }

    template <typename Function>
    auto bind(Function func)
    {
        return details::BindWidgetToEvent { *this, wxEVT_SLIDER, func };
    }

    virtual ~Slider() = default;
    Slider(Slider const&) = default;
    Slider(Slider&&) = default;
    auto operator=(Slider const&) -> Slider& = default;
    auto operator=(Slider&&) -> Slider& = default;
};

static_assert(details::Widget<Slider>);
}
