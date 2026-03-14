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

// This is an auto-generated file. Run checklists/do_buildtests to regenerate.

// This header verifies that all wxUI helper macros have been properly "zapped"
// (undefined) after including a wxUI header. Include this file after including
// any wxUI header to ensure the header is self-contained and doesn't leak macros.

#if defined(WXUI_RULE_OF_SIX_BOILERPLATE)
#error Missed zapping WXUI_RULE_OF_SIX_BOILERPLATE
#endif

#if defined(WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE)
#error Missed zapping WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE
#endif

#if defined(WXUI_WIDGET_CREATE_BOILERPLATE)
#error Missed zapping WXUI_WIDGET_CREATE_BOILERPLATE
#endif

#if defined(WXUI_FORWARD_TO_DETAILS)
#error Missed zapping WXUI_FORWARD_TO_DETAILS
#endif

#if defined(WXUI_FORWARD_BIND_TO_DETAILS)
#error Missed zapping WXUI_FORWARD_BIND_TO_DETAILS
#endif

#if defined(WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS)
#error Missed zapping WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS
#endif

#if defined(WXUI_FORWARD_ALL_TO_DETAILS)
#error Missed zapping WXUI_FORWARD_ALL_TO_DETAILS
#endif
