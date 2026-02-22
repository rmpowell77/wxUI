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
// wxUI "Hello World" example
// This example is inspired from https://docs.wxwidgets.org/latest/overview_helloworld.html

#include "ExtendedExample.hpp"
// snippet Example headers to include
#include <numeric>
#include <wx/wx.h>
#include <wxUI/wxUI.hpp>
// endsnippet Example

class HelloWorldApp : public wxApp {
public:
    bool OnInit() override;
};

class HelloWorldFrame : public wxFrame {
public:
    HelloWorldFrame();
};

enum {
    ExampleDialogWidgetsASpinner = 1100,
    ExampleDialogWidgetsBSpinner,
    ExampleDialogWidgetsGCDResult,
};

class ExampleDialogWidgets : public wxDialog {
public:
    explicit ExampleDialogWidgets(wxWindow* parent);
};

// snippet Example
class ExampleDialog : public wxDialog {
public:
    explicit ExampleDialog(wxWindow* parent);
    wxUI::SpinCtrl::Proxy a, b;
    wxUI::Text::Proxy result;
};
// endsnippet Example

wxIMPLEMENT_APP(HelloWorldApp);

bool HelloWorldApp::OnInit()
{
    HelloWorldFrame* frame = new HelloWorldFrame();
    frame->Show(true);
    return true;
}

// snippet wxUIMenu
// snippet wxUIMenuProxy
HelloWorldFrame::HelloWorldFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxUI::MenuItemProxy proxyItem;
    // endsnippet wxUIMenuProxy
    wxUI::MenuBar {
        wxUI::Menu {
            "&File",
            // endsnippet wxUIMenu
            wxUI::Item { wxID_FIND, "&Find\tCTRL-F", "No-op Find" },
            wxUI::Item { "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item", [] {
                            wxLogMessage("Hello world from wxWidgets!");
                        } },
            wxUI::Separator {},
            wxUI::Item { "&Example...\tCtrl-D", [this] {
                            ExampleDialogWidgets dialog(this);
                            dialog.ShowModal();
                        } },
            // snippet wxUIMenu
            wxUI::Item { "&Example with wxUI...\tCtrl-F", [this] {
                            ExampleDialog dialog(this);
                            dialog.ShowModal();
                        } },
            wxUI::Separator {},
            wxUI::Item { wxID_EXIT, [this] {
                            Close(true);
                        } },
            // endsnippet wxUIMenu
        },
        // snippet wxUIMenuSubMenu
        wxUI::Menu {
            "&Extra",
            wxUI::Menu {
                "Pets",
                wxUI::CheckItem { "Cats", [](wxCommandEvent& event) {
                                     wxLogMessage("Cats %s checked", event.IsChecked() ? "are" : "are not");
                                 } },
                wxUI::CheckItem { "Dogs", [](wxCommandEvent& event) {
                                     wxLogMessage("Dogs %s checked", event.IsChecked() ? "are" : "are not");
                                 } },
            },
            // endsnippet wxUIMenuSubMenu
            wxUI::Menu {
                "Colors",
                wxUI::RadioItem { "Red", [](wxCommandEvent& event) {
                                     wxLogMessage("Red %s checked", event.IsChecked() ? "is" : "is not");
                                 } },
                wxUI::RadioItem { "Green", [](wxCommandEvent& event) {
                                     wxLogMessage("Green %s checked", event.IsChecked() ? "is" : "is not");
                                 } },
                wxUI::RadioItem { "Blue", [](wxCommandEvent& event) {
                                     wxLogMessage("Blue %s checked", event.IsChecked() ? "is" : "is not");
                                 } },
            },
            // snippet wxUIMenuProxy
            wxUI::Menu {
                "ProxyMenu",
                wxUI::Item { "Label Off", [] {
                            } }
                    .withProxy(proxyItem),
                wxUI::CheckItem { "Label Enable", [proxyItem](wxCommandEvent& event) {
                                     proxyItem->SetItemLabel(event.IsChecked() ? "Label On" : "Label Off");
                                 } },
            },
            // endsnippet wxUIMenuProxy
            // snippet wxUIMenuForEach
            wxUI::Menu {
                "Foreach",
                wxUI::MenuForEach { std::vector { "Red", "Green", "Blue" }, [](auto name) {
                                       return wxUI::RadioItem { name, [name](wxCommandEvent& event) {
                                                                   wxLogMessage("%s %s checked", name, event.IsChecked() ? "is" : "is not");
                                                               } };
                                   } },
            },
            // endsnippet wxUIMenuForEach
            // snippet wxUIMenuSubMenu
            // snippet wxUIMenuExample1
            wxUI::Separator {},
            wxUI::Item { "&ExtendedExample...", [this] {
                            ExtendedExample dialog(this);
                            dialog.ShowModal();
                        } },
            wxUI::Item { "&MultibindExample...", [this] {
                            MultibindExample { this }.ShowModal();
                        } },
            // endsnippet wxUIMenuExample1
            wxUI::Item { "&SplitterExample...", [this] {
                            SplitterExample { this }.ShowModal();
                        } },
            wxUI::Item { "&WrapperExample...", [this] {
                            WrapperExample { this }.ShowModal();
                        } },
            wxUI::Item { "&FactoryExample...", [this] {
                            FactoryExample { this }.ShowModal();
                        } },
            wxUI::Item { "&ForEachExample...", [this] {
                            ForEachExample { this }.ShowModal();
                        } },
            wxUI::Item { "&ListExample...", [this] {
                            ListExample(this).ShowModal();
                        } },
            wxUI::Item { "&LayoutIf...", [this] {
                            LayoutIfExample { this }.ShowModal();
                        } },
            wxUI::Item { "&WrapSizer...", [this] {
                            WrapSizerExample { this }.ShowModal();
                        } },
            wxUI::Item { "&GridSizer...", [this] {
                            GridSizerExample { this }.ShowModal();
                        } },
            wxUI::Item { "&FlexGridSizer...", [this] {
                            FlexGridSizerExample { this }.ShowModal();
                        } },
            wxUI::Item { "&UnicodeExample...", [this] {
                            UnicodeExample { this }.ShowModal();
                        } },
            wxUI::Item { "&ComboUpdate...", [this] {
                            ComboUpdate { this }.ShowModal();
                        } },
            // snippet wxUIMenuExample1
            wxUI::Item { "&Example Item...", [] {
                            wxLogMessage("Hello World!");
                        } },
            wxUI::CheckItem { "&Example Checked Item...", [](wxCommandEvent& event) {
                                 wxLogMessage(event.IsChecked() ? "is checked" : "is not checked");
                             } },
            // endsnippet wxUIMenuExample1
        },
        // endsnippet wxUIMenuSubMenu
        wxUI::Menu {
            "&Help",
            wxUI::Item { wxID_ABOUT, [] {
                            wxMessageBox(
                                wxString::Format("The wxUI Hello World example\n\nUsing wxUI version %s", wxUI::version.string),
                                "About Hello World",
                                wxOK | wxICON_INFORMATION);
                        } },

            // snippet wxUIMenu
        }
    }.fitTo(this);
    // endsnippet wxUIMenu

    CreateStatusBar();

    SetStatusText(wxString::Format("Welcome to wxUI! (v%s)", wxUI::version.string));
}

