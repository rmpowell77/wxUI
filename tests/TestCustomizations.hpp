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

#include <format>
#include <list>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
#include <vector>
#include <wx/bmpbuttn.h>
#include <wx/bmpcbox.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/gauge.h>
#include <wx/hyperlink.h>
#include <wx/listbox.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/splitter.h>
#include <wx/statbmp.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/stockitem.h>
#include <wx/textctrl.h>
#include <wx/tglbtn.h>
#include <wxUI/Customizations.hpp>

// Forward declarations to avoid including Widget.hpp here. We want the
// test-parent customization overloads to be visible to translation
// units before Widget.hpp is processed so qualified calls in that header
// can find these overloads. Only a couple of types are needed here.
namespace wxUI::details {
struct BindInfo;
template <typename T>
struct Proxy;
}

// helpful formatters
template <>
struct std::formatter<wxSize, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxSize const& size, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "({},{})", size.GetWidth(), size.GetHeight());
    }
};
template <>
struct std::formatter<wxPoint, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxPoint const& pos, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "({},{})", pos.x, pos.y);
    }
};
template <>
struct std::formatter<wxBitmap, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxBitmap const& bitmap, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "{}", bitmap.GetSize());
    }
};

template <>
struct std::formatter<wxSizerFlags, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxSizerFlags const& size, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "({},0x{:X},{})", size.GetProportion(), size.GetFlags(), size.GetBorderInPixels());
    }
};

template <>
struct std::formatter<wxMenuItem, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxMenuItem const& menu, std::format_context& ctx) const
    {
        // Determine kind via a small helper function
        auto menuKindToString = [](int kind) -> std::string {
            switch (kind) {
            case wxITEM_SEPARATOR:
                return "separator";
            case wxITEM_CHECK:
                return "check";
            case wxITEM_RADIO:
                return "radio";
            case wxITEM_NORMAL:
                return "normal";
            default:
                return std::format("kind({})", static_cast<int>(kind));
            }
        };
        std::string kindStr = menuKindToString(menu.GetKind());

        // Submenu overrides kind description for clarity
        if (menu.GetSubMenu() != nullptr) {
            kindStr = "submenu";
        }

        auto id = menu.GetId();
        auto label = menu.GetItemLabelText().utf8_string();
        auto help = menu.GetHelp().utf8_string();

        return std::format_to(ctx.out(), "(menuItem:id={},kind={},label=\"{}\",help=\"{}\")", id, kindStr, label, help);
    }
};

template <>
struct std::formatter<wxMenu, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxMenu const& menu, std::format_context& ctx) const
    {
        std::format_to(ctx.out(), "[title:{}:[", menu.GetTitle().utf8_string());
        auto& items = menu.GetMenuItems();
        for (auto& item : items) {
            std::format_to(ctx.out(), "{},", *item);
        }
        return std::format_to(ctx.out(), "]");
    }
};

template <>
struct std::formatter<wxMenuBar, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxMenuBar const& bar, std::format_context& ctx) const
    {
        std::format_to(ctx.out(), "[");
        for (auto i = 0UL; i < bar.GetMenuCount(); ++i) {
            std::format_to(ctx.out(), "{},", *(bar.GetMenu(i)));
        }
        return std::format_to(ctx.out(), "]");
    }
};

namespace wxUITests {

// Mock menu structures to avoid creating real wxMenu/wxMenuBar objects in tests
struct TestMenuItem {
    int id {};
    std::string kind; // "normal", "check", "radio"
    std::string label;
    std::string help;
    int numProxyHandles { 0 };
};

struct TestMenu {
    std::string title;
    std::vector<TestMenuItem> items;
    int numProxyHandles { 0 };
};

struct TestMenuBar {
    std::vector<TestMenu> menus;
    int numProxyHandles { 0 };
};

}

// Formatters for test mock menu structures
template <>
struct std::formatter<wxUITests::TestMenuItem, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxUITests::TestMenuItem const& item, std::format_context& ctx) const
    {
        std::format_to(ctx.out(), "(menuItem:id={},kind={},label=\"{}\",help=\"{}\")",
            item.id, item.kind, item.label, item.help);
        if (item.numProxyHandles > 0) {
            std::format_to(ctx.out(), ":numProxyHandles={}", item.numProxyHandles);
        }
        return ctx.out();
    }
};

