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
#include <wxUI/RadioBox.hpp>

#include <array>
#include <string_view>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::RadioBox;
using namespace wxUITests;

struct RadioBoxTestPolicy {
    using TypeUnderTest = wxUI::RadioBox;
    static auto createUUT() { return TypeUnderTest { TypeUnderTest::withChoices {}, std::vector<wxString> { "Hello", "Goodbye" } }; }
    static auto testStyle() { return wxRA_SPECIFY_COLS; }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return RadioBoxTestPolicy::createUUT(); }

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
    return std::format("wxRadioBox[id={}, pos=({},{}), size=({},{}), style={}, text=\"{}\", choices=({}), majorDim=0]", identity, pos.x, pos.y, size.x, size.y, style, text, fmtChoices);
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

TEST_CASE("RadioBox")
{
    SECTION("compile test")
    {
        // This just confirms which of the forms of construction are correct.
        // RadioBox with zero choices is intentionally not supported - semantically meaningless:
        // TypeUnderTest { TypeUnderTest::withChoices {} };
        TypeUnderTest { TypeUnderTest::withChoices {}, "hi" };
        TypeUnderTest { TypeUnderTest::withChoices {}, "hi", "bye" };
        TypeUnderTest { TypeUnderTest::withChoices {}, "hi", "bye", "goodbye" }; // Empty choices list - also not supported:        // TypeUnderTest { TypeUnderTest::withChoices {}, {} };
        TypeUnderTest { TypeUnderTest::withChoices {}, { "hi" } };
        TypeUnderTest { TypeUnderTest::withChoices {}, { "hi", "bye" } };
        TypeUnderTest { TypeUnderTest::withChoices {}, { "hi", "bye", "goodbye" } };
        // TypeUnderTest(TypeUnderTest::withChoices {}); // Not supported
        TypeUnderTest(TypeUnderTest::withChoices {}, "hi");
        TypeUnderTest(TypeUnderTest::withChoices {}, "hi", "bye");
        TypeUnderTest(TypeUnderTest::withChoices {}, "hi", "bye", "goodbye");
        // TypeUnderTest(TypeUnderTest::withChoices {}, {}); // Not supported
        TypeUnderTest(TypeUnderTest::withChoices {}, { "hi" });
        TypeUnderTest(TypeUnderTest::withChoices {}, { "hi", "bye" });
        TypeUnderTest(TypeUnderTest::withChoices {}, { "hi", "bye", "goodbye" });
    }
    SECTION("choices")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT()
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "", { "Hello", "Goodbye" }, 0));
    }

    SECTION("name.choice")
    {
        TestParent frame;
        using namespace std::literals;
        wxUI::VSizer {
            TypeUnderTest { "Greetings 🐨", TypeUnderTest::withChoices {}, { "Hello 🐨"s, "Goodbye"s } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "Greetings 🐨", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("id.choices")
    {
        TestParent frame;
        using namespace std::literals;
        wxUI::VSizer {
            TypeUnderTest { 10000, TypeUnderTest::withChoices {}, { "Hello 🐨"s, "Goodbye"s } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("id.name.choice")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, "Greetings", TypeUnderTest::withChoices {}, { "Hello 🐨", "Goodbye" } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 4, "Greetings", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("choices.single.literal")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { TypeUnderTest::withChoices {}, "Hello 🐨" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨" }, 0));
    }

    SECTION("id.choices.single.literal")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, TypeUnderTest::withChoices {}, "Hello 🐨" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨" }, 0));
    }

    SECTION("choices.initializer_list.string_view")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { TypeUnderTest::withChoices {}, { std::string_view { "Hello 🐨" }, std::string_view { "Goodbye" } } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("choices.ranges.array.string.literals")
    {
        TestParent frame;
        auto const choices = std::array<char const*, 2> { "Hello 🐨", "Goodbye" };
        wxUI::VSizer {
            TypeUnderTest { TypeUnderTest::withChoices {}, choices }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("choices.string.literals.nested.braces")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { TypeUnderTest::withChoices {}, { { "Hello 🐨", "Goodbye" } } }.withSelection(1)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨", "Goodbye" }, 1));
    }

    SECTION("choices.ranges")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello 🐨", "Goodbye" } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("name.choice.ranges")
    {
        TestParent frame;
        using namespace std::literals;
        wxUI::VSizer {
            TypeUnderTest { "Greetings 🐨", TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello 🐨"s, "Goodbye"s } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "Greetings 🐨", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("id.choices.ranges")
    {
        TestParent frame;
        using namespace std::literals;
        wxUI::VSizer {
            TypeUnderTest { 10000, TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello 🐨"s, "Goodbye"s } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("id.name.choice.ranges")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, "Greetings 🐨", TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello 🐨", "Goodbye" } }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 4, "Greetings 🐨", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("choices.variadic.literals")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { TypeUnderTest::withChoices {}, "Hello 🐨", "Goodbye" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("choices.variadic.multiple")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { TypeUnderTest::withChoices {}, "Alpha", "Beta", "Gamma", "Delta" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "", { "Alpha", "Beta", "Gamma", "Delta" }, 0));
    }

    SECTION("name.choices.variadic")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { "Greetings 🐨", TypeUnderTest::withChoices {}, "Hello 🐨", "Goodbye" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, 4, "Greetings 🐨", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("id.choices.variadic")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, TypeUnderTest::withChoices {}, "Hello 🐨", "Goodbye" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 4, "", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("id.name.choices.variadic")
    {
        TestParent frame;
        wxUI::VSizer {
            TypeUnderTest { 10000, "Greetings 🐨", TypeUnderTest::withChoices {}, "Hello 🐨", "Goodbye" }
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(10000, { -1, -1 }, { -1, -1 }, 4, "Greetings 🐨", { "Hello 🐨", "Goodbye" }, 0));
    }

    SECTION("style")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withStyle(wxRA_SPECIFY_COLS)
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { -1, -1 }, wxRA_SPECIFY_COLS, "", { "Hello", "Goodbye" }, 0));
    }

    SECTION("pos")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withPosition({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { 1, 2 }, { -1, -1 }, 4, "", { "Hello", "Goodbye" }, 0));
    }

    SECTION("size")
    {
        TestParent frame;
        wxUI::VSizer {
            createUUT().withSize({ 1, 2 })
        }
            .fitTo(&frame);
        CHECK(frame.dump() == testDump(-1, { -1, -1 }, { 1, 2 }, 4, "", { "Hello", "Goodbye" }, 0));
    }

    COMMON_TESTS(RadioBoxTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
