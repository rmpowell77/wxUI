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

#include <wx/choice.h>
#include <wxUI/GetterSetter.hpp>
#include <wxUI/Widget.hpp>

#include <wxUI/detail/HelperMacros.hpp>

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_choice.html
struct Choice {
    using underlying_t = wxChoice;

    explicit Choice(std::initializer_list<std::string> choices = {})
        : Choice(wxID_ANY, choices)
    {
    }

    explicit Choice(wxWindowID identity, std::initializer_list<std::string> choices = {})
        : details_(identity)
        , choices_(details::Ranges::convertTo(choices))
    {
    }

    explicit Choice(details::Ranges::input_range_of<wxString> auto&& choices)
        : Choice(wxID_ANY, std::forward<decltype(choices)>(choices))
    {
    }

    Choice(wxWindowID identity, details::Ranges::input_range_of<wxString> auto&& choices)
        : details_(identity)
        , choices_(details::Ranges::ToVector<wxString>(std::forward<decltype(choices)>(choices)))
    {
    }

    auto withSelection(int which) & -> Choice&
    {
        selection_ = which;
        return *this;
    }

    auto withSelection(int which) && -> Choice&&
    {
        selection_ = which;
        return std::move(*this);
    }

    template <typename Function>
    auto bind(Function func) & -> Choice&
    {
        details_.bind(wxEVT_CHOICE, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> Choice&&
    {
        details_.bind(wxEVT_CHOICE, func);
        return std::move(*this);
    }

    struct Proxy : details::Proxy<underlying_t> {
        [[nodiscard]] auto selection() const
        {
            auto* controller = control();
            return details::GetterSetter {
                [controller] { return controller->GetSelection(); },
                [controller](int selection) { controller->SetSelection(selection); }
            };
        }

        auto operator*() const { return selection(); }
    };

private:
    details::WidgetDetails<Choice, wxChoice> details_;
    std::vector<wxString> choices_ {};
    int selection_ {};

    template <typename Parent>
    auto createImpl()
    {
        return [&choices = choices_, selection = selection_](Parent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) {
            auto* widget = customizations::ParentCreate<underlying_t>(parent, id, pos, size, static_cast<int>(choices.size()), choices.data(), style);
            widget->SetSelection(selection);
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(Choice)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(Choice);
}

#include <wxUI/detail/ZapMacros.hpp>
