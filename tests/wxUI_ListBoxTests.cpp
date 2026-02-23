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
        TestParent provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=()]",
                  "controller:wxListBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=()]",
                  "SetEnabled:true",
              });
    }

    SECTION("ListBox")
    {
        TestParent provider;
        auto uut = TypeUnderTest { "Hello 🐨", "Goodbye" };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxListBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetEnabled:true",
              });
    }

    SECTION("id")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=()]",
                  "controller:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=()]",
                  "SetEnabled:true",
              });
    }

    SECTION("id.ListBox")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetEnabled:true",
              });
    }

    SECTION("id.ListBox.ranges")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, std::vector<std::string> { "Hello 🐨", "Goodbye" } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetEnabled:true",
              });
    }

    SECTION("string.literals.nested.braces")
    {
        TestParent provider;
        auto uut = TypeUnderTest { { "one 🐨", "two" } }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"one 🐨\",\"two\",)]",
                  "controller:wxListBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"one 🐨\",\"two\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("setSelection")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("setSelections")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }.withStyle(wxLB_MULTIPLE).withSelections({ 0, 1 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=64, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxListBox[id=10000, pos=(-1,-1), size=(-1,-1), style=64, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetSelection:0",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("style")
    {
        TestParent provider;
        auto uut = createUUT().withStyle(wxLB_MULTIPLE);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=-1, pos=(-1,-1), size=(-1,-1), style=64, choices=()]",
                  "controller:wxListBox[id=-1, pos=(-1,-1), size=(-1,-1), style=64, choices=()]",
                  "SetEnabled:true",
              });
    }

    SECTION("pos")
    {
        TestParent provider;
        auto uut = createUUT().withPosition({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxListBox[id=-1, pos=(1,2), size=(-1,-1), style=0, choices=()]",
                  "controller:wxListBox[id=-1, pos=(1,2), size=(-1,-1), style=0, choices=()]",
                  "SetEnabled:true",
              });
    }

    COMMON_TESTS(ListBoxTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity)
