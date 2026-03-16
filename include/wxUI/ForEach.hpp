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

#include <wxUI/Layout.hpp>
#include <wxUI/detail/ForEachDetail.hpp>

namespace wxUI {

template <typename T, typename Function>
auto VForEach(std::initializer_list<T> args, Function&& function)
{
    return VSizer {
        detail::ForEach(std::move(args), std::forward<Function>(function))
    };
}

template <std::ranges::input_range Range, typename Function>
auto VForEach(Range&& args, Function&& function)
{
    return VSizer {
        detail::ForEach(std::forward<Range>(args), std::forward<Function>(function))
    };
}

template <typename T, typename Function>
auto VForEach(wxSizerFlags const& flags, std::initializer_list<T> args, Function&& function)
{
    return VSizer {
        detail::ForEach(flags, std::move(args), std::forward<Function>(function))
    };
}

template <std::ranges::input_range Range, typename Function>
auto VForEach(wxSizerFlags const& flags, Range&& args, Function&& function)
{
    return VSizer {
        detail::ForEach(flags, std::forward<Range>(args), std::forward<Function>(function))
    };
}

template <typename T, typename Function>
auto HForEach(std::initializer_list<T> args, Function&& function)
{
    return HSizer {
        detail::ForEach(std::move(args), std::forward<Function>(function))
    };
}

template <std::ranges::input_range Range, typename Function>
auto HForEach(Range&& args, Function&& function)
{
    return HSizer {
        detail::ForEach(std::forward<Range>(args), std::forward<Function>(function))
    };
}

template <typename T, typename Function>
auto HForEach(wxSizerFlags const& flags, std::initializer_list<T> args, Function&& function)
{
    return HSizer {
        detail::ForEach(flags, std::move(args), std::forward<Function>(function))
    };
}

template <std::ranges::input_range Range, typename Function>
auto HForEach(wxSizerFlags const& flags, Range&& args, Function&& function)
{
    return HSizer {
        detail::ForEach(flags, std::forward<Range>(args), std::forward<Function>(function))
    };
}
}

#include <wxUI/detail/ZapMacros.hpp>
