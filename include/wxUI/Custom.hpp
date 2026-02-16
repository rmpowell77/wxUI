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

#include "Widget.hpp"

namespace wxUI {

// A Custom Controller can only be created with something that supports the CreateAndAdd function.

// snippet requires
template <typename T>
concept CreateAndAddFunction = requires(T function, wxWindow* window, wxSizer* sizer) {
    function(window, sizer, wxSizerFlags {});
};
// endsnippet requires

template <CreateAndAddFunction Function>
struct Custom {
    Custom(wxSizerFlags const& flags, Function const& function)
        : flags_(flags)
        , function_(function)
    {
    }

    explicit Custom(Function const& function)
        : function_(function)
    {
    }

    void createAndAdd(wxWindow* parent, wxSizer* parentSizer, wxSizerFlags const& parentFlags) const
    {
        function_(parent, parentSizer, flags_.value_or(parentFlags));
    }

private:
    std::optional<wxSizerFlags> flags_;
    Function function_;
};

}

#include "ZapMacros.hpp"
