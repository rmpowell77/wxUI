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
#include <wxUI/ListBox.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
using TypeUnderTest = wxUI::ListBox;
using namespace wxUITests;

struct ListBoxTestPolicy {
    using TypeUnderTest = wxUI::ListBox;
    static auto createUUT() { return TypeUnderTest {}; }
    static auto testStyle() { return wxLB_ALWAYS_SB | wxLB_MULTIPLE; }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return ListBoxTestPolicy::createUUT(); }

namespace {
using Dump = std::vector<std::string>;

std::string makeController(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    const std::vector<std::string>& choices)
{
    auto fmtChoices = [&]() {
        std::string result;
        for (const auto& choice : choices) {
            result += std::string { "\"" } + choice + "\",";
        }
        return result;
    }();
    return std::format("wxListBox[id={}, pos=({},{}), size=({},{}), style={}, choices=({})]", identity, pos.x, pos.y, size.x, size.y, style, fmtChoices);
}

Dump testDump(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    const std::vector<std::string>& choices,
    const std::vector<int>& selection)
{
    auto controller = makeController(identity, pos, size, style, choices);
    std::vector<std::string> results = {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:" + controller,
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:" + controller,
    };
    if (!selection.empty()) {
        for (const auto& sel : selection) {
            results.push_back(std::format("SetSelection:{}", sel));
        }
    }
    results.push_back("SetEnabled:true");
    results.push_back("sizer:Sizer[orientation=wxVERTICAL]");
    results.push_back("Add:" + controller + ":flags:(0,0x0,0)");
    results.push_back("SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]");
    return results;
}
}

TEST_CASE("ListBox")
{
    SECTION("compile test")
    {
        // This just confirms which of the forms of construction are correct.
        TypeUnderTest {};
        TypeUnderTest { "hi" };
        TypeUnderTest { "hi", "bye" };
        TypeUnderTest { "hi", "bye", "goodbye" };
        // TypeUnderTest{{}};
        // TypeUnderTest{{"hi"}};
        TypeUnderTest { { "hi", "bye" } };
        TypeUnderTest { { "hi", "bye", "goodbye" } };
        TypeUnderTest();
        // TypeUnderTest( "hi" );
        // TypeUnderTest( "hi", "bye" );
        // TypeUnderTest( "hi", "bye", "goodbye" );
        // TypeUnderTest({});
        TypeUnderTest({ "hi" });
        TypeUnderTest({ "hi", "bye" });
        TypeUnderTest({ "hi", "bye", "goodbye" });
    }
    SECTION("noargs")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT()
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, {}, {}));
    }

    SECTION("ListBox")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { "Hello 🐨", "Goodbye" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, { "Hello 🐨", "Goodbye" }, {}));
    }

    SECTION("id")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000 }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, {}, {}));
    }

    SECTION("id.ListBox")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, { "Hello 🐨", "Goodbye" }, {}));
    }

    SECTION("id.ListBox.ranges")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, std::vector<std::string> { "Hello 🐨", "Goodbye" } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, { "Hello 🐨", "Goodbye" }, {}));
    }

    SECTION("string.literals.nested.braces")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { { "one 🐨", "two" } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, { "one 🐨", "two" }, { 1 }));
    }

    SECTION("setSelection")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, { "Hello 🐨", "Goodbye" }, { 1 }));
    }

    SECTION("setSelections")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }.withStyle(wxLB_MULTIPLE).withSelections({ 0, 1 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 64, { "Hello 🐨", "Goodbye" }, { 0, 1 }));
    }

    SECTION("style")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withStyle(wxLB_MULTIPLE)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 64, {}, {}));
    }

    SECTION("pos")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withPosition({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { 1, 2 }, { -1, -1 }, 0, {}, {}));
    }

    COMMON_TESTS(ListBoxTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
