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
// wxUI "Hello Widgets" example

#include "ExtendedExample.hpp"
#include <wx/artprov.h>
#include <wxUI/wxUI.hpp>

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
            SpinCtrl { std::pair { 1, 3 } }.withProxy(spinProxy),
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
            Text { "Hello" }.withProxy(textProxy),
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
            ComboBox { "hello" }.withProxy(proxy2),
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
        .fitTo(this);
    proxy2->SetFocus();
}

MultibindExample::MultibindExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "MultibindExample", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    wxUI::Text::Proxy timesTyped;
    wxUI::Text::Proxy timesEntered;
    using namespace wxUI;
    VSizer {
        wxSizerFlags().Expand().Border(),
        HSizer {
            TextCtrl {}
                .withWidth(200)
                .withStyle(wxTE_PROCESS_ENTER)
                .bind([timesTyped]() {
                    *timesTyped = std::to_string(stoi(*timesTyped) + 1);
                })
                .bind(wxEVT_TEXT_ENTER, [timesEntered]() {
                    *timesEntered = std::to_string(stoi(*timesEntered) + 1);
                }),
        },
        HSizer {
            Text { "0" }.withProxy(timesTyped),
            Text { "0" }.withProxy(timesEntered),
        },
        CreateStdDialogButtonSizer(wxOK),
    }
        .fitTo(this);
}

SplitterExample::SplitterExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "SplitterExample", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    Factory<wxButton>::Proxy proxy {};
    // snippet SplitterExample
    VSizer {
        wxSizerFlags().Expand().Border(),
        VSplitter {
            TextCtrl { "This is Left Side.\n" }
                .withStyle(wxTE_MULTILINE)
                .withSize(wxSize(200, 100)),
            HSplitter {
                TextCtrl { "This is Right Top.\n" }.withProxy(rightUpper).withStyle(wxTE_MULTILINE).withSize(wxSize(200, 100)),
                Button { "Incr" }
                    .bind([this]() {
                        auto original = std::string { *rightUpper } + "\nThis is Right Top.\n";
                        *rightUpper = original;
                    }),
            } },
        // endsnippet SplitterExample
        VSplitter {
            TextCtrl {}.withStyle(wxTE_MULTILINE | wxHSCROLL | wxTE_PROCESS_TAB),
            Factory {
                [](wxWindow* parent) {
                    return new wxButton(parent, wxID_ANY, "Raw button");
                } },
        },
#if 0
        // snippet SplitterCompileFail
        // This is not supported because the splitter needs to have the controls created with wxSplitterWindow as parent.
        VSplitter {
            TextCtrl {}.withStyle(wxTE_MULTILINE | wxHSCROLL | wxTE_PROCESS_TAB),
            Wrapper {
                [parent = this] {
                    return new wxButton(parent, wxID_ANY, "Raw button");
                }() },
        },
        // endsnippet SplitterCompileFail
#endif
        VSplitter {
            Factory<wxButton> { [](wxWindow* parent) {
                return new wxButton(parent, wxID_ANY, "Raw button");
            } }
                .withProxy(proxy),
            TextCtrl {}.withStyle(wxTE_MULTILINE | wxHSCROLL | wxTE_PROCESS_TAB),
        },
        // snippet SplitterExample
        CreateStdDialogButtonSizer(wxOK),
    }
        .fitTo(this);
    // endsnippet SplitterExample
    *rightUpper = std::string { proxy->GetLabel() };
}

WrapperExample::WrapperExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "WrapperExample", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    Wrapper<wxButton>::Proxy proxy {};
    // snippet WrapperExample
    VSizer {
        wxSizerFlags().Expand().Border(),
        Wrapper {
            [window = this] {
                return new wxButton(window, wxID_ANY, "Wrapper");
            }() },
        // endsnippet WrapperExample
        Wrapper<wxButton> {
            [window = this] {
                return new wxButton(window, wxID_ANY, "Proxy");
            }() }
            .withProxy(proxy),
        // snippet WrapperExample
        CreateStdDialogButtonSizer(wxOK),
    }
        .fitTo(this);
    // endsnippet WrapperExample
    assert(proxy->GetLabel() == "Proxy");
}

FactoryExample::FactoryExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "FactoryExample", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    Factory<wxButton>::Proxy proxy {};
    // snippet FactoryExample
    VSizer {
        wxSizerFlags().Expand().Border(),
        Factory {
            [](wxWindow* window) {
                return new wxButton(window, wxID_ANY, "Factory");
            } },
        // endsnippet FactoryExample
        Factory<wxButton> {
            [](wxWindow* window) {
                return new wxButton(window, wxID_ANY, "Proxy");
            } }
            .withProxy(proxy),
        // snippet FactoryExample
        CreateStdDialogButtonSizer(wxOK),
    }
        .fitTo(this);
    // endsnippet FactoryExample
    assert(proxy->GetLabel() == "Proxy");
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
        .fitTo(this);
}