template <>
struct std::formatter<wxUITests::TestMenu, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxUITests::TestMenu const& menu, std::format_context& ctx) const
    {
        std::format_to(ctx.out(), "[title:{}:[", menu.title);
        for (auto const& item : menu.items) {
            std::format_to(ctx.out(), "{},", item);
        }
        std::format_to(ctx.out(), "]");
        if (menu.numProxyHandles > 0) {
            std::format_to(ctx.out(), ":numProxyHandles={}", menu.numProxyHandles);
        }
        return ctx.out();
    }
};

template <>
struct std::formatter<wxUITests::TestMenuBar, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(wxUITests::TestMenuBar const& bar, std::format_context& ctx) const
    {
        std::format_to(ctx.out(), "[");
        for (auto const& menu : bar.menus) {
            std::format_to(ctx.out(), "{},", menu);
        }
        std::format_to(ctx.out(), "]");
        if (bar.numProxyHandles > 0) {
            std::format_to(ctx.out(), ":numProxyHandles={}", bar.numProxyHandles);
        }
        return ctx.out();
    }
};

namespace wxUITests {

enum class SizerType {
    Box,
    WrapBox,
    Grid,
    FlexGrid,
};
struct TestParent;

}

// Specialize MenuItem type for TestParent (must be after forward declaration)
namespace wxUI::customizations {
template <>
struct MenuItemTypeFor<wxUITests::TestParent> {
    using type = wxUITests::TestMenuItem;
};
}

namespace wxUITests {

struct TestSizer {
    bool top { false };
    SizerType type { SizerType::Box };
    std::optional<std::string> caption {};
    wxOrientation orientation {};
    std::optional<int> cols {};

    void SetSizeHints(TestParent*);
    void Add(TestParent*, wxSizerFlags const& flags);
    void Add(TestSizer*, wxSizerFlags const& flags);
    std::vector<std::string> log {};
};

struct TestParent {
    std::string type;
    int id {};
    wxPoint pos {};
    wxSize size {};
    int64_t style {};
    std::optional<std::string> text {};
    std::optional<std::string> text2 {};
    std::optional<wxBitmap> bitmap {};
    std::optional<std::vector<std::string>> choices {};
    std::optional<int> value {};
    std::optional<std::pair<int, int>> range {};
    std::optional<int> majorDim {};

    std::vector<std::string> log {};
    std::vector<std::string> menuDetails {};
    std::vector<std::string> bookPages {};
    std::list<TestParent> parents {};
    std::list<TestSizer> sizers {};
    std::list<TestMenu> menus {}; // Storage for mock menus
    std::list<TestMenuBar> menuBars {}; // Storage for mock menubars

    TestSizer* currentSizer {};
    TestParent* currentMenu {};
    void SetSizer(TestSizer* sizer) { currentSizer = sizer; }
    void SetSashGravity(double gravity)
    {
        log.push_back(std::format("SetSashGravity:{}", gravity));
    }
    void SplitVertically(TestParent* window1, TestParent* window2);
    void SplitHorizontally(TestParent* window1, TestParent* window2);
    template <typename Bitmap>
    void SetBitmapPressed(Bitmap const& bitmap)
    {
        log.push_back(std::format("SetBitmapPressed:{}", bitmap));
    }
    template <typename Bitmap>
    void SetItemBitmap(unsigned int n, Bitmap const& bitmap)
    {
        log.push_back(std::format("SetItemBitmap:{}:{}", n, bitmap));
    }
    void SetSelection(int selection)
    {
        log.push_back(std::format("SetSelection:{}", selection));
    }
    void SetValue(bool value)
    {
        log.push_back(std::format("SetValue:{}", value));
    }
    void Wrap(bool value)
    {
        log.push_back(std::format("Wrap:{}", value));
    }
    void Enable(bool enabled)
    {
        log.push_back(std::format("SetEnabled:{}", enabled ? "true" : "false"));
    }
    void SetFont(wxFont const& font)
    {
        log.push_back(std::format("SetFont:{}", font.GetPointSize()));
    }
    void EnsureVisible(bool value)
    {
        log.push_back(std::format("EnsureVisible:{}", value));
    }
    void SetDefault()
    {
        log.push_back(std::format("SetDefault:{}", id));
    }
    void SetSizerAndFit(TestSizer* sizer);
    void AddPage(TestParent* page, wxString const& title, bool select);

    auto add(TestParent controller) -> TestParent*;
    auto add(TestSizer controller) -> TestSizer*;
    auto dump() const -> std::vector<std::string>;
};

}

