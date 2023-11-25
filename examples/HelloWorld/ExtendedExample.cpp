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
#include <wx/artprov.h>
#include <wxUI/wxUI.h>

ExtendedExample::ExtendedExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExtendedExample",
        wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    ComboBox::Proxy proxy2;
    VSizer {
        wxSizerFlags().Expand().Border(),
        // incr example
        HSizer {
            spinProxy.bind(SpinCtrl { std::pair { 1, 3 } }),
        },
        HSizer {
            Button { "Incr" }
                .bind([this]() {
                    *spinProxy = 1 + *spinProxy;
                }),
        },
        HSizer {
            BitmapButton { wxBitmap {} },
        },
        HSizer {
            BitmapComboBox { std::tuple { "", wxBitmap {} } },
        },
        HSizer {
            BitmapToggleButton { wxBitmap {} },
        },
        // getHandle example
        HSizer {
            textProxy = Text { "Hello" },
        },
        HSizer {
            Button { "ReduceText" }
                .bind([this]() {
                    auto str = static_cast<std::string>(*textProxy);
                    if (str.size()) {
                        str.pop_back();
                    }
                    *textProxy = str;
                }),
        },
        HSizer {
            Button {},
        },
        HSizer {
            CheckBox {},
        },
        HSizer {
            proxy2 = ComboBox { { "hello" } },
        },
        HSizer {
            Line {},
        },
        HSizer {
            ListBox {},
        },
        HSizer {
            RadioBox { RadioBox::withChoices {}, "hello" },
        },
        HSizer {
            Slider {},
        },
        HSizer {
            SpinCtrl {},
        },
        HSizer {
            Text {},
        },
        HSizer {
            TextCtrl {},
        },
        // bind examples
        HSizer {
            TextCtrl { "Hello" }
                .bind([] {}),
        },
        VSizer {
            wxUI::Choice { std::vector<wxString> { wxT("Numbers"), wxT("Letters") } }
                .bind([](auto& e) { (void)e; }),
        },
        // snippet CustomExample
        HSizer {
            Custom {
                [](wxWindow* window, wxSizer* sizer, wxSizerFlags flags) {
                    for (auto&& title : { "1", "2", "3" }) {
                        Button { title }.createAndAdd(window, sizer, flags);
                    }
                },
            },
        },
        CreateStdDialogButtonSizer(wxOK),
        // endsnippet CustomExample
    }
        .attachTo(this);
    proxy2->SetFocus();
}

MultibindExample::MultibindExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "MultibindExample", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    VSizer {
        wxSizerFlags().Expand().Border(),
        HSizer {
            TextCtrl {}
                .withWidthSize(200)
                .withStyle(wxTE_PROCESS_ENTER)
                .bind([this]() {
                    *timesTyped = std::to_string(stoi(*timesTyped) + 1);
                })
                .bind(wxEVT_TEXT_ENTER, [this]() {
                    *timesEntered = std::to_string(stoi(*timesEntered) + 1);
                }),
        },
        HSizer {
            timesTyped = Text { "0" },
            timesEntered = Text { "0" },
        },
        CreateStdDialogButtonSizer(wxOK),
    }
        .attachTo(this);
}

SplitterExample::SplitterExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "SplitterExample", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    Generic<wxButton>::Proxy proxy {};
    // snippet SplitterExample
    VSizer {
        wxSizerFlags().Expand().Border(),
        VSplitter {
            TextCtrl { "This is Left Side.\n" }
                .withStyle(wxTE_MULTILINE)
                .withSize(wxSize(200, 100)),
            HSplitter {
                rightUpper = TextCtrl { "This is Right Top.\n" }
                                 .withStyle(wxTE_MULTILINE)
                                 .withSize(wxSize(200, 100)),
                Button { "Incr" }
                    .bind([this]() {
                        auto original = std::string { *rightUpper } + "\nThis is Right Top.\n";
                        *rightUpper = original;
                    }),
            } },
        // endsnippet SplitterExample
        VSplitter {
            TextCtrl {}.withStyle(wxTE_MULTILINE | wxHSCROLL | wxTE_PROCESS_TAB),
            Generic {
                [](wxWindow* parent) {
                    return new wxButton(parent, wxID_ANY, "Raw button");
                } },
        },
        VSplitter {
            proxy = [](wxWindow* parent) {
                return new wxButton(parent, wxID_ANY, "Raw button");
            },
            TextCtrl {}.withStyle(wxTE_MULTILINE | wxHSCROLL | wxTE_PROCESS_TAB),
        },
        // snippet SplitterExample
        CreateStdDialogButtonSizer(wxOK),
    }
        .attachTo(this);
    // endsnippet SplitterExample
    *rightUpper = std::string { proxy->GetLabel() };
}