// snippet withwxWidgets : How you build with wxWidgets
ExampleDialogWidgets::ExampleDialogWidgets(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExampleDialogWidgets",
          wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    // Create the controls.
    auto* text = new wxStaticText(this, wxID_ANY, "Example of Text in wxWidgets");
    auto* textTitle = new wxTextCtrl(this, wxID_ANY, "Single line of text");
    // endsnippet withwxWidgets
    auto* textBody = new wxTextCtrl(this, wxID_ANY,
        "Several lines of text.\n"
        "With wxUI the code reflects\n"
        "what the UI looks like.",
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE);
    auto* btnLeft = new wxButton(this, wxID_ANY, "Left");
    auto* btnRight = new wxButton(this, wxID_ANY, "Right");

    auto* spinALabel = new wxStaticText(this, wxID_ANY, "A = ");
    auto* spinA = new wxSpinCtrl(this, ExampleDialogWidgetsASpinner, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 1);
    auto* spinBLabel = new wxStaticText(this, wxID_ANY, "B = ");
    auto* spinB = new wxSpinCtrl(this, ExampleDialogWidgetsBSpinner, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 1);
    auto* gcdLabel = new wxStaticText(this, wxID_ANY, "GCD = ");
    auto* gcdResult = new wxStaticText(this, ExampleDialogWidgetsGCDResult, "1");

    const wxString levels[] = {
        "&Information",
        "&Warning",
        "&Error",
        "&None",
        "&Custom"
    };
    auto logLevels = new wxRadioBox(this, wxID_ANY, "&Log Levels:",
        wxDefaultPosition, wxDefaultSize,
        WXSIZEOF(levels), levels,
        1, wxRA_SPECIFY_ROWS);
    logLevels->SetSelection(1);

    auto checkBox = new wxCheckBox(this, wxID_ANY, "Show");
    const wxString choices[] = {
        "Less",
        "More",
    };

    auto choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices);
    auto blankLine = new wxTextCtrl(this, wxID_ANY, "Fill in the blank");

    textBody->SetMinSize(wxSize(200, 100));

    // snippet withwxWidgets
    // Layout the controls.
    auto sizer = new wxBoxSizer(wxVERTICAL);

    auto sizerText = new wxStaticBoxSizer(wxVERTICAL, this, "Text Examples");
    sizerText->Add(text, wxSizerFlags().Expand().Border());
    sizerText->Add(textTitle, wxSizerFlags().Expand().Border());
    // endsnippet withwxWidgets
    sizerText->Add(textBody, wxSizerFlags(1).Expand().Border());
    sizer->Add(sizerText, wxSizerFlags(1).Expand().Border());

    auto* spinners = new wxBoxSizer(wxHORIZONTAL);
    spinners->Add(spinALabel, wxSizerFlags().Expand().Border());
    spinners->Add(spinA, wxSizerFlags().Expand().Border());
    spinners->Add(spinBLabel, wxSizerFlags().Expand().Border());
    spinners->Add(spinB, wxSizerFlags().Expand().Border());
    spinners->Add(gcdLabel, wxSizerFlags().Expand().Border());
    spinners->Add(gcdResult, wxSizerFlags().Expand().Border());
    sizer->Add(spinners, wxSizerFlags().Expand().Border());

    sizer->Add(logLevels, wxSizerFlags().Expand().Border());

    auto* sizerDetails = new wxStaticBoxSizer(wxHORIZONTAL, this, "Details");
    sizerDetails->Add(checkBox, wxSizerFlags().Border());
    sizerDetails->Add(choice, wxSizerFlags().Border());
    sizerDetails->Add(blankLine, wxSizerFlags(1).Expand().Border());
    sizer->Add(sizerDetails, wxSizerFlags().Expand().Border());

    auto* sizerBtns = new wxBoxSizer(wxHORIZONTAL);
    sizerBtns->Add(btnLeft, wxSizerFlags().Border(wxLEFT));
    sizerBtns->Add(btnRight, wxSizerFlags().Border(wxRIGHT));
    sizer->Add(sizerBtns, wxSizerFlags().Centre().Border());

    sizer->Add(new wxStaticLine(this, wxID_STATIC), 0, wxGROW | wxALL);

    sizer->Add(CreateStdDialogButtonSizer(wxOK), wxSizerFlags().Expand().Border());

    // snippet withwxWidgets
    SetSizerAndFit(sizer);
    // endsnippet withwxWidgets

    // And connect the event handlers.
    btnLeft->Bind(wxEVT_BUTTON, [](wxEvent&) { wxLogMessage("Pressed Left"); });
    btnRight->Bind(wxEVT_BUTTON, [](wxEvent&) { wxLogMessage("Pressed Right"); });

    auto action = [this](wxEvent&) {
        auto valueA = dynamic_cast<wxSpinCtrl*>(FindWindow(ExampleDialogWidgetsASpinner))->GetValue();
        auto valueB = dynamic_cast<wxSpinCtrl*>(FindWindow(ExampleDialogWidgetsBSpinner))->GetValue();
        dynamic_cast<wxStaticText*>(FindWindow(ExampleDialogWidgetsGCDResult))->SetLabel(std::to_string(std::gcd(valueA, valueB)));
    };
    spinA->Bind(wxEVT_SPINCTRL, action);
    spinB->Bind(wxEVT_SPINCTRL, action);

    // snippet withwxWidgets
}
// endsnippet withwxWidgets

