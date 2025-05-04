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

// This header is library details only header, and should not be relevant to users of the library.
// Libary Implementer Notes:
// Put all the Macros in HelperMacros.h
// Put all the undefs in ZapMacros.h
// ZapMacros.h should be the last line included in all Header files.
// Due to the way we are using these header files, they should *not* use #pragma once

// clang-format off

#if !defined(RULE_OF_SIX_BOILERPLATE)
#define RULE_OF_SIX_BOILERPLATE(WIDGET)               \
    virtual ~WIDGET() = default;                      \
    WIDGET(WIDGET const&) = default;                  \
    WIDGET(WIDGET&&) noexcept = default;              \
    auto operator=(WIDGET const&)->WIDGET& = default; \
    auto operator=(WIDGET&&) noexcept -> WIDGET& = default;
#endif

// clang-format on

#if !defined(PROXY_BOILERPLATE)
#define PROXY_BOILERPLATE()                                                           \
    template <typename W>                                                             \
    auto operator=(W&& controller)->W&& { return bind(std::forward<W>(controller)); } \
    template <typename W>                                                             \
    auto bind(W&& widget)->W&&                                                        \
    {                                                                                 \
        widget.setProxyHandle(this);                                                  \
        return std::forward<W>(widget);                                               \
    }
#endif

#if !defined(WIDGET_STATIC_ASSERT_BOILERPLATE)
#define WIDGET_STATIC_ASSERT_BOILERPLATE(WIDGET)                 \
    static_assert(details::CreateAndAddable<WIDGET>);            \
    static_assert(details::Createable<WIDGET>);                  \
    static_assert(std::is_nothrow_move_constructible_v<WIDGET>); \
    static_assert(std::is_nothrow_move_assignable_v<WIDGET>)
#endif
