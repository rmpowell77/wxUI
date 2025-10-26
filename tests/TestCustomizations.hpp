/*
MIT License

Copyright (c) 2022-2025 Richard Powell

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
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>
#include <wx/bmpbuttn.h>
#include <wx/bmpcbox.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/gauge.h>
#include <wx/hyperlink.h>
#include <wx/listbox.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/splitter.h>
#include <wx/statbmp.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/tglbtn.h>

// Forward declarations to avoid including Widget.hpp here. We want the
// test-provider customization overloads to be visible to translation
// units before Widget.hpp is processed so qualified calls in that header
// can find these overloads. Only a couple of types are needed here.
namespace wxUI::details {
struct BindInfo;
template <typename T>
struct WidgetProxy;
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

namespace wxUITests {

struct TestSizer {
    bool top { false };
    std::optional<std::string> caption {};
    wxOrientation orientation {};

    std::vector<std::string> log {};
};

struct TestProvider {
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
    std::list<TestProvider> providers {};
    std::list<TestSizer> sizers {};

    TestSizer* currentSizer {};
    void SetSizer(TestSizer* sizer) { currentSizer = sizer; }

    auto add(TestProvider controller) -> TestProvider*;
    auto add(TestSizer controller) -> TestSizer*;
    auto dump() const -> std::vector<std::string>;
};

}

template <>
struct std::formatter<wxUITests::TestProvider, char> {
    // No format specifications supported.
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(wxUITests::TestProvider const& c, std::format_context& ctx) const
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
        std::format_to(ctx.out(), "{}Sizer[orientation={}", c.top ? "Top" : "", orientationStr);
        if (c.caption.has_value()) {
            std::format_to(ctx.out(), ", caption=\"{}\"", *c.caption);
        }
        return std::format_to(ctx.out(), "]");
    }
};

namespace wxUITests {
inline auto TestProvider::add(TestProvider controller) -> TestProvider*
{
    providers.push_back(controller);
    log.push_back(std::format("Create:{}", controller));
    return &providers.back();
}

inline auto TestProvider::add(TestSizer sizer) -> TestSizer*
{
    sizers.push_back(sizer);
    log.push_back(std::format("Create:{}", sizer));
    return &sizers.back();
}

inline auto TestProvider::dump() const -> std::vector<std::string>
{
    auto result = std::vector<std::string> {};
    if (currentSizer) {
        result.push_back(std::format("topsizer:{}", *currentSizer));
    }
    for (auto controller : providers) {
        result.push_back(std::format("controller:{}", controller));
        result.insert(result.end(), controller.log.begin(), controller.log.end());
    }
    for (auto sizer : sizers) {
        result.push_back(std::format("sizer:{}", sizer));
        result.insert(result.end(), sizer.log.begin(), sizer.log.end());
    }
    return result;
}

}
namespace wxUI::customizations {
// Forward-declare the primary template so we can provide partial
// specializations in this header even if the primary template is
// declared later (e.g., in include/wxUI/Customizations.hpp).
template <typename Underlying, typename Parent>
struct ParentCreateImpl;

template <>
struct ParentCreateImpl<wxStaticBitmap, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, wxBitmap bitmap, wxPoint pos, wxSize size, int64_t style)
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
struct ParentCreateImpl<wxBitmapButton, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, wxBitmap bitmap, wxPoint pos, wxSize size, int64_t style)
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
struct ParentCreateImpl<wxBitmapComboBox, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, std::string first, wxPoint pos, wxSize size, int n, const wxString* choices, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i]);
        }
        return parent->add({
            .type = "wxBitmapComboBox",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = std::move(first),
            .choices = std::move(tchoices),
        });
    }
};

template <>
struct ParentCreateImpl<wxBitmapToggleButton, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, wxBitmap bitmap, wxPoint pos, wxSize size, int64_t style)
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
struct ParentCreateImpl<wxButton, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, std::string text, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxButton",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = std::move(text),
        });
    }
};

template <>
struct ParentCreateImpl<wxCheckBox, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, std::string text, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxCheckBox",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = std::move(text),
        });
    }
};

template <>
struct ParentCreateImpl<wxChoice, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, wxPoint pos, wxSize size, int n, const wxString* choices, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i]);
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
struct ParentCreateImpl<wxComboBox, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, std::string first, wxPoint pos, wxSize size, int n, const wxString* choices, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i]);
        }
        return parent->add({
            .type = "wxComboBox",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = std::move(first),
            .choices = std::move(tchoices),
        });
    }
};

template <>
struct ParentCreateImpl<wxGauge, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, int value, wxPoint pos, wxSize size, int64_t style)
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
struct ParentCreateImpl<wxHyperlinkCtrl, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, std::string text, std::string text2, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxHyperlinkCtrl",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = std::move(text),
            .text2 = std::move(text2),
        });
    }
};

template <>
struct ParentCreateImpl<wxStaticLine, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style)
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
struct ParentCreateImpl<wxListBox, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, wxPoint pos, wxSize size, int n, const wxString* choices, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i]);
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
struct ParentCreateImpl<wxRadioBox, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, std::string text, wxPoint pos, wxSize size, int n, const wxString* choices, int majorDim, int64_t style)
    {
        auto tchoices = std::vector<std::string> {};
        for (auto i = 0; i < n; ++i) {
            tchoices.push_back(choices[i]);
        }
        return parent->add({
            .type = "wxRadioBox",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = std::move(text),
            .choices = std::move(tchoices),
            .majorDim = majorDim,
        });
    }
};

template <>
struct ParentCreateImpl<wxSlider, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, int value, int min, int max, wxPoint pos, wxSize size, int64_t style)
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
struct ParentCreateImpl<wxSpinCtrl, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, [[maybe_unused]] wxString const& text, wxPoint pos, wxSize size, int64_t style, int min, int max, int value)
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
struct ParentCreateImpl<wxTextCtrl, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, std::string text, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxTextCtrl",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = std::move(text),
        });
    }
};

template <>
struct ParentCreateImpl<wxStaticText, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, std::string text, wxPoint pos, wxSize size, int64_t style)
    {
        return parent->add({
            .type = "wxStaticText",
            .id = id,
            .pos = pos,
            .size = size,
            .style = style,
            .text = std::move(text),
        });
    }
};

template <>
struct ParentCreateImpl<wxSplitterWindow, wxUITests::TestProvider> {
    static auto create(wxUITests::TestProvider* parent, wxWindowID id, wxPoint pos, wxSize size, int64_t style)
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

inline void ControllerSetDefault(wxUITests::TestProvider* controller)
{
    controller->log.push_back(std::format("SetDefault:{}", controller->id));
}

inline void ControllerSetFont(wxUITests::TestProvider* controller, wxFont const& font)
{
    controller->log.push_back(std::format("SetFont:{}", font.GetPointSize()));
}

inline void ControllerSetEnabled(wxUITests::TestProvider* controller, bool enabled)
{
    controller->log.push_back(std::format("SetEnabled:{}", enabled ? "true" : "false"));
}

inline void ControllerBindEvent(wxUITests::TestProvider* controller, [[maybe_unused]] wxUI::details::BindInfo const& boundedFunction)
{
    auto count = std::ranges::count_if(controller->log, [](auto const& e) { return e.starts_with("BindEvents:"); });
    controller->log.push_back(std::format("BindEvents:{}", count + 1));
}

template <typename Proxy>
inline void ControllerBindProxy(wxUITests::TestProvider* controller, [[maybe_unused]] Proxy& proxyHandle)
{
    auto count = std::ranges::count_if(controller->log, [](auto const& e) { return e.starts_with("BindProxy:"); });
    controller->log.push_back(std::format("BindProxy:{}", count + 1));
}

inline void ControllerSetSelection(wxUITests::TestProvider* controller, int selection)
{
    controller->log.push_back(std::format("SetSelection:{}", selection));
}

template <typename Bitmap>
inline void ControllerSetItemBitmap(wxUITests::TestProvider* controller, unsigned int n, Bitmap const& bitmap)
{
    controller->log.push_back(std::format("SetItemBitmap:{}:{}", n, bitmap));
}

template <typename Bitmap>
inline void ControllerSetBitmapPressed(wxUITests::TestProvider* controller, Bitmap const& bitmap)
{
    controller->log.push_back(std::format("SetBitmapPressed:{}", bitmap));
}

inline void ControllerSetValue(wxUITests::TestProvider* controller, bool value)
{
    controller->log.push_back(std::format("SetValue:{}", value));
}

inline void ControllerEnsureVisible(wxUITests::TestProvider* controller, bool value)
{
    controller->log.push_back(std::format("EnsureVisible:{}", value));
}

inline void ControllerWrap(wxUITests::TestProvider* controller, bool value)
{
    controller->log.push_back(std::format("Wrap:{}", value));
}

inline void ControllerSplitHorizontal(wxUITests::TestProvider* controller, wxUITests::TestProvider* window1, wxUITests::TestProvider* window2)
{
    controller->log.push_back(std::format("SplitHorizontal:{}:{}", *window1, *window2));
}

inline void ControllerSplitVertical(wxUITests::TestProvider* controller, wxUITests::TestProvider* window1, wxUITests::TestProvider* window2)
{
    controller->log.push_back(std::format("SplitVertical:{}:{}", *window1, *window2));
}

inline void ControllerSetSashGravity(wxUITests::TestProvider* controller, double gravity)
{
    controller->log.push_back(std::format("SetSashGravity:{}", gravity));
}

inline auto SizerCreate(wxUITests::TestProvider* provider, std::optional<std::string> caption, wxOrientation orientation) -> wxUITests::TestSizer*
{
    return provider->add(wxUITests::TestSizer {
        .caption = caption,
        .orientation = orientation,
    });
}

inline void SizerSetSizeHints(wxUITests::TestSizer* sizer, wxUITests::TestProvider* parent)
{
    sizer->log.push_back(std::format("SetSizerHints:{}", *parent));
}

inline void SizerAddController(wxUITests::TestSizer* sizer, wxUITests::TestProvider* controller, wxSizerFlags const& flags)
{
    sizer->log.push_back(std::format("Add:{}:flags:{}", *controller, flags));
}

inline void SizerAddController(wxUITests::TestSizer* sizer, wxUITests::TestSizer* sizer2, wxSizerFlags const& flags)
{
    sizer->log.push_back(std::format("AddSizer:{}:flags:{}", *sizer2, flags));
}

}