namespace {
auto CreateSelect(std::string title, int64_t withStyle)
{
    using namespace wxUI;
    ListBox::Proxy listProxy {};
    Text::Proxy textProxy {};
    auto redoText = [listProxy, textProxy]() {
        std::string selectedStrings;
        for (auto&& item : listProxy.selections().get()) {
            selectedStrings += listProxy->GetString(item) + ", ";
        }
        *textProxy = selectedStrings;
    };
    return HSizer {
        VSizer {
            Text { title },
            ListBox { { "A", "B", "C" } }
                .withStyle(withStyle)
                .withProxy(listProxy)
                .bind(redoText)
                .bindDClick([listProxy, textProxy, redoText]() {
                    std::vector<int> selectedItems = { 0, 1, 2 };
                    listProxy.selections() = selectedItems;
                    redoText();
                }),
            HSizer {
                Text { "Selected: " },
                Text {}.withProxy(textProxy),
            },
        },
        Button { "Add A" }
            .bind([listProxy, redoText]() {
                auto selected = listProxy.selections().get();
                selected.push_back(0);
                listProxy.selections() = selected;
                redoText();
            }),
    };
}
} // namespace

ListExample::ListExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "List Example", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    ListBox::Proxy listProxy {};
    Text::Proxy textProxy {};
    ListBox::Proxy listProxy2 {};
    Text::Proxy textProxy2 {};
    VSizer {
        wxSizerFlags {}.Border(wxALL, 2),
        CreateSelect("Single select", 0),
        CreateSelect("Multi select", wxLB_EXTENDED),

        CreateStdDialogButtonSizer(wxOK),
    }
        .fitTo(this);
}

auto MakeLayout(bool layout)
{
    using namespace wxUI;
    SpinCtrl::Proxy spinProxy;
    return VSizer {
        LayoutIf {
            layout,
            HSizer {
                SpinCtrl { std::pair { 1, 3 } }.withProxy(spinProxy),
            },
            HSizer {
                Button { "Incr" }
                    .bind([spinProxy]() {
                        *spinProxy = 1 + *spinProxy;
                    }),
            },
        },
        // snippet LayoutIfExample
        HSizer {
            LayoutIf { layout, BitmapButton { wxArtProvider::GetBitmap(wxART_PLUS) } },
            BitmapButton { wxArtProvider::GetBitmap(wxART_MINUS) },
        },
        // endsnippet LayoutIfExample
    };
}

LayoutIfExample::LayoutIfExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "LayoutIf",
          wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    VSizer {
        wxSizerFlags().Expand().Border(),
        VSizer { "Layout Enabled", MakeLayout(true) },
        VSizer { "Layout Disabled", MakeLayout(false) },
    }
        .fitTo(this);
}

WrapSizerExample::WrapSizerExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "WrapSizer",
          wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    VSizer {
        wxSizerFlags().Expand().Border(),
        HWrapSizer {
            Button { "Button 1" },
            Text { "Dog" },
            SpinCtrl { std::pair { 1, 3 } },
        },
        VWrapSizer {
            Button { "Button 2" },
            Text { "Cat" },
            SpinCtrl { std::pair { 1, 3 } },
        },
    }
        .fitTo(this);
}

GridSizerExample::GridSizerExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "GridSizer",
          wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    GridSizer {
        2,
        wxSizerFlags().Expand().Border(),
        CheckBox { "Check" },
        CheckBox { "Check" },
        TextCtrl { "Text" }.withSize(wxSize(100, 100)),
        TextCtrl { "Text" },
    }
        .fitTo(this);
}

FlexGridSizerExample::FlexGridSizerExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "FlexGridSizer",
          wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    // snippet FlexGridSizerExample
    FlexGridSizer {
        2,
        wxSizerFlags().Expand().Border(),
        CheckBox { "Check" },
        CheckBox { "Check" },
        TextCtrl { "Text" }.withSize(wxSize(100, 100)),
        TextCtrl { "Text" },
    }
        .fitTo(this);
    // endsnippet FlexGridSizerExample
}

UnicodeExample::UnicodeExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "LayoutIf",
          wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    // snippet UnicodeExample
    VSizer {
        wxSizerFlags().Expand().Border(),
        // These are equivalent - both strings are UTF-8, and convert to wxString internally
        Text { "Hello 🐨" },
        Text { wxUI_String {}, wxString::FromUTF8("Hello 🐨") },
    }
        .fitTo(this);
    // endsnippet UnicodeExample
}

NotebookExample::NotebookExample(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "LayoutIf",
        wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    // snippet NotebookExample
    VSizer {
        wxSizerFlags().Expand().Border(),
        Notebook {
            BookItem { "Item 1", VSizer { Text { "Generic sizer" } } },
            BookItem { "Item 2", HSizer { Text { "Generic sizer" } } },
            LayoutIf {
                true,
                BookItem { "Optional Item", VSizer { Text { "Generic sizer" } } },
            },
        }
    }
        .fitTo(this);
    // endsnippet NotebookExample
}

std::vector<wxString> GenerateStringArray()
{
    return {
        wxT("This is a sample string item #1"),
        wxT("This is a sample string item #2"),
        wxT("This is a sample string item #3"),
        wxT("This is a sample string item #4"),
        wxT("This is a sample string item #5"),
        wxT("This is a sample string item #6"),
        wxT("This is a sample string item #7"),
        wxT("This is a sample string item #7"),
    };
}

ComboUpdate::ComboUpdate(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ComboUpdate",
          wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    VSizer {
        wxSizerFlags().Expand().Border(),
        HSizer {
            ComboBox { "hello" }
                .withStyle(wxTE_PROCESS_ENTER)
                .withProxy(proxy2),
        },
    }
        .fitTo(this);
    auto arr = GenerateStringArray();
    proxy2->Clear();
    proxy2->Append(arr);
}
