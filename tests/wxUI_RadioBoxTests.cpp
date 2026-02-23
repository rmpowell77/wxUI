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

TEST_CASE("RadioBox")
{
    SECTION("compile test")
    {
        // This just confirms which of the forms of construction are correct.
        // TypeUnderTest { TypeUnderTest::withChoices {} };
        TypeUnderTest { TypeUnderTest::withChoices {}, "hi" };
        // TypeUnderTest { TypeUnderTest::withChoices {}, "hi", "bye" };
        // TypeUnderTest { TypeUnderTest::withChoices {}, "hi", "bye", "goodbye" };
        // TypeUnderTest { TypeUnderTest::withChoices {}, {} };
        TypeUnderTest { TypeUnderTest::withChoices {}, { "hi" } };
        TypeUnderTest { TypeUnderTest::withChoices {}, { "hi", "bye" } };
        TypeUnderTest { TypeUnderTest::withChoices {}, { "hi", "bye", "goodbye" } };
        // TypeUnderTest(TypeUnderTest::withChoices {});
        TypeUnderTest(TypeUnderTest::withChoices {}, "hi");
        // TypeUnderTest(TypeUnderTest::withChoices {}, "hi", "bye");
        // TypeUnderTest(TypeUnderTest::withChoices {}, "hi", "bye", "goodbye");
        // TypeUnderTest(TypeUnderTest::withChoices {}, {});
        TypeUnderTest(TypeUnderTest::withChoices {}, { "hi" });
        TypeUnderTest(TypeUnderTest::withChoices {}, { "hi", "bye" });
        TypeUnderTest(TypeUnderTest::withChoices {}, { "hi", "bye", "goodbye" });
    }
    SECTION("choices")
    {
        TestParent provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("name.choice")
    {
        TestParent provider;
        using namespace std::literals;
        auto uut = TypeUnderTest { "Greetings 🐨", TypeUnderTest::withChoices {}, { "Hello 🐨"s, "Goodbye"s } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"Greetings 🐨\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"Greetings 🐨\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id.choices")
    {
        TestParent provider;
        using namespace std::literals;
        auto uut = TypeUnderTest { 10000, TypeUnderTest::withChoices {}, { "Hello 🐨"s, "Goodbye"s } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id.name.choice")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, "Greetings", TypeUnderTest::withChoices {}, { "Hello 🐨", "Goodbye" } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"Greetings\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"Greetings\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("choices.single.literal")
    {
        TestParent provider;
        auto uut = TypeUnderTest { TypeUnderTest::withChoices {}, "Hello 🐨" };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id.choices.single.literal")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, TypeUnderTest::withChoices {}, "Hello 🐨" };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",), majorDim=0]",
                  "controller:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("choices.initializer_list.string_view")
    {
        TestParent provider;
        auto uut = TypeUnderTest { TypeUnderTest::withChoices {}, { std::string_view { "Hello 🐨" }, std::string_view { "Goodbye" } } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("choices.ranges.array.string.literals")
    {
        TestParent provider;
        auto const choices = std::array<char const*, 2> { "Hello 🐨", "Goodbye" };
        auto uut = TypeUnderTest { TypeUnderTest::withChoices {}, choices };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("choices.string.literals.nested.braces")
    {
        TestParent provider;
        auto uut = TypeUnderTest { TypeUnderTest::withChoices {}, { { "Hello 🐨", "Goodbye" } } }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("choices.ranges")
    {
        TestParent provider;
        auto uut = TypeUnderTest { TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello 🐨", "Goodbye" } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("name.choice.ranges")
    {
        TestParent provider;
        using namespace std::literals;
        auto uut = TypeUnderTest { "Greetings 🐨", TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello 🐨"s, "Goodbye"s } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"Greetings 🐨\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"Greetings 🐨\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id.choices.ranges")
    {
        TestParent provider;
        using namespace std::literals;
        auto uut = TypeUnderTest { 10000, TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello 🐨"s, "Goodbye"s } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id.name.choice.ranges")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, "Greetings 🐨", TypeUnderTest::withChoices {}, std::vector<std::string> { "Hello 🐨", "Goodbye" } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"Greetings 🐨\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=10000, pos=(-1,-1), size=(-1,-1), style=4, text=\"Greetings 🐨\", choices=(\"Hello 🐨\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("style")
    {
        TestParent provider;
        auto uut = createUUT().withStyle(wxRA_SPECIFY_COLS);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4, text=\"\", choices=(\"Hello\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("pos")
    {
        TestParent provider;
        auto uut = createUUT().withPosition({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(1,2), size=(-1,-1), style=4, text=\"\", choices=(\"Hello\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(1,2), size=(-1,-1), style=4, text=\"\", choices=(\"Hello\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("size")
    {
        TestParent provider;
        auto uut = createUUT().withSize({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxRadioBox[id=-1, pos=(-1,-1), size=(1,2), style=4, text=\"\", choices=(\"Hello\",\"Goodbye\",), majorDim=0]",
                  "controller:wxRadioBox[id=-1, pos=(-1,-1), size=(1,2), style=4, text=\"\", choices=(\"Hello\",\"Goodbye\",), majorDim=0]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    COMMON_TESTS(RadioBoxTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
