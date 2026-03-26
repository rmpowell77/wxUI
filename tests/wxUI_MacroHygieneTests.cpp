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

// This test verifies that wxUI headers properly "zap" (undefine) internal helper
// macros and don't leak them into user code. This is critical for macro hygiene.

#include <wxUI/wxUI.hpp>

// After including wxUI headers, all internal macros should be undefined.
// If any of these static_asserts fail, it means a header forgot to include
// ZapMacros.hpp at the end.

#if defined(WXUI_RULE_OF_SIX_BOILERPLATE)
#error "WXUI_RULE_OF_SIX_BOILERPLATE leaked from wxUI headers - missing ZapMacros.hpp include"
#endif

#if defined(WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE)
#error "WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE leaked from wxUI headers - missing ZapMacros.hpp include"
#endif

#if defined(WXUI_WIDGET_CREATE_BOILERPLATE)
#error "WXUI_WIDGET_CREATE_BOILERPLATE leaked from wxUI headers - missing ZapMacros.hpp include"
#endif

#if defined(WXUI_FORWARD_TO_DETAILS)
#error "WXUI_FORWARD_TO_DETAILS leaked from wxUI headers - missing ZapMacros.hpp include"
#endif

#if defined(WXUI_FORWARD_BIND_TO_DETAILS)
#error "WXUI_FORWARD_BIND_TO_DETAILS leaked from wxUI headers - missing ZapMacros.hpp include"
#endif

#if defined(WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS)
#error "WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS leaked from wxUI headers - missing ZapMacros.hpp include"
#endif

#if defined(WXUI_FORWARD_ALL_TO_DETAILS)
#error "WXUI_FORWARD_ALL_TO_DETAILS leaked from wxUI headers - missing ZapMacros.hpp include"
#endif

// Compile-time test - no runtime assertions needed.
// The presence of this file in the build verifies macro hygiene.