template <>
struct std::formatter<wxUITests::TestParent, char> {
    // No format specifications supported.
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(wxUITests::TestParent const& c, std::format_context& ctx) const
    {
        std::format_to(
            ctx.out(), "{}[id={}, pos={}, size={}, style={}", c.type, c.id, c.pos, c.size, c.style);
        if (c.text.has_value()) {
            std::format_to(ctx.out(), ", text=\"{}\"", *c.text);
        }
        if (c.text2.has_value()) {
            std::format_to(ctx.out(), ", text2=\"{}\"", *c.text2);
        }
        if (c.bitmap.has_value()) {
            std::format_to(ctx.out(), ", bitmap={}", *c.bitmap);
        }
        if (c.choices.has_value()) {
            std::format_to(ctx.out(), ", choices=(");
            for (auto& i : *c.choices) {
                std::format_to(ctx.out(), "\"{}\",", i);
            }
            std::format_to(ctx.out(), ")");
        }
        if (c.value.has_value()) {
            std::format_to(ctx.out(), ", value={}", *c.value);
        }
        if (c.range.has_value()) {
            std::format_to(ctx.out(), ", range=[{},{}]", c.range->first, c.range->second);
        }
        if (c.majorDim.has_value()) {
            std::format_to(ctx.out(), ", majorDim={}", *c.majorDim);
        }
        return std::format_to(ctx.out(), "]");
    }
};

template <>
struct std::formatter<wxUITests::TestSizer, char> {
    // No format specifications supported.
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(wxUITests::TestSizer const& c, std::format_context& ctx) const
    {
        std::string orientationStr = c.orientation == wxHORIZONTAL
            ? std::string("wxHORIZONTAL")
            : c.orientation == wxVERTICAL
            ? std::string("wxVERTICAL")
            : std::format("{}", static_cast<int>(c.orientation));
        switch (c.type) {
        case wxUITests::SizerType::Box:
            std::format_to(ctx.out(), "{}Sizer[orientation={}", c.top ? "Top" : "", orientationStr);
            break;
        case wxUITests::SizerType::WrapBox:
            std::format_to(ctx.out(), "{}WrapSizer[orientation={}", c.top ? "Top" : "", orientationStr);
            break;
        case wxUITests::SizerType::Grid:
            std::format_to(ctx.out(), "{}GridSizer[cols={}", c.top ? "Top" : "", *c.cols);
            orientationStr = "Grid";
            break;
        case wxUITests::SizerType::FlexGrid:
            std::format_to(ctx.out(), "{}FlexGridSizer[cols={}", c.top ? "Top" : "", *c.cols);
            break;
        }
        if (c.caption.has_value()) {
            std::format_to(ctx.out(), ", caption=\"{}\"", *c.caption);
        }
        return std::format_to(ctx.out(), "]");
    }
};

namespace wxUITests {
inline auto TestParent::add(TestParent controller) -> TestParent*
{
    parents.push_back(controller);
    log.push_back(std::format("Create:{}", controller));
    return &parents.back();
}

inline auto TestParent::add(TestSizer sizer) -> TestSizer*
{
    sizers.push_back(sizer);
    log.push_back(std::format("Create:{}", sizer));
    return &sizers.back();
}

inline void TestParent::SplitVertically(TestParent* window1, TestParent* window2)
{
    log.push_back(std::format("SplitVertical:{}:{}", *window1, *window2));
}

inline void TestParent::SplitHorizontally(TestParent* window1, TestParent* window2)
{
    log.push_back(std::format("SplitHorizontal:{}:{}", *window1, *window2));
}

inline void TestParent::AddPage(TestParent* page, wxString const& title, bool select)
{
    bookPages.push_back(std::format("{}:{}:{}", *page, title.utf8_string(), select));
}

inline void TestParent::SetSizerAndFit(TestSizer* sizer)
{
    SetSizer(sizer);
    auto const& sizerRef = *sizer;
    log.push_back(std::format("SetSizerAndFit:{}", sizerRef));
}

inline auto TestParent::dump() const -> std::vector<std::string>
{
    auto result = std::vector<std::string> {};
    for (auto entry : log) {
        result.push_back(std::format("{}", entry));
    }
    if (currentSizer) {
        result.push_back(std::format("topsizer:{}", *currentSizer));
    }
    for (auto menuDetail : menuDetails) {
        result.push_back(std::format("menu:{}", menuDetail));
    }
    for (auto bookPage : bookPages) {
        result.push_back(std::format("bookPage:{}", bookPage));
    }
    for (auto controller : parents) {
        result.push_back(std::format("controller:{}", controller));
        result.insert(result.end(), controller.log.begin(), controller.log.end());
        for (auto const& bookPage : controller.bookPages) {
            result.push_back(std::format("bookPage:{}", bookPage));
        }
    }
    for (auto sizer : sizers) {
        result.push_back(std::format("sizer:{}", sizer));
        result.insert(result.end(), sizer.log.begin(), sizer.log.end());
    }
    return result;
}

inline void TestSizer::SetSizeHints(TestParent* controller)
{
    log.push_back(std::format("SetSizeHints:{}", *controller));
}

inline void TestSizer::Add(TestParent* controller, wxSizerFlags const& flags)
{
    log.push_back(std::format("Add:{}:flags:{}", *controller, flags));
}

inline void TestSizer::Add(TestSizer* sizer, wxSizerFlags const& flags)
{
    log.push_back(std::format("AddSizer:{}:flags:{}", *sizer, flags));
}

}
namespace wxUI::customizations {
// Forward-declare the primary template so we can provide partial
// specializations in this header even if the primary template is
// declared later (e.g., in include/wxUI/Customizations.hpp).
template <typename Underlying, typename Parent>
struct ParentCreateImpl;

template <>
struct ParentCreateImpl<wxStaticBitmap, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxBitmap bitmap, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxStaticBitmap",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .bitmap = bitmap,
        });
    }
};

