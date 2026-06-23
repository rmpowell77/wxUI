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
#include <ranges>
#include <wxUI/Choice.hpp>
#include <wxUI/Layout.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::Choice;
using namespace wxUITests;

struct ChoiceTestPolicy {
    using TypeUnderTest = wxUI::Choice;
    static auto createUUT() { return TypeUnderTest {}; }
    static auto testStyle() { return (wxCB_SORT); }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return ChoiceTestPolicy::createUUT(); }

namespace {
using Dump = std::vector<std::string>;

std::string makeController(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    std::vector<std::string> const& choices)
{
    auto fmtChoices = [&]() {
        std::string result;
        for (size_t i = 0; i < choices.size(); ++i) {
            result += std::string { "\"" } + choices[i] + "\"";
            result += ",";
        }
        return result;
    }();
    return std::format("wxChoice[id={}, pos=({},{}), size=({},{}), style={}, choices=({})]", identity, pos.x, pos.y, size.x, size.y, style, fmtChoices);
}

Dump testDump(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    std::vector<std::string> const& choices,
    int selection)
{
    auto controller = makeController(identity, pos, size, style, choices);
    return {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:" + controller,
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:" + controller,
        "SetSelection:" + std::to_string(selection),
        "SetEnabled:true",
        "sizer:Sizer[orientation=wxVERTICAL]",
        "Add:" + controller + ":flags:(0,0x0,0)",
        "SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]"
    };
}
}

TEST_CASE("Choice")
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
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, {}, 0));
    }

    SECTION("choice")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { "Hello 🐨", "Goodbye" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("id")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000 }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, {}, 0));
    }

    SECTION("id.choice")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("choice.ranges")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { std::vector<std::string> { "Hello 🐨", "Goodbye" } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("id.choice.ranges")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, std::vector<std::string> { "Hello 🐨", "Goodbye" } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("choice.ranges.1")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { std::views::iota(0, 2) | std::views::transform([](auto i) { return std::to_string(i); }) }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, { "0", "1" }, 0));
    }

    SECTION("setSelection")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, { "Hello 🐨", "Goodbye" }, 1));
    }

    SECTION("style")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withStyle(wxCB_SORT)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 8, {}, 0));
    }

    SECTION("pos")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withPosition({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { 1, 2 }, { -1, -1 }, 0, {}, 0));
    }

    SECTION("size")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withSize({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { 1, 2 }, 0, {}, 0));
    }

    SECTION("AI")
    {
        TestParent frame;
        wxUI::VSizer {
            wxUI::Choice { { "one 🐨", "two", "three" } }.withSelection(1).bind([] { })
        }
            .fitTo(&frame);
        auto result = testDump(-1, { -1, -1 }, { -1, -1 }, 0, { "one 🐨", "two", "three" }, 1);
        result.insert(result.begin() + 6, "BindEvents:1");
        CHECK(frame.dump() == result);
    }

    SECTION("string.literals.nested.braces")
    {
        TestParent frame;
        wxUI::VSizer {
            wxUI::Choice { { "one 🐨", "two" } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, { "one 🐨", "two" }, 1));
    }

    COMMON_TESTS(ChoiceTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
