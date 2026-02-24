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
#include <wx/notebook.h>
#include <wxUI/detail/BookDetails.hpp>

namespace wxUI {

template <typename Book, details::SizerItem... Items>
struct BookCtrl {
    template <details::SizerItem... UItems>
    explicit BookCtrl(UItems&&... items)
        : items_(std::forward_as_tuple(std::forward<UItems>(items)...))
    {
    }

    template <details::SizerItem... UItems>
    explicit BookCtrl(wxSizerFlags const& flags, UItems&&... items)
        : flags_(flags)
        , items_(std::forward_as_tuple(std::forward<UItems>(items)...))
    {
    }

    template <typename Parent, typename Sizer>
    auto createAndAdd(Parent* parent, Sizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        auto currentFlags = flags_.value_or(parentFlags);
        auto* book = createAndAddPages(parent, currentFlags);
        parentSizer->Add(book, currentFlags);
        return book;
    }

private:
    template <typename Parent>
    auto constructBook(Parent* parent) const
    {
        return new Book(parent, wxID_ANY);
    }

    template <typename Parent>
    auto createAndAddPages(Parent* parent, wxSizerFlags const& flags)
    {
        auto* book = constructBook(parent);

        std::apply([book, flags](auto&&... tupleArg) {
            (details::createAndAddVisiter(tupleArg, book, static_cast<wxSizer*>(nullptr), flags), ...);
        },
            items_);

        return book;
    }

    std::optional<wxSizerFlags> flags_ {};
    std::tuple<Items...> items_ {};
};

template <details::SizerItem... Items>
struct Notebook : BookCtrl<wxNotebook, Items...> {
    using Base = BookCtrl<wxNotebook, Items...>;
    using Base::Base; // inherit constructors
};

template <details::SizerItem... Items>
Notebook(Items... items) -> Notebook<Items...>;

template <details::SizerItem... Items>
Notebook(wxSizerFlags const& flags, Items... items) -> Notebook<Items...>;

}