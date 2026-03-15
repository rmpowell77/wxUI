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
#include <wx/choicebk.h>
#include <wx/listbook.h>
#include <wx/notebook.h>
#include <wx/simplebook.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/treebook.h>
#include <wxUI/Customizations.hpp>
#include <wxUI/detail/BookDetails.hpp>

namespace wxUI {

template <details::SizerItem... Items>
struct Notebook : details::BookCtrl<wxNotebook, Items...> {
    using Base = details::BookCtrl<wxNotebook, Items...>;
    using Base::Base; // inherit constructors
};

template <details::SizerItem... Items>
Notebook(Items... items) -> Notebook<Items...>;

template <details::SizerItem... Items>
Notebook(wxSizerFlags const& flags, Items... items) -> Notebook<Items...>;

template <details::SizerItem... Items>
struct Choicebook : details::BookCtrl<wxChoicebook, Items...> {
    using Base = details::BookCtrl<wxChoicebook, Items...>;
    using Base::Base; // inherit constructors
};

template <details::SizerItem... Items>
Choicebook(Items... items) -> Choicebook<Items...>;

template <details::SizerItem... Items>
Choicebook(wxSizerFlags const& flags, Items... items) -> Choicebook<Items...>;

template <details::SizerItem... Items>
struct Listbook : details::BookCtrl<wxListbook, Items...> {
    using Base = details::BookCtrl<wxListbook, Items...>;
    using Base::Base; // inherit constructors
};

template <details::SizerItem... Items>
Listbook(Items... items) -> Listbook<Items...>;

template <details::SizerItem... Items>
Listbook(wxSizerFlags const& flags, Items... items) -> Listbook<Items...>;

template <details::SizerItem... Items>
struct Simplebook : details::BookCtrl<wxSimplebook, Items...> {
    using Base = details::BookCtrl<wxSimplebook, Items...>;
    using Base::Base; // inherit constructors
};

template <details::SizerItem... Items>
Simplebook(Items... items) -> Simplebook<Items...>;

template <details::SizerItem... Items>
Simplebook(wxSizerFlags const& flags, Items... items) -> Simplebook<Items...>;

template <details::SizerItem... Items>
struct Treebook : details::BookCtrl<wxTreebook, Items...> {
    using Base = details::BookCtrl<wxTreebook, Items...>;
    using Base::Base; // inherit constructors
};

template <details::SizerItem... Items>
Treebook(Items... items) -> Treebook<Items...>;

template <details::SizerItem... Items>
Treebook(wxSizerFlags const& flags, Items... items) -> Treebook<Items...>;

}

#include <wxUI/detail/ZapMacros.hpp>
