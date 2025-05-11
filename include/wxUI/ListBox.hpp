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

#include "GetterSetter.hpp"
#include "Widget.hpp"
#include <wx/listbox.h>

#include "HelperMacros.hpp"

namespace wxUI {

// https://docs.wxwidgets.org/latest/classwx_list_box.html
struct ListBox : public details::WidgetDetails<ListBox, wxListBox> {
    using super = details::WidgetDetails<ListBox, wxListBox>;

    ListBox(std::initializer_list<std::string> choices = {})
        : ListBox(wxID_ANY, choices)
    {
    }

    explicit ListBox(wxWindowID identity, std::initializer_list<std::string> choices = {})
        : super(identity)
        , choices_(details::Ranges::convertTo(choices))
    {
    }

    explicit ListBox(details::Ranges::input_range_of<wxString> auto&& choices)
        : ListBox(wxID_ANY, std::forward<decltype(choices)>(choices))
    {
    }

    ListBox(wxWindowID identity, details::Ranges::input_range_of<wxString> auto&& choices)
        : super(identity)
        , choices_(details::Ranges::ToVector<wxString>(std::forward<decltype(choices)>(choices)))
    {
    }

    auto withSelection(int which) & -> ListBox&
    {
        selection_ = which;
        return *this;
    }

    auto withSelection(int which) && -> ListBox&&
    {
        selection_ = which;
        return std::move(*this);
    }

    auto withEnsureVisible(int which) & -> ListBox&
    {
        ensureVisible_ = which;
        return *this;
    }

    auto withEnsureVisible(int which) && -> ListBox&&
    {
        ensureVisible_ = which;
        return std::move(*this);
    }

    using super::bind;
    template <typename Function>
    auto bind(Function func) & -> ListBox&
    {
        return super::bind(wxEVT_LISTBOX, func);
    }

    template <typename Function>
    auto bind(Function func) && -> ListBox&&
    {
        return std::move(*this).super::bind(wxEVT_LISTBOX, func);
    }

    struct Proxy : details::WidgetProxy<underlying_t> {
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
    RULE_OF_SIX_BOILERPLATE(ListBox);

private:
    std::vector<wxString> choices_ {};
    int selection_ = wxNOT_FOUND;
    std::optional<int> ensureVisible_ {};

    auto createImpl(wxWindow* parent) -> wxWindow* override
    {
        auto* widget = bindProxy(new underlying_t(parent, getIdentity(), getPos(), getSize(), static_cast<int>(choices_.size()), choices_.data(), getStyle()));
        widget->SetSelection(selection_);
        if (ensureVisible_) {
            widget->EnsureVisible(*ensureVisible_);
        }
        return widget;
    }
};

WIDGET_STATIC_ASSERT_BOILERPLATE(ListBox);
}

#include "ZapMacros.hpp"