template <>
struct ParentCreateImpl<wxBitmapButton, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxBitmap bitmap, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxBitmapButton",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .bitmap = bitmap,
        });
    }
};

template <>
struct ParentCreateImpl<wxBitmapComboBox, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxString const& first, wxPoint pos, wxSize size, int n, const wxString* choices, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i].utf8_string());
        }
        return parent->add({
            .type = "wxBitmapComboBox",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = first.utf8_string(),
            .choices = std::move(tchoices),
        });
    }
};

template <>
struct ParentCreateImpl<wxBitmapToggleButton, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxBitmap bitmap, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxBitmapToggleButton",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .bitmap = bitmap,
        });
    }
};

template <>
struct ParentCreateImpl<wxButton, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxString const& text, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxButton",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = text.utf8_string(),
        });
    }
};

template <>
struct ParentCreateImpl<wxCheckBox, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxString const& text, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxCheckBox",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = text.utf8_string(),
        });
    }
};

template <>
struct ParentCreateImpl<wxChoice, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxPoint pos, wxSize size, int n, const wxString* choices, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i].utf8_string());
        }
        return parent->add({
            .type = "wxChoice",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .choices = std::move(tchoices),
        });
    }
};

template <>
struct ParentCreateImpl<wxComboBox, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxString const& first, wxPoint pos, wxSize size, int n, const wxString* choices, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i].utf8_string());
        }
        return parent->add({
            .type = "wxComboBox",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = first.utf8_string(),
            .choices = std::move(tchoices),
        });
    }
};

template <>
struct ParentCreateImpl<wxGauge, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, int value, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxGauge",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .value = value,
        });
    }
};

template <>
struct ParentCreateImpl<wxHyperlinkCtrl, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxString const& text, wxString const& text2, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxHyperlinkCtrl",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = text.utf8_string(),
            .text2 = text2.utf8_string(),
        });
    }
};

template <>
struct ParentCreateImpl<wxStaticLine, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxStaticLine",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
        });
    }
};

template <>
struct ParentCreateImpl<wxListBox, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxPoint pos, wxSize size, int n, const wxString* choices, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i].utf8_string());
        }
        return parent->add({
            .type = "wxListBox",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .choices = std::move(tchoices),
        });
    }
};

template <>
struct ParentCreateImpl<wxRadioBox, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxString const& text, wxPoint pos, wxSize size, int n, const wxString* choices, int majorDim, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i].utf8_string());
        }
        return parent->add({
            .type = "wxRadioBox",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = text.utf8_string(),
            .choices = std::move(tchoices),
            .majorDim = majorDim,
        });
    }
};

template <>
struct ParentCreateImpl<wxSlider, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, int value, int min, int max, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxSlider",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .value = value,
            .range = std::pair<int, int> { min, max },
        });
    }
};

