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
#include <iostream>
#include <wx/radiobox.h>

#include "HelperMacros.hpp"

namespace wxUI {

struct Foo {
    Foo()
    {
        std::cout << "Foo none\n";
    }
    Foo(std::initializer_list<std::string> initList)
    {
        std::cout << "Foo std::initializer_list<std::string>\n";
        for (const auto& str : initList) {
            std::cout << str << std::endl;
        }
    }
    explicit Foo(char const* str)
    {
        std::cout << "Foo const char*\n";
        std::cout << str << std::endl;
    }
};

// https://docs.wxwidgets.org/latest/classwx_radio_box.html
struct RadioBox {
    // clang-format off
    struct withChoices { };
    // clang-format on

    using underlying_t = wxRadioBox;

    RadioBox([[maybe_unused]] withChoices unused, std::initializer_list<std::string> choices)
        : RadioBox(wxID_ANY, "", unused, choices)
    {
    }

    RadioBox(wxWindowID identity, [[maybe_unused]] withChoices unused, std::initializer_list<std::string> choices)
        : RadioBox(identity, "", unused, choices)
    {
    }

    explicit RadioBox(std::string text, [[maybe_unused]] withChoices unused, std::initializer_list<std::string> choices)
        : RadioBox(wxID_ANY, std::move(text), unused, choices)
    {
    }

    RadioBox(wxWindowID identity, std::string text, [[maybe_unused]] withChoices unused, std::initializer_list<std::string> choices)
        : details_(identity)
        , text_(std::move(text))
        , choices_(details::Ranges::convertTo(choices))
    {
        details_.setStyle(wxRA_SPECIFY_COLS);
    }

    explicit RadioBox([[maybe_unused]] withChoices unused, details::Ranges::input_range_of<wxString> auto&& choices)
        : RadioBox(wxID_ANY, "", unused, std::forward<decltype(choices)>(choices))
    {
    }

    RadioBox(wxWindowID identity, [[maybe_unused]] withChoices unused, details::Ranges::input_range_of<wxString> auto&& choices)
        : RadioBox(identity, "", unused, std::forward<decltype(choices)>(choices))
    {
    }

    RadioBox(std::string text, [[maybe_unused]] withChoices unused, details::Ranges::input_range_of<wxString> auto&& choices)
        : RadioBox(wxID_ANY, std::move(text), unused, std::forward<decltype(choices)>(choices))
    {
    }

    RadioBox(wxWindowID identity, std::string text, [[maybe_unused]] withChoices unused, details::Ranges::input_range_of<wxString> auto&& choices)
        : details_(identity)
        , text_(std::move(text))
        , choices_(details::Ranges::ToVector<wxString>(std::forward<decltype(choices)>(choices)))
    {
        details_.setStyle(wxRA_SPECIFY_COLS);
    }

    auto withSelection(int which) & -> RadioBox&
    {
        selection_ = which;
        return *this;
    }

    auto withSelection(int which) && -> RadioBox&&
    {
        selection_ = which;
        return std::move(*this);
    }

    auto withMajorDim(int majorDim) & -> RadioBox&
    {
        majorDim_ = majorDim;
        return *this;
    }

    auto withMajorDim(int majorDim) && -> RadioBox&&
    {
        majorDim_ = majorDim;
        return std::move(*this);
    }

    template <typename Function>
    auto bind(Function func) & -> RadioBox&
    {
        details_.bind(wxEVT_RADIOBOX, func);
        return *this;
    }

    template <typename Function>
    auto bind(Function func) && -> RadioBox&&
    {
        details_.bind(wxEVT_RADIOBOX, func);
        return std::move(*this);
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

private:
    details::WidgetDetails<RadioBox, wxRadioBox> details_;
    std::string text_;
    std::vector<wxString> choices_;
    int majorDim_ {};
    int selection_ {};

    auto createImpl()
    {
        return [&text = text_, &choices = choices_, majorDim = majorDim_, selection = selection_](wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style) -> underlying_t* {
            auto* widget = new underlying_t(parent, id, text, pos, size, static_cast<int>(choices.size()), choices.data(), majorDim, style);
            widget->SetSelection(selection);
            return widget;
        };
    }

public:
    WXUI_FORWARD_ALL_TO_DETAILS(RadioBox)
};

WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(RadioBox);
}

#include "ZapMacros.hpp"
