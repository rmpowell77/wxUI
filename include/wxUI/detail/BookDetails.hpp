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

#include <variant>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wxUI/Widget.hpp>
#include <wxUI/detail/HelperMacros.hpp>
#include <wxUI/wxUITypes.hpp>

template <details::SizerItem Item>
struct BookItem {
    template <details::SizerItem UItem>
    explicit BookItem(wxUI_String, wxString const& title, bool select, UItem&& item)
        : title_(std::move(title))
        , item_(std::forward<UItem>(item))
        , select_(select)
    {
    }

    template <details::SizerItem UItem>
    explicit BookItem(wxUI_String, wxString const& title, UItem&& item)
        : BookItem(wxUI_String {}, title, false, std::forward<UItem>(item))
    {
    }

    template <details::SizerItem UItem>
    explicit BookItem(std::string_view title, bool select, UItem&& item)
        : BookItem(wxUI_String {}, wxString::FromUTF8(title.data(), title.size()), select, std::forward<UItem>(item))
    {
    }

    template <details::SizerItem UItem>
    explicit BookItem(std::string_view title, UItem&& item)
        : BookItem(title, false, std::forward<UItem>(item))
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, [[maybe_unused]] Sizer* parentSizer, [[maybe_unused]] wxSizerFlags const& parentFlags)
    {
        auto page = new wxWindow(parent, wxID_ANY);
        auto sizer = new wxBoxSizer(wxVERTICAL);
        auto flags = wxSizerFlags { 1 }.Expand();

        details::createAndAddVisiter(item_, page, sizer, flags);

        page->SetSizerAndFit(sizer);

        if constexpr (requires(Parent p) { p.AddPage(page, title_, select_); }) {
            parent->AddPage(page, title_, select_);
        } else {
            throw std::runtime_error("Cannot add book item to normal window, use a parent that uses wxBookCtrlBase");
        }
    }

private:
    wxString title_ {};
    Item item_;
    bool select_ {};
};

template <details::SizerItem Item>
BookItem(std::string_view caption, Item&& item) -> BookItem<Item>;

template <details::SizerItem Item>
BookItem(std::string_view caption, bool select, Item&& item) -> BookItem<Item>;

template <details::SizerItem Item>
BookItem(wxUI_String, wxString const& caption, Item&& item) -> BookItem<Item>;

template <details::SizerItem Item>
BookItem(wxUI_String, wxString const& caption, bool select, Item&& item) -> BookItem<Item>;
