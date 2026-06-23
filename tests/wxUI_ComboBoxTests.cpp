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
#include <wxUI/ComboBox.hpp>
#include <wxUI/Layout.hpp>

#include <array>
#include <string_view>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::ComboBox;
using namespace wxUITests;

struct ComboBoxTestPolicy {
    using TypeUnderTest = wxUI::ComboBox;
    static auto createUUT() { return TypeUnderTest { std::string {} }; }
    static auto testStyle() { return (wxCB_DROPDOWN | wxCB_READONLY); }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return ComboBoxTestPolicy::createUUT(); }

namespace {
using Dump = std::vector<std::string>;

std::string makeController(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    const std::string& text,
    const std::vector<std::string>& choices)
{
    auto fmtChoices = [&]() {
        std::string result;
        for (const auto& choice : choices) {
            result += std::string { "\"" } + choice + "\",";
        }
        return result;
    }();
    return std::format("wxComboBox[id={}, pos=({},{}), size=({},{}), style={}, text=\"{}\", choices=({})]", identity, pos.x, pos.y, size.x, size.y, style, text, fmtChoices);
}

Dump testDump(
    int identity,
    wxPoint pos,
    wxSize size,
    int style,
    const std::string& text,
    const std::vector<std::string>& choices,
    std::optional<int> selection)
{
    auto controller = makeController(identity, pos, size, style, text, choices);
    std::vector<std::string> results = {
        "Create:Sizer[orientation=wxVERTICAL]",
        "Create:" + controller,
        "topsizer:Sizer[orientation=wxVERTICAL]",
        "controller:" + controller,
    };
    if (selection.has_value()) {
        results.push_back(std::format("SetSelection:{}", selection.value()));
    }
    results.push_back("SetEnabled:true");
    results.push_back("sizer:Sizer[orientation=wxVERTICAL]");
    results.push_back("Add:" + controller + ":flags:(0,0x0,0)");
    results.push_back("SetSizeHints:[id=0, pos=(0,0), size=(0,0), style=0]");
    return results;
}
}

TEST_CASE("ComboBox")
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
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "", { "" }, 0));
    }

    SECTION("id")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000 }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "", {}, std::nullopt));
    }

    SECTION("id.choice")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, { std::string {} } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "", { "" }, 0));
    }

    SECTION("choice.ranges")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { std::vector<std::string> { std::string {} } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "", { "" }, 0));
    }

    SECTION("id.choice.ranges")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, std::vector<std::string> { std::string {} } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "", { "" }, 0));
    }

    SECTION("pos")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withPosition({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { 1, 2 }, { -1, -1 }, 0, "", { "" }, 0));
    }

    SECTION("size")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withSize({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { 1, 2 }, 0, "", { "" }, 0));
    }

    SECTION("setSelection")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }.withSize({ 1, 2 }).withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { 1, 2 }, 0, "Hello 🐨", { "Hello 🐨", "Goodbye" }, 1));
    }

    SECTION("empty")
    {
        TestParent frame;
        wxUI::VSizer {
            wxUI::ComboBox {}
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "", {}, std::nullopt));
    }

    SECTION("string.literals")
    {
        TestParent frame;
        wxUI::VSizer {
            wxUI::ComboBox { "one 🐨", "two" }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1));
    }

    SECTION("id.string.literals")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, { "one 🐨", "two" } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1));
    }

    SECTION("id.string.literals.nested.braces")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, { { "one 🐨", "two" } } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1));
    }

    SECTION("choice.ranges.wxString")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { std::vector<wxString> { wxString("one"), wxString("two") } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "one", { "one", "two" }, 1));
    }

    SECTION("choice.ranges.wxString.utf8")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { std::vector<wxString> { wxString::FromUTF8("one 🐨"), wxString::FromUTF8("two") } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1));
    }

    SECTION("choice.initializer_list.string_view")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { { std::string_view { "one 🐨" }, std::string_view { "two" } } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1));
    }

    SECTION("choice.array.string.literals")
    {
        TestParent frame;
        auto const choices = std::array<char const*, 2> { "one 🐨", "two" };
        wxUI::VSizer {
            TypeUnderTest { choices }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1));
    }

    SECTION("choice.array.strings")
    {
        TestParent frame;
        auto const choices = std::array<std::string, 2> { std::string { "one 🐨" }, std::string { "two" } };
        wxUI::VSizer {
            TypeUnderTest { choices }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1));
    }

    SECTION("id.choice.array.string.literals")
    {
        TestParent frame;
        auto const choices = std::array<char const*, 2> { "one 🐨", "two" };
        wxUI::VSizer {
            TypeUnderTest { 10000, choices }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1));
    }

    SECTION("choice.vector.string.literals")
    {
        TestParent frame;
        auto const choices = std::vector<char const*> { "one 🐨", "two" };
        wxUI::VSizer {
            TypeUnderTest { choices }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1));
    }

    SECTION("string.literals.nested.braces")
    {
        TestParent frame;
        wxUI::VSizer {
            wxUI::ComboBox { { "one 🐨", "two" } }.withSelection(1).bind([] { })
        }
            .fitTo(&frame);

        auto result = testDump(-1, { -1, -1 }, { -1, -1 }, 0, "one 🐨", { "one 🐨", "two" }, 1);
        result.insert(result.begin() + 6, "BindEvents:1");
        CHECK(frame.dump() == result);
    }

    COMMON_TESTS(ComboBoxTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
