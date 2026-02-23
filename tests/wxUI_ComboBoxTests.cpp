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
        TestParent provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=()]",
                  "controller:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=()]",
                  "SetEnabled:true",
              });
    }

    SECTION("id.choice")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { std::string {} } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
                  "controller:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("choice.ranges")
    {
        TestParent provider;
        auto uut = TypeUnderTest { std::vector<std::string> { std::string {} } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id.choice.ranges")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, std::vector<std::string> { std::string {} } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
                  "controller:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
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
                  "Create:wxComboBox[id=-1, pos=(1,2), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
                  "controller:wxComboBox[id=-1, pos=(1,2), size=(-1,-1), style=0, text=\"\", choices=(\"\",)]",
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
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(1,2), style=0, text=\"\", choices=(\"\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(1,2), style=0, text=\"\", choices=(\"\",)]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("setSelection")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }.withSize({ 1, 2 }).withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=10000, pos=(-1,-1), size=(1,2), style=0, text=\"Hello 🐨\", choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxComboBox[id=10000, pos=(-1,-1), size=(1,2), style=0, text=\"Hello 🐨\", choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("empty")
    {
        TestParent provider;
        auto uut = wxUI::ComboBox {};
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=()]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\", choices=()]",
                  "SetEnabled:true",
              });
    }

    SECTION("string.literals")
    {
        TestParent provider;
        auto uut = wxUI::ComboBox { "one 🐨", "two" }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("id.string.literals")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { "one 🐨", "two" } }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("id.string.literals.nested.braces")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { { "one 🐨", "two" } } }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("choice.ranges.wxString")
    {
        TestParent provider;
        auto uut = TypeUnderTest { std::vector<wxString> { wxString("one"), wxString("two") } }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one\", choices=(\"one\",\"two\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one\", choices=(\"one\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("choice.ranges.wxString.utf8")
    {
        TestParent provider;
        auto uut = TypeUnderTest { std::vector<wxString> { wxString::FromUTF8("one 🐨"), wxString::FromUTF8("two") } }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("choice.initializer_list.string_view")
    {
        TestParent provider;
        auto uut = TypeUnderTest { { std::string_view { "one 🐨" }, std::string_view { "two" } } }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("choice.array.string.literals")
    {
        TestParent provider;
        auto const choices = std::array<char const*, 2> { "one 🐨", "two" };
        auto uut = TypeUnderTest { choices }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("choice.array.strings")
    {
        TestParent provider;
        auto const choices = std::array<std::string, 2> { std::string { "one 🐨" }, std::string { "two" } };
        auto uut = TypeUnderTest { choices }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("id.choice.array.string.literals")
    {
        TestParent provider;
        auto const choices = std::array<char const*, 2> { "one 🐨", "two" };
        auto uut = TypeUnderTest { 10000, choices }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("choice.vector.string.literals")
    {
        TestParent provider;
        auto const choices = std::vector<char const*> { "one 🐨", "two" };
        auto uut = TypeUnderTest { choices }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("string.literals.nested.braces")
    {
        TestParent provider;
        auto uut = wxUI::ComboBox { { "one 🐨", "two" } }.withSelection(1).bind([] { });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxComboBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"one 🐨\", choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
                  "BindEvents:1",
              });
    }

    COMMON_TESTS(ComboBoxTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
