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

#include <tuple>
#include <type_traits>

namespace wxUI {
// Tag struct for explicit wxString constructor usage
// Users must write: Text(wxUI_String{}, myWxString)
// This makes UTF-8 handling explicit and avoids ambiguity with string literals
struct wxUI_String { };
}

namespace wxUI::details {

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

template <class>
inline constexpr bool always_false_v = false;

}
