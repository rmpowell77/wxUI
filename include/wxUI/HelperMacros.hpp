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

#if !defined(WXUI_RULE_OF_SIX_BOILERPLATE)
#define WXUI_RULE_OF_SIX_BOILERPLATE(WIDGET)               \
    WIDGET(WIDGET const&) = default;                  \
    WIDGET(WIDGET&&) noexcept = default;              \
    auto operator=(WIDGET const&)->WIDGET& = default; \
    auto operator=(WIDGET&&) noexcept -> WIDGET& = default;
#endif

#if !defined(WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE)
#define WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE(WIDGET)                 \
    static_assert(details::CreateAndAddable<WIDGET>);            \
    static_assert(details::Createable<WIDGET>);                  \
    static_assert(std::is_nothrow_move_constructible_v<WIDGET>); \
    static_assert(std::is_nothrow_move_assignable_v<WIDGET>)
#endif

#if !defined(WXUI_WIDGET_CREATE_BOILERPLATE)
#define WXUI_WIDGET_CREATE_BOILERPLATE                                                                         \
    auto create(wxWindow* parent) -> underlying_t*                                                        \
    {                                                                                                     \
        return details_.create(createImpl(), parent);                                                     \
    }                                                                                                     \
    auto createAndAdd(wxWindow* parent, wxSizer* sizer, wxSizerFlags const& parentFlags) -> underlying_t* \
    {                                                                                                     \
        return details_.createAndAdd(createImpl(), parent, sizer, parentFlags);                           \
    }
#endif

// Helper macro for forwarding simple fluent builder calls from a controller
// to its `details_` member. Usage from inside a controller `C`:
//   WXUI_FORWARD_TO_DETAILS(C, methodName, ArgType, argName)
// This generates both & and && overloads that call `details_.methodName(...)`
// and return the appropriate `C&` or `C&&`.
#if !defined(WXUI_FORWARD_TO_DETAILS)
#define WXUI_FORWARD_TO_DETAILS(ControllerType, MethodName, ArgType, ArgName) \
    auto MethodName(ArgType ArgName)& -> ControllerType&                      \
    {                                                                         \
        details_.MethodName(ArgName);                                         \
        return *this;                                                         \
    }                                                                         \
    auto MethodName(ArgType ArgName)&& -> ControllerType&&                    \
    {                                                                         \
        details_.MethodName(ArgName);                                         \
        return std::move(*this);                                              \
    }
#endif

// Forward the two-argument bind(event, handler) templates to details_.
#if !defined(WXUI_FORWARD_BIND_TO_DETAILS)
#define WXUI_FORWARD_BIND_TO_DETAILS(ControllerType)                            \
    template <typename Event, typename Function>                                \
    auto bind(wxEventTypeTag<Event> event, Function func)& -> ControllerType&   \
    {                                                                           \
        details_.bind(event, func);                                             \
        return *this;                                                           \
    }                                                                           \
    template <typename Event, typename Function>                                \
    auto bind(wxEventTypeTag<Event> event, Function func)&& -> ControllerType&& \
    {                                                                           \
        details_.bind(event, func);                                             \
        return std::move(*this);                                                \
    }
#endif

#if !defined(WXUI_FORWARD_ALL_TO_DETAILS)
#define WXUI_FORWARD_ALL_TO_DETAILS(ControllerType)                                                      \
    WXUI_RULE_OF_SIX_BOILERPLATE(ControllerType)                                                         \
    WXUI_WIDGET_CREATE_BOILERPLATE                                                                       \
    WXUI_FORWARD_BIND_TO_DETAILS(ControllerType)                                                         \
    WXUI_FORWARD_TO_DETAILS(ControllerType, withFlags, wxSizerFlags, flags)                              \
    WXUI_FORWARD_TO_DETAILS(ControllerType, withPosition, wxPoint, pos)                                  \
    WXUI_FORWARD_TO_DETAILS(ControllerType, withSize, wxSize, size)                                      \
    WXUI_FORWARD_TO_DETAILS(ControllerType, withWidth, int, size)                                        \
    WXUI_FORWARD_TO_DETAILS(ControllerType, withHeight, int, size)                                       \
    WXUI_FORWARD_TO_DETAILS(ControllerType, setStyle, int64_t, style)                                    \
    WXUI_FORWARD_TO_DETAILS(ControllerType, withStyle, int64_t, style)                                   \
    WXUI_FORWARD_TO_DETAILS(ControllerType, withoutStyle, int64_t, style)                                \
    WXUI_FORWARD_TO_DETAILS(ControllerType, withFont, wxFontInfo const&, fontInfo)                       \
    WXUI_FORWARD_TO_DETAILS(ControllerType, setEnabled, bool, enabled)                                   \
    WXUI_FORWARD_TO_DETAILS(ControllerType, withProxy, details::WidgetProxy<underlying_t> const&, proxy) \
    [[nodiscard]] auto getIdentity() const { return details_.getIdentity(); }                            \
    [[nodiscard]] auto getPos() const { return details_.getPos(); }                                      \
    [[nodiscard]] auto getSize() const { return details_.getSize(); }                                    \
    [[nodiscard]] auto getStyle() const { return details_.getStyle(); }
#endif

// clang-format on