GenericExample::GenericExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "SplitterExample", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    Generic<wxButton>::Proxy proxy1 {};
    Generic<wxButton>::Proxy proxy2 {};
    // snippet GenericExample
    VSizer {
        wxSizerFlags().Expand().Border(),
        Generic {
            [](wxWindow* window) {
                return new wxButton(window, wxID_ANY, "Generic");
            } },
        // endsnippet GenericExample
        proxy1 = Generic<wxButton> {
            [](wxWindow* window) {
                return new wxButton(window, wxID_ANY, "Raw 1");
            } },
        proxy2 = [](wxWindow* window) {
            return new wxButton(window, wxID_ANY, "Raw 2");
        },
        // snippet GenericExample
        CreateStdDialogButtonSizer(wxOK),
    }
        .attachTo(this);
    // endsnippet GenericExample
    assert(proxy1->GetLabel() == "Raw 1");
    assert(proxy2->GetLabel() == "Raw 2");
}

ForEachExample::ForEachExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ForEach Example", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    using namespace std::literals;
    VSizer {
        wxSizerFlags {}.Border(wxALL, 2),
        HSizer {
            ForEach {
                std::vector<std::string> { "A", "B", "C" },
                [](auto name) {
                    return wxUI::Button { name };
                } },
        },
        HForEach(
            std::vector<std::string> { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            }),
        HForEach(
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            }),
        HForEach(
            { "A"s, "B"s, "C"s },
            [](auto name) {
                return wxUI::Button { name };
            }),
        HForEach(
            { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            }),
        HForEach(
            std::vector { "A", "B", "C" },
            [](auto name) {
                return wxUI::Button { name };
            }),
        HForEach(
            std::vector<wxArtID> { wxART_PLUS, wxART_MINUS, wxART_FIND },
            [](auto identity) {
                return wxUI::BitmapButton { wxArtProvider::GetBitmap(identity) };
            }),
        HForEach(
            std::vector<wxArtID> { wxART_PLUS, wxART_MINUS, wxART_FIND },
            [](auto identity) {
                return wxUI::BitmapButton { wxArtProvider::GetBitmap(identity) };
            }),
        HForEach(
            std::vector { wxART_PLUS, wxART_MINUS, wxART_FIND },
            [](auto identity) {
                return wxUI::BitmapButton { wxArtProvider::GetBitmap(identity) };
            }),
        // snippet ForEachExample
        HSizer {
            ForEach {
                { wxART_PLUS, wxART_MINUS, wxART_FIND },
                [](auto identity) {
                    return wxUI::BitmapButton { wxArtProvider::GetBitmap(identity) };
                } },
        },
        // endsnippet ForEachExample
        // snippet HForEach
        HForEach(
            std::vector { wxART_PLUS, wxART_MINUS, wxART_FIND },
            [](auto identity) {
                return wxUI::BitmapButton { wxArtProvider::GetBitmap(identity) };
            }),
        // endsnippet HForEach
        HForEach(
            std::vector { "Long string", "Ball", "S", "Tools" } | std::views::filter([](auto s) { return std::string(s).size() < 5; }),
            [](auto name) {
                return wxUI::Button { name };
            }),
        HForEach(
            std::vector { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" } },
            [](auto identityAndName) {
                return wxUI::Button { std::get<0>(identityAndName), std::get<1>(identityAndName) };
            }),
        HForEach(
            std::vector { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }),
        HForEach(
            { std::tuple<wxWindowID, std::string> { wxID_CANCEL, "A" }, std::tuple<wxWindowID, std::string> { wxID_OK, "B" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }),
        HForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" } },
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }),
        // snippet ComplicatedForEachExample
        HForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" } } | std::views::filter([](auto s) { return std::get<1>(s) == "B"; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }),
        // endsnippet ComplicatedForEachExample
        HForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" } } | std::views::filter([](auto s) { return std::get<1>(s) == "B"; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }),
        HForEach(
            std::vector<std::tuple<wxWindowID, std::string>> { { wxID_CANCEL, "A" }, { wxID_OK, "B" } } | std::views::filter([](auto s) { return std::get<1>(s) == "B"; }),
            [](auto identity, auto name) {
                return wxUI::Button { identity, name };
            }),
        CreateStdDialogButtonSizer(wxOK),
    }
        .attachTo(this);
}
