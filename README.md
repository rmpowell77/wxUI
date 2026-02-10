# wxUI
C++ header-only library to make declarative UIs for wxWidgets.

```cpp
#include <numeric>
#include <wx/wx.h>
#include <wxUI/wxUI.hpp>

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
            SpinCtrl { std::pair { 1, 10000 } }.withProxy(a).bind([this]() {
                *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b)));
            }),

            Text { "B =" },
            SpinCtrl { std::pair { 1, 10000 } }.withProxy(b).bind([this]() { *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b))); }),

            Text { "GCD = " },
            Text { "1" }.withProxy(result),
        },
        RadioBox { "&Log Levels:", RadioBox::withChoices {}, { "&Information", "&Warning", "&Error", "&None", "&Custom" } }
            .setStyle(wxRA_SPECIFY_ROWS)
            .withMajorDim(1)
            .withSelection(1),

        HSizer {
            "Details",
            CheckBox { "Show" },
            Choice { "Less", "More" },
            TextCtrl { "Fill in the blank" }
                .withFlags(wxSizerFlags(1).Expand().Border())
                .withStyle(wxALIGN_LEFT),
        },

        HSizer {
            wxSizerFlags().Center().Border(),
            Button { "Left" }
                .withFlags(wxSizerFlags().Border(wxRIGHT))
                .bind([] { wxLogMessage("Pressed Left"); }),
            Button { "Right" }
                .withFlags(wxSizerFlags().Border(wxLEFT))
                .bind([](wxCommandEvent&) { wxLogMessage("Pressed Right"); }),
        },

        CreateStdDialogButtonSizer(wxOK),
    }
        .fitTo(this);
}
```
<img src="docs/images/ExampleDialog.png"/>

## Quick Start

### Installing

**Option 1: FetchContent (recommended for most projects)**

```cmake
include(FetchContent)

FetchContent_Declare(
	wxUI
	GIT_REPOSITORY https://github.com/rmpowell77/wxUI.git
	GIT_TAG        <tag-or-commit>
)
FetchContent_MakeAvailable(wxUI)
```

**Option 2: Build/install locally, then `find_package`**

```bash
cmake -S . -B build
cmake --build build
cmake --install build --prefix <install-prefix>
```

Then in your project:

```cmake
find_package(wxUI REQUIRED)
```

### Including

**CMake**

```cmake
target_link_libraries(your_target
	PRIVATE
	wx::core
	wxUI::wxUI
)
```

**C++**

```cpp
#include <wxUI/wxUI.hpp>
```


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
            SpinCtrl { std::pair { 1, 10000 } }.withProxy(a).bind([this]() {
                *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b)));
            }),

            Text { "B =" },
            SpinCtrl { std::pair { 1, 10000 } }.withProxy(b).bind([this]() { *result = std::to_string(std::gcd(static_cast<int>(*a), static_cast<int>(*b))); }),

            Text { "GCD = " },
            Text { "1" }.withProxy(result),
        },
    // ...
    }
        .fitTo(this);
}
```

`wxUI` is not intended to be a replacement for `wxWidgets`, but instead is a library to simplify usage.  It is written such that it should be easy to "step outside" of `wxUI` and access `wxWidgets` directly.

Please consult the [Programmer's Guide](docs/ProgrammersGuide.md) for more information on Library details.

## Contributing

Contributions are welcome! If you’d like to contribute to `wxUI`, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Submit a pull request with a clear description of your changes.
