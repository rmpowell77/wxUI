# wxUI
C++ header-only library to make declarative UIs for wxWidgets.

## Quick Start

```cpp
#include <numeric>
#include <wx/wx.h>
#include <wxUI/wxUI.h>

class ExampleDialog : public wxDialog {
public:
    explicit ExampleDialog(wxWindow* parent);
    wxUI::SpinCtrl::Proxy a, b;
    wxUI::Text::Proxy result;
};

ExampleDialog::ExampleDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExampleDialog",
        wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;

    VSizer {
        wxSizerFlags().Expand().Border(),
        VSizer {
            "Text examples",
            Text { "Example of Text in wxUI" },
            TextCtrl { "Single line of text" }
                .withStyle(wxALIGN_LEFT),
            TextCtrl {
                "Several lines of text.\n"
                "With wxUI the code reflects\n"
                "what the UI looks like." }
                .withStyle(wxTE_MULTILINE)
                .withSize(wxSize(200, 100))
        },
        HSizer {
            Text { "A =" },
            a = SpinCtrl { std::pair { 1, 10000 } }
                    .bind([this]() {
                        *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b)));
                    }),

            Text { "B =" },
            b = SpinCtrl { std::pair { 1, 10000 } }
                    .bind([this]() { *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b))); }),

            Text { "GCD = " },
            result = Text { "1" },
        },
        RadioBox { "&Log Levels:", RadioBox::withChoices {}, { "&Information", "&Warning", "&Error", "&None", "&Custom" } }
            .setStyle(wxRA_SPECIFY_ROWS)
            .withMajorDim(1)
            .withSelection(1),

        HSizer {
            "Details",
            CheckBox { "Show" },
            Choice { "Less", "More" },
            TextCtrl { wxSizerFlags(1).Expand().Border(), "Fill in the blank" }
                .withStyle(wxALIGN_LEFT),
        },

        HSizer {
            wxSizerFlags().Center().Border(),
            Button { wxSizerFlags().Border(wxRIGHT), "Left" }
                .bind([] { wxLogMessage("Pressed Left"); }),
            Button { wxSizerFlags().Border(wxLEFT), "Right" }
                .bind([](wxCommandEvent&) { wxLogMessage("Pressed Right"); }),
        },

        CreateStdDialogButtonSizer(wxOK),
    }
        .attachTo(this);
}
```
<img src="docs/images/ExampleDialog.png"/>



## Motivation
This library grew out of work to create a C++ library that allowed a simple declarative style for [`wxWidgets`](https://www.wxwidgets.org) to create the UI for the [CalChart](https://github.com/calband/calchart) project.  

[`wxWidgets`](https://www.wxwidgets.org) is a C++ library that lets developers create applications for Windows, macOS, Linux and other platforms with a single code base.  The programmer creates "UI Elements" such as [`wxControl`](https://docs.wxwidgets.org/3.2/classwx_control.html) elements and inserts them in [`wxSizer`](https://docs.wxwidgets.org/3.2/classwx_sizer.html) objects to describe the layout of the UI in code.

For instance, a usual programming pattern for [`wxWidgets`](https://www.wxwidgets.org) is to create the UI objects first, and then put them in a layout: 

```
ExampleDialogWidgets::ExampleDialogWidgets(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExampleDialogWidgets",
        wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    // Create the controls.
    auto* text = new wxStaticText(this, wxID_ANY, "Example of Text in wxWidgets");
    auto* textTitle = new wxTextCtrl(this, wxID_ANY, "Single line of text");
    // ...
    // Layout the controls.
    auto sizer = new wxBoxSizer(wxVERTICAL);

    auto sizerText = new wxStaticBoxSizer(wxVERTICAL, this, "Text Examples");
    sizerText->Add(text, wxSizerFlags().Expand().Border());
    sizerText->Add(textTitle, wxSizerFlags().Expand().Border());
    // ...
    SetSizerAndFit(sizer);
    // ...
}
```

This separates the "what" from the "where", and often makes it challenging to identify what the structure of the UI will be from code inspection.

`wxUI` is a library that allows the user to declare a number of elements in a "struct-like" declaration that creates a "factory" that manufactures at runtime the ownership and sizer heirarchy.


```
ExampleDialog::ExampleDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "ExampleDialog",
        wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    using namespace wxUI;

    VSizer {
        wxSizerFlags().Expand().Border(),
        VSizer {
            "Text examples",
            Text { "Example of Text in wxUI" },
            TextCtrl { "Single line of text" }
                .withStyle(wxALIGN_LEFT),
    // ...
        },
        HSizer {
            Text { "A =" },
            a = SpinCtrl { std::pair { 1, 10000 } }
                    .bind([this]() {
                        *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b)));
                    }),

            Text { "B =" },
            b = SpinCtrl { std::pair { 1, 10000 } }
                    .bind([this]() { *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b))); }),

            Text { "GCD = " },
            result = Text { "1" },
        },
    // ...
    }
        .attachTo(this);
}
```

`wxUI` is not intended to be a replacement for `wxWidgets`, but instead is a library to simplify usage.  It is written such that it should be easy to "step outside" of `wxUI` and access `wxWidgets` directly.

Please consult the [Programmer's Guide](docs/ProgrammersGuide.md) for more information on Library details.
