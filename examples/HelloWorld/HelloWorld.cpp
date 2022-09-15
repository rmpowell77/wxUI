// wxUI "Hello World" example
// This example is inspired from https://docs.wxwidgets.org/latest/overview_helloworld.html

#include <wx/wx.h>
#include <wxUI/wxUI.h>

class HelloWorldApp : public wxApp {
public:
    virtual bool OnInit();
};

class HelloWorldFrame : public wxFrame {
public:
    HelloWorldFrame();
};

class ExampleDialog1 : public wxDialog {
public:
    ExampleDialog1(wxWindow* parent);
};

class ExampleDialog2 : public wxDialog {
public:
    ExampleDialog2(wxWindow* parent);
};

wxIMPLEMENT_APP(HelloWorldApp);

bool HelloWorldApp::OnInit()
{
    HelloWorldFrame* frame = new HelloWorldFrame();
    frame->Show(true);
    return true;
}

HelloWorldFrame::HelloWorldFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxUI::MenuBar {
        wxUI::Menu {
            "&File",
            wxUI::Item { "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item", [this](wxCommandEvent& event) {
                            wxLogMessage("Hello world from wxWidgets!");
                        } },
            wxUI::Item { "&Example1...\tCtrl-D", [this]() {
                            wxLogMessage("Hello World!");
                        } },
            wxUI::CheckItem { "&Example2...\tCtrl-D", [this](wxCommandEvent& event) {
                                 wxLogMessage(event.IsChecked() ? "is checked" : "is not checked");
                             } },
            wxUI::Separator {},
            wxUI::Item { "&Example...\tCtrl-D", [this]() {
                            ExampleDialog1 dialog(this);
                            dialog.ShowModal();
                        } },
            wxUI::Item { "&Example with wxUI...\tCtrl-F", [this]() {
                            ExampleDialog2 dialog(this);
                            dialog.ShowModal();
                        } },
            wxUI::Item { wxID_EXIT, [this]() {
                            Close(true);
                        } },
        },
        wxUI::Menu {
            "&Extra",
            wxUI::Menu {
                "Pets",
                wxUI::CheckItem { "Cats", [this](wxCommandEvent& event) {
                                     wxLogMessage("Cats %s checked", event.IsChecked() ? "are" : "are not");
                                 } },
                wxUI::CheckItem { "Dogs", [this](wxCommandEvent& event) {
                                     wxLogMessage("Dogs %s checked", event.IsChecked() ? "are" : "are not");
                                 } },
            },
            wxUI::Menu {
                "Colors",
                wxUI::RadioItem { "Red", [this](wxCommandEvent& event) {
                                     wxLogMessage("Red %s checked", event.IsChecked() ? "is" : "is not");
                                 } },
                wxUI::RadioItem { "Green", [this](wxCommandEvent& event) {
                                     wxLogMessage("Green %s checked", event.IsChecked() ? "is" : "is not");
                                 } },
                wxUI::RadioItem { "Blue", [this](wxCommandEvent& event) {
                                     wxLogMessage("Blue %s checked", event.IsChecked() ? "is" : "is not");
                                 } },
            },
        },
        wxUI::Menu {
            "&Help",
            wxUI::Item { wxID_ABOUT, [this]() {
                            wxMessageBox("The wxUI Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
                        } },

        }
    }.attachTo(this);

    CreateStatusBar();

    SetStatusText("Welcome to wxUI!");
}

ExampleDialog1::ExampleDialog1(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExampleDialog",
        wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    // Create the controls.
    auto text = new wxStaticText(this, wxID_ANY, "Example of Text in wxUI");
    auto textTitle = new wxTextCtrl(this, wxID_ANY, "Single line of text");
    auto textBody = new wxTextCtrl(this, wxID_ANY,
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

    // Lay out the UI
    textBody->SetMinSize(wxSize(200, 100));

    auto sizer = new wxBoxSizer(wxVERTICAL);

    auto sizerText = new wxStaticBoxSizer(wxVERTICAL, this, "Text Examples");
    sizerText->Add(text, wxSizerFlags().Expand().Border());
    sizerText->Add(textTitle, wxSizerFlags().Expand().Border());
    sizerText->Add(textBody, wxSizerFlags(1).Expand().Border());
    sizer->Add(sizerText, wxSizerFlags(1).Expand().Border());

    sizer->Add(logLevels, wxSizerFlags().Expand().Border());

    auto sizerDetails = new wxStaticBoxSizer(wxHORIZONTAL, this, "Details");
    sizerDetails->Add(checkBox, wxSizerFlags().Border());
    sizerDetails->Add(choice, wxSizerFlags().Border());
    sizerDetails->Add(blankLine, wxSizerFlags(1).Expand().Border());
    sizer->Add(sizerDetails, wxSizerFlags().Expand().Border());

    auto sizerBtns = new wxBoxSizer(wxHORIZONTAL);
    sizerBtns->Add(btnLeft, wxSizerFlags().Border(wxLEFT));
    sizerBtns->Add(btnRight, wxSizerFlags().Border(wxRIGHT));
    sizer->Add(sizerBtns, wxSizerFlags().Centre().Border());

    sizer->Add(CreateStdDialogButtonSizer(wxOK), wxSizerFlags().Expand().Border());

    SetSizerAndFit(sizer);

    // And connect the event handlers.
    btnLeft->Bind(wxEVT_BUTTON, [](wxEvent&) { wxLogMessage("Pressed Left"); });
    btnRight->Bind(wxEVT_BUTTON, [](wxEvent&) { wxLogMessage("Pressed Right"); });
}

ExampleDialog2::ExampleDialog2(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExampleDialog",
        wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;
    VStack {
        wxSizerFlags().Expand().Border(),
        VStack { "Text examples",
            Text { "Example of Text in wxUI" },
            TextCtrl { "Single line of text" }
                .style(wxALIGN_LEFT),
            TextCtrl {
                "Several lines of text.\n"
                "With wxUI the code reflects\n"
                "what the UI looks like." }
                .style(wxTE_MULTILINE)
                .withSize(wxSize(200, 100)) },
        RadioBox { "&Log Levels:", { "&Information", "&Warning", "&Error", "&None", "&Custom" } }
            .style(wxRA_SPECIFY_ROWS)
            .majorDim(1)
            .withSelection(1),

        HStack {
            "Details",
            CheckBox { "Show" },
            Choice { { "Less", "More" } },
            TextCtrl { wxSizerFlags(1).Expand().Border(), "Fill in the blank" }
                .style(wxALIGN_LEFT),
        },

        HStack {
            wxSizerFlags().Center().Border(),
            Button { wxSizerFlags().Border(wxRIGHT), "Left" }
                .bind([](wxCommandEvent&) { wxLogMessage("Pressed Left"); }),
            Button { wxSizerFlags().Border(wxLEFT), "Right" }
                .bind([](wxCommandEvent&) { wxLogMessage("Pressed Right"); }),
        },

        Generic { CreateStdDialogButtonSizer(wxOK) },
    }
        .asTopLevel(this);
}
