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
#include "TestCustomizations.hpp"
#include <catch2/catch_test_macros.hpp>
#include <ranges>
#include <wxUI/Button.hpp>
#include <wxUI/LayoutIf.hpp>
#include <wxUI/Menu.hpp>
#include <wxUI/wxUITypes.hpp>

#include <wx/wx.h>

using namespace wxUITests;
using namespace wxUI;

namespace {
using Dump = std::vector<std::string>;

std::string sizerOrientation(wxOrientation orientation)
{
    return orientation == wxVERTICAL ? "wxVERTICAL" : "wxHORIZONTAL";
}

std::string sizerDescriptor(wxOrientation orientation, std::string_view caption = {})
{
    std::string descriptor = "Sizer[orientation=" + sizerOrientation(orientation);
    if (!caption.empty()) {
        descriptor += ", caption=\"";
        descriptor += caption;
        descriptor += "\"";
    }
    descriptor += "]";
    return descriptor;
}

Dump sizerDump(wxOrientation orientation, std::string_view caption = {})
{
    return {
        "Create:" + sizerDescriptor(orientation, caption),
        "topsizer:" + sizerDescriptor(orientation, caption),
        "sizer:" + sizerDescriptor(orientation, caption),
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
    };
}

Dump sizerWithChildDump(wxOrientation parentOrientation,
    std::string_view parentCaption,
    wxOrientation childOrientation,
    std::string_view childCaption,
    int flags = 0)
{
    return {
        "Create:" + sizerDescriptor(parentOrientation, parentCaption),
        "Create:" + sizerDescriptor(childOrientation, childCaption),
        "topsizer:" + sizerDescriptor(parentOrientation, parentCaption),
        "sizer:" + sizerDescriptor(parentOrientation, parentCaption),
        "AddSizer:" + sizerDescriptor(childOrientation, childCaption) + ":flags:(" + std::to_string(flags) + ",0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
        "sizer:" + sizerDescriptor(childOrientation, childCaption),
    };
}
} // namespace

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
TEST_CASE("LayoutIf")
{
    // <<<< LayoutIf >>>>
    SECTION("layoutif.true.1sizer")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::LayoutIf { true, wxUI::HSizer { "Child" } } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxHORIZONTAL, "Child"));
    }
    SECTION("layoutif.false.1sizer")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::LayoutIf { false, wxUI::HSizer { "Child" } } }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxHORIZONTAL, "Parent"));
    }
    SECTION("layoutif.true.button")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::LayoutIf { true, wxUI::Button { "Child" } } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "Create:Sizer[orientation=wxHORIZONTAL, caption=\"Parent\"]",
                  "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]",
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Parent\"]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]",
                  "SetEnabled:true",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Parent\"]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]:flags:(0,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("layoutif.false.button")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::LayoutIf { false, wxUI::Button { "Child" } } }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxHORIZONTAL, "Parent"));
    }
    SECTION("layoutif.true.true.button")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::LayoutIf { true, wxUI::Button { "Child" }, wxUI::LayoutIf { true, wxUI::Button { "Child" } } } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "Create:Sizer[orientation=wxHORIZONTAL, caption=\"Parent\"]",
                  "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]",
                  "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]",
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Parent\"]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]",
                  "SetEnabled:true",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]",
                  "SetEnabled:true",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Parent\"]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]:flags:(0,0x0,0)",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]:flags:(0,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("layoutif.true.false.button")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::LayoutIf { true, wxUI::Button { "Child" }, wxUI::LayoutIf { false, wxUI::Button { "Child" } } } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "Create:Sizer[orientation=wxHORIZONTAL, caption=\"Parent\"]",
                  "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]",
                  "topsizer:Sizer[orientation=wxHORIZONTAL, caption=\"Parent\"]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]",
                  "SetEnabled:true",
                  "sizer:Sizer[orientation=wxHORIZONTAL, caption=\"Parent\"]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Child\"]:flags:(0,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