// snippet Example Dialog example
// snippet withwxUI Dialog example
ExampleDialog::ExampleDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExampleDialog",
          wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;

    // snippet wxUILayoutBasic
    VSizer {
        wxSizerFlags().Expand().Border(),
        VSizer {
            "Text examples",
            // endsnippet wxUILayoutBasic
            Text { "Example of Text in wxUI" },
            TextCtrl { "Single line of text" }
                .withStyle(wxALIGN_LEFT),
            // endsnippet withwxUI
            TextCtrl {
                "Several lines of text.\n"
                "With wxUI the code reflects\n"
                "what the UI looks like." }
                .withStyle(wxTE_MULTILINE)
                .withSize(wxSize(200, 100))
            // snippet withwxUI
        },
        HSizer {
            Text { "A =" },
            SpinCtrl { std::pair { 1, 10000 } }.withProxy(a).bind([this]() {
                *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b)));
            }),

            Text { "B =" },
            SpinCtrl { std::pair { 1, 10000 } }.withProxy(b).bind([this]() { *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b))); }),

            Text { "GCD = " },
            Text { "1" }.withProxy(result),
        },
        // endsnippet withwxUI
        RadioBox { "&Log Levels:", RadioBox::withChoices {}, { "&Information", "&Warning", "&Error", "&None", "&Custom" } }
            .setStyle(wxRA_SPECIFY_ROWS)
            .withMajorDim(1)
            .withSelection(1),

        // snippet wxUIController
        HSizer {
            "Details",
            CheckBox { "Show" },
            Choice { "Less", "More" },
            TextCtrl { "Fill in the blank" }
                .withFlags(wxSizerFlags(1).Expand().Border())
                .withStyle(wxALIGN_LEFT),
        },
        // endsnippet wxUIController

        HSizer {
            wxSizerFlags().Center().Border(),
            // snippet wxUIBind
            Button { "Left" }
                .withFlags(wxSizerFlags().Border(wxRIGHT))
                .bind([] { wxLogMessage("Pressed Left"); }),
            Button { "Right" }
                .withFlags(wxSizerFlags().Border(wxLEFT))
                .bind([](wxCommandEvent&) { wxLogMessage("Pressed Right"); }),
            // endsnippet wxUIBind
        },

        CreateStdDialogButtonSizer(wxOK),
        // snippet withwxUI
        // snippet wxUILayoutBasic
    }
        .fitTo(this);
    // endsnippet wxUILayoutBasic
}
// endsnippet withwxUI
// endsnippet Example
