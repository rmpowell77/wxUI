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

#include <wx/artprov.h>
#include <wx/treelist.h>

#include <wxUI/wxUITypes.hpp>

namespace wxUI {

// A column helper with a fluent builder. Implicitly constructible from
// strings, so a bare "Type" is a valid column with default settings.
struct Column {
    wxString title;
    int width = wxCOL_WIDTH_DEFAULT;
    wxAlignment align = wxALIGN_LEFT;
    int flags = wxCOL_RESIZABLE;

    Column(char const* text)
        : Column(std::string_view(text))
    {
    }
    Column(std::string_view text = "")
        : Column(wxUI_String {}, details::toWxString(text))
    {
    }
    Column(wxUI_String, wxString text)
        : title(std::move(text))
    {
    }

    auto autoSize() & -> Column&
    {
        width = wxCOL_WIDTH_AUTOSIZE;
        return *this;
    }
    auto autoSize() && -> Column&&
    {
        width = wxCOL_WIDTH_AUTOSIZE;
        return std::move(*this);
    }
    auto pixels(int w) & -> Column&
    {
        width = w;
        return *this;
    }
    auto pixels(int w) && -> Column&&
    {
        width = w;
        return std::move(*this);
    }
    auto alignLeft() & -> Column&
    {
        align = wxALIGN_LEFT;
        return *this;
    }
    auto alignLeft() && -> Column&&
    {
        align = wxALIGN_LEFT;
        return std::move(*this);
    }
    auto alignRight() & -> Column&
    {
        align = wxALIGN_RIGHT;
        return *this;
    }
    auto alignRight() && -> Column&&
    {
        align = wxALIGN_RIGHT;
        return std::move(*this);
    }
    auto alignCenter() & -> Column&
    {
        align = wxALIGN_CENTER;
        return *this;
    }
    auto alignCenter() && -> Column&&
    {
        align = wxALIGN_CENTER;
        return std::move(*this);
    }
    auto fixed() & -> Column&
    {
        flags &= ~wxCOL_RESIZABLE;
        return *this;
    }
    auto fixed() && -> Column&&
    {
        flags &= ~wxCOL_RESIZABLE;
        return std::move(*this);
    }
    auto sortable() & -> Column&
    {
        flags |= wxCOL_SORTABLE;
        return *this;
    }
    auto sortable() && -> Column&&
    {
        flags |= wxCOL_SORTABLE;
        return std::move(*this);
    }
};

inline namespace literals {
    // "Name"_col.autoSize()  -- string literals can't have methods, this can.
    inline Column operator""_col(char const* s, [[maybe_unused]] std::size_t n)
    {
        return Column(s);
    }
}

template <std::size_t N>
struct Columns {
    std::array<Column, N> columns;

    template <class... S>
    requires(sizeof...(S) == N && (std::convertible_to<S, Column> && ...))
    Columns(S&&... s)
        : columns { Column(std::forward<S>(s))... }
    {
    }
};

template <class... S>
Columns(S&&...) -> Columns<sizeof...(S)>;

// Row is exactly N cells, each anything convertible to wxString.
template <std::size_t N>
struct Row {
    std::array<wxString, N> cells;

    template <class... S>
    requires(sizeof...(S) == N && (requires { wxUI::details::toWxString(std::declval<S>()); } && ...))
    Row(S&&... s)
        : cells { wxUI::details::toWxString(std::forward<S>(s))... }
    {
    }
};

// A row plus its child rows. Aggregate: Entry<3>{ {"a","b","c"}, { children } }
template <std::size_t N>
struct Entry {
    Row<N> parent;
    std::vector<Entry<N>> children = {};
    std::optional<wxArtID> imageClosed = std::nullopt;
    std::optional<wxArtID> imageOpened = std::nullopt;
};

}
