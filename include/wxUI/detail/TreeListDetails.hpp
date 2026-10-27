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

#include <wx/treelist.h>
#include <wxUI/Widget.hpp>

#include <set>
#include <wxUI/detail/Column.hpp>
#include <wxUI/detail/HelperMacros.hpp>
#include <wxUI/detail/Images.hpp>

namespace wxUI::details {
template <std::size_t N>
static auto FindAllArt(std::vector<Entry<N>> const& items) -> std::set<wxArtID>
{
    std::set<wxArtID> normalImages;
    for (const auto& it : items) {
        if (it.imageClosed.has_value()) {
            normalImages.insert(*it.imageClosed);
        }
        if (it.imageOpened.has_value()) {
            normalImages.insert(*it.imageOpened);
        }
        auto childImages = FindAllArt(it.children);
        normalImages.insert(childImages.begin(), childImages.end());
    }
    return normalImages;
}

} // namespace wxUI::details
