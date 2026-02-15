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
#include <string>
#include <string_view>
#include <wxUI/Button.hpp>
#include <wxUI/Layout.hpp>
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
TEST_CASE("Size")
{
    // <<<< VSizer >>>>
    SECTION("vSizer.empty")
    {
        TestParent frame;
        wxUI::VSizer {}.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxVERTICAL));
    }
    SECTION("vSizer.empty1")
    {
        TestParent frame;
        wxUI::VSizer { wxUI::Button { "Hello" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "Create:Sizer[orientation=wxVERTICAL]",
                  "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "topsizer:Sizer[orientation=wxVERTICAL]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "SetEnabled:true",
                  "sizer:Sizer[orientation=wxVERTICAL]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]:flags:(0,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("vSizer.named.empty")
    {
        TestParent frame;
        wxUI::VSizer { "Inner" }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxVERTICAL, "Inner"));
    }
    SECTION("vSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption { "Inner" };
        wxUI::VSizer { wxUI_String {}, caption }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxVERTICAL, "Inner"));
    }
    SECTION("vSizer.collapse.vSizer.empty")
    {
        TestParent frame;
        wxUI::VSizer { wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxVERTICAL));
    }
    SECTION("vSizer.collapse.vSizer.named.empty")
    {
        TestParent frame;
        wxUI::VSizer { wxUI::VSizer { "Inner" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxVERTICAL, "Inner"));
    }
    SECTION("vSizer.collapse.vSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption { "Inner" };
        wxUI::VSizer { wxUI::VSizer { wxUI_String {}, caption } }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxVERTICAL, "Inner"));
    }
    SECTION("vSizer.hSizer.empty")
    {
        TestParent frame;
        wxUI::VSizer { wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "", wxHORIZONTAL, ""));
    }
    SECTION("vSizer.hSizer.empty.flags")
    {
        TestParent frame;
        wxUI::VSizer { wxUI::HSizer {} }.withFlags(wxSizerFlags(1)).fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "", wxHORIZONTAL, "", 1));
    }
    SECTION("vSizer.flags.hSizer.empty")
    {
        TestParent frame;
        wxUI::VSizer { wxSizerFlags(1), wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "", wxHORIZONTAL, "", 1));
    }
    SECTION("vSizer.named.flags.hSizer.empty")
    {
        TestParent frame;
        wxUI::VSizer { "Parent", wxSizerFlags(1), wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxHORIZONTAL, "", 1));
    }
    SECTION("vSizer.named.tag.flags.hSizer.empty")
    {
        TestParent frame;
        wxString caption { "Parent" };
        wxUI::VSizer { wxUI_String {}, caption, wxSizerFlags(1), wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxHORIZONTAL, "", 1));
    }
    SECTION("vSizer.hSizer.named.empty")
    {
        TestParent frame;
        wxUI::VSizer { wxUI::HSizer { "Child" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "", wxHORIZONTAL, "Child"));
    }
    SECTION("vSizer.hSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption { "Child" };
        wxUI::VSizer { wxUI::HSizer { wxUI_String {}, caption } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "", wxHORIZONTAL, "Child"));
    }

    SECTION("vSizer.named.vSizer.empty")
    {
        TestParent frame;
        wxUI::VSizer { "Parent", wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxVERTICAL, ""));
    }
    SECTION("vSizer.named.tag.vSizer.empty")
    {
        TestParent frame;
        wxString caption { "Parent" };
        wxUI::VSizer { wxUI_String {}, caption, wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxVERTICAL, ""));
    }

    SECTION("vSizer.named.vSizer.named.empty")
    {
        TestParent frame;
        wxUI::VSizer { "Parent", wxUI::VSizer { "Inner" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxVERTICAL, "Inner"));
    }
    SECTION("vSizer.named.tag.vSizer.named.empty")
    {
        TestParent frame;
        wxString caption1 { "Parent" };
        wxUI::VSizer { wxUI_String {}, caption1, wxUI::VSizer { "Inner" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxVERTICAL, "Inner"));
    }
    SECTION("vSizer.named.vSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption2 { "Inner" };
        wxUI::VSizer { "Parent", wxUI::VSizer { wxUI_String {}, caption2 } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxVERTICAL, "Inner"));
    }
    SECTION("vSizer.named.tag.vSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption1 { "Parent" };
        wxString caption2 { "Inner" };
        wxUI::VSizer { wxUI_String {}, caption1, wxUI::VSizer { wxUI_String {}, caption2 } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxVERTICAL, "Inner"));
    }
    SECTION("vSizer.named.hSizer.empty")
    {
        TestParent frame;
        wxUI::VSizer { "Parent", wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxHORIZONTAL, ""));
    }
    SECTION("vSizer.named.tag.hSizer.empty")
    {
        TestParent frame;
        wxString caption { "Parent" };
        wxUI::VSizer { wxUI_String {}, caption, wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxHORIZONTAL, ""));
    }
    SECTION("vSizer.named.hSizer.named.empty")
    {
        TestParent frame;
        wxUI::VSizer { "Parent", wxUI::HSizer { "Child" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxHORIZONTAL, "Child"));
    }
    SECTION("vSizer.named.tag.hSizer.named.empty")
    {
        TestParent frame;
        wxString caption { "Parent" };
        wxUI::VSizer { wxUI_String {}, caption, wxUI::HSizer { "Child" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxHORIZONTAL, "Child"));
    }
    SECTION("vSizer.named.hSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption2 { "Child" };
        wxUI::VSizer { "Parent", wxUI::HSizer { wxUI_String {}, caption2 } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxHORIZONTAL, "Child"));
    }
    SECTION("vSizer.named.tag.hSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption1 { "Parent" };
        wxString caption2 { "Child" };
        wxUI::VSizer { wxUI_String {}, caption1, wxUI::HSizer { wxUI_String {}, caption2 } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxVERTICAL, "Parent", wxHORIZONTAL, "Child"));
    }

    // <<<< HSizer >>>>
    SECTION("hSizer.empty")
    {
        TestParent frame;
        wxUI::HSizer {}.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxHORIZONTAL));
    }
    SECTION("hSizer.empty1")
    {
        TestParent frame;
        wxUI::HSizer { wxUI::Button { "Hello" } }.fitTo(&frame);
        CHECK(frame.dump() == std::vector<std::string> {
                  "Create:Sizer[orientation=wxHORIZONTAL]",
                  "Create:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "topsizer:Sizer[orientation=wxHORIZONTAL]",
                  "controller:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "SetEnabled:true",
                  "sizer:Sizer[orientation=wxHORIZONTAL]",
                  "Add:wxButton[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]:flags:(0,0x0,0)",
                  "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]",
              });
    }
    SECTION("hSizer.named.empty")
    {
        TestParent frame;
        wxUI::HSizer { "Child" }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxHORIZONTAL, "Child"));
    }
    SECTION("hSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption { "Child" };
        wxUI::HSizer { wxUI_String {}, caption }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxHORIZONTAL, "Child"));
    }
    SECTION("hSizer.collapse.hSizer.empty")
    {
        TestParent frame;
        wxUI::HSizer { wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxHORIZONTAL));
    }
    SECTION("hSizer.collapse.hSizer.named.empty")
    {
        TestParent frame;
        wxUI::HSizer { wxUI::HSizer { "Inner" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxHORIZONTAL, "Inner"));
    }
    SECTION("hSizer.collapse.hSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption { "Inner" };
        wxUI::HSizer { wxUI::HSizer { wxUI_String {}, caption } }.fitTo(&frame);
        CHECK(frame.dump() == sizerDump(wxHORIZONTAL, "Inner"));
    }
    SECTION("hSizer.vSizer.empty")
    {
        TestParent frame;
        wxUI::HSizer { wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "", wxVERTICAL, ""));
    }
    SECTION("hSizer.vSizer.empty.flags")
    {
        TestParent frame;
        wxUI::HSizer { wxUI::VSizer {} }.withFlags(wxSizerFlags(1)).fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "", wxVERTICAL, "", 1));
    }
    SECTION("hSizer.flags.vSizer.empty")
    {
        TestParent frame;
        wxUI::HSizer { wxSizerFlags(1), wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "", wxVERTICAL, "", 1));
    }
    SECTION("hSizer.named.flags.vSizer.empty")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxSizerFlags(1), wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxVERTICAL, "", 1));
    }
    SECTION("hSizer.named.tag.flags.vSizer.empty")
    {
        TestParent frame;
        wxString caption { "Parent" };
        wxUI::HSizer { wxUI_String {}, caption, wxSizerFlags(1), wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxVERTICAL, "", 1));
    }
    SECTION("hSizer.vSizer.named.empty")
    {
        TestParent frame;
        wxUI::HSizer { wxUI::VSizer { "Inner" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "", wxVERTICAL, "Inner"));
    }
    SECTION("hSizer.vSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption { "Inner" };
        wxUI::HSizer { wxUI::VSizer { wxUI_String {}, caption } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "", wxVERTICAL, "Inner"));
    }

    SECTION("hSizer.named.vSizer.empty")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxVERTICAL, ""));
    }
    SECTION("hSizer.named.tag.vSizer.empty")
    {
        TestParent frame;
        wxString caption { "Parent" };
        wxUI::HSizer { wxUI_String {}, caption, wxUI::VSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxVERTICAL, ""));
    }
    SECTION("hSizer.named.vSizer.named.empty")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::VSizer { "Inner" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxVERTICAL, "Inner"));
    }
    SECTION("hSizer.named.tag.vSizer.named.empty")
    {
        TestParent frame;
        wxString caption1 { "Parent" };
        wxUI::HSizer { wxUI_String {}, caption1, wxUI::VSizer { "Inner" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxVERTICAL, "Inner"));
    }
    SECTION("hSizer.named.vSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption2 { "Inner" };
        wxUI::HSizer { "Parent", wxUI::VSizer { wxUI_String {}, caption2 } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxVERTICAL, "Inner"));
    }
    SECTION("hSizer.named.tag.vSizer.named.tag.empty")
    {
        TestParent frame;
        wxString caption1 { "Parent" };
        wxString caption2 { "Inner" };
        wxUI::HSizer { wxUI_String {}, caption1, wxUI::VSizer { wxUI_String {}, caption2 } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxVERTICAL, "Inner"));
    }
    SECTION("hSizer.named.hSizer.empty")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxHORIZONTAL, ""));
    }
    SECTION("hSizer.named.tag.hSizer.empty")
    {
        TestParent frame;
        wxString caption { "Parent" };
        wxUI::HSizer { wxUI_String {}, caption, wxUI::HSizer {} }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxHORIZONTAL, ""));
    }
    SECTION("hSizer.named.hSizer.named.empty")
    {
        TestParent frame;
        wxUI::HSizer { "Parent", wxUI::HSizer { "Child" } }.fitTo(&frame);
        CHECK(frame.dump() == sizerWithChildDump(wxHORIZONTAL, "Parent", wxHORIZONTAL, "Child"));
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
