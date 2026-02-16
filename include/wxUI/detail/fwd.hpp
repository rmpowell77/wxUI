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

// Forward declarations for commonly-used wxUI types
// This header can be included to avoid circular dependencies

namespace wxUI {

// Tag struct for explicit wxString constructor usage
struct wxUI_String;

// Common template types
template <typename Window>
struct Wrapper;

template <typename... Items>
struct VSizer;

template <typename... Items>
struct HSizer;

template <typename... Items>
struct FlexGridSizer;

template <typename... Items>
struct WrapSizer;

template <typename... Items>
struct LayoutIf;

template <typename... Items>
struct ForEach;

}

namespace wxUI::details {

// Proxy template for accessing underlying wxWidgets controls
template <typename T>
struct Proxy;

// Type traits
template <typename, typename, typename = void>
struct CanApply;

template <typename F, typename Arg, typename = void>
struct invoke_apply_result;

template <typename F, typename... Args>
using invoke_apply_result_t = typename invoke_apply_result<F, Args...>::type;

}
