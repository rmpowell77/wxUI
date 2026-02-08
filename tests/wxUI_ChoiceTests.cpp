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

TEST_CASE("Choice")
{
    SECTION("noargs")
    {
        TestProvider provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=()]",
                  "controller:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=()]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("choice")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { "Hello 🐨", "Goodbye" };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { 10000 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=()]",
                  "controller:wxChoice[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=()]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id.choice")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxChoice[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("choice.ranges")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { std::vector<std::string> { "Hello 🐨", "Goodbye" } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("id.choice.ranges")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { 10000, std::vector<std::string> { "Hello 🐨", "Goodbye" } };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxChoice[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("choice.ranges.1")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { std::views::iota(0, 2) | std::views::transform([](auto i) { return std::to_string(i); }) };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"0\",\"1\",)]",
                  "controller:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"0\",\"1\",)]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("setSelection")
    {
        TestProvider provider;
        auto uut = TypeUnderTest { 10000, { "Hello 🐨", "Goodbye" } }.withSelection(1);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "controller:wxChoice[id=10000, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"Hello 🐨\",\"Goodbye\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
              });
    }

    SECTION("style")
    {
        TestProvider provider;
        auto uut = createUUT().withStyle(wxCB_SORT);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=8, choices=()]",
                  "controller:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=8, choices=()]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("pos")
    {
        TestProvider provider;
        auto uut = createUUT().withPosition({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=-1, pos=(1,2), size=(-1,-1), style=0, choices=()]",
                  "controller:wxChoice[id=-1, pos=(1,2), size=(-1,-1), style=0, choices=()]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("size")
    {
        TestProvider provider;
        auto uut = createUUT().withSize({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=-1, pos=(-1,-1), size=(1,2), style=0, choices=()]",
                  "controller:wxChoice[id=-1, pos=(-1,-1), size=(1,2), style=0, choices=()]",
                  "SetSelection:0",
                  "SetEnabled:true",
              });
    }

    SECTION("AI")
    {
        TestProvider provider;
        auto uut = wxUI::Choice { { "one 🐨", "two", "three" } }.withSelection(1).bind([] {});
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"one 🐨\",\"two\",\"three\",)]",
                  "controller:wxChoice[id=-1, pos=(-1,-1), size=(-1,-1), style=0, choices=(\"one 🐨\",\"two\",\"three\",)]",
                  "SetSelection:1",
                  "SetEnabled:true",
                  "BindEvents:1",
              });
    }

    COMMON_TESTS(ChoiceTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
