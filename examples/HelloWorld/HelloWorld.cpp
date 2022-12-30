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
// This example is inspired from https://docs.wxwidgets.org/latest/overview_helloworld.html

// snippet Example headers to include
#include <wx/wx.h>
#include <wxUI/wxUI.h>
// endsnippet Example

class HelloWorldApp : public wxApp {
public:
    bool OnInit() override;
};

class HelloWorldFrame : public wxFrame {
public:
    HelloWorldFrame();
};

class ExampleDialogWidgets : public wxDialog {
public:
    explicit ExampleDialogWidgets(wxWindow* parent);
};

// snippet Example
class ExampleDialog : public wxDialog {
public:
    explicit ExampleDialog(wxWindow* parent);
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
HelloWorldFrame::HelloWorldFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxUI::MenuBar {
        wxUI::Menu {
            "&File",
            // endsnippet wxUIMenu
            wxUI::Item { "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item", [] {
                            wxLogMessage("Hello world from wxWidgets!");
                        } },
            // snippet wxUIMenuExample1
            wxUI::Item { "&Example1...\tCtrl-D", [] {
                            wxLogMessage("Hello World!");
                        } },
            wxUI::CheckItem { "&Example2...\tCtrl-D", [](wxCommandEvent& event) {
                                 wxLogMessage(event.IsChecked() ? "is checked" : "is not checked");
                             } },
            // endsnippet wxUIMenuExample1
            wxUI::Separator {}, wxUI::Item { "&Example...\tCtrl-D", [this] {
                                                ExampleDialogWidgets dialog(this);
                                                dialog.ShowModal();
                                            } },
            // snippet wxUIMenu
            wxUI::Item { "&Example with wxUI...\tCtrl-F", [this] {
                            ExampleDialog dialog(this);
                            dialog.ShowModal();
                        } },
            wxUI::Separator {}, wxUI::Item { wxID_EXIT, [this] {
                                                Close(true);
                                            } },
            // endsnippet wxUIMenu
        },
        // snippet wxUIMenuSubMenu
        wxUI::Menu {
            "&Extra", wxUI::Menu {
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
            // snippet wxUIMenuSubMenu
        },
        // endsnippet wxUIMenuSubMenu
        wxUI::Menu {
            "&Help", wxUI::Item { wxID_ABOUT, [] {
                                     wxMessageBox("The wxUI Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
                                 } },

            // snippet wxUIMenu
        }
    }.attachTo(this);
    // endsnippet wxUIMenu

    CreateStatusBar();

    SetStatusText("Welcome to wxUI!");
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
    auto btnLeft = new wxButton(this, wxID_ANY, "Left");
    auto btnRight = new wxButton(this, wxID_ANY, "Right");

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

    sizer->Add(CreateStdDialogButtonSizer(wxOK), wxSizerFlags().Expand().Border());

    // snippet withwxWidgets
    SetSizerAndFit(sizer);
    // endsnippet withwxWidgets

    // And connect the event handlers.
    btnLeft->Bind(wxEVT_BUTTON, [](wxEvent&) { wxLogMessage("Pressed Left"); });
    btnRight->Bind(wxEVT_BUTTON, [](wxEvent&) { wxLogMessage("Pressed Right"); });
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
    // snippet wxUISizerBasic
    // snippet wxUIGeneric
    VStack {
        wxSizerFlags().Expand().Border(),
        // endsnippet wxUIGeneric
        VStack {
            "Text examples",
            // endsnippet wxUISizerBasic
            Text { "Example of Text in wxUI" },
            TextCtrl { "Single line of text" }
                .style(wxALIGN_LEFT),
            // endsnippet withwxUI
            TextCtrl {
                "Several lines of text.\n"
                "With wxUI the code reflects\n"
                "what the UI looks like." }
                .style(wxTE_MULTILINE)
                .withSize(wxSize(200, 100))
            // snippet withwxUI
        },
        // endsnippet withwxUI
        RadioBox { "&Log Levels:", { "&Information", "&Warning", "&Error", "&None", "&Custom" } }
            .style(wxRA_SPECIFY_ROWS)
            .majorDim(1)
            .withSelection(1),

        // snippet wxUIController
        HStack {
            "Details",
            CheckBox { "Show" },
            Choice { { "Less", "More" } },
            TextCtrl { wxSizerFlags(1).Expand().Border(), "Fill in the blank" }
                .style(wxALIGN_LEFT),
        },
        // endsnippet wxUIController

        HStack {
            wxSizerFlags().Center().Border(),
            // snippet wxUIBind
            Button { wxSizerFlags().Border(wxRIGHT), "Left" }
                .bind([] { wxLogMessage("Pressed Left"); }),
            Button { wxSizerFlags().Border(wxLEFT), "Right" }
                .bind([](wxCommandEvent&) { wxLogMessage("Pressed Right"); }),
            // endsnippet wxUIBind
        },

        // snippet wxUIGeneric
        Generic { CreateStdDialogButtonSizer(wxOK) },
        // snippet withwxUI
        // snippet wxUISizerBasic
    }
        .attachTo(this);
    // endsnippet wxUIGeneric
    // endsnippet wxUISizerBasic
}
// endsnippet withwxUI
// endsnippet Example
