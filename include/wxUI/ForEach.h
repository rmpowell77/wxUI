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

#include "Layout.h"

namespace wxUI::details {
// clang-format off
// Big help to Dennis Kormalev (https://www.linkedin.com/in/dkormalev/) for the example at:
// https://godbolt.org/z/sv5seP79q
template <typename, typename, typename = void>
struct CanApply : std::false_type { };

template <typename F, typename... Arg>
struct CanApply<F, std::tuple<Arg...>, std::enable_if_t<std::is_invocable_v<F, Arg...>, void>> : std::true_type { };

template <typename F, typename Arg, typename = void>
struct invoke_apply_result : std::invoke_result<F, Arg> { };

template <typename F, typename... Args1>
struct invoke_apply_result<F, std::tuple<Args1...>, std::enable_if_t<std::is_invocable_v<F, Args1...>, void>> : std::invoke_result<F, Args1...> { };

template <typename F, typename... Args>
using invoke_apply_result_t = typename invoke_apply_result<F, Args...>::type;

template <typename F, typename Arg> 
concept ForEachFunction = CreateAndAddable<typename invoke_apply_result<F, Arg>::type>;
// clang-format on

}

namespace wxUI {

// clang-format off
template <std::ranges::input_range Range, typename Function>
requires(details::ForEachFunction<Function, std::ranges::range_value_t<Range>>)
struct ForEach {
    // clang-format on

    ForEach(Range&& args, Function&& createFunction)
        : args(std::forward<Range>(args))
        , createFunction(std::forward<Function>(createFunction))
    {
    }

    ForEach(wxSizerFlags const& flags, Range&& args, Function&& createFunction)
        : flags(flags)
        , args(std::forward<Range>(args))
        , createFunction(std::forward<Function>(createFunction))
    {
    }

    void createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags)
    {
        using RawArg = std::remove_cvref_t<std::ranges::range_value_t<Range>>;
        for (auto&& item : args) {
            if constexpr (details::CanApply<Function, RawArg>::value) {
                std::apply(createFunction, item).createAndAdd(parent, parentSizer, flags ? *flags : parentFlags);
            } else {
                createFunction(item).createAndAdd(parent, parentSizer, flags ? *flags : parentFlags);
            }
        }
    }

private:
    std::optional<wxSizerFlags> flags {};
    Range args;
    Function createFunction;
};

template <std::ranges::input_range Range, typename Function>
ForEach(Range&&, Function&&) -> ForEach<Range, Function>;

template <std::ranges::input_range Range, typename Function>
ForEach(wxSizerFlags const& flags, Range&&, Function&&) -> ForEach<Range, Function>;

// initializer_list is like a string_view, we want a copy.  So deduce as vector so we have a copy
template <typename Function, typename T>
ForEach(std::initializer_list<T>, Function&&) -> ForEach<std::vector<T>, Function>;

template <typename Function, typename T>
ForEach(wxSizerFlags const& flags, std::initializer_list<T>, Function&&) -> ForEach<std::vector<T>, Function>;

template <typename T, typename Function>
auto VForEach(std::initializer_list<T> args, Function&& function)
{
    return VSizer {
        ForEach { std::move(args), std::forward<Function>(function) }
    };
}

template <std::ranges::input_range Range, typename Function>
auto VForEach(Range&& args, Function&& function)
{
    return VSizer {
        ForEach { std::forward<Range>(args), std::forward<Function>(function) }
    };
}

template <typename T, typename Function>
auto VForEach(wxSizerFlags const& flags, std::initializer_list<T> args, Function&& function)
{
    return VSizer {
        ForEach { flags, std::move(args), std::forward<Function>(function) }
    };
}

template <std::ranges::input_range Range, typename Function>
auto VForEach(wxSizerFlags const& flags, Range&& args, Function&& function)
{
    return VSizer {
        ForEach { flags, std::forward<Range>(args), std::forward<Function>(function) }
    };
}

template <typename T, typename Function>
auto HForEach(std::initializer_list<T> args, Function&& function)
{
    return HSizer {
        ForEach { std::move(args), std::forward<Function>(function) }
    };
}

template <std::ranges::input_range Range, typename Function>
auto HForEach(Range&& args, Function&& function)
{
    return HSizer {
        ForEach { std::forward<Range>(args), std::forward<Function>(function) }
    };
}

template <typename T, typename Function>
auto HForEach(wxSizerFlags const& flags, std::initializer_list<T> args, Function&& function)
{
    return HSizer {
        ForEach { flags, std::move(args), std::forward<Function>(function) }
    };
}

template <std::ranges::input_range Range, typename Function>
auto HForEach(wxSizerFlags const& flags, Range&& args, Function&& function)
{
    return HSizer {
        ForEach { flags, std::forward<Range>(args), std::forward<Function>(function) }
    };
}
}

#include "ZapMacros.h"