template <>
struct ParentCreateImpl<wxSpinCtrl, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxString const&, wxPoint pos, wxSize size, int64_t style, int min, int max, int value)
    {
        return parent->add({
            .type = "wxSpinCtrl",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .value = value,
            .range = std::pair<int, int> { min, max },
        });
    }
};

template <>
struct ParentCreateImpl<wxTextCtrl, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxString const& text, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxTextCtrl",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = text.utf8_string(),
        });
    }
};

template <>
struct ParentCreateImpl<wxStaticText, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxString text, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxStaticText",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = text.utf8_string(),
        });
    }
};

template <>
struct ParentCreateImpl<wxSplitterWindow, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxSplitterWindow",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
        });
    }
};

template <>
struct ParentCreateImpl<wxNotebook, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id)
    {
        return parent->add({
            .type = "wxNotebook",
            .id = id,
            .pos = wxPoint { -1, -1 },
            .size = wxSize { -1, -1 },
            .style = 0,
        });
    }
};

template <>
struct ParentCreateImpl<wxWindow, wxUITests::TestParent> {
    static auto create(wxUITests::TestParent* parent, wxWindowID id)
    {
        return parent->add({
            .type = "wxWindow",
            .id = id,
            .pos = wxPoint { -1, -1 },
            .size = wxSize { -1, -1 },
            .style = 0,
        });
    }
};

inline void ControllerBindEvent(wxUITests::TestParent* controller, wxUI::details::BindInfo const&)
{
    auto count = std::ranges::count_if(controller->log, [](auto const& e) { return e.starts_with("BindEvents:"); });
    controller->log.push_back(std::format("BindEvents:{}", count + 1));
}

template <typename Proxy>
inline void ControllerBindProxy(wxUITests::TestParent* controller, Proxy&)
{
    auto count = std::ranges::count_if(controller->log, [](auto const& e) { return e.starts_with("BindProxy:"); });
    controller->log.push_back(std::format("BindProxy:{}", count + 1));
}

template <typename Proxy>
inline void SizerBindProxy(wxUITests::TestSizer* controller, Proxy&)
{
    auto count = std::ranges::count_if(controller->log, [](auto const& e) { return e.starts_with("SizerBindProxy:"); });
    controller->log.push_back(std::format("SizerBindProxy:{}", count + 1));
}

inline auto SizerCreate(wxUITests::TestParent* parent, SizerInfo const& info) -> wxUITests::TestSizer*
{
    return std::visit(overloaded {
                          [parent](BoxSizerInfo const& info) -> wxUITests::TestSizer* {
                              return parent->add(wxUITests::TestSizer {
                                  .type = wxUITests::SizerType::Box,
                                  .caption = info.caption.has_value() ? std::optional { info.caption->utf8_string() } : std::nullopt,
                                  .orientation = info.orientation,
                              });
                          },
                          [parent](WrapSizerInfo const& info) -> wxUITests::TestSizer* {
                              return parent->add(wxUITests::TestSizer {
                                  .type = wxUITests::SizerType::WrapBox,
                                  .orientation = info.orientation,
                              });
                          },
                          [parent](GridSizerInfo const& info) -> wxUITests::TestSizer* {
                              return parent->add(wxUITests::TestSizer {
                                  .type = wxUITests::SizerType::Grid,
                                  .cols = info.cols,
                              });
                          },
                          [parent](FlexGridSizerInfo const& info) -> wxUITests::TestSizer* {
                              return parent->add(wxUITests::TestSizer {
                                  .type = wxUITests::SizerType::FlexGrid,
                                  .cols = info.cols,
                              });
                          } },

        info);
}

// Menu customization overloads for TestParent - creates mock menus instead of real wx objects
inline auto MenuCreate(wxUITests::TestParent& parent) -> wxUITests::TestMenu*
{
    parent.menus.emplace_back();
    return &parent.menus.back();
}

inline auto MenuBarCreate(wxUITests::TestParent* parent) -> wxUITests::TestMenuBar*
{
    parent->menuBars.emplace_back();
    return &parent->menuBars.back();
}

// Helper to get stock label/help for standard IDs
namespace {
    inline std::string getStockLabel(int id)
    {
        auto label = wxGetStockLabel(static_cast<wxWindowID>(id), wxSTOCK_NOFLAGS); // No mnemonic
        return label.utf8_string();
    }

    inline std::string getStockHelp(int id)
    {
        // wxWidgets doesn't expose wxGetStockHelpString publicly, so we provide our own mapping
        switch (id) {
        case wxID_EXIT:
            return "Quit this program";
        case wxID_ABOUT:
            return "Show about dialog";
        default:
            return "";
        }
    }
}

