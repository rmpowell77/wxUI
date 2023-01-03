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
// wxUI "Hello World" example

#include "ExtendedExample.h"
#include <wxUI/wxUI.h>

ExtendedExample::ExtendedExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExtendedExample",
        wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    wxStaticText* text = nullptr;
    VStack {
        wxSizerFlags().Expand().Border(),
        HStack {
            BitmapButton { wxBitmap {} },
        },
        HStack {
            BitmapToggleButton { wxBitmap {} },
        },
        HStack {
            Button {},
        },
        HStack {
            CheckBox {},
        },
        HStack {
            RadioBox { std::vector<wxString> { "hello" } },
        },
        HStack {
            SpinCtrl {},
        },
        HStack {
            Text {},
        },
        HStack {
            TextCtrl {},
        },
        HStack {
            Text { "Hello" }
                .getHandle(&text),
        },
        // snippet CustomExample
        HStack {
            Custom { [](wxWindow* window, wxSizer* sizer, wxSizerFlags flags) {
                for (auto&& title : { "1", "2", "3" }) {
                    Button { title }.createAndAdd(window, sizer, flags);
                }
            } } },
        // endsnippet CustomExample
    }
        .attachTo(this);
    if (text->GetLabel() != "Hello") {
        throw std::runtime_error("Error, could not create reference");
    }
}
