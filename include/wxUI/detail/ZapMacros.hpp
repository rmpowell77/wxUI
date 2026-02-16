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
// #pragma once

// This header is library details only header, and should not be relevant to users of the library.
// Libary Implementer Notes:
// Put all the Macros in HelperMacros.h
// Put all the undefs in ZapMacros.h
// ZapMacros.h should be the last line included in all Header files.
// Due to the way we are using these header files, they should *not* use #pragma once

#if defined(WXUI_RULE_OF_SIX_BOILERPLATE)
#undef WXUI_RULE_OF_SIX_BOILERPLATE
#endif

#if defined(WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE)
#undef WXUI_WIDGET_STATIC_ASSERT_BOILERPLATE
#endif

#if defined(WXUI_WIDGET_CREATE_BOILERPLATE)
#undef WXUI_WIDGET_CREATE_BOILERPLATE
#endif

#if defined(WXUI_FORWARD_TO_DETAILS)
#undef WXUI_FORWARD_TO_DETAILS
#endif

#if defined(WXUI_FORWARD_BIND_TO_DETAILS)
#undef WXUI_FORWARD_BIND_TO_DETAILS
#endif

#if defined(WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS)
#undef WXUI_FORWARD_TEMPLATEPTR_TO_DETAILS
#endif

#if defined(WXUI_FORWARD_ALL_TO_DETAILS)
#undef WXUI_FORWARD_ALL_TO_DETAILS
#endif
