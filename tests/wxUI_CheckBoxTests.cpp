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
#include <wxUI/CheckBox.hpp>
#include <wxUI/wxUITypes.hpp>

#include <wx/wx.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
using TypeUnderTest = wxUI::CheckBox;
using namespace wxUITests;
using namespace wxUI;

struct CheckBoxTestPolicy {
    using TypeUnderTest = wxUI::CheckBox;
    static auto createUUT() { return TypeUnderTest {}; }
    static auto testStyle() { return (wxCHK_3STATE); }
    static auto testPosition() { return wxPoint { 1, 2 }; }
    static auto testSize() { return wxSize { 10, 12 }; }
    static auto expectedStyle() { return testStyle(); }
    static auto expectedPosition() { return testPosition(); }
    static auto expectedSize() { return testSize(); }
};
static auto createUUT() { return CheckBoxTestPolicy::createUUT(); }

TEST_CASE("CheckBox")
{
    SECTION("noargs")
    {
        TestParent provider;
        auto uut = createUUT();
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "controller:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "SetValue:false",
                  "SetEnabled:true",
              });
    }

    SECTION("name")
    {
        TestParent provider;
        auto uut = TypeUnderTest { "Hello" };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "controller:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "SetValue:false",
                  "SetEnabled:true",
              });
    }

    SECTION("name_unicode")
    {
        TestParent provider;
        auto uut = TypeUnderTest { "🐨" };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"🐨\"]",
                  "controller:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"🐨\"]",
                  "SetValue:false",
                  "SetEnabled:true",
              });
    }

    SECTION("name_tag")
    {
        TestParent provider;
        wxString wx_label { "Hello" };
        auto uut = TypeUnderTest { wxUI_String {}, wx_label };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "controller:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "SetValue:false",
                  "SetEnabled:true",
              });
    }

    SECTION("id")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000 };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxCheckBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "controller:wxCheckBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"\"]",
                  "SetValue:false",
                  "SetEnabled:true",
              });
    }

    SECTION("id.name")
    {
        TestParent provider;
        auto uut = TypeUnderTest { 10000, "Hello" };
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxCheckBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "controller:wxCheckBox[id=10000, pos=(-1,-1), size=(-1,-1), style=0, text=\"Hello\"]",
                  "SetValue:false",
                  "SetEnabled:true",
              });
    }

    SECTION("style")
    {
        TestParent provider;
        auto uut = createUUT().withStyle(wxCHK_3STATE);
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4096, text=\"\"]",
                  "controller:wxCheckBox[id=-1, pos=(-1,-1), size=(-1,-1), style=4096, text=\"\"]",
                  "SetValue:false",
                  "SetEnabled:true",
              });
    }

    SECTION("pos")
    {
        TestParent provider;
        auto uut = createUUT().withPosition({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxCheckBox[id=-1, pos=(1,2), size=(-1,-1), style=0, text=\"\"]",
                  "controller:wxCheckBox[id=-1, pos=(1,2), size=(-1,-1), style=0, text=\"\"]",
                  "SetValue:false",
                  "SetEnabled:true",
              });
    }

    SECTION("size")
    {
        TestParent provider;
        auto uut = createUUT().withSize({ 1, 2 });
        uut.create(&provider);
        CHECK(provider.dump() == std::vector<std::string> {
                  "Create:wxCheckBox[id=-1, pos=(-1,-1), size=(1,2), style=0, text=\"\"]",
                  "controller:wxCheckBox[id=-1, pos=(-1,-1), size=(1,2), style=0, text=\"\"]",
                  "SetValue:false",
                  "SetEnabled:true",
              });
    }

    COMMON_TESTS(CheckBoxTestPolicy)
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, readability-function-cognitive-complexity, misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
