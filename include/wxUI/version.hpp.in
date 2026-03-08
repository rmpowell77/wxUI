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

// wxUI version information

// clang-format off
#define WXUI_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define WXUI_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define WXUI_VERSION_PATCH @PROJECT_VERSION_PATCH@
#define WXUI_VERSION_STRING "@PROJECT_VERSION@"
// clang-format on

namespace wxUI {

/// Version information for the wxUI library
inline constexpr struct version_info {
    int major = WXUI_VERSION_MAJOR;
    int minor = WXUI_VERSION_MINOR;
    int patch = WXUI_VERSION_PATCH;
    const char* string = WXUI_VERSION_STRING;

    /// Returns the version as a comparable integer (major * 10000 + minor * 100 + patch)
    constexpr int as_number() const
    {
        return major * 10000 + minor * 100 + patch;
    }
} version {};

}
