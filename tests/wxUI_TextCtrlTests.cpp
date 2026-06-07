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
#include "wxUI_TestControlCommon.hpp"
#include <catch2/catch_test_macros.hpp>
#include <wxUI/Layout.hpp>
#include <wxUI/TextCtrl.hpp>
#include <wxUI/wxUITypes.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::TextCtrl;
using namespace wxUITests;
using namespace wxUI;

struct TextCtrlTestPolicy {
    using TypeUnderTest = wxUI::TextCtrl;
    static auto createUUT() { return TypeUnderTest {}; }
    static auto testStyle() { return wxTE_PROCESS_ENTER; }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return TextCtrlTestPolicy::createUUT(); }

namespace {
using Dump = std::vector<std::string>;

std::string makeController(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    const std::string& text)
{
    return std::format("wxTextCtrl[id={}, pos=({},{}), size=({},{}), style={}, text=\"{}\"]", identity, pos.x, pos.y, size.x, size.y, style, text);
}

Dump testDump(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    const std::string& text)
{
    auto controller = makeController(identity, pos, size, style, text);
    return {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:" + controller,
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:" + controller,
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxVERTICAL]",
        "Add:" + controller + ":flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]"
    };
}
}

TEST_CASE("TextCtrl")
{
    SECTION("noargs")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT()
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, ""));
    }

    SECTION("name")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { "Hello" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "Hello"));
    }

    SECTION("name_unicode")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { "🐨" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "🐨"));
    }

    SECTION("name_tag")
    {
        TestParent frame;
        wxString wx_label { "Hello" };
        wxUI::VSizer {
            TypeUnderTest { wxUI_String {}, wx_label }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "Hello"));
    }

    SECTION("id")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000 }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, ""));
    }

    SECTION("id.name")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, "Hello" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "Hello"));
    }

    SECTION("style")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withStyle(wxTE_PROCESS_ENTER)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 1024, ""));
    }

    SECTION("pos")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withPosition({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { 1, 2 }, { -1, -1 }, 0, ""));
    }

    SECTION("size")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withSize({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { 1, 2 }, 0, ""));
    }

    COMMON_TESTS(TextCtrlTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