inline auto MenuAppend(wxUITests::TestMenu* menu, int id, wxString const& item, wxString const& help) -> wxUITests::TestMenuItem*
{
    // Mimic wxWidgets behavior: if item is empty and id is a standard ID, use default label/help
    std::string label = item.utf8_string();
    std::string helpText = help.utf8_string();
    if (label.empty()) {
        label = getStockLabel(id);
    }
    if (helpText.empty()) {
        helpText = getStockHelp(id);
    }
    menu->items.push_back({ id, "normal", label, helpText });
    return &menu->items.back();
}

inline auto MenuAppendCheckItem(wxUITests::TestMenu* menu, int id, wxString const& item, wxString const& help) -> wxUITests::TestMenuItem*
{
    // Mimic wxWidgets behavior: if item is empty and id is a standard ID, use default label/help
    std::string label = item.utf8_string();
    std::string helpText = help.utf8_string();
    if (label.empty()) {
        label = getStockLabel(id);
    }
    if (helpText.empty()) {
        helpText = getStockHelp(id);
    }
    menu->items.push_back({ id, "check", label, helpText });
    return &menu->items.back();
}

inline auto MenuAppendRadioItem(wxUITests::TestMenu* menu, int id, wxString const& item, wxString const& help) -> wxUITests::TestMenuItem*
{
    // Mimic wxWidgets behavior: if item is empty and id is a standard ID, use default label/help
    std::string label = item.utf8_string();
    std::string helpText = help.utf8_string();
    if (label.empty()) {
        label = getStockLabel(id);
    }
    if (helpText.empty()) {
        helpText = getStockHelp(id);
    }
    menu->items.push_back({ id, "radio", label, helpText });
    return &menu->items.back();
}

inline void MenuBarAppend(wxUITests::TestMenuBar* menuBar, wxUITests::TestMenu* menu, wxString const& name)
{
    menu->title = name.utf8_string();
    menuBar->menus.push_back(*menu);
}

inline void MenuAppendSubMenu(wxUITests::TestMenu* parentMenu, wxUITests::TestMenu* subMenu, wxString const& name)
{
    subMenu->title = name.utf8_string();
    // Represent submenu as a special menu item
    parentMenu->items.push_back({ -1, std::format("submenu:{}", *subMenu), "", "" });
}

inline void MenuSetMenuBar(wxUITests::TestParent* parent, wxUITests::TestMenuBar* menuBar)
{
    // Just format it for inspection - it's already stored in parent->menuBars
    parent->menuDetails.push_back(std::format("MenuBar:{}", *menuBar));
}

inline void MenuBindToFrame(wxUITests::TestParent& frame, int identity, std::variant<std::function<void(wxCommandEvent&)>, std::function<void()>> const&)
{
    auto count = std::ranges::count_if(frame.log, [identity](auto const& e) { return e.starts_with(std::format("BindMenu:{}", identity)); });
    frame.log.push_back(std::format("BindMenu:{}:{}", identity, count + 1));
}

// Proxy binding customization points for menu items in tests
// Note: In tests, we can't actually set the proxy because TestMenuItem* != wxMenuItem*
// But calling the customization point is sufficient for test coverage
template <typename Proxy>
inline void MenuBindProxy(wxUITests::TestMenuItem* menu, Proxy&)
{
    menu->numProxyHandles += 1;
}
template <typename Proxy>
inline void MenuBindProxy(wxUITests::TestMenu* menu, Proxy&)
{
    menu->numProxyHandles += 1;
}
template <typename Proxy>
inline void MenuBindProxy(wxUITests::TestMenuBar* menu, Proxy&)
{
    menu->numProxyHandles += 1;
}
// template <typename ProxyType>
// inline void MenuItemBindProxy(wxUITests::TestMenuItem*, ProxyType const&)
// {
//     // No-op in tests - the proxy type (Proxy<wxMenuItem>) doesn't match TestMenuItem*
//     // This is acceptable because we're testing the code paths, not runtime behavior
// }

// template <typename ProxyType>
// inline void MenuBindProxy(wxUITests::TestMenu*, ProxyType const&)
// {
//     // No-op in tests
// }

// template <typename ProxyType>
// inline void MenuBarBindProxy(wxUITests::TestMenuBar*, ProxyType const&)
// {
//     // No-op in tests
// }
}
